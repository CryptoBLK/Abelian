// Copyright (c) 2014-2018, The Monero Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <unistd.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/shared_ptr.hpp>

#include "common/varint.h"
#include "warnings.h"
#include "crypto.h"
#include "hash.h"

// Add some log capabilities
#include "misc_log_ex.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "crypto"

namespace {
  static void local_abort(const char *msg)
  {
    fprintf(stderr, "%s\n", msg);
#ifdef NDEBUG
    _exit(1);
#else
    abort();
#endif
  }
}

namespace crypto {

  using std::abort;
  using std::int32_t;
  using std::int64_t;
  using std::size_t;
  using std::uint32_t;
  using std::uint64_t;

  extern "C" {
#include "crypto-ops.h"
#include "random.h"
#include "dilithium/randombytes.h"
  }

  const crypto::public_key null_pkey = crypto::public_key{};
  const crypto::secret_key null_skey = crypto::secret_key{};

  static inline unsigned char *operator &(ec_point &point) {
    return &reinterpret_cast<unsigned char &>(point);
  }

  static inline const unsigned char *operator &(const ec_point &point) {
    return &reinterpret_cast<const unsigned char &>(point);
  }

  static inline unsigned char *operator &(ec_scalar &scalar) {
    return &reinterpret_cast<unsigned char &>(scalar);
  }

  static inline const unsigned char *operator &(const ec_scalar &scalar) {
    return &reinterpret_cast<const unsigned char &>(scalar);
  }

  void generate_random_bytes_thread_safe(size_t N, uint8_t *bytes)
  {
    static boost::mutex random_lock;
    boost::lock_guard<boost::mutex> lock(random_lock);
    generate_random_bytes_not_thread_safe(N, bytes);
  }

  static inline bool less32(const unsigned char *k0, const unsigned char *k1)
  {
    for (int n = 31; n >= 0; --n)
    {
      if (k0[n] < k1[n])
        return true;
      if (k0[n] > k1[n])
        return false;
    }
    return false;
  }

  void random32_unbiased(unsigned char *bytes)
  {
    // l = 2^252 + 27742317777372353535851937790883648493.
    // it fits 15 in 32 bytes
    static const unsigned char limit[32] = { 0xe3, 0x6a, 0x67, 0x72, 0x8b, 0xce, 0x13, 0x29, 0x8f, 0x30, 0x82, 0x8c, 0x0b, 0xa4, 0x10, 0x39, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0 };
    do
    {
      generate_random_bytes_thread_safe(32, bytes);
    } while (!sc_isnonzero(bytes) && !less32(bytes, limit)); // should be good about 15/16 of the time
    sc_reduce32(bytes);
  }
  /* generate a random 32-byte (256-bit) integer and copy it to res */
  static inline void random_scalar(pq_seed &res) {
      randombytes((unsigned char*)res.data, 32U);
  }

  void hash_to_scalar(const void *data, size_t length, ec_scalar &res) {
    cn_fast_hash(data, length, reinterpret_cast<hash &>(res));
    sc_reduce32(&res);
  }

  /* 
   * generate public and secret keys from a random 256-bit integer
   * TODO: allow specifying random value (for wallet recovery)
   * 
   */
  rand_seed crypto_ops::generate_keys(public_key &pub, secret_key &sec, const rand_seed& recovery_key, bool recover) {
    LOG_PRINT_L1("crypto_ops " <<__func__);
    rand_seed rng;

    unsigned char pk[CRYPTO_PUBLICKEYBYTES];
    unsigned char sk[CRYPTO_SECRETKEYBYTES];

    if (recover)
    {
        rng = recovery_key;
    }
    else
    {
        random_scalar(rng);
    }
    // Dilithium Key generation with Monero 256 bit generated bytes
    crypto::crypto_sign_keypair(pk, sk, (unsigned char *)&rng);

    std::memcpy(&sec, sk, CRYPTO_SECRETKEYBYTES);
    std::memcpy(&pub, pk, CRYPTO_PUBLICKEYBYTES);

    // Returns the random 32 byte seed
    return rng;
  }

  bool crypto_ops::check_key(const public_key &key) {
    LOG_PRINT_L1("crypto_ops " <<__func__);

    ge_p3 point;
    return ge_frombytes_vartime(&point, &key) == 0;
  }

  bool crypto_ops::secret_key_to_public_key(const secret_key &sec, public_key &pub) {
    LOG_PRINT_L1("crypto_ops " <<__func__);
    ge_p3 point;
    if (sc_check(&unwrap(sec)) != 0) {
      return false;
    }
    ge_scalarmult_base(&point, &unwrap(sec));
    ge_p3_tobytes(&pub, &point);
    return true;
  }

  bool crypto_ops::generate_key_derivation(const public_key &key1, const secret_key &key2, key_derivation &derivation) {
    LOG_PRINT_L1("crypto_ops " <<__func__);
    /*ge_p3 point;
    ge_p2 point2;
    ge_p1p1 point3;
    assert(sc_check(&key2) == 0);
    if (ge_frombytes_vartime(&point, &key1) != 0) {
      return false;
    }
    ge_scalarmult(&point2, &unwrap(key2), &point);
    ge_mul8(&point3, &point2);
    ge_p1p1_to_p2(&point2, &point3);
    ge_tobytes(&derivation, &point2);*/

    // TODO: No Dilithium implementation yet, so derivation = PVk
    std::memcpy(&derivation, &key1, CRYPTO_PUBLICKEYBYTES);

    return true;
  }

  void crypto_ops::derivation_to_scalar(const key_derivation &derivation, size_t output_index, ec_scalar &res) {
    LOG_PRINT_L1("crypto_ops " <<__func__);
    struct {
      key_derivation derivation;
      char output_index[(sizeof(size_t) * 8 + 6) / 7];
    } buf;
    char *end = buf.output_index;
    buf.derivation = derivation;
    tools::write_varint(end, output_index);
    assert(end <= buf.output_index + sizeof buf.output_index);
    hash_to_scalar(&buf, end - reinterpret_cast<char *>(&buf), res);
    //TODO: Hash function for the one time public key - output_index to ensure key uniqueness
  }

  bool crypto_ops::derive_public_key(const key_derivation &derivation, size_t output_index,
    const public_key &base, public_key &derived_key) {
    LOG_PRINT_L1("crypto_ops " <<__func__);
    /*ec_scalar scalar;
    ge_p3 point1;
    ge_p3 point2;
    ge_cached point3;
    ge_p1p1 point4;
    ge_p2 point5;
    if (ge_frombytes_vartime(&point1, &base) != 0) {
      return false;
    }
    derivation_to_scalar(derivation, output_index, scalar);
    ge_scalarmult_base(&point2, &scalar);
    ge_p3_to_cached(&point3, &point2);
    ge_add(&point4, &point1, &point3);
    ge_p1p1_to_p2(&point5, &point4);
    ge_tobytes(&derived_key, &point5);*/

    // TODO: No Dilithium implementation yet, so derived_key = PSk
    std::memcpy(&derived_key, &base, CRYPTO_PUBLICKEYBYTES);

    return true;
  }

  void crypto_ops::derive_secret_key(const key_derivation &derivation, size_t output_index,
    const secret_key &base, secret_key &derived_key) {
    LOG_PRINT_L1("crypto_ops " <<__func__);
    //ec_scalar scalar;
    //assert(sc_check(&base) == 0);
    //derivation_to_scalar(derivation, output_index, scalar);
    //sc_add(&unwrap(derived_key), &unwrap(base), &scalar);

    // TODO: No Dilithium implementation yet, so derived_key = sSk
    std::memcpy(&derived_key, &base, CRYPTO_SECRETKEYBYTES);
  }

  bool crypto_ops::derive_subaddress_public_key(const public_key &out_key, const key_derivation &derivation, std::size_t output_index, public_key &derived_key) {
    LOG_PRINT_L1("crypto_ops " <<__func__);
    /*ec_scalar scalar;
    ge_p3 point1;
    ge_p3 point2;
    ge_cached point3;
    ge_p1p1 point4;
    ge_p2 point5;
    if (ge_frombytes_vartime(&point1, &out_key) != 0) {
      return false;
    }
    derivation_to_scalar(derivation, output_index, scalar);
    ge_scalarmult_base(&point2, &scalar);
    ge_p3_to_cached(&point3, &point2);
    ge_sub(&point4, &point1, &point3);
    ge_p1p1_to_p2(&point5, &point4);
    ge_tobytes(&derived_key, &point5);*/

    // TODO: No Dilithium implementation yet, so derived_key = Poutk
    std::memcpy(&derived_key, &out_key, CRYPTO_PUBLICKEYBYTES);

    return true;
  }

  struct s_comm {
    hash h;
    ec_point key;
    ec_point comm;
  };

  struct s_comm_2 {
    hash msg;
    ec_point D;
    ec_point X;
    ec_point Y;
  };
// Dilithium Signature - crypto_sign
  void crypto_ops::generate_signature(const hash &prefix_hash, const public_key &pub, const secret_key &sec, signature &sig) {
    LOG_PRINT_L1("crypto_ops " <<__func__);
    unsigned long long signatureLen = sizeof(sig);
    LOG_PRINT_L1("::generate_signature size: " << signatureLen << "message length: " << sizeof(prefix_hash));
    auto result = crypto::crypto_sign((unsigned char *)&sig, &signatureLen, (unsigned char *)&prefix_hash, sizeof(prefix_hash), &sec);

    // Try logging here!
    LOG_PRINT_L1("::generate signature result = " << result);

    assert(result == 0);
  }
// Dilithium Signature - crypto_open
  bool crypto_ops::check_signature(const hash &prefix_hash, const public_key &pub, const signature &sig) {
    LOG_PRINT_L1("crypto_ops " << __func__);

    unsigned long long len = sizeof(prefix_hash);
    auto result = crypto::crypto_sign_open((unsigned char *)&prefix_hash, &len, (unsigned char *)&sig, sizeof(sig), &pub);
    
    LOG_PRINT_L1("::check_signature result = " <<result);

    return result ? true : false;
  }

  void crypto_ops::generate_tx_proof(const hash &prefix_hash, const public_key &R, const public_key &A, const boost::optional<public_key> &B, const public_key &D, const secret_key &r, signature &sig) {
    LOG_PRINT_L1("crypto_ops " <<__func__);

  }

  bool crypto_ops::check_tx_proof(const hash &prefix_hash, const public_key &R, const public_key &A, const boost::optional<public_key> &B, const public_key &D, const signature &sig) {
    LOG_PRINT_L1("crypto_ops " <<__func__);

      return true;
  }

  static void hash_to_ec(const public_key &key, ge_p3 &res) {
    hash h;
    ge_p2 point;
    ge_p1p1 point2;
    cn_fast_hash(std::addressof(key), sizeof(public_key), h);
    ge_fromfe_frombytes_vartime(&point, reinterpret_cast<const unsigned char *>(&h));
    ge_mul8(&point2, &point);
    ge_p1p1_to_p3(&res, &point2);
  }

  void crypto_ops::generate_key_image(const public_key &pub, const secret_key &sec, key_image &image) {
    LOG_PRINT_L1("crypto_ops " <<__func__);

    std::memcpy(&image, &pub, CRYPTO_PUBLICKEYBYTES);
  }

PUSH_WARNINGS
DISABLE_VS_WARNINGS(4200)
  struct ec_point_pair {
    ec_point a, b;
  };
  struct rs_comm {
    hash h;
    struct ec_point_pair ab[];
  };
POP_WARNINGS

  static inline size_t rs_comm_size(size_t pubs_count) {
    return sizeof(rs_comm) + pubs_count * sizeof(ec_point_pair);
  }

  void crypto_ops::generate_ring_signature(const hash &prefix_hash, const key_image &image,
    const public_key *const *pubs, size_t pubs_count,
    const secret_key &sec, size_t sec_index,
    signature *sig) {
    LOG_PRINT_L1("crypto_ops " <<__func__);

  }

  bool crypto_ops::check_ring_signature(const hash &prefix_hash, const key_image &image,
    const public_key *const *pubs, size_t pubs_count,
    const signature *sig) {
    LOG_PRINT_L1("crypto_ops " <<__func__);

    return true;
  }
}

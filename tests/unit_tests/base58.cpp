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

#include "gtest/gtest.h"

#include <cstdint>

#include "common/base58.cpp"
#include "cryptonote_basic/cryptonote_basic_impl.h"
#include "serialization/binary_utils.h"

using namespace tools;

#define MAKE_STR(arr) std::string(arr, sizeof(arr) - 1)

namespace
{
  void do_test_uint_8be_to_64(uint64_t expected, const uint8_t* data, size_t size)
  {
    uint64_t val = base58::uint_8be_to_64(data, size);
    ASSERT_EQ(val, expected);
  }

  void do_test_uint_64_to_8be(uint64_t num, const std::string& expected_str)
  {
    std::string data(expected_str.size(), '\x0');
    base58::uint_64_to_8be(num, data.size(), reinterpret_cast<uint8_t*>(&data[0]));
    ASSERT_EQ(data, expected_str);
  }

  void do_test_encode_block(const std::string& block, const std::string& expected)
  {
    ASSERT_TRUE(1 <= block.size() && block.size() <= base58::full_block_size);
    std::string enc(base58::encoded_block_sizes[block.size()], base58::alphabet[0]);
    base58::encode_block(block.data(), block.size(), &enc[0]);
    ASSERT_EQ(enc, expected);

    std::string dec(block.size(), '\0');
    ASSERT_TRUE(base58::decode_block(enc.data(), enc.size(), &dec[0]));
    ASSERT_EQ(block, dec);
  }

  void do_test_decode_block_pos(const std::string& enc, const std::string& expected)
  {
    std::string data(base58::decoded_block_sizes::instance(enc.size()), '\0');
    ASSERT_TRUE(base58::decode_block(enc.data(), enc.size(), &data[0]));
    ASSERT_EQ(data, expected);
  }

  void do_test_decode_block_neg(const std::string& enc)
  {
    std::string data(base58::full_block_size, '\0');
    ASSERT_FALSE(base58::decode_block(enc.data(), enc.size(), &data[0]));
  }

  void do_test_encode(const std::string& data, const std::string& expected)
  {
    std::string enc = base58::encode(data);
    ASSERT_EQ(enc, expected);

    std::string dec;
    ASSERT_TRUE(base58::decode(enc, dec));
    ASSERT_EQ(dec, data);
  }

  void do_test_decode_pos(const std::string& enc, const std::string& expected)
  {
    std::string dec;
    ASSERT_TRUE(base58::decode(enc, dec));
    ASSERT_EQ(dec, expected);
  }

  void do_test_decode_neg(const std::string& enc)
  {
    std::string dec;
    ASSERT_FALSE(base58::decode(enc, dec));
  }

  void do_test_encode_decode_addr(uint64_t tag, const std::string& data, const std::string& expected)
  {
    std::string addr = base58::encode_addr(tag, data);
    ASSERT_EQ(addr, expected);

    uint64_t dec_tag;
    std::string dec_data;
    ASSERT_TRUE(base58::decode_addr(addr, dec_tag, dec_data));
    ASSERT_EQ(tag, dec_tag);
    ASSERT_EQ(data, dec_data);
  }
}

#define TEST_uint_8be_to_64(expected, str)                                                        \
  TEST(base58_uint_8be_to_64, handles_bytes_##expected)                                           \
  {                                                                                               \
    std::string data = str;                                                                       \
    do_test_uint_8be_to_64(expected, reinterpret_cast<const uint8_t*>(data.data()), data.size()); \
  }

TEST_uint_8be_to_64(0x0000000000000001, "\x1");
TEST_uint_8be_to_64(0x0000000000000102, "\x1\x2");
TEST_uint_8be_to_64(0x0000000000010203, "\x1\x2\x3");
TEST_uint_8be_to_64(0x0000000001020304, "\x1\x2\x3\x4");
TEST_uint_8be_to_64(0x0000000102030405, "\x1\x2\x3\x4\x5");
TEST_uint_8be_to_64(0x0000010203040506, "\x1\x2\x3\x4\x5\x6");
TEST_uint_8be_to_64(0x0001020304050607, "\x1\x2\x3\x4\x5\x6\x7");
TEST_uint_8be_to_64(0x0102030405060708, "\x1\x2\x3\x4\x5\x6\x7\x8");


#define TEST_uint_64_to_8be(num, expected_str)     \
  TEST(base58_uint_64_to_8be, handles_bytes_##num) \
  {                                                \
    do_test_uint_64_to_8be(num, expected_str);     \
  }

TEST_uint_64_to_8be(0x0000000000000001, "\x1");
TEST_uint_64_to_8be(0x0000000000000102, "\x1\x2");
TEST_uint_64_to_8be(0x0000000000010203, "\x1\x2\x3");
TEST_uint_64_to_8be(0x0000000001020304, "\x1\x2\x3\x4");
TEST_uint_64_to_8be(0x0000000102030405, "\x1\x2\x3\x4\x5");
TEST_uint_64_to_8be(0x0000010203040506, "\x1\x2\x3\x4\x5\x6");
TEST_uint_64_to_8be(0x0001020304050607, "\x1\x2\x3\x4\x5\x6\x7");
TEST_uint_64_to_8be(0x0102030405060708, "\x1\x2\x3\x4\x5\x6\x7\x8");

TEST(reverse_alphabet, is_correct)
{
  ASSERT_EQ(-1, base58::reverse_alphabet::instance(0));
  ASSERT_EQ(-1, base58::reverse_alphabet::instance(std::numeric_limits<char>::min()));
  ASSERT_EQ(-1, base58::reverse_alphabet::instance(std::numeric_limits<char>::max()));
  ASSERT_EQ(-1, base58::reverse_alphabet::instance('1' - 1));
  ASSERT_EQ(-1, base58::reverse_alphabet::instance('z' + 1));
  ASSERT_EQ(-1, base58::reverse_alphabet::instance('0'));
  ASSERT_EQ(-1, base58::reverse_alphabet::instance('I'));
  ASSERT_EQ(-1, base58::reverse_alphabet::instance('O'));
  ASSERT_EQ(-1, base58::reverse_alphabet::instance('l'));
  ASSERT_EQ(0,  base58::reverse_alphabet::instance('1'));
  ASSERT_EQ(8,  base58::reverse_alphabet::instance('9'));
  ASSERT_EQ(base58::alphabet_size - 1, base58::reverse_alphabet::instance('z'));
}


#define TEST_encode_block(block, expected)            \
  TEST(base58_encode_block, handles_##expected)       \
  {                                                   \
    do_test_encode_block(MAKE_STR(block), #expected); \
  }

TEST_encode_block("\x00",                             11);
TEST_encode_block("\x39",                             1z);
TEST_encode_block("\xFF",                             5Q);

TEST_encode_block("\x00\x00",                         111);
TEST_encode_block("\x00\x39",                         11z);
TEST_encode_block("\x01\x00",                         15R);
TEST_encode_block("\xFF\xFF",                         LUv);

TEST_encode_block("\x00\x00\x00",                     11111);
TEST_encode_block("\x00\x00\x39",                     1111z);
TEST_encode_block("\x01\x00\x00",                     11LUw);
TEST_encode_block("\xFF\xFF\xFF",                     2UzHL);

TEST_encode_block("\x00\x00\x00\x39",                 11111z);
TEST_encode_block("\xFF\xFF\xFF\xFF",                 7YXq9G);
TEST_encode_block("\x00\x00\x00\x00\x39",             111111z);
TEST_encode_block("\xFF\xFF\xFF\xFF\xFF",             VtB5VXc);
TEST_encode_block("\x00\x00\x00\x00\x00\x39",         11111111z);
TEST_encode_block("\xFF\xFF\xFF\xFF\xFF\xFF",         3CUsUpv9t);
TEST_encode_block("\x00\x00\x00\x00\x00\x00\x39",     111111111z);
TEST_encode_block("\xFF\xFF\xFF\xFF\xFF\xFF\xFF",     Ahg1opVcGW);
TEST_encode_block("\x00\x00\x00\x00\x00\x00\x00\x39", 1111111111z);
TEST_encode_block("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", jpXCZedGfVQ);

TEST_encode_block("\x00\x00\x00\x00\x00\x00\x00\x00", 11111111111);
TEST_encode_block("\x00\x00\x00\x00\x00\x00\x00\x01", 11111111112);
TEST_encode_block("\x00\x00\x00\x00\x00\x00\x00\x08", 11111111119);
TEST_encode_block("\x00\x00\x00\x00\x00\x00\x00\x09", 1111111111A);
TEST_encode_block("\x00\x00\x00\x00\x00\x00\x00\x3A", 11111111121);
TEST_encode_block("\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 1Ahg1opVcGW);
TEST_encode_block("\x06\x15\x60\x13\x76\x28\x79\xF7", 22222222222);
TEST_encode_block("\x05\xE0\x22\xBA\x37\x4B\x2A\x00", 1z111111111);


#define TEST_decode_block_pos(enc, expected)            \
  TEST(base58_decode_block, handles_pos_##enc)          \
  {                                                     \
    do_test_decode_block_pos(#enc, MAKE_STR(expected)); \
  }

#define TEST_decode_block_neg(enc)             \
  TEST(base58_decode_block, handles_neg_##enc) \
  {                                            \
    do_test_decode_block_neg(#enc);            \
  }

// 1-byte block
TEST_decode_block_neg(1);
TEST_decode_block_neg(z);
// 2-bytes block
TEST_decode_block_pos(11,          "\x00");
TEST_decode_block_pos(5Q,          "\xFF");
TEST_decode_block_neg(5R);
TEST_decode_block_neg(zz);
// 3-bytes block
TEST_decode_block_pos(111,         "\x00\x00");
TEST_decode_block_pos(LUv,         "\xFF\xFF");
TEST_decode_block_neg(LUw);
TEST_decode_block_neg(zzz);
// 4-bytes block
TEST_decode_block_neg(1111);
TEST_decode_block_neg(zzzz);
// 5-bytes block
TEST_decode_block_pos(11111,       "\x00\x00\x00");
TEST_decode_block_pos(2UzHL,       "\xFF\xFF\xFF");
TEST_decode_block_neg(2UzHM);
TEST_decode_block_neg(zzzzz);
// 6-bytes block
TEST_decode_block_pos(111111,      "\x00\x00\x00\x00");
TEST_decode_block_pos(7YXq9G,      "\xFF\xFF\xFF\xFF");
TEST_decode_block_neg(7YXq9H);
TEST_decode_block_neg(zzzzzz);
// 7-bytes block
TEST_decode_block_pos(1111111,     "\x00\x00\x00\x00\x00");
TEST_decode_block_pos(VtB5VXc,     "\xFF\xFF\xFF\xFF\xFF");
TEST_decode_block_neg(VtB5VXd);
TEST_decode_block_neg(zzzzzzz);
// 8-bytes block
TEST_decode_block_neg(11111111);
TEST_decode_block_neg(zzzzzzzz);
// 9-bytes block
TEST_decode_block_pos(111111111,   "\x00\x00\x00\x00\x00\x00");
TEST_decode_block_pos(3CUsUpv9t,   "\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_block_neg(3CUsUpv9u);
TEST_decode_block_neg(zzzzzzzzz);
// 10-bytes block
TEST_decode_block_pos(1111111111,  "\x00\x00\x00\x00\x00\x00\x00");
TEST_decode_block_pos(Ahg1opVcGW,  "\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_block_neg(Ahg1opVcGX);
TEST_decode_block_neg(zzzzzzzzzz);
// 11-bytes block
TEST_decode_block_pos(11111111111, "\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_decode_block_pos(jpXCZedGfVQ, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_block_neg(jpXCZedGfVR);
TEST_decode_block_neg(zzzzzzzzzzz);
// Invalid symbols
TEST_decode_block_neg(01111111111);
TEST_decode_block_neg(11111111110);
TEST_decode_block_neg(11111011111);
TEST_decode_block_neg(I1111111111);
TEST_decode_block_neg(O1111111111);
TEST_decode_block_neg(l1111111111);
TEST_decode_block_neg(_1111111111);


#define TEST_encode(expected, data)            \
  TEST(base58_encode, handles_##expected)      \
  {                                            \
    do_test_encode(MAKE_STR(data), #expected); \
  }

TEST_encode(11,                     "\x00");
TEST_encode(111,                    "\x00\x00");
TEST_encode(11111,                  "\x00\x00\x00");
TEST_encode(111111,                 "\x00\x00\x00\x00");
TEST_encode(1111111,                "\x00\x00\x00\x00\x00");
TEST_encode(111111111,              "\x00\x00\x00\x00\x00\x00");
TEST_encode(1111111111,             "\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(11111111111,            "\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(1111111111111,          "\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(11111111111111,         "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(1111111111111111,       "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(11111111111111111,      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(111111111111111111,     "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(11111111111111111111,   "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(111111111111111111111,  "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(1111111111111111111111, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode(22222222222VtB5VXc,     "\x06\x15\x60\x13\x76\x28\x79\xF7\xFF\xFF\xFF\xFF\xFF");


#define TEST_decode_pos(enc, expected)            \
  TEST(base58_decode_pos, handles_pos_##enc)      \
  {                                               \
    do_test_decode_pos(#enc, MAKE_STR(expected)); \
  }

#define TEST_decode_neg(enc)                 \
  TEST(base58_decode_neg, handles_neg_##enc) \
  {                                          \
    do_test_decode_neg(#enc);                \
  }

TEST_decode_pos(,                       "");
TEST_decode_pos(5Q,                     "\xFF");
TEST_decode_pos(LUv,                    "\xFF\xFF");
TEST_decode_pos(2UzHL,                  "\xFF\xFF\xFF");
TEST_decode_pos(7YXq9G,                 "\xFF\xFF\xFF\xFF");
TEST_decode_pos(VtB5VXc,                "\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(3CUsUpv9t,              "\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(Ahg1opVcGW,             "\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQ,            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQ5Q,          "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQLUv,         "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQ2UzHL,       "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQ7YXq9G,      "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQVtB5VXc,     "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQ3CUsUpv9t,   "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQAhg1opVcGW,  "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_decode_pos(jpXCZedGfVQjpXCZedGfVQ, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
// Invalid length
TEST_decode_neg(1);
TEST_decode_neg(z);
TEST_decode_neg(1111);
TEST_decode_neg(zzzz);
TEST_decode_neg(11111111);
TEST_decode_neg(zzzzzzzz);
TEST_decode_neg(123456789AB1);
TEST_decode_neg(123456789ABz);
TEST_decode_neg(123456789AB1111);
TEST_decode_neg(123456789ABzzzz);
TEST_decode_neg(123456789AB11111111);
TEST_decode_neg(123456789ABzzzzzzzz);
// Overflow
TEST_decode_neg(5R);
TEST_decode_neg(zz);
TEST_decode_neg(LUw);
TEST_decode_neg(zzz);
TEST_decode_neg(2UzHM);
TEST_decode_neg(zzzzz);
TEST_decode_neg(7YXq9H);
TEST_decode_neg(zzzzzz);
TEST_decode_neg(VtB5VXd);
TEST_decode_neg(zzzzzzz);
TEST_decode_neg(3CUsUpv9u);
TEST_decode_neg(zzzzzzzzz);
TEST_decode_neg(Ahg1opVcGX);
TEST_decode_neg(zzzzzzzzzz);
TEST_decode_neg(jpXCZedGfVR);
TEST_decode_neg(zzzzzzzzzzz);
TEST_decode_neg(123456789AB5R);
TEST_decode_neg(123456789ABzz);
TEST_decode_neg(123456789ABLUw);
TEST_decode_neg(123456789ABzzz);
TEST_decode_neg(123456789AB2UzHM);
TEST_decode_neg(123456789ABzzzzz);
TEST_decode_neg(123456789AB7YXq9H);
TEST_decode_neg(123456789ABzzzzzz);
TEST_decode_neg(123456789ABVtB5VXd);
TEST_decode_neg(123456789ABzzzzzzz);
TEST_decode_neg(123456789AB3CUsUpv9u);
TEST_decode_neg(123456789ABzzzzzzzzz);
TEST_decode_neg(123456789ABAhg1opVcGX);
TEST_decode_neg(123456789ABzzzzzzzzzz);
TEST_decode_neg(123456789ABjpXCZedGfVR);
TEST_decode_neg(123456789ABzzzzzzzzzzz);
TEST_decode_neg(zzzzzzzzzzz11);
// Invalid symbols
TEST_decode_neg(10);
TEST_decode_neg(11I);
TEST_decode_neg(11O11);
TEST_decode_neg(11l111);
TEST_decode_neg(11_11111111);
TEST_decode_neg(1101111111111);
TEST_decode_neg(11I11111111111111);
TEST_decode_neg(11O1111111111111111111);
TEST_decode_neg(1111111111110);
TEST_decode_neg(111111111111l1111);
TEST_decode_neg(111111111111_111111111);


#define TEST_encode_decode_addr(addr, tag, data)                      \
  TEST(base58_encode_decode_addr, handles_##addr)                     \
  {                                                                   \
    do_test_encode_decode_addr(UINT64_C(tag), MAKE_STR(data), #addr); \
  }

TEST_encode_decode_addr(21D35quxec71111111111111111111111111111111111111111111111111111111111111111111111111111116Q5tCH, 6,
                        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode_decode_addr(2Aui6ejTFscjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQVqegMoV, 6,
                        "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                        "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_encode_decode_addr(1119XrkPuSmLzdHXgVgrZKjepg5hZAxffLzdHXgVgrZKjepg5hZAxffLzdHXgVgrZKjepg5hZAxffLzdHXgVgrZKVphZRvn, 0,
                        "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF"
                        "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF");
TEST_encode_decode_addr(111111111111111111111111111111111111111111111111111111111111111111111111111111111111111115TXfiA, 0,
                        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode_decode_addr(PuT7GAdgbA83qvSEivPLYo11111111111111111111111111111111111111111111111111111111111111111111111111111169tWrH, 0x1122334455667788,
                        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
TEST_encode_decode_addr(PuT7GAdgbA841d7FXjswpJjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQjpXCZedGfVQVq4LL1v, 0x1122334455667788,
                        "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                        "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF");
TEST_encode_decode_addr(PuT7GAdgbA819VwdWVDP,            0x1122334455667788, "\x11");
TEST_encode_decode_addr(PuT7GAdgbA81efAfdCjPg,           0x1122334455667788, "\x22\x22");
TEST_encode_decode_addr(PuT7GAdgbA83sryEt3YC8Q,          0x1122334455667788, "\x33\x33\x33");
TEST_encode_decode_addr(PuT7GAdgbA83tWUuc54PFP3b,        0x1122334455667788, "\x44\x44\x44\x44");
TEST_encode_decode_addr(PuT7GAdgbA83u9zaKrtRKZ1J6,       0x1122334455667788, "\x55\x55\x55\x55\x55");
TEST_encode_decode_addr(PuT7GAdgbA83uoWF3eanGG1aRoG,     0x1122334455667788, "\x66\x66\x66\x66\x66\x66");
TEST_encode_decode_addr(PuT7GAdgbA83vT1umSHMYJ4oNVdu,    0x1122334455667788, "\x77\x77\x77\x77\x77\x77\x77");
TEST_encode_decode_addr(PuT7GAdgbA83w6XaVDyvpoGQBEWbB,   0x1122334455667788, "\x88\x88\x88\x88\x88\x88\x88\x88");
TEST_encode_decode_addr(PuT7GAdgbA83wk3FD1gW7J2KVGofA1r, 0x1122334455667788, "\x99\x99\x99\x99\x99\x99\x99\x99\x99");
TEST_encode_decode_addr(15p2yAV,                         0, "");
TEST_encode_decode_addr(FNQ3D6A,                         0x7F, "");
TEST_encode_decode_addr(26k9QWweu,                       0x80, "");
TEST_encode_decode_addr(3BzAD7n3y,                       0xFF, "");
TEST_encode_decode_addr(11efCaY6UjG7JrxuB,               0, "\x11\x22\x33\x44\x55\x66\x77");
TEST_encode_decode_addr(21rhHRT48LN4PriP9,               6, "\x11\x22\x33\x44\x55\x66\x77");


#define TEST_decode_addr_neg(addr, test_name)                     \
  TEST(base58_decode_addr_neg, test_name)                         \
  {                                                               \
    uint64_t tag;                                                 \
    std::string data;                                             \
    ASSERT_FALSE(base58::decode_addr(MAKE_STR(addr), tag, data)); \
  }

TEST_decode_addr_neg("zuT7GAdgbA819VwdWVDP", decode_fails_due_overflow);
TEST_decode_addr_neg("0uT7GAdgbA819VwdWVDP", decode_fails_due_invalid_char_0);
TEST_decode_addr_neg("IuT7GAdgbA819VwdWVDP", decode_fails_due_invalid_char_I);
TEST_decode_addr_neg("OuT7GAdgbA819VwdWVDP", decode_fails_due_invalid_char_O);
TEST_decode_addr_neg("luT7GAdgbA819VwdWVDP", decode_fails_due_invalid_char_l);
TEST_decode_addr_neg("\0uT7GAdgbA819VwdWVDP", decode_fails_due_invalid_char_00);
TEST_decode_addr_neg("PuT7GAdgbA819VwdWVD", decode_fails_due_invalid_lenght);
TEST_decode_addr_neg("11efCaY6UjG7JrxuC", handles_invalid_checksum);
TEST_decode_addr_neg("jerj2e4mESo", handles_non_correct_tag); // "jerj2e4mESo" == "\xFF\x00\xFF\xFF\x5A\xD9\xF1\x1C"
TEST_decode_addr_neg("1", decode_fails_due_invalid_block_len_0);
TEST_decode_addr_neg("1111", decode_fails_due_invalid_block_len_1);
TEST_decode_addr_neg("11", decode_fails_due_address_too_short_0);
TEST_decode_addr_neg("111", decode_fails_due_address_too_short_1);
TEST_decode_addr_neg("11111", decode_fails_due_address_too_short_2);
TEST_decode_addr_neg("111111", decode_fails_due_address_too_short_3);
TEST_decode_addr_neg("999999", decode_fails_due_address_too_short_4);
TEST_decode_addr_neg("ZZZZZZ", decode_fails_due_address_too_short_5);

namespace
{
  auto test_serialized_dilithium_keys = MAKE_STR
      ("\x39\x0e\xb8\x40\x39\x18\xd3\x82\xdc\x32\x6e\x45\x66\xf0\x49\x40\x8d"
       "\x35\xf0\xce\x39\x44\x86\x8c\x55\xdf\xce\x11\x0a\xd8\xae\xc9\xe3\x15"
       "\x59\xe0\xad\xf0\xab\xd3\x74\xa8\x80\xba\x84\xb2\xdd\x97\x52\x29\x94"
       "\x47\x3d\x8f\x8f\x11\x0f\x39\x84\xa7\xa0\xb9\x72\x29\x91\x26\x11\xbe"
       "\xf1\x6b\xaf\x33\x85\xcb\x17\xdf\x1d\x51\xe7\x38\x56\x5f\xc4\x2f\x89"
       "\x84\x03\x70\xdf\x48\xa3\x09\x79\x76\xac\x6a\x22\xee\xd5\xcb\xbc\x34"
       "\xdf\x86\xde\x4e\xe2\x04\xb7\xc5\xd7\xd1\x87\xb5\xe2\xd9\xdd\x62\xf6"
       "\x20\xf9\x8e\x3f\xfb\xb3\x55\x48\x46\xed\x1e\xc3\xa2\x57\xd2\x1d\x2d"
       "\x04\x23\xe2\x48\xc6\x6b\x4b\x72\x92\xbd\xfa\x5c\xe5\x99\xc8\xa4\x4c"
       "\x99\xd4\x04\xe8\xd7\xcd\x32\xd2\xec\xc9\xe5\xfb\xde\xd0\xe2\x13\xb3"
       "\x28\xef\xed\x0e\x9e\x55\xd8\x49\x8b\x3e\x46\x3c\xb6\x50\x12\x66\xbc"
       "\x42\x48\x2b\x52\xaf\x2c\xfa\x65\x29\x26\xb4\x79\xcd\xa0\x06\xaa\xc9"
       "\x30\x62\xe3\xa6\x1c\xbf\x37\x61\xe6\xe0\x86\xb5\xda\xa3\x1b\x5b\x6f"
       "\x0d\xaa\x62\xbe\x99\xec\xb6\xe1\xa2\xa2\xbf\xc0\x7e\x12\x01\x42\x77"
       "\x14\xa9\xcc\x7e\x63\x6a\x61\x35\x35\xd6\x0c\x8a\xe7\x3a\x4e\x4f\xef"
       "\xf0\xea\x63\xe1\x59\x7c\x2b\x8b\x55\x82\x36\xc0\x54\x00\xf3\xf4\x7d"
       "\xb0\xca\xcf\xc4\xba\x91\x64\xc1\xf1\xd9\x73\x34\xda\xdb\x15\x62\x89"
       "\x4a\x13\x77\x45\xa8\x7c\x92\xe4\x0e\x89\x27\x27\xf2\xfa\xd5\xed\x70"
       "\x12\x9d\x1c\x45\x25\x4f\x55\x3e\x66\x24\x13\x6b\xe6\x9c\x7c\x11\x22"
       "\x4c\x03\xfd\x9a\xc1\xae\x82\xbc\x84\x85\x6a\x54\xcb\xc1\x70\x71\xa1"
       "\xda\x83\x15\x76\x65\xc0\x22\xe1\x4f\x25\x85\x0d\xd7\x94\xfe\xcf\x74"
       "\x07\xcf\x57\xb4\xf9\x6f\xc6\x14\x95\x01\x16\x3f\x22\xb8\x41\x3d\xa3"
       "\x4b\x88\xf9\x69\xdc\x10\xb3\x9c\x15\xc3\x3c\x55\xcc\xbc\xa7\xc2\x3f"
       "\x73\x10\x4f\x38\x44\x12\x58\x10\xd0\xfc\x92\xb7\x83\x6d\x36\xfc\x95"
       "\x7b\xa9\x66\xe3\x3d\x02\x8e\x2d\xce\x26\xc5\xc8\x86\x3c\x0c\xc9\x69"
       "\x36\x6d\x7d\x11\x7e\x98\x14\xc8\xe9\x96\x9d\x7f\x01\x52\x33\xe3\x2c"
       "\x5c\x5b\x92\x7f\x07\x66\x1c\x06\x6c\x71\xe3\x75\x90\x2b\x19\x29\x68"
       "\x8b\xb3\x47\x50\x0d\x6f\xf7\x49\x33\x1b\x58\xa1\x15\xa9\x8b\xd9\x7c"
       "\xf3\x36\xe0\xce\x78\xaa\xb9\xe7\xfd\x9d\x2c\xc1\x53\x82\x9e\x26\x67"
       "\xd7\xaa\x22\x4e\x2b\x05\x87\x38\x95\x4f\x48\x29\xb7\x84\x7b\xd7\x7b"
       "\x9f\x43\xef\x79\xea\xfd\xd1\x61\xb9\xc3\xf6\xd9\x52\x0a\x36\x66\x3b"
       "\xac\x86\x6a\x88\x0e\x1d\xef\x2a\x86\x77\x6c\x7e\x14\x73\x51\xb7\x7a"
       "\xa1\x6d\x44\x6a\xdb\x92\x0e\xde\xba\xc3\xd3\x6c\x26\x8b\xeb\x9f\x90"
       "\x07\xdd\x40\x66\x05\x1a\x57\x79\xf1\x34\x23\x8e\xd8\x54\xb0\x86\x04"
       "\x4a\xfc\x5e\xb9\xc7\x96\x59\x5b\x39\xa7\x25\xf8\x97\xbc\x0c\x6a\xf0"
       "\x3d\x2e\x8a\x4b\xc6\xb8\xfc\x3e\x36\x24\x6f\x7d\xae\xb4\xda\xe7\x7f"
       "\xb3\x4f\xa3\x0b\x5c\x35\x91\xbe\xbf\x64\x95\xeb\xf5\xb0\x0c\x0b\x25"
       "\x2c\x31\x29\x88\xfb\x74\xf2\xba\xe3\x3e\xd2\x0a\xab\x55\x80\xe2\xd4"
       "\xdc\xfc\xb3\x04\x49\xe6\x05\xc5\x96\x70\xcf\xe9\x06\x90\x1b\x35\x69"
       "\x05\x90\x4d\x2e\x6d\x8c\xc0\xf2\x34\xea\x27\xbf\xfe\x2c\x8f\xd4\xe1"
       "\xa5\xc8\x46\xb4\x23\x0e\xd7\x4a\xf2\x77\x27\xd2\x2d\xf6\x86\x08\xbc"
       "\x3c\xe0\x42\xef\x60\x6a\x44\x5b\xf0\x6b\x69\xc2\xfc\xda\xd1\x34\x26"
       "\x27\xf0\xf7\xb5\xa4\xcc\xe7\x7f\x0e\x6f\x61\x5a\xe3\xbd\x64\x8f\x77"
       "\x28\xf8\x57\x16\x33\xf6\x7b\x85\x9b\x87\xf3\x71\xb9\x74\xac\xf6\xbb"
       "\x8a\x5e\x14\xa8\x87\xa0\xeb\xcc\x9e\x2d\xde\x0a\x69\x5e\xe2\x3d\x29"
       "\xbf\x82\x9a\xc2\xcb\x4c\xaa\x3d\x65\x47\xc0\x3b\x11\xef\xab\xc7\x35"
       "\x39\xaa\x46\x90\xd4\x47\x2b\xcf\xe0\x56\x14\x66\x53\x65\x94\xe4\xc5"
       "\x87\x55\xc1\x70\x79\x6e\x76\xad\x76\xa1\xf7\xea\x2d\x92\x04\x93\x9f"
       "\x0c\x59\x98\xe1\x03\x3d\x6a\xb6\xb1\xbf\x3d\x9c\xc7\xcc\x2c\xc5\x96"
       "\x2e\xff\xee\xe6\xb8\x01\x50\xb6\x69\x91\xcb\xed\xc5\x64\x88\x58\x53"
       "\x17\x6f\x60\x22\x8b\xb0\x9a\x6d\x49\x81\x6c\x16\x97\x5a\x9b\xdf\x2f"
       "\x8d\xb3\xfb\x21\x31\x64\x2a\xe8\xaa\x27\xd7\x96\x16\x31\x6d\x2a\xa7"
       "\xa1\xb9\x4d\x0a\x12\xf7\xe3\x32\x41\x11\x07\x13\x1f\x56\xfe\xc4\x48"
       "\x39\xed\x45\x91\x21\x5c\x7b\xa7\x17\x40\x21\x81\xec\x36\x5c\x0b\x93"
       "\x49\xa7\xae\x1b\x58\x65\xa1\x1d\xf5\x38\x14\x30\xea\x2c\xad\x7b\x3d"
       "\xd2\x73\x8e\x45\xc6\x72\xbb\xcf\xb9\xdb\x59\xab\x74\x32\xb1\x39\x49"
       "\x70\x3b\x6f\x5c\x7a\x95\x0d\x13\xbe\x9f\x68\xa0\x33\x25\x70\x2e\xdf"
       "\x08\xb2\x57\x81\x35\xc9\xb4\x41\x3b\xc2\xa8\x3e\x62\x31\x80\x88\x65"
       "\xc7\x0b\xdd\x6a\x4b\x0f\xfd\x84\x2f\x70\xdf\x4d\xaa\xee\x60\x2f\x72"
       "\x30\xc3\xb8\x0a\x56\x07\x08\x5a\x29\xd9\x1e\x56\x3b\xb6\x58\x9b\xa4"
       "\x32\xd6\x40\xef\x66\x28\x9e\x0e\xb1\xdd\x02\x18\x23\xc6\xc8\xf5\x98"
       "\x12\x96\x76\x03\x86\x01\xee\x55\x62\x6f\x63\x12\xd6\x57\x07\xcc\x37"
       "\x56\x55\x1e\xf2\x20\xe5\x3d\xec\x61\xd8\x69\x76\x8b\x0d\x38\x87\xc0"
       "\x0a\x88\x50\x2c\xca\x0e\x39\xbe\x30\xbd\xad\x35\x2e\x35\x77\x70\x45"
       "\x07\x4c\xba\xf1\xb3\xc8\x12\xe4\x6f\xa3\x6a\xc1\xc1\x5b\x53\x16\xe2"
       "\x69\xc4\x33\xa9\xbc\x9c\xfd\xf8\xcf\x83\x06\xc7\x91\x81\x5e\x4d\x09"
       "\x68\x41\x85\x38\xa2\xfd\xe3\x33\x40\x5f\x6b\xc8\x32\x7d\x89\x36\x7f"
       "\x23\xd4\x64\x1a\x46\xd3\x93\xd4\x21\x7d\x0c\x07\x7d\x25\x10\x72\x05"
       "\xa1\x78\x46\x4a\xe1\x58\x93\xa6\xaf\x3a\x9f\x97\x48\x88\x7b\x4d\xfb"
       "\x22\x05\x0c\x9f\xe9\xe2\xa4\x09\x44\x32\xa3\xb5\xc9\x7d\x88\xc9\x87"
       "\x37\xc1\x83\xf8\xcf\xbd\xea\x47\x11\xbe\xb3\x26\xf7\xdd\xe2\x31\x00"
       "\xf8\x76\xc7\xaa\x4b\x1e\x3c\x12\x8e\x87\x43\x4f\x4a\x11\xfe\x64\x57"
       "\x97\x6d\x55\xff\x31\x28\x86\xe7\x7e\xb6\x3f\x6a\x32\x32\xf0\xfb\xb3"
       "\x87\x39\xfa\x14\x0a\xa1\x58\x4c\xb8\xfe\x7e\xe4\x40\x02\x14\x1a\xf3"
       "\x94\x6f\xdd\x65\x87\x4f\x78\x7d\x2b\x95\x70\x54\xc8\xeb\x7b\xdd\x79"
       "\x60\xc1\x78\xdd\x95\x53\x17\xf9\x30\xfb\xc3\x47\x37\xdb\xde\x29\x8b"
       "\xd9\xb1\x6d\x19\x9d\xe6\x48\x28\xe0\x77\x48\x51\xaa\x94\x46\x53\xe9"
       "\xb9\x3b\x1c\x66\x15\x01\xd6\xfb\xa7\xc2\xe4\x7a\x2a\xb3\xd3\x69\x8f"
       "\x6f\x34\xb6\x54\x93\x88\x15\x61\x4c\x49\x1a\x13\x63\xc3\x82\x3e\x75"
       "\x77\xd3\x04\x0b\xa5\x99\x05\x65\x02\xad\xab\xc5\x4c\xb4\xa0\xb4\x3f"
       "\x98\x8f\x56\xb8\x87\x1f\x6f\xfe\xdd\xf6\x69\xec\xf0\xd7\xff\xb6\x47"
       "\xae\x79\x1c\x52\x7f\x7c\x5c\x4f\x7c\x09\x46\x0b\x6a\xff\x64\xde\xcc"
       "\x73\x31\x8c\xa9\x9e\x1b\x57\xe3\x92\x4e\x3a\x0b\x1e\x01\x6a\x63\x95"
       "\xa7\x16\xec\xec\x49\x19\xd0\x1c\x7f\x2c\xc7\x64\xb0\xaa\xba\xe1\x1d"
       "\xdc\x33\x09\x93\xbc\xe9\xfb\x87\x8d\x43\xc0\x54\x05\x1b\xf6\xef\x43"
       "\xd1\xcf\xc6\x71\xc1\x5c\xd0\xa1\xab\x9a\xb5\xfe\xb3\xdb\x94\x50\xd0"
       "\x62\x04\xc9\x1d\x57\x9b\x07\xee\x6b\xfb\x3d\x73\x0f\x99\x51\x92\x50"
       "\xb7\x86\x93\x85\x3a\x5a\x6c\xd8\x4c\x8d\x8c\x46\xa5\xf3\x24\x7c\x8f"
       "\x68\xb7\x65\x84\xd7\xcd\xf1\x90\x97\x91\x68\xab\xca\x85\xdf\xa3\x95"
       "\x97\x0a\x22\x83\x06\x49\x9a\x23\xd3\x49\xbb\xd7\xc2\x8a\x27\xa7\x5e"
       "\x40\x81\x74\x84\x20\x74\xe4\xd0\x04\xeb\x9c\xe9\x68\x25\x0d\xf6\xd5"
       "\x8f\xfb\x57\x69\x93\xec\xb9\xb5\xe0\x6b\xb3\x54\xd4\x82\x28\xf1\x88"
       "\x9e\x61\xc8\x4f\x02\xdb\x5f\x3e\x1e\x1b\xa4\xbe\xd3\xaf\xd5\x41\x90"
       "\x23\x15\x8d\x7e\x8b\x0d\xba\xdd\xa1\x5e\x07\x48\xf6\x0c\x40\xce\xf4"
       "\x23\x5f\x26\x58\x5d\xcb\x05\x04\x73\x2c\x9b\x89\x47\xf8\x72\xbb\x22"
       "\xde\x83\x8f\x9e\xe5\x87\x2f\xf8\xec\x58\x30\xb4\xdc\xe6\x56\xbc\xd3"
       "\xa2\xcd\xa2\x0b\xe4\xc4\x3c\x04\xe7\xb8\x33\x6d\xdd\xe8\x56\xa9\x69"
       "\x7d\xa2\xa1\x94\x70\xab\x35\xeb\xdd\xdb\xee\xcd\x94\x12\x94\x9c\x5a"
       "\xec\xf0\x73\xa5\x25\xb4\xc4\x89\xea\xbe\x62\xcd\xe0\xc6\xda\x5f\xf1"
       "\xe8\xc6\xce\x62\x60\x99\x8c\xa6\xc2\xc9\x14\x42\x5b\x58\x69\x1c\xe6"
       "\xb1\x37\x25\xbe\x84\x5c\x67\x54\x48\x52\xb1\xd8\xbc\x40\x37\x58\x36"
       "\xf1\x80\x2d\xd0\x12\x6c\xb7\xa8\xc2\x5e\xb9\xa8\x5e\xf2\x26\x42\x54"
       "\xb3\x99\xbd\xe1\xc0\x6d\x01\x15\x26\x56\x6a\xd4\x10\x7f\x46\x5f\xf1"
       "\xa1\x35\xb1\x7c\xdf\x75\x83\x22\xab\xfb\x41\xfb\x20\x07\xb1\x20\x26"
       "\xfe\xbe\x18\x05\x45\xc2\xbb\x8e\xd8\xc7\x8a\xf0\x1a\x8a\x25\xb2\xce"
       "\x70\xbf\x6e\x4e\x93\x76\x48");

  // Dilithium MAINNET
  auto test_keys_addr_dilithium_str =
      "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"
      "fd";

}

TEST(get_account_address_as_str_dilithium, works_correctly)
{
    cryptonote::account_public_address addr{};
    ASSERT_TRUE(serialization::parse_binary(test_serialized_dilithium_keys, addr));
    std::string addr_str = cryptonote::get_account_address_as_str(cryptonote::MAINNET, false, addr);
    ASSERT_EQ(addr_str, test_keys_addr_dilithium_str);
}

TEST(get_account_address_from_str_dilithium, handles_valid_address)
{
    cryptonote::address_parse_info info{};
    ASSERT_TRUE(cryptonote::get_account_address_from_str(info, cryptonote::MAINNET, test_keys_addr_dilithium_str));

    std::string blob;
    ASSERT_TRUE(serialization::dump_binary(info.address, blob));
    ASSERT_EQ(blob, test_serialized_dilithium_keys);
}

TEST(get_account_address_from_str, fails_on_invalid_address_format)
{
  cryptonote::address_parse_info info{};
  std::string addr_str = test_keys_addr_dilithium_str;
  addr_str[0] = '0';

  ASSERT_FALSE(cryptonote::get_account_address_from_str(info, cryptonote::MAINNET, addr_str));
}

TEST(get_account_address_from_str, fails_on_invalid_address_prefix)
{
  std::string addr_str = base58::encode_addr(0, test_serialized_dilithium_keys);

  cryptonote::address_parse_info info{};
  ASSERT_FALSE(cryptonote::get_account_address_from_str(info, cryptonote::MAINNET, addr_str));
}

TEST(get_account_address_from_str, fails_on_invalid_address_content)
{
  std::string addr_str = base58::encode_addr(config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX, test_serialized_dilithium_keys.substr(1));

  cryptonote::address_parse_info info{};
  ASSERT_FALSE(cryptonote::get_account_address_from_str(info, cryptonote::MAINNET, addr_str));
}

// Dilithium - this test will fail because we removed the means of checking the key.
// TODO: Removing 2 tests, there is no way to do check spend/view key.
/*TEST(get_account_address_from_str, fails_on_invalid_address_spend_key)
{
  std::string serialized_keys_copy = test_serialized_dilithium_keys;
  serialized_keys_copy[0] = '\0';
  std::string addr_str = base58::encode_addr(config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX, serialized_keys_copy);

  cryptonote::address_parse_info info{};
  ASSERT_FALSE(cryptonote::get_account_address_from_str(info, cryptonote::MAINNET, addr_str));
}*/

// Dilithium - this test will fail because we removed the means of checking the key.
/*TEST(get_account_address_from_str, fails_on_invalid_address_view_key)
{
  std::string serialized_keys_copy = test_serialized_dilithium_keys;
  serialized_keys_copy.back() = '\x01';
  std::string addr_str = base58::encode_addr(config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX, serialized_keys_copy);

  cryptonote::address_parse_info info{};
  ASSERT_FALSE(cryptonote::get_account_address_from_str(info, cryptonote::MAINNET, addr_str));
}*/

// Copyright (c) 2014-2019, The Monero Project
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

#include <unordered_set>
#include <random>
#include "include_base_utils.h"
#include "string_tools.h"
using namespace epee;

#include "common/apply_permutation.h"
#include "cryptonote_tx_utils.h"
#include "cryptonote_config.h"
#include "cryptonote_basic/miner.h"
#include "cryptonote_basic/tx_extra.h"
#include "crypto/crypto.h"
#include "crypto/hash.h"
#include "ringct/rctSigs.h"
#include "multisig/multisig.h"



using namespace crypto;

namespace cryptonote
{
    extern "C" {
    #include "dilithium/ref/rng.h"
    }
  //---------------------------------------------------------------
  void classify_addresses(const std::vector<tx_destination_entry> &destinations, const boost::optional<cryptonote::account_public_address>& change_addr, size_t &num_stdaddresses, size_t &num_subaddresses, account_public_address &single_dest_subaddress)
  {
    num_stdaddresses = 0;
    num_subaddresses = 0;
    std::unordered_set<cryptonote::account_public_address> unique_dst_addresses;
    for(const tx_destination_entry& dst_entr: destinations)
    {
      if (change_addr && dst_entr.addr == change_addr)
        continue;
      if (unique_dst_addresses.count(dst_entr.addr) == 0)
      {
        unique_dst_addresses.insert(dst_entr.addr);
        if (dst_entr.is_subaddress)
        {
          ++num_subaddresses;
          single_dest_subaddress = dst_entr.addr;
        }
        else
        {
          ++num_stdaddresses;
        }
      }
    }
    LOG_PRINT_L2("destinations include " << num_stdaddresses << " standard addresses and " << num_subaddresses << " subaddresses");
  }
  //---------------------------------------------------------------
  bool construct_miner_tx(size_t height, size_t median_weight, uint64_t already_generated_coins, size_t current_block_weight, uint64_t fee, const account_public_address &miner_address, transaction& tx, const blobdata& extra_nonce, size_t max_outs, uint8_t hard_fork_version) {
    tx.vin.clear();
    tx.vout.clear();
    tx.extra.clear();

    keypair txkey = keypair::generate(hw::get_device("default"));
    add_tx_pub_key_to_extra(tx, txkey.pub);
    if(!extra_nonce.empty())
      if(!add_extra_nonce_to_tx_extra(tx.extra, extra_nonce))
        return false;
    if (!sort_tx_extra(tx.extra, tx.extra))
      return false;

    txin_gen in{};
    in.height = height;

    uint64_t block_reward;
    if(!get_block_reward(median_weight, current_block_weight, already_generated_coins, block_reward, hard_fork_version))
    {
      LOG_PRINT_L0("Block is too big");
      return false;
    }

#if defined(DEBUG_CREATE_BLOCK_TEMPLATE)
    LOG_PRINT_L1("Creating block template: reward " << block_reward <<
      ", fee " << fee);
#endif
    block_reward += fee;

    // from hard fork 2, we cut out the low significant digits. This makes the tx smaller, and
    // keeps the paid amount almost the same. The unpaid remainder gets pushed back to the
    // emission schedule
    // from hard fork 4, we use a single "dusty" output. This makes the tx even smaller,
    // and avoids the quantization. These outputs will be added as rct outputs with identity
    // masks, to they can be used as rct inputs.

    // Disable RCT impelementation from AEON
    if (hard_fork_version >= 2 && hard_fork_version < HF_VERSION_ALLOW_RCT) {
      block_reward = block_reward - block_reward % ::config::BASE_REWARD_CLAMP_THRESHOLD;
    }

    std::vector<uint64_t> out_amounts;
    decompose_amount_into_digits(block_reward, hard_fork_version >= 2 ? 0 : ::config::DEFAULT_DUST_THRESHOLD,
      [&out_amounts](uint64_t a_chunk) { out_amounts.push_back(a_chunk); },
      [&out_amounts](uint64_t a_dust) { out_amounts.push_back(a_dust); });

    CHECK_AND_ASSERT_MES(1 <= max_outs, false, "max_out must be non-zero");
    // Disable RCT impelementation from AEON
    if (height == 0 || hard_fork_version >= HF_VERSION_ALLOW_RCT)
    {
      // the genesis block was not decomposed, for unknown reasons
      while (max_outs < out_amounts.size())
      {
        //out_amounts[out_amounts.size() - 2] += out_amounts.back();
        //out_amounts.resize(out_amounts.size() - 1);
        out_amounts[1] += out_amounts[0];
        for (size_t n = 1; n < out_amounts.size(); ++n)
          out_amounts[n - 1] = out_amounts[n];
        out_amounts.pop_back();
      }
    }
    else
    {
      CHECK_AND_ASSERT_MES(max_outs >= out_amounts.size(), false, "max_out exceeded");
    }

    uint64_t summary_amounts = 0;
    for (size_t no = 0; no < out_amounts.size(); no++)
    {
      crypto::key_derivation derivation = AUTO_VAL_INIT(derivation);;
      crypto::public_key out_eph_public_key = AUTO_VAL_INIT(out_eph_public_key);
      bool r = crypto::generate_key_derivation(miner_address.m_view_public_key, txkey.sec, derivation);
      CHECK_AND_ASSERT_MES(r, false, "while creating outs: failed to generate_key_derivation(" << miner_address.m_view_public_key << " ---- " << txkey.sec << ")");

      r = crypto::derive_public_key(derivation, no, miner_address.m_spend_public_key, out_eph_public_key);
      CHECK_AND_ASSERT_MES(r, false, "while creating outs: failed to derive_public_key(" << derivation << " ---- " << no << " ---- "<< miner_address.m_spend_public_key << ")");

      txout_to_key tk;
      tk.key = out_eph_public_key;

      tx_out out;
      summary_amounts += out.amount = out_amounts[no];
      out.target = tk;
      dilithium_randombytes((unsigned char *)&out.random, 32U);
      tx.vout.push_back(out);
    }

    CHECK_AND_ASSERT_MES(summary_amounts == block_reward, false, "Failed to construct miner tx, summary_amounts = " << summary_amounts << " not equal block_reward = " << block_reward);

    // Disable RCT impelementation from AEON
    if (hard_fork_version >= HF_VERSION_ALLOW_RCT)
      tx.version = 2;
    else
      tx.version = 1;

    //lock
    tx.unlock_time = height + CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW;
    tx.vin.push_back(in);

    tx.invalidate_hashes();

    //LOG_PRINT("MINER_TX generated ok, block_reward=" << print_money(block_reward) << "("  << print_money(block_reward - fee) << "+" << print_money(fee)
    //  << "), current_block_size=" << current_block_size << ", already_generated_coins=" << already_generated_coins << ", tx_id=" << get_transaction_hash(tx), LOG_LEVEL_2);
    return true;
  }
  //---------------------------------------------------------------
  crypto::public_key get_destination_view_key_pub(const std::vector<tx_destination_entry> &destinations, const boost::optional<cryptonote::account_public_address>& change_addr)
  {
    account_public_address addr = {null_pkey, null_pkey};
    size_t count = 0;
    for (const auto &i : destinations)
    {
      if (i.amount == 0)
        continue;
      if (change_addr && i.addr == *change_addr)
        continue;
      if (i.addr == addr)
        continue;
      if (count > 0)
        return null_pkey;
      addr = i.addr;
      ++count;
    }
    if (count == 0 && change_addr)
      return change_addr->m_view_public_key;
    return addr.m_view_public_key;
  }
  //---------------------------------------------------------------
  bool construct_tx_with_tx_key(const account_keys& sender_account_keys, const std::unordered_map<crypto::public_key, subaddress_index>& subaddresses, std::vector<tx_source_entry>& sources, std::vector<tx_destination_entry>& destinations, const boost::optional<cryptonote::account_public_address>& change_addr, std::vector<uint8_t> extra,
          transaction& tx, uint64_t unlock_time, const crypto::secret_key &tx_key, const std::vector<crypto::secret_key> &additional_tx_keys, bool rct, const rct::RCTConfig &rct_config, rct::multisig_out *msout, bool shuffle_outs)
  {
    hw::device &hwdev = sender_account_keys.get_device();

    if (sources.empty())
    {
      LOG_ERROR("Empty sources");
      return false;
    }

    std::vector<rct::key> amount_keys;
    tx.set_null();
    amount_keys.clear();
    if (msout)
    {
      msout->c.clear();
    }

    tx.version = rct ? 2 : 1;
    tx.unlock_time = unlock_time;

    tx.extra = extra;
    crypto::public_key txkey_pub;

    // if we have a stealth payment id, find it and encrypt it with the tx key now
    std::vector<tx_extra_field> tx_extra_fields;
    if (parse_tx_extra(tx.extra, tx_extra_fields))
    {
      bool add_dummy_payment_id = true;
      tx_extra_nonce extra_nonce;
      if (find_tx_extra_field_by_type(tx_extra_fields, extra_nonce))
      {
        crypto::hash payment_id = null_hash;
        crypto::hash8 payment_id8 = null_hash8;
        if (get_encrypted_payment_id_from_tx_extra_nonce(extra_nonce.nonce, payment_id8))
        {
          LOG_PRINT_L2("Encrypting payment id " << payment_id8);
          crypto::public_key view_key_pub = get_destination_view_key_pub(destinations, change_addr);
          if (view_key_pub == null_pkey)
          {
            LOG_ERROR("Destinations have to have exactly one output to support encrypted payment ids");
            return false;
          }

          if (!hwdev.encrypt_payment_id(payment_id8, view_key_pub, tx_key))
          {
            LOG_ERROR("Failed to encrypt payment id");
            return false;
          }

          std::string extra_nonce;
          set_encrypted_payment_id_to_tx_extra_nonce(extra_nonce, payment_id8);
          remove_field_from_tx_extra(tx.extra, typeid(tx_extra_nonce));
          if (!add_extra_nonce_to_tx_extra(tx.extra, extra_nonce))
          {
            LOG_ERROR("Failed to add encrypted payment id to tx extra");
            return false;
          }
          LOG_PRINT_L1("Encrypted payment ID: " << payment_id8);
          add_dummy_payment_id = false;
        }
        else if (get_payment_id_from_tx_extra_nonce(extra_nonce.nonce, payment_id))
        {
          add_dummy_payment_id = false;
        }
      }

      // we don't add one if we've got more than the usual 1 destination plus change
      if (destinations.size() > 2)
        add_dummy_payment_id = false;

      if (add_dummy_payment_id)
      {
        // if we have neither long nor short payment id, add a dummy short one,
        // this should end up being the vast majority of txes as time goes on
        std::string extra_nonce;
        crypto::hash8 payment_id8 = null_hash8;
        crypto::public_key view_key_pub = get_destination_view_key_pub(destinations, change_addr);
        if (view_key_pub == null_pkey)
        {
          LOG_ERROR("Failed to get key to encrypt dummy payment id with");
        }
        else
        {
          hwdev.encrypt_payment_id(payment_id8, view_key_pub, tx_key);
          set_encrypted_payment_id_to_tx_extra_nonce(extra_nonce, payment_id8);
          if (!add_extra_nonce_to_tx_extra(tx.extra, extra_nonce))
          {
            LOG_ERROR("Failed to add dummy encrypted payment id to tx extra");
            // continue anyway
          }
        }
      }
    }
    else
    {
      MWARNING("Failed to parse tx extra");
      tx_extra_fields.clear();
    }

    struct input_generation_context_data
    {
      keypair in_ephemeral;
    };
    std::vector<input_generation_context_data> in_contexts;

    uint64_t summary_inputs_money = 0;
    //fill inputs
    int idx = -1;
    for(const tx_source_entry& src_entr:  sources)
    {
      ++idx;
      if(src_entr.real_output >= src_entr.outputs.size())
      {
        LOG_ERROR("real_output index (" << src_entr.real_output << ")bigger than output_keys.size()=" << src_entr.outputs.size());
        return false;
      }
      summary_inputs_money += src_entr.amount;

      //key_derivation recv_derivation;
      in_contexts.push_back(input_generation_context_data());
      keypair& in_ephemeral = in_contexts.back().in_ephemeral;
      crypto::key_image img;

      //Dilithium change
      std::memcpy(&img, &sender_account_keys.m_account_address.m_spend_public_key, CRYPTO_PUBLICKEYBYTES);
      //const auto& out_key = reinterpret_cast<const crypto::public_key&>(src_entr.outputs[src_entr.real_output].second.dest);
      //if(!generate_key_image_helper(sender_account_keys, subaddresses, out_key, src_entr.real_out_tx_key, src_entr.real_out_additional_tx_keys, src_entr.real_output_in_tx_index, in_ephemeral,img, hwdev))
      //{
      //  LOG_ERROR("Key image generation failed!");
      //  return false;
      //}

      std::memcpy(&in_ephemeral.pub, &src_entr.outputs[src_entr.real_output].second, CRYPTO_PUBLICKEYBYTES);
      //check that derivated key is equal with real output key (if non multisig)
      if(!msout && !(in_ephemeral.pub == src_entr.outputs[src_entr.real_output].second) )
      {
        LOG_ERROR("derived public key mismatch with output public key at index " << idx << ", real out " << src_entr.real_output << "! "<< ENDL << "derived_key:"
          << string_tools::pod_to_hex(in_ephemeral.pub) << ENDL << "real output_public_key:"
          << string_tools::pod_to_hex(src_entr.outputs[src_entr.real_output].second) );
        LOG_ERROR("amount " << src_entr.amount << ", rct " << src_entr.rct);
        LOG_ERROR("tx pubkey " << src_entr.real_out_tx_key << ", real_output_in_tx_index " << src_entr.real_output_in_tx_index);
        return false;
      }

      //put key image into tx input
      txin_to_key input_to_key;
      input_to_key.amount = src_entr.amount;
      input_to_key.k_image = msout ? rct::rct2ki(src_entr.multisig_kLRki.ki) : img;

      //fill outputs array and use relative offsets
      for(const tx_source_entry::output_entry& out_entry: src_entr.outputs)
        input_to_key.key_offsets.push_back(out_entry.first);

      input_to_key.key_offsets = absolute_output_offsets_to_relative(input_to_key.key_offsets);
      tx.vin.push_back(input_to_key);
    }

    if (shuffle_outs)
    {
      std::shuffle(destinations.begin(), destinations.end(), std::default_random_engine(crypto::rand<unsigned int>()));
    }

    // sort ins by their key image
    std::vector<size_t> ins_order(sources.size());
    for (size_t n = 0; n < sources.size(); ++n)
      ins_order[n] = n;
    std::sort(ins_order.begin(), ins_order.end(), [&](const size_t i0, const size_t i1) {
      const txin_to_key &tk0 = boost::get<txin_to_key>(tx.vin[i0]);
      const txin_to_key &tk1 = boost::get<txin_to_key>(tx.vin[i1]);
      return memcmp(&tk0.k_image, &tk1.k_image, sizeof(tk0.k_image)) > 0;
    });
    tools::apply_permutation(ins_order, [&] (size_t i0, size_t i1) {
      std::swap(tx.vin[i0], tx.vin[i1]);
      std::swap(in_contexts[i0], in_contexts[i1]);
      std::swap(sources[i0], sources[i1]);
    });

    // figure out if we need to make additional tx pubkeys
    size_t num_stdaddresses = 0;
    size_t num_subaddresses = 0;
    account_public_address single_dest_subaddress;
    classify_addresses(destinations, change_addr, num_stdaddresses, num_subaddresses, single_dest_subaddress);

    // if this is a single-destination transfer to a subaddress, we set the tx pubkey to R=s*D
    if (num_stdaddresses == 0 && num_subaddresses == 1)
    {
      txkey_pub = rct::rct2pk(hwdev.scalarmultKey(rct::pk2rct(single_dest_subaddress.m_spend_public_key), rct::sk2rct(tx_key)));
    }
    else
    {
      txkey_pub = rct::rct2pk(hwdev.scalarmultBase(rct::sk2rct(tx_key)));
    }
    remove_field_from_tx_extra(tx.extra, typeid(tx_extra_pub_key));
    add_tx_pub_key_to_extra(tx, txkey_pub);

    std::vector<crypto::public_key> additional_tx_public_keys;

    // we don't need to include additional tx keys if:
    //   - all the destinations are standard addresses
    //   - there's only one destination which is a subaddress
    bool need_additional_txkeys = num_subaddresses > 0 && (num_stdaddresses > 0 || num_subaddresses > 1);
    if (need_additional_txkeys)
      CHECK_AND_ASSERT_MES(destinations.size() == additional_tx_keys.size(), false, "Wrong amount of additional tx keys");

    uint64_t summary_outs_money = 0;
    //fill outputs
    size_t output_index = 0;
    for(const tx_destination_entry& dst_entr: destinations)
    {
      CHECK_AND_ASSERT_MES(dst_entr.amount > 0 || tx.version > 1, false, "Destination with wrong amount: " << dst_entr.amount);
      crypto::public_key out_eph_public_key;

      hwdev.generate_output_ephemeral_keys(tx.version,sender_account_keys, txkey_pub, tx_key,
                                           dst_entr, change_addr, output_index,
                                           need_additional_txkeys, additional_tx_keys,
                                           additional_tx_public_keys, amount_keys, out_eph_public_key);

      tx_out out;
      out.amount = dst_entr.amount;
      txout_to_key tk;
      tk.key = out_eph_public_key;
      out.target = tk;
      dilithium_randombytes((unsigned char *)&out.random, 32U);
      tx.vout.push_back(out);
      output_index++;
      summary_outs_money += dst_entr.amount;
    }
    CHECK_AND_ASSERT_MES(additional_tx_public_keys.size() == additional_tx_keys.size(), false, "Internal error creating additional public keys");

    remove_field_from_tx_extra(tx.extra, typeid(tx_extra_additional_pub_keys));

    LOG_PRINT_L2("tx pubkey: " << txkey_pub);
    if (need_additional_txkeys)
    {
      LOG_PRINT_L2("additional tx pubkeys: ");
      for (size_t i = 0; i < additional_tx_public_keys.size(); ++i)
        LOG_PRINT_L2(additional_tx_public_keys[i]);
      add_additional_tx_pub_keys_to_extra(tx.extra, additional_tx_public_keys);
    }

    if (!sort_tx_extra(tx.extra, tx.extra))
      return false;

    //check money
    if(summary_outs_money > summary_inputs_money )
    {
      LOG_ERROR("Transaction inputs money ("<< summary_inputs_money << ") less than outputs money (" << summary_outs_money << ")");
      return false;
    }

    // check for watch only wallet
    bool zero_secret_key = true;
    for (size_t i = 0; i < sizeof(sender_account_keys.m_spend_secret_key); ++i)
      zero_secret_key &= (sender_account_keys.m_spend_secret_key.data[i] == 0);
    if (zero_secret_key)
    {
      MDEBUG("Null secret key, skipping signatures");
    }

    if (tx.version == 1)
    {
      //generate ring signatures
      crypto::hash tx_prefix_hash;
      get_transaction_prefix_hash(tx, tx_prefix_hash);

      std::stringstream ss_ring_s;
      size_t i = 0;
      bool alreadySigned = false;
      for(const tx_source_entry& src_entr:  sources)
      {
        ss_ring_s << "pub_keys:" << ENDL;
        std::vector<const crypto::public_key*> keys_ptrs;
        std::vector<crypto::public_key> keys(src_entr.outputs.size());
        size_t ii = 0;
        for(const tx_source_entry::output_entry& o: src_entr.outputs)
        {
            keys_ptrs.push_back(&o.second);
            ss_ring_s << o.second << ENDL;
        }

        tx.signatures.push_back(std::vector<crypto::signature>());
        std::vector<crypto::signature>& sigs = tx.signatures.back();
        sigs.resize(src_entr.outputs.size());
        LOG_PRINT_L1("Outputs: " << src_entr.real_output <<" Fake: "<<src_entr.outputs.size());
        if (!zero_secret_key)
        {
            // Dilithium - signature
            crypto::public_key k_i;
            crypto::secret_key sec;

            std::memcpy(&k_i, &sender_account_keys.m_account_address.m_spend_public_key, CRYPTO_PUBLICKEYBYTES);
            std::memcpy(&sec, &sender_account_keys.m_spend_secret_key, CRYPTO_SECRETKEYBYTES);

            crypto::generate_signature(tx_prefix_hash, k_i, sec, *sigs.data());
        }
        ss_ring_s << "signatures:" << ENDL;
        std::for_each(sigs.begin(), sigs.end(), [&](const crypto::signature& s){ss_ring_s << s << ENDL;});
        ss_ring_s << "prefix_hash:" << tx_prefix_hash << ENDL << "in_ephemeral_key: " << in_contexts[i].in_ephemeral.sec << ENDL << "real_output: " << src_entr.real_output << ENDL;
        i++;
      }

      MCINFO("construct_tx", "transaction_created: " << get_transaction_hash(tx) << ENDL << obj_to_json_str(tx) << ENDL << ss_ring_s.str());
    }

    tx.invalidate_hashes();

    return true;
  }
  //---------------------------------------------------------------
  bool construct_tx_with_tx_key(const account_keys& sender_account_keys, const std::unordered_map<crypto::public_key, subaddress_index>& subaddresses, std::vector<tx_source_entry>& sources, std::vector<tx_destination_entry>& destinations, const boost::optional<cryptonote::account_public_address>& change_addr, std::vector<uint8_t> extra,
                                transaction& tx, uint64_t unlock_time, const crypto::secret_key &tx_key, const crypto::public_key &tx_pub_key, const std::vector<crypto::secret_key> &additional_tx_keys, bool rct, const rct::RCTConfig &rct_config, rct::multisig_out *msout, bool shuffle_outs)
  {
      hw::device &hwdev = sender_account_keys.get_device();

      if (sources.empty())
      {
          LOG_ERROR("Empty sources");
          return false;
      }

      std::vector<rct::key> amount_keys;
      tx.set_null();
      amount_keys.clear();
      if (msout)
      {
          msout->c.clear();
      }

      tx.version = rct ? 2 : 1;
      tx.unlock_time = unlock_time;

      tx.extra = extra;
      crypto::public_key txkey_pub = tx_pub_key;

      // if we have a stealth payment id, find it and encrypt it with the tx key now
      std::vector<tx_extra_field> tx_extra_fields;
      if (parse_tx_extra(tx.extra, tx_extra_fields))
      {
          bool add_dummy_payment_id = true;
          tx_extra_nonce extra_nonce;
          if (find_tx_extra_field_by_type(tx_extra_fields, extra_nonce))
          {
              crypto::hash payment_id = null_hash;
              crypto::hash8 payment_id8 = null_hash8;
              if (get_encrypted_payment_id_from_tx_extra_nonce(extra_nonce.nonce, payment_id8))
              {
                  LOG_PRINT_L2("Encrypting payment id " << payment_id8);
                  crypto::public_key view_key_pub = get_destination_view_key_pub(destinations, change_addr);
                  if (view_key_pub == null_pkey)
                  {
                      LOG_ERROR("Destinations have to have exactly one output to support encrypted payment ids");
                      return false;
                  }

                  if (!hwdev.encrypt_payment_id(payment_id8, view_key_pub, tx_key))
                  {
                      LOG_ERROR("Failed to encrypt payment id");
                      return false;
                  }

                  std::string extra_nonce;
                  set_encrypted_payment_id_to_tx_extra_nonce(extra_nonce, payment_id8);
                  remove_field_from_tx_extra(tx.extra, typeid(tx_extra_nonce));
                  if (!add_extra_nonce_to_tx_extra(tx.extra, extra_nonce))
                  {
                      LOG_ERROR("Failed to add encrypted payment id to tx extra");
                      return false;
                  }
                  LOG_PRINT_L1("Encrypted payment ID: " << payment_id8);
                  add_dummy_payment_id = false;
              }
              else if (get_payment_id_from_tx_extra_nonce(extra_nonce.nonce, payment_id))
              {
                  add_dummy_payment_id = false;
              }
          }

          // we don't add one if we've got more than the usual 1 destination plus change
          if (destinations.size() > 2)
              add_dummy_payment_id = false;

          if (add_dummy_payment_id)
          {
              // if we have neither long nor short payment id, add a dummy short one,
              // this should end up being the vast majority of txes as time goes on
              std::string extra_nonce;
              crypto::hash8 payment_id8 = null_hash8;
              crypto::public_key view_key_pub = get_destination_view_key_pub(destinations, change_addr);
              if (view_key_pub == null_pkey)
              {
                  LOG_ERROR("Failed to get key to encrypt dummy payment id with");
              }
              else
              {
                  hwdev.encrypt_payment_id(payment_id8, view_key_pub, tx_key);
                  set_encrypted_payment_id_to_tx_extra_nonce(extra_nonce, payment_id8);
                  if (!add_extra_nonce_to_tx_extra(tx.extra, extra_nonce))
                  {
                      LOG_ERROR("Failed to add dummy encrypted payment id to tx extra");
                      // continue anyway
                  }
              }
          }
      }
      else
      {
          LOG_ERROR("Failed to parse tx extra");
          return false;
      }

      struct input_generation_context_data
      {
          keypair in_ephemeral;
      };
      std::vector<input_generation_context_data> in_contexts;

      uint64_t summary_inputs_money = 0;
      //fill inputs
      int idx = -1;
      for(const tx_source_entry& src_entr:  sources)
      {
          ++idx;
          if(src_entr.real_output >= src_entr.outputs.size())
          {
              LOG_ERROR("real_output index (" << src_entr.real_output << ")bigger than output_keys.size()=" << src_entr.outputs.size());
              return false;
          }
          summary_inputs_money += src_entr.amount;

          //key_derivation recv_derivation;
          in_contexts.push_back(input_generation_context_data());
          keypair& in_ephemeral = in_contexts.back().in_ephemeral;
          crypto::key_image img;

          const auto& out_key = reinterpret_cast<const crypto::public_key&>(src_entr.outputs[src_entr.real_output].second);
          // From Hydrogen Helix, Point Release 4
          if(!generate_key_image_helper(sender_account_keys, out_key, src_entr.real_output_in_tx_index, in_ephemeral, img))
          {
            LOG_ERROR("Key image generation failed!");
            return false;
          }

          //check that derivated key is equal with real output key (if non multisig)
          if(!msout && !(in_ephemeral.pub == src_entr.outputs[src_entr.real_output].second) )
          {
              LOG_ERROR("derived public key mismatch with output public key at index " << idx << ", real out " << src_entr.real_output << "! "<< ENDL << "derived_key:"
                                                                                       << string_tools::pod_to_hex(in_ephemeral.pub) << ENDL << "real output_public_key:"
                                                                                       << string_tools::pod_to_hex(src_entr.outputs[src_entr.real_output].second) );
              LOG_ERROR("amount " << src_entr.amount << ", rct " << src_entr.rct);
              LOG_ERROR("tx pubkey " << src_entr.real_out_tx_key << ", real_output_in_tx_index " << src_entr.real_output_in_tx_index);
              return false;
          }

          //put key image into tx input
          txin_to_key input_to_key;
          input_to_key.amount = src_entr.amount;
          input_to_key.k_image = msout ? rct::rct2ki(src_entr.multisig_kLRki.ki) : img;
          dilithium_randombytes((unsigned char *)&input_to_key.random, 32U);

          //fill outputs array and use relative offsets
          for(const tx_source_entry::output_entry& out_entry: src_entr.outputs)
              input_to_key.key_offsets.push_back(out_entry.first);

          input_to_key.key_offsets = absolute_output_offsets_to_relative(input_to_key.key_offsets);
          tx.vin.push_back(input_to_key);
      }

      if (shuffle_outs)
      {
          std::shuffle(destinations.begin(), destinations.end(), std::default_random_engine(crypto::rand<unsigned int>()));
      }

      // sort ins by their key image
      std::vector<size_t> ins_order(sources.size());
      for (size_t n = 0; n < sources.size(); ++n)
          ins_order[n] = n;
      std::sort(ins_order.begin(), ins_order.end(), [&](const size_t i0, const size_t i1) {
          const txin_to_key &tk0 = boost::get<txin_to_key>(tx.vin[i0]);
          const txin_to_key &tk1 = boost::get<txin_to_key>(tx.vin[i1]);
          return memcmp(&tk0.k_image, &tk1.k_image, sizeof(tk0.k_image)) > 0;
      });
      tools::apply_permutation(ins_order, [&] (size_t i0, size_t i1) {
          std::swap(tx.vin[i0], tx.vin[i1]);
          std::swap(in_contexts[i0], in_contexts[i1]);
          std::swap(sources[i0], sources[i1]);
      });

      // figure out if we need to make additional tx pubkeys
      size_t num_stdaddresses = 0;
      size_t num_subaddresses = 0;
      account_public_address single_dest_subaddress;
      classify_addresses(destinations, change_addr, num_stdaddresses, num_subaddresses, single_dest_subaddress);

      // if this is a single-destination transfer to a subaddress, we set the tx pubkey to R=s*D
      // TODO: Will probably be look on where this heads to
      if(rct)
      {
          LOG_PRINT_L0("Version is RCT");
          if (num_stdaddresses == 0 && num_subaddresses == 1){
              txkey_pub = rct::rct2pk(hwdev.scalarmultKey(rct::pk2rct(single_dest_subaddress.m_spend_public_key), rct::sk2rct(tx_key)));
          }
          else
          {
              txkey_pub = rct::rct2pk(hwdev.scalarmultBase(rct::sk2rct(tx_key)));
          }
      }

      remove_field_from_tx_extra(tx.extra, typeid(tx_extra_pub_key));
      add_tx_pub_key_to_extra(tx, txkey_pub);

      std::vector<crypto::public_key> additional_tx_public_keys;

      // we don't need to include additional tx keys if:
      //   - all the destinations are standard addresses
      //   - there's only one destination which is a subaddress
      bool need_additional_txkeys = num_subaddresses > 0 && (num_stdaddresses > 0 || num_subaddresses > 1);
      if (need_additional_txkeys)
          CHECK_AND_ASSERT_MES(destinations.size() == additional_tx_keys.size(), false, "Wrong amount of additional tx keys");

      uint64_t summary_outs_money = 0;
      //fill outputs
      size_t output_index = 0;
      for(const tx_destination_entry& dst_entr: destinations)
      {
          CHECK_AND_ASSERT_MES(dst_entr.amount > 0 || tx.version > 1, false, "Destination with wrong amount: " << dst_entr.amount);
          crypto::public_key out_eph_public_key;

          hwdev.generate_output_ephemeral_keys(tx.version,sender_account_keys, txkey_pub, tx_key,
                                               dst_entr, change_addr, output_index,
                                               need_additional_txkeys, additional_tx_keys,
                                               additional_tx_public_keys, amount_keys, out_eph_public_key);

          tx_out out;
          out.amount = dst_entr.amount;
          txout_to_key tk;
          tk.key = out_eph_public_key;
          out.target = tk;
          dilithium_randombytes((unsigned char *)&out.random, 32U);
          tx.vout.push_back(out);
          output_index++;
          summary_outs_money += dst_entr.amount;
      }
      CHECK_AND_ASSERT_MES(additional_tx_public_keys.size() == additional_tx_keys.size(), false, "Internal error creating additional public keys");

      remove_field_from_tx_extra(tx.extra, typeid(tx_extra_additional_pub_keys));

      LOG_PRINT_L2("tx pubkey: " << txkey_pub);
      if (need_additional_txkeys)
      {
          LOG_PRINT_L2("additional tx pubkeys: ");
          for (size_t i = 0; i < additional_tx_public_keys.size(); ++i)
              LOG_PRINT_L2(additional_tx_public_keys[i]);
          add_additional_tx_pub_keys_to_extra(tx.extra, additional_tx_public_keys);
      }

      //check money
      if(summary_outs_money > summary_inputs_money )
      {
          LOG_ERROR("Transaction inputs money ("<< summary_inputs_money << ") less than outputs money (" << summary_outs_money << ")");
          return false;
      }

      // check for watch only wallet
      bool zero_secret_key = true;
      for (size_t i = 0; i < sizeof(sender_account_keys.m_spend_secret_key); ++i)
          zero_secret_key &= (sender_account_keys.m_spend_secret_key.data[i] == 0);
      if (zero_secret_key)
      {
          MDEBUG("Null secret key, skipping signatures");
      }

      if (tx.version == 1)
      {
          //generate ring signatures
          crypto::hash tx_prefix_hash;
          get_transaction_prefix_hash(tx, tx_prefix_hash);

          std::stringstream ss_ring_s;
          size_t i = 0;
          for(const tx_source_entry& src_entr:  sources)
          {
              ss_ring_s << "pub_keys:" << ENDL;
              std::vector<const crypto::public_key*> keys_ptrs;
              for(const tx_source_entry::output_entry& o: src_entr.outputs)
              {
                  keys_ptrs.push_back(&o.second);
                  ss_ring_s << o.second << ENDL;
              }

              tx.signatures.push_back(std::vector<crypto::signature>());
              std::vector<crypto::signature>& sigs = tx.signatures.back();
              sigs.resize(src_entr.outputs.size());
              LOG_PRINT_L1("Outputs: " << src_entr.real_output <<" Fake: "<<src_entr.outputs.size());
              if (!zero_secret_key)
              {
                  // Dilithium - signature
                  crypto::public_key k_i;
                  crypto::secret_key sec;

                  std::memcpy(&k_i, &sender_account_keys.m_account_address.m_spend_public_key, CRYPTO_PUBLICKEYBYTES);
                  std::memcpy(&sec, &sender_account_keys.m_spend_secret_key, CRYPTO_SECRETKEYBYTES);

                  crypto::generate_signature(tx_prefix_hash, k_i, sec, *sigs.data());
              }
              ss_ring_s << "signatures:" << ENDL;
              std::for_each(sigs.begin(), sigs.end(), [&](const crypto::signature& s){ss_ring_s << s << ENDL;});
              ss_ring_s << "prefix_hash:" << tx_prefix_hash << ENDL << "in_ephemeral_key: " << in_contexts[i].in_ephemeral.sec << ENDL << "real_output: " << src_entr.real_output << ENDL;
              i++;
          }

          MCINFO("construct_tx", "transaction_created: " << get_transaction_hash(tx) << ENDL << obj_to_json_str(tx) << ENDL << ss_ring_s.str());
      }
      tx.invalidate_hashes();

      return true;
  }
  //---------------------------------------------------------------
  bool construct_tx_and_get_tx_key(const account_keys& sender_account_keys, const std::unordered_map<crypto::public_key, subaddress_index>& subaddresses, std::vector<tx_source_entry>& sources, std::vector<tx_destination_entry>& destinations, const boost::optional<cryptonote::account_public_address>& change_addr, std::vector<uint8_t> extra,
          transaction& tx, uint64_t unlock_time, crypto::secret_key &tx_key, std::vector<crypto::secret_key> &additional_tx_keys, bool rct, const rct::RCTConfig &rct_config, rct::multisig_out *msout)
  {
    hw::device &hwdev = sender_account_keys.get_device();
    crypto::public_key tx_pub_key{};

    hwdev.open_tx(tx_key, tx_pub_key);

    // figure out if we need to make additional tx pubkeys
    size_t num_stdaddresses = 0;
    size_t num_subaddresses = 0;
    account_public_address single_dest_subaddress;
    classify_addresses(destinations, change_addr, num_stdaddresses, num_subaddresses, single_dest_subaddress);
    bool need_additional_txkeys = num_subaddresses > 0 && (num_stdaddresses > 0 || num_subaddresses > 1);
    if (need_additional_txkeys)
    {
      additional_tx_keys.clear();
      for (const auto &d: destinations)
        additional_tx_keys.push_back(keypair::generate(sender_account_keys.get_device()).sec);
    }

    bool r = construct_tx_with_tx_key(sender_account_keys, subaddresses, sources, destinations, change_addr, extra, tx, unlock_time, tx_key, tx_pub_key, additional_tx_keys, rct, rct_config, msout);
    hwdev.close_tx();
    return r;
  }
  //---------------------------------------------------------------
  bool construct_tx(const account_keys& sender_account_keys, std::vector<tx_source_entry>& sources, const std::vector<tx_destination_entry>& destinations, const boost::optional<cryptonote::account_public_address>& change_addr, const std::vector<uint8_t> &extra, transaction& tx, uint64_t unlock_time)
  {
     std::unordered_map<crypto::public_key, cryptonote::subaddress_index> subaddresses;
     subaddresses[sender_account_keys.m_account_address.m_spend_public_key] = {0,0};
     crypto::secret_key tx_key;
     std::vector<crypto::secret_key> additional_tx_keys;
     std::vector<tx_destination_entry> destinations_copy = destinations;
     return construct_tx_and_get_tx_key(sender_account_keys, subaddresses, sources, destinations_copy, change_addr, extra, tx, unlock_time, tx_key, additional_tx_keys, false, { rct::RangeProofBorromean, 0}, NULL);
  }
  //---------------------------------------------------------------
  bool generate_genesis_block(
      block& bl
    , std::string const & genesis_tx
    , uint32_t nonce
    )
  {
    //genesis block
    bl = boost::value_initialized<block>();

    blobdata tx_bl;
    bool r = string_tools::parse_hexstr_to_binbuff(genesis_tx, tx_bl);
    CHECK_AND_ASSERT_MES(r, false, "failed to parse coinbase tx from hard coded blob");
    r = parse_and_validate_tx_from_blob(tx_bl, bl.miner_tx);
    CHECK_AND_ASSERT_MES(r, false, "failed to parse coinbase tx from hard coded blob");
    bl.major_version = CURRENT_BLOCK_MAJOR_VERSION;
    bl.minor_version = CURRENT_BLOCK_MINOR_VERSION;
    bl.timestamp = 0;
    bl.nonce = nonce;
    miner::find_nonce_for_given_block(bl, 1, 0);
    bl.invalidate_hashes();
    return true;
  }

  //-----------------------------------------------------------------------------------------------
  // Helper function to generate genesis transaction
  // https://monero.stackexchange.com/questions/8468/how-can-i-get-my-own-genesis-tx-to-use-in-my-monero-fork/8630#8630
  void print_genesis_tx_hex(uint8_t nettype) {

      using namespace cryptonote;

      account_base miner_acc1;
      miner_acc1.generate();

      std::cout << "Gennerating miner wallet..." << std::endl;
      std::cout << "Miner account address:" << std::endl;
      std::cout << cryptonote::get_account_address_as_str((network_type)nettype, false, miner_acc1.get_keys().m_account_address);
      std::cout << std::endl << "Miner spend secret key:"  << std::endl;
      epee::to_hex::formatted(std::cout, epee::as_byte_span(miner_acc1.get_keys().m_spend_secret_key));
      std::cout << std::endl << "Miner view secret key:" << std::endl;
      epee::to_hex::formatted(std::cout, epee::as_byte_span(miner_acc1.get_keys().m_view_secret_key));
      std::cout << std::endl << std::endl;

//Create file with miner keys information
      auto t = std::time(nullptr);
      auto tm = *std::localtime(&t);
      std::stringstream key_fine_name_ss;
      key_fine_name_ss << "./miner01_keys" << std::put_time(&tm, "%Y%m%d%H%M%S") << ".dat";
      std::string key_file_name = key_fine_name_ss.str();
      std::ofstream miner_key_file;
      miner_key_file.open (key_file_name);
      miner_key_file << "Miner account address:" << std::endl;
      miner_key_file << cryptonote::get_account_address_as_str((network_type)nettype, false, miner_acc1.get_keys().m_account_address);
      miner_key_file << std::endl<< "Miner spend secret key:"  << std::endl;
      epee::to_hex::formatted(miner_key_file, epee::as_byte_span(miner_acc1.get_keys().m_spend_secret_key));
      miner_key_file << std::endl << "Miner view secret key:" << std::endl;
      epee::to_hex::formatted(miner_key_file, epee::as_byte_span(miner_acc1.get_keys().m_view_secret_key));
      miner_key_file << std::endl << std::endl;
      miner_key_file.close();


//Prepare genesis_tx
      cryptonote::transaction tx_genesis;
      cryptonote::construct_miner_tx(0, 0, 0, 0, 0, miner_acc1.get_keys().m_account_address, tx_genesis);

      std::cout << "Object:" << std::endl;
      std::cout << obj_to_json_str(tx_genesis) << std::endl << std::endl;


      std::stringstream ss;
      binary_archive<true> ba(ss);
      ::serialization::serialize(ba, tx_genesis);
      std::string tx_hex = ss.str();
      std::cout << "Insert this line into your coin configuration file: " << std::endl;
      std::cout << "std::string const GENESIS_TX = \"" << string_tools::buff_to_hex_nodelimer(tx_hex) << "\";" << std::endl;

      return;
  }
}

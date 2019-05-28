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

#include <vector>

#include "common/util.h"
#include "cryptonote_core/cryptonote_tx_utils.h"

namespace
{
  uint64_t const TEST_FEE = 5000000000; // 5 * 10^9
}

TEST(parse_tx_extra, handles_empty_extra)
{
  std::vector<uint8_t> extra;;
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_TRUE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
  ASSERT_TRUE(tx_extra_fields.empty());
}

TEST(parse_tx_extra, handles_padding_only_size_1)
{
  const uint8_t extra_arr[] = {0};
  std::vector<uint8_t> extra(&extra_arr[0], &extra_arr[0] + sizeof(extra_arr));
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_TRUE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
  ASSERT_EQ(1, tx_extra_fields.size());
  ASSERT_EQ(typeid(cryptonote::tx_extra_padding), tx_extra_fields[0].type());
  ASSERT_EQ(1, boost::get<cryptonote::tx_extra_padding>(tx_extra_fields[0]).size);
}

TEST(parse_tx_extra, handles_padding_only_size_2)
{
  const uint8_t extra_arr[] = {0, 0};
  std::vector<uint8_t> extra(&extra_arr[0], &extra_arr[0] + sizeof(extra_arr));
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_TRUE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
  ASSERT_EQ(1, tx_extra_fields.size());
  ASSERT_EQ(typeid(cryptonote::tx_extra_padding), tx_extra_fields[0].type());
  ASSERT_EQ(2, boost::get<cryptonote::tx_extra_padding>(tx_extra_fields[0]).size);
}

TEST(parse_tx_extra, handles_padding_only_max_size)
{
  std::vector<uint8_t> extra(TX_EXTRA_NONCE_MAX_COUNT, 0);
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_TRUE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
  ASSERT_EQ(1, tx_extra_fields.size());
  ASSERT_EQ(typeid(cryptonote::tx_extra_padding), tx_extra_fields[0].type());
  ASSERT_EQ(TX_EXTRA_NONCE_MAX_COUNT, boost::get<cryptonote::tx_extra_padding>(tx_extra_fields[0]).size);
}

TEST(parse_tx_extra, handles_padding_only_exceed_max_size)
{
  std::vector<uint8_t> extra(TX_EXTRA_NONCE_MAX_COUNT + 1, 0);
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_FALSE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
}

TEST(parse_tx_extra, handles_invalid_padding_only)
{
  std::vector<uint8_t> extra(2, 0);
  extra[1] = 42;
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_FALSE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
}

TEST(parse_tx_extra, handles_pub_key_only)
{
  const uint8_t extra_arr[] = {1, 180, 34, 115, 182, 161, 48, 172, 23, 65, 222, 248, 205, 39, 186, 158, 19, 15, 76, 67, 37, 227, 93, 22, 15, 127, 124, 6, 4, 10, 254, 251, 160, 98, 117, 195, 45, 187, 7, 118, 87, 59, 215, 140, 244, 185, 163, 178, 68, 61, 248, 61, 76, 209, 213, 186, 100, 2, 195, 115, 131, 135, 42, 192, 75, 209, 236, 228, 32, 23, 29, 161, 202, 246, 92, 39, 145, 157, 188, 31, 152, 124, 34, 209, 172, 76, 246, 64, 238, 17, 38, 2, 173, 79, 23, 186, 47, 7, 200, 242, 138, 31, 253, 68, 216, 112, 226, 191, 42, 129, 18, 122, 82, 140, 198, 127, 115, 85, 249, 44, 63, 206, 65, 193, 109, 216, 17, 181, 137, 70, 190, 210, 190, 14, 80, 113, 46, 226, 204, 236, 7, 113, 160, 203, 101, 159, 66, 75, 143, 65, 42, 147, 143, 54, 200, 231, 116, 118, 135, 247, 0, 6, 100, 146, 18, 3, 120, 188, 79, 4, 235, 220, 221, 43, 149, 117, 34, 133, 137, 37, 2, 158, 243, 46, 32, 232, 194, 27, 197, 58, 216, 206, 193, 161, 245, 149, 132, 154, 94, 226, 161, 26, 244, 66, 38, 173, 170, 192, 136, 193, 32, 74, 44, 13, 88, 198, 138, 73, 104, 236, 57, 99, 78, 112, 8, 43, 88, 165, 136, 23, 89, 86, 123, 32, 10, 211, 91, 77, 198, 30, 20, 172, 28, 165, 30, 182, 101, 23, 164, 113, 105, 118, 102, 236, 234, 197, 42, 213, 1, 179, 221, 131, 245, 255, 214, 29, 97, 47, 47, 129, 17, 39, 30, 93, 59, 174, 124, 166, 183, 235, 219, 5, 217, 135, 102, 129, 160, 25, 119, 104, 103, 88, 129, 252, 148, 50, 205, 191, 67, 36, 180, 97, 218, 116, 196, 119, 35, 124, 251, 153, 126, 57, 211, 121, 40, 168, 113, 219, 159, 58, 107, 52, 145, 165, 49, 70, 57, 87, 145, 129, 121, 61, 4, 27, 14, 33, 176, 106, 107, 167, 61, 165, 197, 113, 211, 166, 150, 139, 203, 92, 114, 91, 157, 125, 144, 26, 98, 152, 23, 74, 0, 44, 159, 251, 88, 104, 75, 208, 251, 233, 162, 66, 161, 79, 118, 153, 50, 85, 133, 100, 172, 227, 124, 20, 112, 61, 3, 139, 130, 182, 255, 163, 51, 202, 116, 223, 72, 202, 90, 33, 55, 143, 148, 169, 16, 58, 72, 245, 3, 144, 246, 234, 46, 53, 231, 141, 54, 158, 180, 182, 190, 125, 97, 39, 130, 153, 60, 77, 242, 145, 134, 148, 139, 5, 211, 103, 40, 21, 133, 243, 166, 90, 145, 78, 180, 110, 239, 105, 78, 50, 109, 232, 30, 202, 89, 177, 97, 112, 96, 23, 17, 243, 0, 127, 101, 154, 160, 90, 77, 33, 140, 190, 187, 177, 212, 33, 126, 40, 239, 225, 99, 188, 167, 184, 48, 253, 228, 243, 202, 241, 138, 246, 55, 62, 233, 32, 131, 6, 156, 19, 50, 89, 199, 197, 209, 148, 62, 36, 74, 188, 124, 233, 223, 86, 216, 87, 32, 222, 204, 99, 173, 162, 90, 113, 45, 141, 169, 197, 56, 181, 88, 243, 153, 184, 242, 46, 222, 24, 126, 161, 56, 149, 168, 218, 150, 176, 89, 163, 153, 26, 76, 11, 148, 41, 195, 56, 163, 8, 125, 251, 247, 222, 193, 147, 108, 132, 13, 207, 125, 21, 248, 145, 72, 41, 221, 33, 235, 106, 89, 190, 243, 158, 7, 45, 221, 95, 4, 74, 144, 196, 255, 47, 204, 116, 244, 53, 2, 20, 239, 38, 139, 115, 148, 62, 169, 0, 74, 229, 115, 128, 56, 151, 211, 53, 191, 37, 117, 124, 254, 90, 97, 61, 134, 37, 117, 28, 171, 229, 77, 52, 153, 181, 83, 54, 213, 37, 155, 193, 74, 54, 7, 141, 39, 55, 224, 205, 155, 32, 122, 60, 119, 66, 149, 79, 90, 115, 80, 180, 200, 86, 246, 104, 145, 249, 235, 205, 221, 176, 33, 180, 170, 74, 2, 210, 246, 163, 24, 86, 189, 191, 15, 190, 22, 178, 239, 244, 209, 57, 54, 219, 56, 0, 206, 125, 158, 183, 162, 160, 159, 126, 179, 110, 36, 162, 198, 30, 6, 66, 64, 38, 111, 171, 82, 193, 128, 112, 227, 34, 180, 220, 216, 58, 22, 95, 102, 241, 108, 124, 34, 235, 156, 250, 20, 48, 235, 180, 62, 239, 110, 148, 41, 34, 250, 121, 51, 39, 46, 5, 105, 140, 198, 59, 240, 96, 39, 170, 233, 249, 38, 80, 169, 150, 2, 231, 0, 249, 133, 250, 53, 155, 250, 134, 3, 146, 244, 67, 29, 209, 17, 253, 25, 55, 77, 1, 139, 32, 232, 186, 89, 245, 111, 95, 153, 89, 128, 147, 94, 137, 190, 184, 38, 254, 155, 67, 159, 17, 54, 237, 167, 5, 67, 42, 161, 70, 124, 241, 34, 245, 252, 216, 79, 128, 99, 93, 237, 91, 91, 141, 139, 79, 16, 227, 203, 32, 208, 227, 184, 196, 159, 91, 99, 49, 156, 251, 201, 228, 97, 226, 235, 161, 243, 60, 86, 246, 55, 226, 38, 22, 67, 206, 71, 78, 153, 243, 72, 206, 41, 254, 253, 235, 88, 239, 131, 170, 194, 124, 147, 37, 112, 207, 233, 182, 238, 246, 144, 104, 130, 98, 53, 233, 203, 226, 202, 101, 23, 240, 247};
  std::vector<uint8_t> extra(&extra_arr[0], &extra_arr[0] + sizeof(extra_arr));
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_TRUE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
  ASSERT_EQ(1, tx_extra_fields.size());
  ASSERT_EQ(typeid(cryptonote::tx_extra_pub_key), tx_extra_fields[0].type());
}

TEST(parse_tx_extra, handles_extra_nonce_only)
{
  const uint8_t extra_arr[] = {2, 1, 42};
  std::vector<uint8_t> extra(&extra_arr[0], &extra_arr[0] + sizeof(extra_arr));
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_TRUE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
  ASSERT_EQ(1, tx_extra_fields.size());
  ASSERT_EQ(typeid(cryptonote::tx_extra_nonce), tx_extra_fields[0].type());
  cryptonote::tx_extra_nonce extra_nonce = boost::get<cryptonote::tx_extra_nonce>(tx_extra_fields[0]);
  ASSERT_EQ(1, extra_nonce.nonce.size());
  ASSERT_EQ(42, extra_nonce.nonce[0]);
}

TEST(parse_tx_extra, handles_pub_key_and_padding)
{
  const uint8_t extra_arr[] = {1, 180, 34, 115, 182, 161, 48, 172, 23, 65, 222, 248, 205, 39, 186, 158, 19, 15, 76, 67, 37, 227, 93, 22, 15, 127, 124, 6, 4, 10, 254, 251, 160, 98, 117, 195, 45, 187, 7, 118, 87, 59, 215, 140, 244, 185, 163, 178, 68, 61, 248, 61, 76, 209, 213, 186, 100, 2, 195, 115, 131, 135, 42, 192, 75, 209, 236, 228, 32, 23, 29, 161, 202, 246, 92, 39, 145, 157, 188, 31, 152, 124, 34, 209, 172, 76, 246, 64, 238, 17, 38, 2, 173, 79, 23, 186, 47, 7, 200, 242, 138, 31, 253, 68, 216, 112, 226, 191, 42, 129, 18, 122, 82, 140, 198, 127, 115, 85, 249, 44, 63, 206, 65, 193, 109, 216, 17, 181, 137, 70, 190, 210, 190, 14, 80, 113, 46, 226, 204, 236, 7, 113, 160, 203, 101, 159, 66, 75, 143, 65, 42, 147, 143, 54, 200, 231, 116, 118, 135, 247, 0, 6, 100, 146, 18, 3, 120, 188, 79, 4, 235, 220, 221, 43, 149, 117, 34, 133, 137, 37, 2, 158, 243, 46, 32, 232, 194, 27, 197, 58, 216, 206, 193, 161, 245, 149, 132, 154, 94, 226, 161, 26, 244, 66, 38, 173, 170, 192, 136, 193, 32, 74, 44, 13, 88, 198, 138, 73, 104, 236, 57, 99, 78, 112, 8, 43, 88, 165, 136, 23, 89, 86, 123, 32, 10, 211, 91, 77, 198, 30, 20, 172, 28, 165, 30, 182, 101, 23, 164, 113, 105, 118, 102, 236, 234, 197, 42, 213, 1, 179, 221, 131, 245, 255, 214, 29, 97, 47, 47, 129, 17, 39, 30, 93, 59, 174, 124, 166, 183, 235, 219, 5, 217, 135, 102, 129, 160, 25, 119, 104, 103, 88, 129, 252, 148, 50, 205, 191, 67, 36, 180, 97, 218, 116, 196, 119, 35, 124, 251, 153, 126, 57, 211, 121, 40, 168, 113, 219, 159, 58, 107, 52, 145, 165, 49, 70, 57, 87, 145, 129, 121, 61, 4, 27, 14, 33, 176, 106, 107, 167, 61, 165, 197, 113, 211, 166, 150, 139, 203, 92, 114, 91, 157, 125, 144, 26, 98, 152, 23, 74, 0, 44, 159, 251, 88, 104, 75, 208, 251, 233, 162, 66, 161, 79, 118, 153, 50, 85, 133, 100, 172, 227, 124, 20, 112, 61, 3, 139, 130, 182, 255, 163, 51, 202, 116, 223, 72, 202, 90, 33, 55, 143, 148, 169, 16, 58, 72, 245, 3, 144, 246, 234, 46, 53, 231, 141, 54, 158, 180, 182, 190, 125, 97, 39, 130, 153, 60, 77, 242, 145, 134, 148, 139, 5, 211, 103, 40, 21, 133, 243, 166, 90, 145, 78, 180, 110, 239, 105, 78, 50, 109, 232, 30, 202, 89, 177, 97, 112, 96, 23, 17, 243, 0, 127, 101, 154, 160, 90, 77, 33, 140, 190, 187, 177, 212, 33, 126, 40, 239, 225, 99, 188, 167, 184, 48, 253, 228, 243, 202, 241, 138, 246, 55, 62, 233, 32, 131, 6, 156, 19, 50, 89, 199, 197, 209, 148, 62, 36, 74, 188, 124, 233, 223, 86, 216, 87, 32, 222, 204, 99, 173, 162, 90, 113, 45, 141, 169, 197, 56, 181, 88, 243, 153, 184, 242, 46, 222, 24, 126, 161, 56, 149, 168, 218, 150, 176, 89, 163, 153, 26, 76, 11, 148, 41, 195, 56, 163, 8, 125, 251, 247, 222, 193, 147, 108, 132, 13, 207, 125, 21, 248, 145, 72, 41, 221, 33, 235, 106, 89, 190, 243, 158, 7, 45, 221, 95, 4, 74, 144, 196, 255, 47, 204, 116, 244, 53, 2, 20, 239, 38, 139, 115, 148, 62, 169, 0, 74, 229, 115, 128, 56, 151, 211, 53, 191, 37, 117, 124, 254, 90, 97, 61, 134, 37, 117, 28, 171, 229, 77, 52, 153, 181, 83, 54, 213, 37, 155, 193, 74, 54, 7, 141, 39, 55, 224, 205, 155, 32, 122, 60, 119, 66, 149, 79, 90, 115, 80, 180, 200, 86, 246, 104, 145, 249, 235, 205, 221, 176, 33, 180, 170, 74, 2, 210, 246, 163, 24, 86, 189, 191, 15, 190, 22, 178, 239, 244, 209, 57, 54, 219, 56, 0, 206, 125, 158, 183, 162, 160, 159, 126, 179, 110, 36, 162, 198, 30, 6, 66, 64, 38, 111, 171, 82, 193, 128, 112, 227, 34, 180, 220, 216, 58, 22, 95, 102, 241, 108, 124, 34, 235, 156, 250, 20, 48, 235, 180, 62, 239, 110, 148, 41, 34, 250, 121, 51, 39, 46, 5, 105, 140, 198, 59, 240, 96, 39, 170, 233, 249, 38, 80, 169, 150, 2, 231, 0, 249, 133, 250, 53, 155, 250, 134, 3, 146, 244, 67, 29, 209, 17, 253, 25, 55, 77, 1, 139, 32, 232, 186, 89, 245, 111, 95, 153, 89, 128, 147, 94, 137, 190, 184, 38, 254, 155, 67, 159, 17, 54, 237, 167, 5, 67, 42, 161, 70, 124, 241, 34, 245, 252, 216, 79, 128, 99, 93, 237, 91, 91, 141, 139, 79, 16, 227, 203, 32, 208, 227, 184, 196, 159, 91, 99, 49, 156, 251, 201, 228, 97, 226, 235, 161, 243, 60, 86, 246, 55, 226, 38, 22, 67, 206, 71, 78, 153, 243, 72, 206, 41, 254, 253, 235, 88, 239, 131, 170, 194, 124, 147, 37, 112, 207, 233, 182, 238, 246, 144, 104, 130, 98, 53, 233, 203, 226, 202, 101, 23, 240, 247, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::vector<uint8_t> extra(&extra_arr[0], &extra_arr[0] + sizeof(extra_arr));
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_TRUE(cryptonote::parse_tx_extra(extra, tx_extra_fields));
  ASSERT_EQ(2, tx_extra_fields.size());
  ASSERT_EQ(typeid(cryptonote::tx_extra_pub_key), tx_extra_fields[0].type());
  ASSERT_EQ(typeid(cryptonote::tx_extra_padding), tx_extra_fields[1].type());
}

TEST(parse_and_validate_tx_extra, is_valid_tx_extra_parsed)
{
  cryptonote::transaction tx = AUTO_VAL_INIT(tx);
  cryptonote::account_base acc;
  acc.generate();
  cryptonote::blobdata b = "dsdsdfsdfsf";
  ASSERT_TRUE(cryptonote::construct_miner_tx(0, 0, 10000000000000, 1000, TEST_FEE, acc.get_keys().m_account_address, tx, b, 1));
  crypto::public_key tx_pub_key = cryptonote::get_tx_pub_key_from_extra(tx);
  ASSERT_NE(tx_pub_key, crypto::null_pkey);
}
TEST(parse_and_validate_tx_extra, fails_on_big_extra_nonce)
{
  cryptonote::transaction tx = AUTO_VAL_INIT(tx);
  cryptonote::account_base acc;
  acc.generate();
  cryptonote::blobdata b(TX_EXTRA_NONCE_MAX_COUNT + 1, 0);
  ASSERT_FALSE(cryptonote::construct_miner_tx(0, 0, 10000000000000, 1000, TEST_FEE, acc.get_keys().m_account_address, tx, b, 1));
}
TEST(parse_and_validate_tx_extra, fails_on_wrong_size_in_extra_nonce)
{
  cryptonote::transaction tx = AUTO_VAL_INIT(tx);
  tx.extra.resize(20, 0);
  tx.extra[0] = TX_EXTRA_NONCE;
  tx.extra[1] = 255;
  std::vector<cryptonote::tx_extra_field> tx_extra_fields;
  ASSERT_FALSE(cryptonote::parse_tx_extra(tx.extra, tx_extra_fields));
}
TEST(validate_parse_amount_case, validate_parse_amount)
{
  uint64_t res = 0;
  bool r = cryptonote::parse_amount(res, "0.0001");
  ASSERT_TRUE(r);
  ASSERT_EQ(res, 100000000);

  r = cryptonote::parse_amount(res, "100.0001");
  ASSERT_TRUE(r);
  ASSERT_EQ(res, 100000100000000);

  r = cryptonote::parse_amount(res, "000.0000");
  ASSERT_TRUE(r);
  ASSERT_EQ(res, 0);

  r = cryptonote::parse_amount(res, "0");
  ASSERT_TRUE(r);
  ASSERT_EQ(res, 0);


  r = cryptonote::parse_amount(res, "   100.0001    ");
  ASSERT_TRUE(r);
  ASSERT_EQ(res, 100000100000000);

  r = cryptonote::parse_amount(res, "   100.0000    ");
  ASSERT_TRUE(r);
  ASSERT_EQ(res, 100000000000000);

  r = cryptonote::parse_amount(res, "   100. 0000    ");
  ASSERT_FALSE(r);

  r = cryptonote::parse_amount(res, "100. 0000");
  ASSERT_FALSE(r);

  r = cryptonote::parse_amount(res, "100 . 0000");
  ASSERT_FALSE(r);

  r = cryptonote::parse_amount(res, "100.00 00");
  ASSERT_FALSE(r);

  r = cryptonote::parse_amount(res, "1 00.00 00");
  ASSERT_FALSE(r);
}

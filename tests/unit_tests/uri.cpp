// Copyright (c) 2016-2019, The Monero Project
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

#include "gtest/gtest.h"
#include "wallet/wallet2.h"

#define TEST_ADDRESS "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"
#define TEST_INTEGRATED_ADDRESS "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"
// included payment id: <daa03600a3075bf0>

#define PARSE_URI(uri, expected) \
  std::string address, payment_id, recipient_name, description, error; \
  uint64_t amount; \
  std::vector<std::string> unknown_parameters; \
  tools::wallet2 w(cryptonote::TESTNET); \
  bool ret = w.parse_uri(uri, address, payment_id, amount, description, recipient_name, unknown_parameters, error); \
  ASSERT_EQ(ret, expected);

TEST(uri, empty_string)
{
  PARSE_URI("", false);
}

TEST(uri, no_scheme)
{
  PARSE_URI("abelian", false);
}

TEST(uri, bad_scheme)
{
  PARSE_URI("http://foo", false);
}

TEST(uri, scheme_not_first)
{
  PARSE_URI(" abelian:", false);
}

TEST(uri, no_body)
{
  PARSE_URI("abelian:", false);
}

TEST(uri, no_address)
{
  PARSE_URI("abelian:?", false);
}

TEST(uri, bad_address)
{
  PARSE_URI("abelian:44444", false);
}

TEST(uri, good_address)
{
  PARSE_URI("abelian:" TEST_ADDRESS, true);
  ASSERT_EQ(address, TEST_ADDRESS);
}

TEST(uri, good_integrated_address)
{
  PARSE_URI("abelian:" TEST_INTEGRATED_ADDRESS, true);
}

TEST(uri, parameter_without_inter)
{
  PARSE_URI("abelian:" TEST_ADDRESS"&amount=1", false);
}

TEST(uri, parameter_without_equals)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?amount", false);
}

TEST(uri, parameter_without_value)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_amount=", false);
}

TEST(uri, negative_amount)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_amount=-1", false);
}

TEST(uri, bad_amount)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_amount=alphanumeric", false);
}

TEST(uri, duplicate_parameter)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_amount=1&tx_amount=1", false);
}

TEST(uri, unknown_parameter)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?unknown=1", true);
  ASSERT_EQ(unknown_parameters.size(), 1);
  ASSERT_EQ(unknown_parameters[0], "unknown=1");
}

TEST(uri, unknown_parameters)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_amount=1&unknown=1&tx_description=desc&foo=bar", true);
  ASSERT_EQ(unknown_parameters.size(), 2);
  ASSERT_EQ(unknown_parameters[0], "unknown=1");
  ASSERT_EQ(unknown_parameters[1], "foo=bar");
}

TEST(uri, empty_payment_id)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_payment_id=", false);
}

TEST(uri, bad_payment_id)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_payment_id=1234567890", false);
}

TEST(uri, short_payment_id)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_payment_id=1234567890123456", false);
}

TEST(uri, long_payment_id)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_payment_id=1234567890123456789012345678901234567890123456789012345678901234", true);
  ASSERT_EQ(address, TEST_ADDRESS);
  ASSERT_EQ(payment_id, "1234567890123456789012345678901234567890123456789012345678901234");
}

TEST(uri, payment_id_with_integrated_address)
{
  PARSE_URI("abelian:" TEST_INTEGRATED_ADDRESS"?tx_payment_id=1234567890123456", false);
}

TEST(uri, empty_description)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_description=", true);
  ASSERT_EQ(description, "");
}

TEST(uri, empty_recipient_name)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?recipient_name=", true);
  ASSERT_EQ(recipient_name, "");
}

TEST(uri, non_empty_description)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_description=foo", true);
  ASSERT_EQ(description, "foo");
}

TEST(uri, non_empty_recipient_name)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?recipient_name=foo", true);
  ASSERT_EQ(recipient_name, "foo");
}

TEST(uri, url_encoding)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_description=foo%20bar", true);
  ASSERT_EQ(description, "foo bar");
}

TEST(uri, non_alphanumeric_url_encoding)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_description=foo%2x", true);
  ASSERT_EQ(description, "foo%2x");
}

TEST(uri, truncated_url_encoding)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_description=foo%2", true);
  ASSERT_EQ(description, "foo%2");
}

TEST(uri, percent_without_url_encoding)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_description=foo%", true);
  ASSERT_EQ(description, "foo%");
}

TEST(uri, url_encoded_once)
{
  PARSE_URI("abelian:" TEST_ADDRESS"?tx_description=foo%2020", true);
  ASSERT_EQ(description, "foo 20");
}


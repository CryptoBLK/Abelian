// Copyright (c) 2014-2019, The Monero Project
// 

// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list
//    of conditions and the following disclaimer in the documentation and/or
//    other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be
//    used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// FIXME: move this into a full wallet2 unit test suite, if possible

#include "gtest/gtest.h"

#include "common/dns_utils.h"
#include "simplewallet/simplewallet.h"
#include "wallet/wallet2.h"
#include <string>

TEST(AddressFromTXT, Success) {
  std::string addr =
      "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"
      "2jKGAErB91BooRQuVFD8WET";

  std::string txtr = "oa1:xmr";
  txtr += " recipient_address=";
  txtr += addr;
  txtr += ";";

  std::string res = tools::dns_utils::address_from_txt_record(txtr);

  EXPECT_STREQ(addr.c_str(), res.c_str());

  std::string txtr2 = "foobar";

  txtr2 += txtr;

  txtr2 += "more foobar";

  res = tools::dns_utils::address_from_txt_record(txtr2);

  EXPECT_STREQ(addr.c_str(), res.c_str());

  std::string txtr3 = "foobar oa1:xmr tx_description=\"Donation for Abelian "
                      "Development Fund\"; ";
  txtr3 += "recipient_address=";
  txtr3 += addr;
  txtr3 += "; foobar";

  res = tools::dns_utils::address_from_txt_record(txtr3);

  EXPECT_STREQ(addr.c_str(), res.c_str());
}

TEST(AddressFromTXT, Failure) {
  std::string txtr = "oa1:xmr recipient_address=not a real address";

  std::string res = tools::dns_utils::address_from_txt_record(txtr);

  ASSERT_STREQ("", res.c_str());

  txtr += ";";

  res = tools::dns_utils::address_from_txt_record(txtr);
  ASSERT_STREQ("", res.c_str());
}

TEST(AddressFromURL, Success) {
  const std::string addr = MONERO_DONATION_ADDR;

  bool dnssec_result = false;

  std::vector<std::string> addresses = tools::dns_utils::addresses_from_url(
      "donate.getmonero.org", dnssec_result);

  EXPECT_EQ(1, addresses.size());
  if (addresses.size() == 1) {
    EXPECT_STREQ(addr.c_str(), addresses[0].c_str());
  }

  // OpenAlias address with an @ instead of first .
  addresses = tools::dns_utils::addresses_from_url("donate@getmonero.org",
                                                   dnssec_result);
  EXPECT_EQ(1, addresses.size());
  if (addresses.size() == 1) {
    EXPECT_STREQ(addr.c_str(), addresses[0].c_str());
  }
}

TEST(AddressFromURL, Failure) {
  bool dnssec_result = false;

  std::vector<std::string> addresses = tools::dns_utils::addresses_from_url(
      "example.veryinvalid.abelian", dnssec_result);

  // for a non-existing domain such as "example.invalid", the non-existence is
  // proved with NSEC records
  ASSERT_TRUE(dnssec_result);

  ASSERT_EQ(0, addresses.size());
}

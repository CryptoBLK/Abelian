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

#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

#define CRYPTONOTE_DNS_TIMEOUT_MS                       20000

#define CRYPTONOTE_MAX_BLOCK_NUMBER                     500000000
#define CRYPTONOTE_MAX_BLOCK_SIZE                       500000000  // block header blob limit, never used!
#define CRYPTONOTE_GETBLOCKTEMPLATE_MAX_BLOCK_SIZE	196608 //size of block (bytes) that is the maximum that miners will produce
#define CRYPTONOTE_MAX_TX_SIZE                          1000000000
#define CRYPTONOTE_PUBLIC_ADDRESS_TEXTBLOB_VER          0
#define CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW            60
#define CURRENT_TRANSACTION_VERSION                     2
#define CURRENT_BLOCK_MAJOR_VERSION                     1
#define CURRENT_BLOCK_MINOR_VERSION                     0
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT              60*60*2
#define CRYPTONOTE_DEFAULT_TX_SPENDABLE_AGE             10

#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW               60

// MONEY_SUPPLY - total number coins to be generated
#define MONEY_SUPPLY                                    ((uint64_t)(-1))
#define EMISSION_SPEED_FACTOR_PER_MINUTE                (20)
#define FINAL_SUBSIDY_PER_MINUTE                        ((uint64_t)300000000000) // 3 * pow(10, 11)

#define CRYPTONOTE_REWARD_BLOCKS_WINDOW                 100
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2    60000 //size of block (bytes) after which reward for block calculated using block size
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1    20000 //size of block (bytes) after which reward for block calculated using block size - before first fork
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5    300000 //size of block (bytes) after which reward for block calculated using block size - second change, from v5
#define CRYPTONOTE_LONG_TERM_BLOCK_WEIGHT_WINDOW_SIZE   100000 // size in blocks of the long term block weight median window
#define CRYPTONOTE_SHORT_TERM_BLOCK_WEIGHT_SURGE_FACTOR 50
#define CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE          600
#define CRYPTONOTE_DISPLAY_DECIMAL_POINT                12
// COIN - number of smallest units in one coin
#define COIN                                            ((uint64_t)1000000000000) // pow(10, 12)

#define FEE_PER_KB_OLD                                  ((uint64_t)10000000000) // pow(10, 10)
#define FEE_PER_KB                                      ((uint64_t)2000000000) // 2 * pow(10, 9)
#define FEE_PER_BYTE                                    ((uint64_t)300000)
#define DYNAMIC_FEE_PER_KB_BASE_FEE                     ((uint64_t)2000000000) // 2 * pow(10,9)
#define DYNAMIC_FEE_PER_KB_BASE_BLOCK_REWARD            ((uint64_t)10000000000000) // 10 * pow(10,12)
#define DYNAMIC_FEE_PER_KB_BASE_FEE_V5                  ((uint64_t)2000000000 * (uint64_t)CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2 / CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5)
#define DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT         ((uint64_t)3000)

#define ORPHANED_BLOCKS_MAX_COUNT                       100


#define DIFFICULTY_TARGET_V2                            120  // seconds
#define DIFFICULTY_TARGET_V1                            60  // seconds - before first fork
#define DIFFICULTY_WINDOW                               720 // blocks
#define DIFFICULTY_LAG                                  15  // !!!
#define DIFFICULTY_CUT                                  60  // timestamps to cut after sorting
#define DIFFICULTY_BLOCKS_COUNT                         DIFFICULTY_WINDOW + DIFFICULTY_LAG


#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V1   DIFFICULTY_TARGET_V1 * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V2   DIFFICULTY_TARGET_V2 * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS       1


#define DIFFICULTY_BLOCKS_ESTIMATE_TIMESPAN             DIFFICULTY_TARGET_V1 //just alias; used by tests


#define BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT          10000  //by default, blocks ids count in synchronizing
#define BLOCKS_SYNCHRONIZING_DEFAULT_COUNT_PRE_V4       100    //by default, blocks count in blocks downloading
#define BLOCKS_SYNCHRONIZING_DEFAULT_COUNT              20     //by default, blocks count in blocks downloading

#define CRYPTONOTE_MEMPOOL_TX_LIVETIME                    (86400*3) //seconds, three days
#define CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME     604800 //seconds, one week

#define COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT           1000

#define P2P_LOCAL_WHITE_PEERLIST_LIMIT                  1000
#define P2P_LOCAL_GRAY_PEERLIST_LIMIT                   5000

#define P2P_DEFAULT_CONNECTIONS_COUNT                   8
#define P2P_DEFAULT_HANDSHAKE_INTERVAL                  60           //secondes
#define P2P_DEFAULT_PACKET_MAX_SIZE                     50000000     //50000000 bytes maximum packet size
#define P2P_DEFAULT_PEERS_IN_HANDSHAKE                  250
#define P2P_DEFAULT_CONNECTION_TIMEOUT                  5000       //5 seconds
#define P2P_DEFAULT_PING_CONNECTION_TIMEOUT             2000       //2 seconds
#define P2P_DEFAULT_INVOKE_TIMEOUT                      60*2*1000  //2 minutes
#define P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT            5000       //5 seconds
#define P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT       70
#define P2P_DEFAULT_ANCHOR_CONNECTIONS_COUNT            2

#define P2P_FAILED_ADDR_FORGET_SECONDS                  (60*60)     //1 hour
#define P2P_IP_BLOCKTIME                                (60*60*24)  //24 hour
#define P2P_IP_FAILS_BEFORE_BLOCK                       10
#define P2P_IDLE_CONNECTION_KILL_INTERVAL               (5*60) //5 minutes

#define P2P_SUPPORT_FLAG_FLUFFY_BLOCKS                  0x01
#define P2P_SUPPORT_FLAGS                               P2P_SUPPORT_FLAG_FLUFFY_BLOCKS

#define ALLOW_DEBUG_COMMANDS

#define CRYPTONOTE_NAME                         "bitmonero"
#define CRYPTONOTE_POOLDATA_FILENAME            "poolstate.bin"
#define CRYPTONOTE_BLOCKCHAINDATA_FILENAME      "data.mdb"
#define CRYPTONOTE_BLOCKCHAINDATA_LOCK_FILENAME "lock.mdb"
#define P2P_NET_DATA_FILENAME                   "p2pstate.bin"
#define MINER_CONFIG_FILE_NAME                  "miner_conf.json"

#define THREAD_STACK_SIZE                       5 * 1024 * 1024

#define HF_VERSION_DYNAMIC_FEE                  4
#define HF_VERSION_MIN_MIXIN_4                  6
#define HF_VERSION_MIN_MIXIN_6                  7
#define HF_VERSION_MIN_MIXIN_10                 8
#define HF_VERSION_ENFORCE_RCT                  6
#define HF_VERSION_PER_BYTE_FEE                 8
#define HF_VERSION_LONG_TERM_BLOCK_WEIGHT       10
#define HF_VERSION_SMALLER_BP                   10

#define PER_KB_FEE_QUANTIZATION_DECIMALS        8

#define HASH_OF_HASHES_STEP                     256

#define DEFAULT_TXPOOL_MAX_WEIGHT               648000000ull // 3 days at 300000, in bytes

#define BULLETPROOF_MAX_OUTPUTS                 16

// New constants are intended to go here
namespace config
{
  uint64_t const DEFAULT_FEE_ATOMIC_XMR_PER_KB = 500; // Just a placeholder!  Change me!
  uint8_t const FEE_CALCULATION_MAX_RETRIES = 10;
  uint64_t const DEFAULT_DUST_THRESHOLD = ((uint64_t)2000000000); // 2 * pow(10, 9)
  uint64_t const BASE_REWARD_CLAMP_THRESHOLD = ((uint64_t)100000000); // pow(10, 8)
  std::string const P2P_REMOTE_DEBUG_TRUSTED_PUB_KEY = "0000000000000000000000000000000000000000000000000000000000000000";

  uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 18;
  uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 19;
  uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 42;
  uint16_t const P2P_DEFAULT_PORT = 18080;
  uint16_t const RPC_DEFAULT_PORT = 18081;
  uint16_t const ZMQ_RPC_DEFAULT_PORT = 18082;
  boost::uuids::uuid const NETWORK_ID = { {
      0x12 ,0x30, 0xF1, 0x71 , 0x61, 0x04 , 0x41, 0x61, 0x17, 0x31, 0x00, 0x82, 0x16, 0xA1, 0xA1, 0x10
    } }; // Bender's nightmare
  //std::string const GENESIS_TX = "013c01ff0001ffffffffffff03029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd08807121017767aafcde9be00dcfd098715ebcf7f410daebc582fda69d24a28e9d0bc890d1";
  std::string const GENESIS_TX = "013c01ff0001ffffffffffff0302544ac62e69148006dca38b49332f210c41374c4fcd8fc727186d57980348167d57266ac227f12045d0b30719de3dd0ce333e8f57e0a58c07962c8ba94b15e125ac1296348b3f42d87145e57a91eba74b53d2947c82151d3fbc8c504e70ffad44573f302bd7ce6418ec83a1817184ebc6f45b45e3d9fb5b15512d0b1c4fe040785181b944941eaf454ac75202ba6c88db4a911d4e129c905d3a3cc739f36b5e63bde9f82c332435e433c1028faf15403e20dc7c73b46feee75b89e56222ceed46f773a81c7d314d55ca6eff8edd14cc4e4319ee930e374548c433305d2ea1c1c3e9b1ccc5913f8721f33bb1fe390f268b993f297a6f00511a5630e91b605519b1e0376ab9c086db924264961e708b3b67716a2596acb8aec3fa83d8fd9fb2594652560e83d185199bd2bdd2353582afb6dde12f4439a9d7e617b5a35537ecf3564f1a82b73b2ede6e106229085a148cd8f3c98999ec2234e348e6f1561560a71287c63191830c09c04d5d33f074b6c868c66ed44c411420d7f111ee8633c90cbb1aa2f6e2ca79ec6dc2e62fb144d11d1fac17ae55ddfbb169775512fbe6e6eef091a6b4aadfe59ba39dbd699d6e2899d53e9d9c7d3d42ee4ef8f1e9dee9e8d6e52fb9aaea31fff12247c8e8fc5d50f1a3106a7a73860d7f524a4357ba29e5784f5942d3ca89ff091c68bf85adcb515fd50a1e044c8505248c64a26fbd3bb851d5d008fd877efdda8828a5e96f5bd4049017bd92c86b93797637d9325968f05773cab806af1c634822ed7a021a1d9694d043bd04c93a12b6c7e056d0d561a8c534c9ecf990591dc3a939725af7f46370b574c479267e0655fc186244735908b1c8e7281b47099593901090872fa280961b96d2fd425d98d496b8644344f7a11ba5e9c35d1de571093a8400a96401186db098e988a81986082f01783fc5258ba54713b1b0608c3f2340dc2061d0a5534c41f7a1bc0cea18a22557ed28dd2013d5aab253bb17cd712f590eab3905187838fbdb333af8864f8f0de428be0ac52a38328dbb1fda18fe4afa12e4866829874c038f6cf09e24b0a0ee0ec8a6beab2c17e4d160b89257f5149e9631f868cd493d21fcc7ba0e068869db02ef9dc9dc2d3abe46c639ceada560beeb040bd397348df8904b73539afdae6b44dc5fffe3a4994c33bd9f2dcf952a50b169ae159ae6e9466cc5e13c27c3c235e6d1ce82d6ca6685b44b9e511d74326468385d578c52b64332de0d79e7e8ee115340fd81b0e234c68e5d96ecd0176c13b6b43d11609da343b720ebc8dfc20e6ff8327a242ba1153339f32130ffcbde5bcdfcdca0676074c50c55fdc694261531ec7aba3b39bc74e60b92d0ad21470a9ba789bc019ac492427ec971ca277c822030fdd5c0cb58f9e680fe14759285ab67476a9e7aaf2d16f86ac988d63529f50e3086a8492685e6f98093e5a0ed3fd23f503cedf093811e4222b4dd4f86e5107d470c3c595ce601c25c4b9fbb7ded9a5fc46f00b5059d5aa1e44db55ee2d96ae5c88e50bc4d8cf92ca1a8ab2073c92517d60dfd26f167ed8314c143b29540d284d7d0a67df141c4ceb06cd482ca086cd39bb33985756f62a587b4a5780ae415a3e39f3428b2acf9dad3d27a8767d1821d17cf0e9b92a91f1df527dc4443f388541730d6f94ec5247200";
  uint32_t const GENESIS_NONCE = 10000;

  namespace testnet
  {
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 53;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 54;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 63;
    uint16_t const P2P_DEFAULT_PORT = 28080;
    uint16_t const RPC_DEFAULT_PORT = 28081;
    uint16_t const ZMQ_RPC_DEFAULT_PORT = 28082;
    boost::uuids::uuid const NETWORK_ID = { {
        0x12 ,0x30, 0xF1, 0x71 , 0x61, 0x04 , 0x41, 0x61, 0x17, 0x31, 0x00, 0x82, 0x16, 0xA1, 0xA1, 0x11
      } }; // Bender's daydream

    //33 byte public key OK
    //std::string const GENESIS_TX = "013c01ff0001ffffffffffff03029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd088071aa22017767aafcde9be00dcfd098715ebcf7f410daebc582fda69d24a28e9d0bc890d1bb";

    //33 byte public key, no extra OK
    //std::string const GENESIS_TX = "013c01ff0001ffffffffffff03029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd088071aa00";

    //257 byte public key, OK (set pk to 258 array is fine, but 256 will fail)
    //std::string const GENESIS_TX = "013c01ff0001ffffffffffff03029b9436053cab089b3e4d902a8149f4e1d5e62a42c1aa413ac05ad593e5e1b4336a65effbb7d980325baf24bea322508091aeb990ea12bd92d1c1ad8ab81187f2e02719e12eed07dcb57b1c54ae9166b0ce19b768c405902e2005228103ae776dcada3601a40bb5534698d6c4dbe86ec9617c71825aac87cf62debc454ca3589ffac5b4a2637026be02ca9a8ea18013c3d12e8e7746ebe04d090a7ffcbf22e1bfdd74024f48cd42d3d9b91be80772e351a77e7a5a907252ebc1da05ec54946bea5f188ab57a3fb9936c7116b42071884f06bc51629b514c255a55b5dad5c62ddb9cd881fa5f42bb1f62705bb03f68a0d20835bf1cddb8a2baaac5242ee33cc42f7000";

    //1184 byte public key, OK (need to change mdb.c to a big key size)
    std::string const GENESIS_TX = "013c01ff0001ffffffffffff0302544ac62e69148006dca38b49332f210c41374c4fcd8fc727186d57980348167d57266ac227f12045d0b30719de3dd0ce333e8f57e0a58c07962c8ba94b15e125ac1296348b3f42d87145e57a91eba74b53d2947c82151d3fbc8c504e70ffad44573f302bd7ce6418ec83a1817184ebc6f45b45e3d9fb5b15512d0b1c4fe040785181b944941eaf454ac75202ba6c88db4a911d4e129c905d3a3cc739f36b5e63bde9f82c332435e433c1028faf15403e20dc7c73b46feee75b89e56222ceed46f773a81c7d314d55ca6eff8edd14cc4e4319ee930e374548c433305d2ea1c1c3e9b1ccc5913f8721f33bb1fe390f268b993f297a6f00511a5630e91b605519b1e0376ab9c086db924264961e708b3b67716a2596acb8aec3fa83d8fd9fb2594652560e83d185199bd2bdd2353582afb6dde12f4439a9d7e617b5a35537ecf3564f1a82b73b2ede6e106229085a148cd8f3c98999ec2234e348e6f1561560a71287c63191830c09c04d5d33f074b6c868c66ed44c411420d7f111ee8633c90cbb1aa2f6e2ca79ec6dc2e62fb144d11d1fac17ae55ddfbb169775512fbe6e6eef091a6b4aadfe59ba39dbd699d6e2899d53e9d9c7d3d42ee4ef8f1e9dee9e8d6e52fb9aaea31fff12247c8e8fc5d50f1a3106a7a73860d7f524a4357ba29e5784f5942d3ca89ff091c68bf85adcb515fd50a1e044c8505248c64a26fbd3bb851d5d008fd877efdda8828a5e96f5bd4049017bd92c86b93797637d9325968f05773cab806af1c634822ed7a021a1d9694d043bd04c93a12b6c7e056d0d561a8c534c9ecf990591dc3a939725af7f46370b574c479267e0655fc186244735908b1c8e7281b47099593901090872fa280961b96d2fd425d98d496b8644344f7a11ba5e9c35d1de571093a8400a96401186db098e988a81986082f01783fc5258ba54713b1b0608c3f2340dc2061d0a5534c41f7a1bc0cea18a22557ed28dd2013d5aab253bb17cd712f590eab3905187838fbdb333af8864f8f0de428be0ac52a38328dbb1fda18fe4afa12e4866829874c038f6cf09e24b0a0ee0ec8a6beab2c17e4d160b89257f5149e9631f868cd493d21fcc7ba0e068869db02ef9dc9dc2d3abe46c639ceada560beeb040bd397348df8904b73539afdae6b44dc5fffe3a4994c33bd9f2dcf952a50b169ae159ae6e9466cc5e13c27c3c235e6d1ce82d6ca6685b44b9e511d74326468385d578c52b64332de0d79e7e8ee115340fd81b0e234c68e5d96ecd0176c13b6b43d11609da343b720ebc8dfc20e6ff8327a242ba1153339f32130ffcbde5bcdfcdca0676074c50c55fdc694261531ec7aba3b39bc74e60b92d0ad21470a9ba789bc019ac492427ec971ca277c822030fdd5c0cb58f9e680fe14759285ab67476a9e7aaf2d16f86ac988d63529f50e3086a8492685e6f98093e5a0ed3fd23f503cedf093811e4222b4dd4f86e5107d470c3c595ce601c25c4b9fbb7ded9a5fc46f00b5059d5aa1e44db55ee2d96ae5c88e50bc4d8cf92ca1a8ab2073c92517d60dfd26f167ed8314c143b29540d284d7d0a67df141c4ceb06cd482ca086cd39bb33985756f62a587b4a5780ae415a3e39f3428b2acf9dad3d27a8767d1821d17cf0e9b92a91f1df527dc4443f388541730d6f94ec5247200";

    uint32_t const GENESIS_NONCE = 10001;
  }

  namespace stagenet
  {
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 24;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 25;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 36;
    uint16_t const P2P_DEFAULT_PORT = 38080;
    uint16_t const RPC_DEFAULT_PORT = 38081;
    uint16_t const ZMQ_RPC_DEFAULT_PORT = 38082;
    boost::uuids::uuid const NETWORK_ID = { {
        0x12 ,0x30, 0xF1, 0x71 , 0x61, 0x04 , 0x41, 0x61, 0x17, 0x31, 0x00, 0x82, 0x16, 0xA1, 0xA1, 0x12
      } }; // Bender's daydream
    //std::string const GENESIS_TX = "013c01ff0001ffffffffffff0302df5d56da0c7d643ddd1ce61901c7bdc5fb1738bfe39fbe69c28a3a7032729c0f2101168d0c4ca86fb55a4cf6a36d31431be1c53a3bd7411bb24e8832410289fa6f3b";
    std::string const GENESIS_TX = "";
    uint32_t const GENESIS_NONCE = 10002;
  }
}

namespace cryptonote
{
  enum network_type : uint8_t
  {
    MAINNET = 0,
    TESTNET,
    STAGENET,
    FAKECHAIN,
    UNDEFINED = 255
  };
  struct config_t
  {
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX;
    uint16_t const P2P_DEFAULT_PORT;
    uint16_t const RPC_DEFAULT_PORT;
    uint16_t const ZMQ_RPC_DEFAULT_PORT;
    boost::uuids::uuid const NETWORK_ID;
    std::string const GENESIS_TX;
    uint32_t const GENESIS_NONCE;
  };
  inline const config_t& get_config(network_type nettype)
  {
    static const config_t mainnet = {
      ::config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
      ::config::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
      ::config::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
      ::config::P2P_DEFAULT_PORT,
      ::config::RPC_DEFAULT_PORT,
      ::config::ZMQ_RPC_DEFAULT_PORT,
      ::config::NETWORK_ID,
      ::config::GENESIS_TX,
      ::config::GENESIS_NONCE
    };
    static const config_t testnet = {
      ::config::testnet::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
      ::config::testnet::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
      ::config::testnet::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
      ::config::testnet::P2P_DEFAULT_PORT,
      ::config::testnet::RPC_DEFAULT_PORT,
      ::config::testnet::ZMQ_RPC_DEFAULT_PORT,
      ::config::testnet::NETWORK_ID,
      ::config::testnet::GENESIS_TX,
      ::config::testnet::GENESIS_NONCE
    };
    static const config_t stagenet = {
      ::config::stagenet::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
      ::config::stagenet::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
      ::config::stagenet::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
      ::config::stagenet::P2P_DEFAULT_PORT,
      ::config::stagenet::RPC_DEFAULT_PORT,
      ::config::stagenet::ZMQ_RPC_DEFAULT_PORT,
      ::config::stagenet::NETWORK_ID,
      ::config::stagenet::GENESIS_TX,
      ::config::stagenet::GENESIS_NONCE
    };
    switch (nettype)
    {
      case MAINNET: return mainnet;
      case TESTNET: return testnet;
      case STAGENET: return stagenet;
      case FAKECHAIN: return mainnet;
      default: throw std::runtime_error("Invalid network type");
    }
  };
}

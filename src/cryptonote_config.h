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
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1    600000 //size of block (bytes) after which reward for block calculated using block size - before first fork, used by Abelian need to change and calculate
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

#define CRYPTONOTE_NAME                         "abelian"
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
  uint16_t const P2P_DEFAULT_PORT = 19090;
  uint16_t const RPC_DEFAULT_PORT = 19091;
  uint16_t const ZMQ_RPC_DEFAULT_PORT = 19092;
  boost::uuids::uuid const NETWORK_ID = { {
      0x12 ,0x30, 0xF1, 0x71 , 0x61, 0x04 , 0x41, 0x61, 0x17, 0x31, 0x00, 0x82, 0x16, 0xA1, 0xA1, 0x10
    } }; // Bender's nightmare
  std::string const GENESIS_TX =
          "013c01ff0006ff9fdb5802e47a8825ced8d5e9f791edaf20bad22335f04fd80ee24800f37745154c097fdcda3315c9a83e6e0638a8cd8bdeb6bc32e7366809d76eb78a57941ca3105bbdc6d590d5d3be340e5b62dd0a0c9ceac88b73d8f917af6a269b9418dd92a22bad22f8aaa78ec5b1e685f0b671b86f18cf1dda6827b6a824eef335041f66dab563b813bd103004d6124ee06de436fbe6514eaed64d36d9e6205a5465d17c5dd12c407191a7987885d14bb0ac25f4fc74c9e1b60fabffd275db1935c3c4267c3c68ee6e26ccd91e51294c57131a6d900dfcd5a6a42531944de7c40e8dd7f95f5a591a7fbc7fa9c46a2f5d04dcc36542f58b90f7491ad8f2347663a7850898058371b8dcb4e42b5f46a393ea52fc1fc248cd9e40cc0746144173136f7bfd042ea5a9c8ea770fd9fc6e9c3fb992bfbd7a38676f12faddfcaa63be5ad93b1214786232bbc483e7e799601f187e74776d9877a01972d0702d9e421fea488f32e817304167aa3b11f71143cc9cbc41a488c4532539b62941027ddaa7d2aa107c6fad6349fa210bc676f5293dca89d86358c1e9b81c9ca19d37bbfab4ac098405e99d1dd36b8a21dbe37f09502e718873c52dbd903395adfffcfc6a40647493d448eb3232ea8253bffa3bc96ce19b9cfbb91c9cd0be127a218e0f0dbde9f72b1cf044ba5265f6706a4ee4cd2fbdd4de1b7a9de7237034d07c1505a1f5139fe3e78835152a47ddbfce03e03b4dd12483972ed8b42518da9c931c3ad6d87fb6bd3178584dc9e28386361b6f3ebf54f68d4428de603767b63af69a5144f37ff5ae4338f807e4e4d02073532efc60a2abb59c24a9596a673dda1cbcd4e2a1d544b5f7765e020d894f2ab28041cebb855f235854efca70e9973c3578d07209c2b57b050732b02900c7ea9e5caee69138f9d2a711405b53d5fbddaee06d592536ca00a3fce7871568f3a87215fcbd01e3a6bd9c8ddf5170a566a59b937be6eeb62c993c337bb82f7c588358ed78709fc964a829dd4d087514e450c304125e0c6308c99b0c39bff643ba010e875dc9dd73ecb0fc00acf250b483e33072ce0430062347bf1ba4e442175a9b78b4c7855e35e0150089b3f966a0064583dd36a62027cbab325175f2c6616dccb2728ae490592605f391890dea53387f13c917ffbf27c8f2d8402013e1a8359c3e0343de37b8689a0295a852da5de9860880d10896f8e6eb1cfdd7bedc1853cf4f2e413902ae0b3cfa44f8fcd071dd614c01537f9556710e6c8b4e28406c530f8afbc74536b9a963b4f1c4cb738bcea7403d4d606b6e074ec5d3baf39d1880a8d6b90702e47a8825ced8d5e9f791edaf20bad22335f04fd80ee24800f37745154c097fdcda3315c9a83e6e0638a8cd8bdeb6bc32e7366809d76eb78a57941ca3105bbdc6d590d5d3be340e5b62dd0a0c9ceac88b73d8f917af6a269b9418dd92a22bad22f8aaa78ec5b1e685f0b671b86f18cf1dda6827b6a824eef335041f66dab563b813bd103004d6124ee06de436fbe6514eaed64d36d9e6205a5465d17c5dd12c407191a7987885d14bb0ac25f4fc74c9e1b60fabffd275db1935c3c4267c3c68ee6e26ccd91e51294c57131a6d900dfcd5a6a42531944de7c40e8dd7f95f5a591a7fbc7fa9c46a2f5d04dcc36542f58b90f7491ad8f2347663a7850898058371b8dcb4e42b5f46a393ea52fc1fc248cd9e40cc0746144173136f7bfd042ea5a9c8ea770fd9fc6e9c3fb992bfbd7a38676f12faddfcaa63be5ad93b1214786232bbc483e7e799601f187e74776d9877a01972d0702d9e421fea488f32e817304167aa3b11f71143cc9cbc41a488c4532539b62941027ddaa7d2aa107c6fad6349fa210bc676f5293dca89d86358c1e9b81c9ca19d37bbfab4ac098405e99d1dd36b8a21dbe37f09502e718873c52dbd903395adfffcfc6a40647493d448eb3232ea8253bffa3bc96ce19b9cfbb91c9cd0be127a218e0f0dbde9f72b1cf044ba5265f6706a4ee4cd2fbdd4de1b7a9de7237034d07c1505a1f5139fe3e78835152a47ddbfce03e03b4dd12483972ed8b42518da9c931c3ad6d87fb6bd3178584dc9e28386361b6f3ebf54f68d4428de603767b63af69a5144f37ff5ae4338f807e4e4d02073532efc60a2abb59c24a9596a673dda1cbcd4e2a1d544b5f7765e020d894f2ab28041cebb855f235854efca70e9973c3578d07209c2b57b050732b02900c7ea9e5caee69138f9d2a711405b53d5fbddaee06d592536ca00a3fce7871568f3a87215fcbd01e3a6bd9c8ddf5170a566a59b937be6eeb62c993c337bb82f7c588358ed78709fc964a829dd4d087514e450c304125e0c6308c99b0c39bff643ba010e875dc9dd73ecb0fc00acf250b483e33072ce0430062347bf1ba4e442175a9b78b4c7855e35e0150089b3f966a0064583dd36a62027cbab325175f2c6616dccb2728ae490592605f391890dea53387f13c917ffbf27c8f2d8402013e1a8359c3e0343de37b8689a0295a852da5de9860880d10896f8e6eb1cfdd7bedc1853cf4f2e413902ae0b3cfa44f8fcd071dd614c01537f9556710e6c8b4e28406c2115a78259b2dd38d3dbc36f2c62bca7e24cbe0225765e1685802e89627ac26b8088aca3cf0202e47a8825ced8d5e9f791edaf20bad22335f04fd80ee24800f37745154c097fdcda3315c9a83e6e0638a8cd8bdeb6bc32e7366809d76eb78a57941ca3105bbdc6d590d5d3be340e5b62dd0a0c9ceac88b73d8f917af6a269b9418dd92a22bad22f8aaa78ec5b1e685f0b671b86f18cf1dda6827b6a824eef335041f66dab563b813bd103004d6124ee06de436fbe6514eaed64d36d9e6205a5465d17c5dd12c407191a7987885d14bb0ac25f4fc74c9e1b60fabffd275db1935c3c4267c3c68ee6e26ccd91e51294c57131a6d900dfcd5a6a42531944de7c40e8dd7f95f5a591a7fbc7fa9c46a2f5d04dcc36542f58b90f7491ad8f2347663a7850898058371b8dcb4e42b5f46a393ea52fc1fc248cd9e40cc0746144173136f7bfd042ea5a9c8ea770fd9fc6e9c3fb992bfbd7a38676f12faddfcaa63be5ad93b1214786232bbc483e7e799601f187e74776d9877a01972d0702d9e421fea488f32e817304167aa3b11f71143cc9cbc41a488c4532539b62941027ddaa7d2aa107c6fad6349fa210bc676f5293dca89d86358c1e9b81c9ca19d37bbfab4ac098405e99d1dd36b8a21dbe37f09502e718873c52dbd903395adfffcfc6a40647493d448eb3232ea8253bffa3bc96ce19b9cfbb91c9cd0be127a218e0f0dbde9f72b1cf044ba5265f6706a4ee4cd2fbdd4de1b7a9de7237034d07c1505a1f5139fe3e78835152a47ddbfce03e03b4dd12483972ed8b42518da9c931c3ad6d87fb6bd3178584dc9e28386361b6f3ebf54f68d4428de603767b63af69a5144f37ff5ae4338f807e4e4d02073532efc60a2abb59c24a9596a673dda1cbcd4e2a1d544b5f7765e020d894f2ab28041cebb855f235854efca70e9973c3578d07209c2b57b050732b02900c7ea9e5caee69138f9d2a711405b53d5fbddaee06d592536ca00a3fce7871568f3a87215fcbd01e3a6bd9c8ddf5170a566a59b937be6eeb62c993c337bb82f7c588358ed78709fc964a829dd4d087514e450c304125e0c6308c99b0c39bff643ba010e875dc9dd73ecb0fc00acf250b483e33072ce0430062347bf1ba4e442175a9b78b4c7855e35e0150089b3f966a0064583dd36a62027cbab325175f2c6616dccb2728ae490592605f391890dea53387f13c917ffbf27c8f2d8402013e1a8359c3e0343de37b8689a0295a852da5de9860880d10896f8e6eb1cfdd7bedc1853cf4f2e413902ae0b3cfa44f8fcd071dd614c01537f9556710e6c8b4e28406cca6275ff3c696d796caaac4306171448b1c9f9e8482726ab6c6986d77cfc4d1f8090cad2c60e02e47a8825ced8d5e9f791edaf20bad22335f04fd80ee24800f37745154c097fdcda3315c9a83e6e0638a8cd8bdeb6bc32e7366809d76eb78a57941ca3105bbdc6d590d5d3be340e5b62dd0a0c9ceac88b73d8f917af6a269b9418dd92a22bad22f8aaa78ec5b1e685f0b671b86f18cf1dda6827b6a824eef335041f66dab563b813bd103004d6124ee06de436fbe6514eaed64d36d9e6205a5465d17c5dd12c407191a7987885d14bb0ac25f4fc74c9e1b60fabffd275db1935c3c4267c3c68ee6e26ccd91e51294c57131a6d900dfcd5a6a42531944de7c40e8dd7f95f5a591a7fbc7fa9c46a2f5d04dcc36542f58b90f7491ad8f2347663a7850898058371b8dcb4e42b5f46a393ea52fc1fc248cd9e40cc0746144173136f7bfd042ea5a9c8ea770fd9fc6e9c3fb992bfbd7a38676f12faddfcaa63be5ad93b1214786232bbc483e7e799601f187e74776d9877a01972d0702d9e421fea488f32e817304167aa3b11f71143cc9cbc41a488c4532539b62941027ddaa7d2aa107c6fad6349fa210bc676f5293dca89d86358c1e9b81c9ca19d37bbfab4ac098405e99d1dd36b8a21dbe37f09502e718873c52dbd903395adfffcfc6a40647493d448eb3232ea8253bffa3bc96ce19b9cfbb91c9cd0be127a218e0f0dbde9f72b1cf044ba5265f6706a4ee4cd2fbdd4de1b7a9de7237034d07c1505a1f5139fe3e78835152a47ddbfce03e03b4dd12483972ed8b42518da9c931c3ad6d87fb6bd3178584dc9e28386361b6f3ebf54f68d4428de603767b63af69a5144f37ff5ae4338f807e4e4d02073532efc60a2abb59c24a9596a673dda1cbcd4e2a1d544b5f7765e020d894f2ab28041cebb855f235854efca70e9973c3578d07209c2b57b050732b02900c7ea9e5caee69138f9d2a711405b53d5fbddaee06d592536ca00a3fce7871568f3a87215fcbd01e3a6bd9c8ddf5170a566a59b937be6eeb62c993c337bb82f7c588358ed78709fc964a829dd4d087514e450c304125e0c6308c99b0c39bff643ba010e875dc9dd73ecb0fc00acf250b483e33072ce0430062347bf1ba4e442175a9b78b4c7855e35e0150089b3f966a0064583dd36a62027cbab325175f2c6616dccb2728ae490592605f391890dea53387f13c917ffbf27c8f2d8402013e1a8359c3e0343de37b8689a0295a852da5de9860880d10896f8e6eb1cfdd7bedc1853cf4f2e413902ae0b3cfa44f8fcd071dd614c01537f9556710e6c8b4e28406c0d437cce91ed1ed896eb4befc95f82c4a6c33d0b812cf3c01b7551cd43b706be80e08d84ddcb0102e47a8825ced8d5e9f791edaf20bad22335f04fd80ee24800f37745154c097fdcda3315c9a83e6e0638a8cd8bdeb6bc32e7366809d76eb78a57941ca3105bbdc6d590d5d3be340e5b62dd0a0c9ceac88b73d8f917af6a269b9418dd92a22bad22f8aaa78ec5b1e685f0b671b86f18cf1dda6827b6a824eef335041f66dab563b813bd103004d6124ee06de436fbe6514eaed64d36d9e6205a5465d17c5dd12c407191a7987885d14bb0ac25f4fc74c9e1b60fabffd275db1935c3c4267c3c68ee6e26ccd91e51294c57131a6d900dfcd5a6a42531944de7c40e8dd7f95f5a591a7fbc7fa9c46a2f5d04dcc36542f58b90f7491ad8f2347663a7850898058371b8dcb4e42b5f46a393ea52fc1fc248cd9e40cc0746144173136f7bfd042ea5a9c8ea770fd9fc6e9c3fb992bfbd7a38676f12faddfcaa63be5ad93b1214786232bbc483e7e799601f187e74776d9877a01972d0702d9e421fea488f32e817304167aa3b11f71143cc9cbc41a488c4532539b62941027ddaa7d2aa107c6fad6349fa210bc676f5293dca89d86358c1e9b81c9ca19d37bbfab4ac098405e99d1dd36b8a21dbe37f09502e718873c52dbd903395adfffcfc6a40647493d448eb3232ea8253bffa3bc96ce19b9cfbb91c9cd0be127a218e0f0dbde9f72b1cf044ba5265f6706a4ee4cd2fbdd4de1b7a9de7237034d07c1505a1f5139fe3e78835152a47ddbfce03e03b4dd12483972ed8b42518da9c931c3ad6d87fb6bd3178584dc9e28386361b6f3ebf54f68d4428de603767b63af69a5144f37ff5ae4338f807e4e4d02073532efc60a2abb59c24a9596a673dda1cbcd4e2a1d544b5f7765e020d894f2ab28041cebb855f235854efca70e9973c3578d07209c2b57b050732b02900c7ea9e5caee69138f9d2a711405b53d5fbddaee06d592536ca00a3fce7871568f3a87215fcbd01e3a6bd9c8ddf5170a566a59b937be6eeb62c993c337bb82f7c588358ed78709fc964a829dd4d087514e450c304125e0c6308c99b0c39bff643ba010e875dc9dd73ecb0fc00acf250b483e33072ce0430062347bf1ba4e442175a9b78b4c7855e35e0150089b3f966a0064583dd36a62027cbab325175f2c6616dccb2728ae490592605f391890dea53387f13c917ffbf27c8f2d8402013e1a8359c3e0343de37b8689a0295a852da5de9860880d10896f8e6eb1cfdd7bedc1853cf4f2e413902ae0b3cfa44f8fcd071dd614c01537f9556710e6c8b4e28406cc30678fa1dbce0a9ec0e4ba6095302d692da8b6c6c4a97c1bd07880f72c80e5580c0caf384a30202e47a8825ced8d5e9f791edaf20bad22335f04fd80ee24800f37745154c097fdcda3315c9a83e6e0638a8cd8bdeb6bc32e7366809d76eb78a57941ca3105bbdc6d590d5d3be340e5b62dd0a0c9ceac88b73d8f917af6a269b9418dd92a22bad22f8aaa78ec5b1e685f0b671b86f18cf1dda6827b6a824eef335041f66dab563b813bd103004d6124ee06de436fbe6514eaed64d36d9e6205a5465d17c5dd12c407191a7987885d14bb0ac25f4fc74c9e1b60fabffd275db1935c3c4267c3c68ee6e26ccd91e51294c57131a6d900dfcd5a6a42531944de7c40e8dd7f95f5a591a7fbc7fa9c46a2f5d04dcc36542f58b90f7491ad8f2347663a7850898058371b8dcb4e42b5f46a393ea52fc1fc248cd9e40cc0746144173136f7bfd042ea5a9c8ea770fd9fc6e9c3fb992bfbd7a38676f12faddfcaa63be5ad93b1214786232bbc483e7e799601f187e74776d9877a01972d0702d9e421fea488f32e817304167aa3b11f71143cc9cbc41a488c4532539b62941027ddaa7d2aa107c6fad6349fa210bc676f5293dca89d86358c1e9b81c9ca19d37bbfab4ac098405e99d1dd36b8a21dbe37f09502e718873c52dbd903395adfffcfc6a40647493d448eb3232ea8253bffa3bc96ce19b9cfbb91c9cd0be127a218e0f0dbde9f72b1cf044ba5265f6706a4ee4cd2fbdd4de1b7a9de7237034d07c1505a1f5139fe3e78835152a47ddbfce03e03b4dd12483972ed8b42518da9c931c3ad6d87fb6bd3178584dc9e28386361b6f3ebf54f68d4428de603767b63af69a5144f37ff5ae4338f807e4e4d02073532efc60a2abb59c24a9596a673dda1cbcd4e2a1d544b5f7765e020d894f2ab28041cebb855f235854efca70e9973c3578d07209c2b57b050732b02900c7ea9e5caee69138f9d2a711405b53d5fbddaee06d592536ca00a3fce7871568f3a87215fcbd01e3a6bd9c8ddf5170a566a59b937be6eeb62c993c337bb82f7c588358ed78709fc964a829dd4d087514e450c304125e0c6308c99b0c39bff643ba010e875dc9dd73ecb0fc00acf250b483e33072ce0430062347bf1ba4e442175a9b78b4c7855e35e0150089b3f966a0064583dd36a62027cbab325175f2c6616dccb2728ae490592605f391890dea53387f13c917ffbf27c8f2d8402013e1a8359c3e0343de37b8689a0295a852da5de9860880d10896f8e6eb1cfdd7bedc1853cf4f2e413902ae0b3cfa44f8fcd071dd614c01537f9556710e6c8b4e28406cffbe3bc8be47bee228b2e81f99717a84d44fe5eb24bbb70c588a973e872f82c38107011adacfb58efea94dc3f2f876a4a04759eaccea0b906abb5aa4ac06a1458bde117b17dce5c82cd19135292b205be45466814d507a0faa2c577b32465b06580f39e9bc8a3205d7e9f287585dae77972fb7361c9cb831d8c21013b14273b56466abec4f6b0f431d7eda50e14c377fd61413e1b6c03a0a41286dafda4eaf208f84ef0377f8912566b9b0b8fe9b2dfd512ba0d34ae74acf914f3a89f8901643aff21487d4f31718fbe6eacbcd22d0b65fbebb8d486e28a4f302b73fdaf6b29b72b48af69dd750583d1532eef66cee6bf091c83d7f7188ed5359ad12d85e484185a2871cb8e0d4b4314469dd3b35b4dd6ed7944debd6bd2b53d322884d74fee6460316090106182aefd648f6d9d3ef9c3a461541c5f9feb238027ec7f57b0832dfb22cb55746992298e9b4931d0e0cf456fb10424d9206a316e8a67b55e4529fb01e731cda5c0b9ef4154231b73ea02e04e33d9d0d766ea15dcca7152ea4588022190396134fdf54ba333f570d989e88dcf984aa2e22b9ccace855a772cbb68e20712e782a2dad3493d74e299f313587048c79474fa13b028a562e04133eaed9974791f9723399cd4d7043adc5d43d7f06e0193d7fa46c3f7e402982ef125335f24b8dae67a969f3781c65cec956825602ce5712f082bf41474a7be0d3db880aced1c029258c2926030146ceea6def8f0ed5bee356dc460754a69c2254c014000b54cb70f58b17f5826821a3166f1065f712d060349755cb967cf46be1727e17bbac0d6183dfa7bdf0379140f21e0120d50d66e87b0f3acf36f3be0e9b1b8ca6dd82b439415eaf79a1c79d6f756b63b03fb3cb0e72cabbcd21d4a9a76081c46e9d13f77c80ec9858ad4b1c6cefb60f77331a525525542746df0db184ea17f86be5875450694f136a4f0fee3e8c9f3ee770786f7675b7d3810ff466b8a10b8f9cbe7c7f61ba6535f4e18c351812f59ed6c1f9b9a423a61b16de78cee758f4d9f82834a2f53e5af5bedcdb9aaf3bca882b6f38ec65edb58fd5597b17cf159cd16f39c06508d6794aab628503cdd0594c447967833e9188177e25a12a7354bae647ee551992b38e33bf7cf5b3837bc58cefc1ce592d71e3eab4034f44052eec8441227dce3c19c27eddba7ac3efe04cbf2780615573dc5a72aba46ec053bbe4a766abd86805d9145bafda7e89d33aa51246b4d19b10e2a527bcd00b7ff70b464b8689067ba6bb0f46df19aa311ab5d56402b47b6a22508db0709600ae6755188396667a6b";
  uint32_t const GENESIS_NONCE = 10000;

  namespace testnet
  {
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 53;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 54;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 63;
    uint16_t const P2P_DEFAULT_PORT = 29090;
    uint16_t const RPC_DEFAULT_PORT = 29091;
    uint16_t const ZMQ_RPC_DEFAULT_PORT = 29092;
    boost::uuids::uuid const NETWORK_ID = { {
        0x12 ,0x30, 0xF1, 0x71 , 0x61, 0x04 , 0x41, 0x61, 0x17, 0x31, 0x00, 0x82, 0x16, 0xA1, 0xA1, 0x11
      } }; // Bender's daydream
      std::string const GENESIS_TX =
              "013c01ff0006ff9fdb580260f3b6ffbd3b7ea0d1b2629d26ccee7e243251e7d86cd69b1770daf8ea513cc4c1fd29e771a848511c967af8a8882a00546c217d22f84ba150395f6f62be605f07d6142f23118c992ea7853b975c0dce2a0c0c2e167fc7fc38b2105198ce19d129d8ff7a2adfb9093b91446948e35b475b559991b78acd6281128a6c1c84dd70d1820b7528f940618c3263b8fc8810ffdb8d271fd27d9c7729a335e63bbdf5931378262dd99149d299532212a341fa9da1a3d9c9ffdee11af5b8ba8596043ed49c4d454d09ee8ce9e7067bd93cdd29ae5ec9d18704dc971697c471272268e897ffa34e64fec63369af0aebdf469ba450dabeb60720dd45a8a0f61655f066827a08f07cd75fd9efee6249291f3b7905926bd7a8265da659430c8a21ef36bcf8518fa9ef60391e1e72334ee72e9ffa7f06a6b1a9afe912bd02978b332b3f2fd18ee3ea443be9447b8e50d665841c081dc7e61867c0101886f29ab0f73f596cee685617556f757974fa5744fe020babb7b8504da1f20d8b8c5b92fa3cabf95e6701e448d290777bc25cb19d77426e8c37ae891bb49cba1206c339579e3b0105e21029f088a0e7340ac2cea1b88d61c7f22e82ff1d07a49b8ed107ce2e5ec033331469fb85a46ef440e544a562b03940e6a7a7d9b4be52e1b5817c2bc085e54efd37c7fb1b5dd3d736e8d755e65221b22eab0cebe9aa5474271c11420dd9d761a315cb6f73e1d22ea9ea4b2482b34675f7b50583a1d059e4e4af859555f992b5b2bb5387a4f3d04e8f0612db9af53b0753756e66be9bc607c00d4ba54a1276e6dfe5dbeaac6b4a379591d4b67278d3077c4337319ee4156532447108a2cabaa347f7f17055c7bc88b4cdebb1e2c6fc7d92c1da1d06bff4eb892f1349d6f745ccae576832c505791e8f6c0ae1369e388a6946d8e66464524ec15a3727fb6a25f319fd8432404dd0362970c3b8ace91d9eaf26a0d8238c65c4670b8470376f9910c3154a1f589f7f54ef5da1df8106f3db19ea337d28ed507455ffbea07e4bcba29bed67491681a3bfa572f36bccd9a906eefce038907e2a8fa73b6f32c7e56bcfdc31cf7db462929b386787961539a04d9cb0bc724be55d2f759ec115903267d1fb3d09be4ab2a28abaad9c2ae47352c5c33ca49b7c27a69514129e23c0fe03c5f0669034a5be9786821b59ba7638bd06098fea23ce175838f2f5a8da75de72f80b7c142f7c3023970bdf262d9333839fff1e6dc9ec58d2d192a81888b0d0a39bd378530f8afbc74536b9a963b4f1c4cb738bcea7403d4d606b6e074ec5d3baf39d1880a8d6b9070260f3b6ffbd3b7ea0d1b2629d26ccee7e243251e7d86cd69b1770daf8ea513cc4c1fd29e771a848511c967af8a8882a00546c217d22f84ba150395f6f62be605f07d6142f23118c992ea7853b975c0dce2a0c0c2e167fc7fc38b2105198ce19d129d8ff7a2adfb9093b91446948e35b475b559991b78acd6281128a6c1c84dd70d1820b7528f940618c3263b8fc8810ffdb8d271fd27d9c7729a335e63bbdf5931378262dd99149d299532212a341fa9da1a3d9c9ffdee11af5b8ba8596043ed49c4d454d09ee8ce9e7067bd93cdd29ae5ec9d18704dc971697c471272268e897ffa34e64fec63369af0aebdf469ba450dabeb60720dd45a8a0f61655f066827a08f07cd75fd9efee6249291f3b7905926bd7a8265da659430c8a21ef36bcf8518fa9ef60391e1e72334ee72e9ffa7f06a6b1a9afe912bd02978b332b3f2fd18ee3ea443be9447b8e50d665841c081dc7e61867c0101886f29ab0f73f596cee685617556f757974fa5744fe020babb7b8504da1f20d8b8c5b92fa3cabf95e6701e448d290777bc25cb19d77426e8c37ae891bb49cba1206c339579e3b0105e21029f088a0e7340ac2cea1b88d61c7f22e82ff1d07a49b8ed107ce2e5ec033331469fb85a46ef440e544a562b03940e6a7a7d9b4be52e1b5817c2bc085e54efd37c7fb1b5dd3d736e8d755e65221b22eab0cebe9aa5474271c11420dd9d761a315cb6f73e1d22ea9ea4b2482b34675f7b50583a1d059e4e4af859555f992b5b2bb5387a4f3d04e8f0612db9af53b0753756e66be9bc607c00d4ba54a1276e6dfe5dbeaac6b4a379591d4b67278d3077c4337319ee4156532447108a2cabaa347f7f17055c7bc88b4cdebb1e2c6fc7d92c1da1d06bff4eb892f1349d6f745ccae576832c505791e8f6c0ae1369e388a6946d8e66464524ec15a3727fb6a25f319fd8432404dd0362970c3b8ace91d9eaf26a0d8238c65c4670b8470376f9910c3154a1f589f7f54ef5da1df8106f3db19ea337d28ed507455ffbea07e4bcba29bed67491681a3bfa572f36bccd9a906eefce038907e2a8fa73b6f32c7e56bcfdc31cf7db462929b386787961539a04d9cb0bc724be55d2f759ec115903267d1fb3d09be4ab2a28abaad9c2ae47352c5c33ca49b7c27a69514129e23c0fe03c5f0669034a5be9786821b59ba7638bd06098fea23ce175838f2f5a8da75de72f80b7c142f7c3023970bdf262d9333839fff1e6dc9ec58d2d192a81888b0d0a39bd3782115a78259b2dd38d3dbc36f2c62bca7e24cbe0225765e1685802e89627ac26b8088aca3cf020260f3b6ffbd3b7ea0d1b2629d26ccee7e243251e7d86cd69b1770daf8ea513cc4c1fd29e771a848511c967af8a8882a00546c217d22f84ba150395f6f62be605f07d6142f23118c992ea7853b975c0dce2a0c0c2e167fc7fc38b2105198ce19d129d8ff7a2adfb9093b91446948e35b475b559991b78acd6281128a6c1c84dd70d1820b7528f940618c3263b8fc8810ffdb8d271fd27d9c7729a335e63bbdf5931378262dd99149d299532212a341fa9da1a3d9c9ffdee11af5b8ba8596043ed49c4d454d09ee8ce9e7067bd93cdd29ae5ec9d18704dc971697c471272268e897ffa34e64fec63369af0aebdf469ba450dabeb60720dd45a8a0f61655f066827a08f07cd75fd9efee6249291f3b7905926bd7a8265da659430c8a21ef36bcf8518fa9ef60391e1e72334ee72e9ffa7f06a6b1a9afe912bd02978b332b3f2fd18ee3ea443be9447b8e50d665841c081dc7e61867c0101886f29ab0f73f596cee685617556f757974fa5744fe020babb7b8504da1f20d8b8c5b92fa3cabf95e6701e448d290777bc25cb19d77426e8c37ae891bb49cba1206c339579e3b0105e21029f088a0e7340ac2cea1b88d61c7f22e82ff1d07a49b8ed107ce2e5ec033331469fb85a46ef440e544a562b03940e6a7a7d9b4be52e1b5817c2bc085e54efd37c7fb1b5dd3d736e8d755e65221b22eab0cebe9aa5474271c11420dd9d761a315cb6f73e1d22ea9ea4b2482b34675f7b50583a1d059e4e4af859555f992b5b2bb5387a4f3d04e8f0612db9af53b0753756e66be9bc607c00d4ba54a1276e6dfe5dbeaac6b4a379591d4b67278d3077c4337319ee4156532447108a2cabaa347f7f17055c7bc88b4cdebb1e2c6fc7d92c1da1d06bff4eb892f1349d6f745ccae576832c505791e8f6c0ae1369e388a6946d8e66464524ec15a3727fb6a25f319fd8432404dd0362970c3b8ace91d9eaf26a0d8238c65c4670b8470376f9910c3154a1f589f7f54ef5da1df8106f3db19ea337d28ed507455ffbea07e4bcba29bed67491681a3bfa572f36bccd9a906eefce038907e2a8fa73b6f32c7e56bcfdc31cf7db462929b386787961539a04d9cb0bc724be55d2f759ec115903267d1fb3d09be4ab2a28abaad9c2ae47352c5c33ca49b7c27a69514129e23c0fe03c5f0669034a5be9786821b59ba7638bd06098fea23ce175838f2f5a8da75de72f80b7c142f7c3023970bdf262d9333839fff1e6dc9ec58d2d192a81888b0d0a39bd378ca6275ff3c696d796caaac4306171448b1c9f9e8482726ab6c6986d77cfc4d1f8090cad2c60e0260f3b6ffbd3b7ea0d1b2629d26ccee7e243251e7d86cd69b1770daf8ea513cc4c1fd29e771a848511c967af8a8882a00546c217d22f84ba150395f6f62be605f07d6142f23118c992ea7853b975c0dce2a0c0c2e167fc7fc38b2105198ce19d129d8ff7a2adfb9093b91446948e35b475b559991b78acd6281128a6c1c84dd70d1820b7528f940618c3263b8fc8810ffdb8d271fd27d9c7729a335e63bbdf5931378262dd99149d299532212a341fa9da1a3d9c9ffdee11af5b8ba8596043ed49c4d454d09ee8ce9e7067bd93cdd29ae5ec9d18704dc971697c471272268e897ffa34e64fec63369af0aebdf469ba450dabeb60720dd45a8a0f61655f066827a08f07cd75fd9efee6249291f3b7905926bd7a8265da659430c8a21ef36bcf8518fa9ef60391e1e72334ee72e9ffa7f06a6b1a9afe912bd02978b332b3f2fd18ee3ea443be9447b8e50d665841c081dc7e61867c0101886f29ab0f73f596cee685617556f757974fa5744fe020babb7b8504da1f20d8b8c5b92fa3cabf95e6701e448d290777bc25cb19d77426e8c37ae891bb49cba1206c339579e3b0105e21029f088a0e7340ac2cea1b88d61c7f22e82ff1d07a49b8ed107ce2e5ec033331469fb85a46ef440e544a562b03940e6a7a7d9b4be52e1b5817c2bc085e54efd37c7fb1b5dd3d736e8d755e65221b22eab0cebe9aa5474271c11420dd9d761a315cb6f73e1d22ea9ea4b2482b34675f7b50583a1d059e4e4af859555f992b5b2bb5387a4f3d04e8f0612db9af53b0753756e66be9bc607c00d4ba54a1276e6dfe5dbeaac6b4a379591d4b67278d3077c4337319ee4156532447108a2cabaa347f7f17055c7bc88b4cdebb1e2c6fc7d92c1da1d06bff4eb892f1349d6f745ccae576832c505791e8f6c0ae1369e388a6946d8e66464524ec15a3727fb6a25f319fd8432404dd0362970c3b8ace91d9eaf26a0d8238c65c4670b8470376f9910c3154a1f589f7f54ef5da1df8106f3db19ea337d28ed507455ffbea07e4bcba29bed67491681a3bfa572f36bccd9a906eefce038907e2a8fa73b6f32c7e56bcfdc31cf7db462929b386787961539a04d9cb0bc724be55d2f759ec115903267d1fb3d09be4ab2a28abaad9c2ae47352c5c33ca49b7c27a69514129e23c0fe03c5f0669034a5be9786821b59ba7638bd06098fea23ce175838f2f5a8da75de72f80b7c142f7c3023970bdf262d9333839fff1e6dc9ec58d2d192a81888b0d0a39bd3780d437cce91ed1ed896eb4befc95f82c4a6c33d0b812cf3c01b7551cd43b706be80e08d84ddcb010260f3b6ffbd3b7ea0d1b2629d26ccee7e243251e7d86cd69b1770daf8ea513cc4c1fd29e771a848511c967af8a8882a00546c217d22f84ba150395f6f62be605f07d6142f23118c992ea7853b975c0dce2a0c0c2e167fc7fc38b2105198ce19d129d8ff7a2adfb9093b91446948e35b475b559991b78acd6281128a6c1c84dd70d1820b7528f940618c3263b8fc8810ffdb8d271fd27d9c7729a335e63bbdf5931378262dd99149d299532212a341fa9da1a3d9c9ffdee11af5b8ba8596043ed49c4d454d09ee8ce9e7067bd93cdd29ae5ec9d18704dc971697c471272268e897ffa34e64fec63369af0aebdf469ba450dabeb60720dd45a8a0f61655f066827a08f07cd75fd9efee6249291f3b7905926bd7a8265da659430c8a21ef36bcf8518fa9ef60391e1e72334ee72e9ffa7f06a6b1a9afe912bd02978b332b3f2fd18ee3ea443be9447b8e50d665841c081dc7e61867c0101886f29ab0f73f596cee685617556f757974fa5744fe020babb7b8504da1f20d8b8c5b92fa3cabf95e6701e448d290777bc25cb19d77426e8c37ae891bb49cba1206c339579e3b0105e21029f088a0e7340ac2cea1b88d61c7f22e82ff1d07a49b8ed107ce2e5ec033331469fb85a46ef440e544a562b03940e6a7a7d9b4be52e1b5817c2bc085e54efd37c7fb1b5dd3d736e8d755e65221b22eab0cebe9aa5474271c11420dd9d761a315cb6f73e1d22ea9ea4b2482b34675f7b50583a1d059e4e4af859555f992b5b2bb5387a4f3d04e8f0612db9af53b0753756e66be9bc607c00d4ba54a1276e6dfe5dbeaac6b4a379591d4b67278d3077c4337319ee4156532447108a2cabaa347f7f17055c7bc88b4cdebb1e2c6fc7d92c1da1d06bff4eb892f1349d6f745ccae576832c505791e8f6c0ae1369e388a6946d8e66464524ec15a3727fb6a25f319fd8432404dd0362970c3b8ace91d9eaf26a0d8238c65c4670b8470376f9910c3154a1f589f7f54ef5da1df8106f3db19ea337d28ed507455ffbea07e4bcba29bed67491681a3bfa572f36bccd9a906eefce038907e2a8fa73b6f32c7e56bcfdc31cf7db462929b386787961539a04d9cb0bc724be55d2f759ec115903267d1fb3d09be4ab2a28abaad9c2ae47352c5c33ca49b7c27a69514129e23c0fe03c5f0669034a5be9786821b59ba7638bd06098fea23ce175838f2f5a8da75de72f80b7c142f7c3023970bdf262d9333839fff1e6dc9ec58d2d192a81888b0d0a39bd378c30678fa1dbce0a9ec0e4ba6095302d692da8b6c6c4a97c1bd07880f72c80e5580c0caf384a3020260f3b6ffbd3b7ea0d1b2629d26ccee7e243251e7d86cd69b1770daf8ea513cc4c1fd29e771a848511c967af8a8882a00546c217d22f84ba150395f6f62be605f07d6142f23118c992ea7853b975c0dce2a0c0c2e167fc7fc38b2105198ce19d129d8ff7a2adfb9093b91446948e35b475b559991b78acd6281128a6c1c84dd70d1820b7528f940618c3263b8fc8810ffdb8d271fd27d9c7729a335e63bbdf5931378262dd99149d299532212a341fa9da1a3d9c9ffdee11af5b8ba8596043ed49c4d454d09ee8ce9e7067bd93cdd29ae5ec9d18704dc971697c471272268e897ffa34e64fec63369af0aebdf469ba450dabeb60720dd45a8a0f61655f066827a08f07cd75fd9efee6249291f3b7905926bd7a8265da659430c8a21ef36bcf8518fa9ef60391e1e72334ee72e9ffa7f06a6b1a9afe912bd02978b332b3f2fd18ee3ea443be9447b8e50d665841c081dc7e61867c0101886f29ab0f73f596cee685617556f757974fa5744fe020babb7b8504da1f20d8b8c5b92fa3cabf95e6701e448d290777bc25cb19d77426e8c37ae891bb49cba1206c339579e3b0105e21029f088a0e7340ac2cea1b88d61c7f22e82ff1d07a49b8ed107ce2e5ec033331469fb85a46ef440e544a562b03940e6a7a7d9b4be52e1b5817c2bc085e54efd37c7fb1b5dd3d736e8d755e65221b22eab0cebe9aa5474271c11420dd9d761a315cb6f73e1d22ea9ea4b2482b34675f7b50583a1d059e4e4af859555f992b5b2bb5387a4f3d04e8f0612db9af53b0753756e66be9bc607c00d4ba54a1276e6dfe5dbeaac6b4a379591d4b67278d3077c4337319ee4156532447108a2cabaa347f7f17055c7bc88b4cdebb1e2c6fc7d92c1da1d06bff4eb892f1349d6f745ccae576832c505791e8f6c0ae1369e388a6946d8e66464524ec15a3727fb6a25f319fd8432404dd0362970c3b8ace91d9eaf26a0d8238c65c4670b8470376f9910c3154a1f589f7f54ef5da1df8106f3db19ea337d28ed507455ffbea07e4bcba29bed67491681a3bfa572f36bccd9a906eefce038907e2a8fa73b6f32c7e56bcfdc31cf7db462929b386787961539a04d9cb0bc724be55d2f759ec115903267d1fb3d09be4ab2a28abaad9c2ae47352c5c33ca49b7c27a69514129e23c0fe03c5f0669034a5be9786821b59ba7638bd06098fea23ce175838f2f5a8da75de72f80b7c142f7c3023970bdf262d9333839fff1e6dc9ec58d2d192a81888b0d0a39bd378ffbe3bc8be47bee228b2e81f99717a84d44fe5eb24bbb70c588a973e872f82c38107018cb58f7b0a79ea38c2cec367e29b9bb515d1ac96847156203d346116bbd4e94fa2d9833f4ceb29854538798046983a5affa88204ae7df172dc98f071ac445e4034ef18ee90f20ab9fd2f057ed5c3ad79ee2d228d11b83cafd1f1b806e6ebda861db36b65d219e8117f26976cc97b7d06cd83c2be26e7521db6afb78207ca2f428e608607941de67ed3ad5e0b6e97ffe809ca58e7e232f96261537528e8fd08aaae772988ef97cb35e39edd8f9f951fc84e5a5a8775c7d931e214a06a2f6ecec2c904290a95232be1aa99c0b457b9666499e39265659d6b0c72874e24b6ce49544069decd103dcf6cb40d3c0ce463f8838fa10414251d72476cdfce3cc078b4c57373f27c89798229d5247d3f51153d96b066aa7a8bb82e6ffa6d0a306292180189ecf22ba4e4372b1ca6356bef0a7a2b83d605ec9e1c63a0fe52b294e5cf59bf89d635839063c3e78a1cbe2447ecb84104a434eb86fdedccc6b9521ce6241c11364231875b44583257fda1346416af0729262b49e28fa2459619c670c49881808d10535698987bc8203290c0e8f44c50f6013e359ff617fbdc82f22da3b0df78d8162b8dd10aed1a7588eb9975db4dab9376fab49ebb1992c2f726ea6da84002a316a343710ac0666db06c27e2b36e0a49209e699806caed25e4f7bf531536a15b289c1e8db770a6a77d2ae58bedc5d9174cac1f58a71ecfcd64550ab4a77753e1c7034629afade068fa282439155a2c6bfeb402067a6d3da360b09ac14ed10d5bf3c094f4e32336172f782c9795b763977c03b84dbc3d584e40aa974d908eadc519d5b16241bd4ab49bc0b7f657d3b7e2320d78699f1a73808741cbc9db54841e0472e755b3628edffb1debd85f51e9266c5fd8ef747fde1b021b71eaa46299c8e351337c654aeb4ebf8fc1f568aa84c194f50aad8faedbb5893a71151d08cb627eb89938429ec6e8a0ce1ac1bcdb8c612cdc4e134920938a99d5bd560e8a5b198f74334ff84528b147f27b96dd67d02000aaff402037a4d4b7f82524ed747e2abb9049d903e9a6e13f67ca0bcafd29250504f354663d9ddf4b6fc58c35d39808590f2f697506571d59279abac703f6f15692c127400ecb5f8302cd5ae5c29135dc6114a12f47d97613ab2354b123102d0037348a64532e4d136dd2b6c0ff75dad8c934a0b16c27544bbc42015e4db8c0d3e58595749fc743ab5844bdd9f4b3e86ebd7b43dbdd7446ee60c25986ed75e8a122ec2da1035c80f62772bd968361";
    uint32_t const GENESIS_NONCE = 10001;
  }

  namespace stagenet
  {
    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 24;
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 25;
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 36;
    uint16_t const P2P_DEFAULT_PORT = 39090;
    uint16_t const RPC_DEFAULT_PORT = 39091;
    uint16_t const ZMQ_RPC_DEFAULT_PORT = 39092;
    boost::uuids::uuid const NETWORK_ID = { {
        0x12 ,0x30, 0xF1, 0x71 , 0x61, 0x04 , 0x41, 0x61, 0x17, 0x31, 0x00, 0x82, 0x16, 0xA1, 0xA1, 0x12
      } }; // Bender's daydream
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

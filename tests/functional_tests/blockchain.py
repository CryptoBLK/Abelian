#!/usr/bin/env python3

# Copyright (c) 2018 The Monero Project
# 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this list of
#    conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice, this list
#    of conditions and the following disclaimer in the documentation and/or other
#    materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its contributors may be
#    used to endorse or promote products derived from this software without specific
#    prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Test blockchain RPC calls

Test the following RPCs:
    - get_info
    - generateblocks
    - [TODO: many tests still need to be written]

"""

from test_framework.daemon import Daemon
from test_framework.wallet import Wallet

mainnet_address ='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'

class BlockchainTest():
    def run_test(self):
        self._test_get_info()
        self._test_hardfork_info()
        self._test_generateblocks(5)

    def _test_get_info(self):
        print('Test get_info')

        daemon = Daemon()
        res = daemon.get_info()

        # difficulty should be set to 1 for this test
        assert 'difficulty' in res.keys()
        assert res['difficulty'] == 1;

        # nettype should not be TESTNET
        assert 'testnet' in res.keys()
        assert res['testnet'] == False;

        # nettype should not be STAGENET
        assert 'stagenet' in res.keys()
        assert res['stagenet'] == False;

        # nettype should be FAKECHAIN
        assert 'nettype' in res.keys()
        assert res['nettype'] == "fakechain";

        # free_space should be > 0
        assert 'free_space' in res.keys()
        assert res['free_space'] > 0

        # height should be greater or equal to 1
        assert 'height' in res.keys()
        assert res['height'] >= 1


    def _test_hardfork_info(self):
        print('Test hard_fork_info')

        daemon = Daemon()
        res = daemon.hard_fork_info()

        print(res['earliest_height'])

        assert 'earliest_height' in res.keys()
        assert res['earliest_height'] == 1;


    def _test_generateblocks(self, blocks):
        print("Test generating", blocks, 'blocks')

        daemon = Daemon()
        res = daemon.get_info()
        height = res['height']
        res = daemon.generateblocks(mainnet_address, blocks)

        assert res['height'] == height + blocks - 1


if __name__ == '__main__':
    BlockchainTest().run_test()

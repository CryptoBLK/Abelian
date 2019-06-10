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

# Testnet 1.0 Address
mainnet_address = '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 '

# seed
'''vats austere phase jargon drinks pierce puffin ounce
inmate goggles payment gyrate ferry illness rest reunion
nodes worry bevel stockpile casket soil inactive knee worry'''


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
        assert res['difficulty'] == 1

        # Remove the checks below for Testnet 1.0

        # nettype should not be Mainnet
        assert 'mainnet' in res.keys()
        assert res['mainnet'] == False;

        # nettype should not be TESTNET
        # assert 'testnet' in res.keys()
        # assert res['testnet'] == False;

        # nettype should not be STAGENET
        # assert 'stagenet' in res.keys()
        # assert res['stagenet'] == False;

        # nettype should be FAKECHAIN
        # assert 'nettype' in res.keys()
        # assert res['nettype'] == "fakechain";

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

        assert 'earliest_height' in res.keys()
        assert res['earliest_height'] == 1

    def _test_generateblocks(self, blocks):
        print("Test generating", blocks, 'blocks')

        daemon = Daemon()
        res = daemon.get_info()
        height = res['height']
        res = daemon.generateblocks(mainnet_address, blocks)

        assert res['height'] == height + blocks - 1


if __name__ == '__main__':
    BlockchainTest().run_test()

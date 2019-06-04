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

"""Test speed of various procedures

Test the following RPCs:
    - generateblocks
    - transfer
    - [TODO: many tests still need to be written]

"""

mainnet_address_1 = \
    '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'

mainnet_address_2 = \
    '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'

# seed 9vAWKqSNpqYNj
'''website elapse unusual relic wallets suede joining husband
randomly maximum junk obtains timber unsafe upright estate
last withdrawn odometer duets today faked abyss ajar website'''

# seed 9x3o33gbooG1PZ
'''vats austere phase jargon drinks pierce puffin ounce
inmate goggles payment gyrate ferry illness rest reunion
nodes worry bevel stockpile casket soil inactive knee worry'''

import time
from time import sleep
from decimal import Decimal

from test_framework.daemon import Daemon
from test_framework.wallet import Wallet


class SpeedTest():
    def set_test_params(self):
        self.num_nodes = 1

    def run_test(self):
        daemon = Daemon()
        wallet = Wallet()

        destinations = wallet.make_uniform_destinations(mainnet_address_2,1,3)

        self._test_speed_generateblocks(daemon=daemon, blocks=70)
        for i in range(1, 10):
            while wallet.get_balance()['unlocked_balance'] == 0:
                print('Waiting for wallet to refresh...')
                sleep(1)
            self._test_speed_transfer_split(wallet=wallet)
        self._test_speed_generateblocks(daemon=daemon, blocks=10)

    def _test_speed_generateblocks(self, daemon, blocks):
        print('Test speed of block generation')
        start = time.time()

        res = daemon.generateblocks(mainnet_address_1, blocks)

        print('generating ', blocks, 'blocks took: ', time.time() - start, 'seconds')

    def _test_speed_transfer_split(self, wallet):
        print('Test speed of transfer')
        start = time.time()

        destinations = wallet.make_uniform_destinations(mainnet_address_2,1)
        res = wallet.transfer_split(destinations, 1)

        print('generating tx took: ', time.time() - start, 'seconds')


if __name__ == '__main__':
    SpeedTest().run_test()

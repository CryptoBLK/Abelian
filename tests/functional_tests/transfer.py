#!/usr/bin/env python3
""" Test transfer RPC calls

Test for the following RPC calls:
    - get_balance
    - transfer
"""
import time
from time import sleep

from test_framework.daemon import Daemon
from test_framework.wallet import Wallet

mainnet_address_1 = '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'

mainnet_address_2 = '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'

class AbelianTransferTest():
    def set_test_params(self):
        self.num_nodes = 1

    def run_test(self):
        daemon = Daemon()
        wallet = Wallet()

        destination = wallet.make_uniform_destinations(
            mainnet_address_2, 1)

        # Pre-mining
        self.mine(daemon=daemon, blocks=400)

        # Start Transfer - 1
        self.transfer_100(wallet)
        self.mine(daemon=daemon, blocks=10)

        # Start Transfer - 2
        self.transfer_100_random(wallet)
        self.mine(daemon=daemon, blocks=10)

    def mine(self, daemon, blocks):
        print("Mining some blocks")
        start = time.time()
        daemon.generateblocks(mainnet_address_1, blocks)
        print('Generating 500 blocks took: ', time.time() - start, 'seconds')

    def transfer_100(self, wallet):
        print("Initiating 100 transfers(1 ABE)")

        start = time.time()

        dest = wallet.make_uniform_destinations(mainnet_address_2, 1)
        for x in range (0, 100):
            while wallet.get_balance()['unlocked_balance'] == 0:
                print('Waiting for wallet to refresh...')
                sleep(1)
            print('Sending % 2d transaction' % (x))
            res = wallet.transfer(dest, 1)
            print(res)

        print('Balance: %2d' % wallet.get_balance()['unlocked_balance'])
        print('Transfer tx took: ', time.time() - start, 'seconds')

    def transfer_100_random(self, wallet):
        print("Initiating 100 transfers(10, 100, 1000 ABE)")

        start = time.time()

        dest_10 = wallet.make_uniform_destinations(mainnet_address_2, 10)
        dest_100 = wallet.make_uniform_destinations(mainnet_address_2, 100)
        dest_1000 = wallet.make_uniform_destinations(mainnet_address_2, 1000)

        for x in range (0, 10):
            while wallet.get_balance()['unlocked_balance'] == 0:
                print('Waiting for wallet to refresh...')
                sleep(1)

            print('Sending % 2d transaction' % (x))
            res = wallet.transfer(dest_10, 1)
            print(res)

            res = wallet.transfer(dest_100, 1)
            print(res)

            res = wallet.transfer(dest_1000)
            print(res)

        print('Transfer tx took: ', time.time() - start, 'seconds')



if __name__ == '__main__':
    AbelianTransferTest().run_test()


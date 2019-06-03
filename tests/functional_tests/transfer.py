#!/usr/bin/env python3

# Copyright (c) 2019 The Monero Project
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

from __future__ import print_function
import json

"""Test simple transfers
"""

from framework.daemon import Daemon
from framework.wallet import Wallet

class TransferTest():
""" Test transfer RPC calls

Test for the following RPC calls:
    - get_balance
    - transfer
"""

from test_framework.daemon import Daemon
from test_framework.wallet import Wallet

mainnet_address_1 = '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'

mainnet_address_2 = '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'

mainnet_address_3 = '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'

wallet_seeds = ['humid examine camp major orphans stick testing acumen rays etiquette goat jabbed arbitrary pheasants dice hedgehog emotion psychic boyfriend sulking slug fowls vehicle vowels examine',
'siren certain menu lamb love huts dullness sagabicycle ungainly anvil foes aching oneself skirting picked hydrogen mirror else abnormal otherwise magically pigment popular anvil', 'deity jetting guest vague tiers fossil rapid vacation system afraid jaws cool duckling comb elope guys jagged abducts eden inundate aunt evicted abort amidst evicted']

class AbelianTransferTest():
    def run_test(self):
        self.reset()
        self.create()
        self.mine()
        self.transfer()
        self.check_get_bulk_payments()
        self.check_double_spend_detection()
        self.sweep_single()

    def reset(self):
        print('Resetting blockchain')

    def reset(self):
        print('Reset Abelian Blockchain')
        daemon = Daemon()
        daemon.pop_blocks(1000)
        daemon.flush_txpool()

    def create(self):
        print('Creating wallets')
        seeds = [
          'velvet lymph giddy number token physics poetry unquoted nibs useful sabotage limits benches lifestyle eden nitrogen anvil fewest avoid batch vials washing fences goat unquoted',
          'peeled mixture ionic radar utopia puddle buying illness nuns gadget river spout cavernous bounced paradise drunk looking cottage jump tequila melting went winter adjust spout',
          'dilute gutter certain antics pamphlet macro enjoy left slid guarded bogeys upload nineteen bomb jubilee enhanced irritate turnip eggs swung jukebox loudly reduce sedan slid',
        ]
        self.wallet = [None] * len(seeds)
        for i in range(len(seeds)):
            self.wallet[i] = Wallet(idx = i)
            # close the wallet if any, will throw if none is loaded
            try: self.wallet[i].close_wallet()
            except: pass
            res = self.wallet[i].restore_deterministic_wallet(seed = seeds[i])

    def mine(self):
        print("Mining some blocks")
        daemon = Daemon()

        res = daemon.get_info()
        height = res.height

        daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 80)
        for i in range(len(self.wallet)):
            self.wallet[i].refresh()
            res = self.wallet[i].get_height()
            assert res.height == height + 80

    def transfer(self):
        daemon = Daemon()

        print("Creating transfer to self")

        dst = {'address': '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 'amount': 1000000000000}
        payment_id = '1234500000012345abcde00000abcdeff1234500000012345abcde00000abcde'

        start_balances = [0] * len(self.wallet)
        running_balances = [0] * len(self.wallet)
        for i in range(len(self.wallet)):
          res = self.wallet[i].get_balance()
          start_balances[i] = res.balance
          running_balances[i] = res.balance
          assert res.unlocked_balance <= res.balance
          if i == 0:
            assert res.blocks_to_unlock == 59 # we've been mining to it
          else:
            assert res.blocks_to_unlock == 0

        print ('Checking short payment IDs cannot be used when not in an integrated address')
        ok = False
        try: self.wallet[0].transfer([dst], ring_size = 11, payment_id = '1234567812345678', get_tx_key = False)
        except: ok = True
        assert ok

        print ('Checking empty destination is rejected')
        ok = False
        try: self.wallet[0].transfer([], ring_size = 11, get_tx_key = False)
        except: ok = True
        assert ok

        res = self.wallet[0].transfer([dst], ring_size = 11, payment_id = payment_id, get_tx_key = False)
        assert len(res.tx_hash) == 32*2
        txid = res.tx_hash
        assert len(res.tx_key) == 0
        assert res.amount > 0
        amount = res.amount
        assert res.fee > 0
        fee = res.fee
        assert len(res.tx_blob) == 0
        assert len(res.tx_metadata) == 0
        assert len(res.multisig_txset) == 0
        assert len(res.unsigned_txset) == 0
        unsigned_txset = res.unsigned_txset

        self.wallet[0].refresh()

        res = daemon.get_info()
        height = res.height

        res = self.wallet[0].get_transfers()
        assert len(res['in']) == height - 1 # coinbases
        assert not 'out' in res or len(res.out) == 0 # not mined yet
        assert len(res.pending) == 1
        assert not 'pool' in res or len(res.pool) == 0
        assert not 'failed' in res or len(res.failed) == 0
        for e in res['in']:
          assert e.type == 'block'
        e = res.pending[0]
        assert e.txid == txid
        assert e.payment_id == payment_id
        assert e.type == 'pending'
        assert e.unlock_time == 0
        assert e.subaddr_index.major == 0
        assert e.subaddr_indices == [{'major': 0, 'minor': 0}]
        assert e.address == '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm'
        assert e.double_spend_seen == False
        assert e.confirmations == 0

        running_balances[0] -= 1000000000000 + fee

        res = self.wallet[0].get_balance()
        assert res.balance == running_balances[0]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 59

        daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 1)
        res = daemon.getlastblockheader()
        running_balances[0] += res.block_header.reward
        self.wallet[0].refresh()

        running_balances[0] += 1000000000000

        res = self.wallet[0].get_transfers()
        assert len(res['in']) == height # coinbases
        assert len(res.out) == 1 # not mined yet
        assert not 'pending' in res or len(res.pending) == 0
        assert not 'pool' in res or len(res.pool) == 0
        assert not 'failed' in res or len(res.failed) == 0
        for e in res['in']:
          assert e.type == 'block'
        e = res.out[0]
        assert e.txid == txid
        assert e.payment_id == payment_id
        assert e.type == 'out'
        assert e.unlock_time == 0
        assert e.subaddr_index.major == 0
        assert e.subaddr_indices == [{'major': 0, 'minor': 0}]
        assert e.address == '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm'
        assert e.double_spend_seen == False
        assert e.confirmations == 1

        res = self.wallet[0].get_height()
        wallet_height = res.height
        res = self.wallet[0].get_transfer_by_txid(txid)
        assert len(res.transfers) == 1
        assert res.transfers[0] == res.transfer
        t = res.transfer
        assert t.txid == txid
        assert t.payment_id == payment_id
        assert t.height == wallet_height - 1
        assert t.timestamp > 0
        assert t.amount == 0 # to self, so it's just "pay a fee" really
        assert t.fee == fee
        assert t.note == ''
        assert len(t.destinations) == 1
        assert t.destinations[0] == {'address': '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 'amount': 1000000000000}
        assert t.type == 'out'
        assert t.unlock_time == 0
        assert t.address == '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm'
        assert t.double_spend_seen == False
        assert t.confirmations == 1

        res = self.wallet[0].get_balance()
        assert res.balance == running_balances[0]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 59

        print("Creating transfer to another, manual relay")

        dst = {'address': '44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW', 'amount': 1000000000000}
        res = self.wallet[0].transfer([dst], ring_size = 11, payment_id = payment_id, get_tx_key = True, do_not_relay = True, get_tx_hex = True)
        assert len(res.tx_hash) == 32*2
        txid = res.tx_hash
        assert len(res.tx_key) == 32*2
        assert res.amount == 1000000000000
        amount = res.amount
        assert res.fee > 0
        fee = res.fee
        assert len(res.tx_blob) > 0
        assert len(res.tx_metadata) == 0
        assert len(res.multisig_txset) == 0
        assert len(res.unsigned_txset) == 0
        tx_blob = res.tx_blob

        res = daemon.send_raw_transaction(tx_blob)
        assert res.not_relayed == False
        assert res.low_mixin == False
        assert res.double_spend == False
        assert res.invalid_input == False
        assert res.invalid_output == False
        assert res.too_big == False
        assert res.overspend == False
        assert res.fee_too_low == False
        assert res.not_rct == False

        self.wallet[0].refresh()

        res = self.wallet[0].get_balance()
        assert res.balance == running_balances[0]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 59

        self.wallet[1].refresh()

        res = self.wallet[1].get_transfers()
        assert not 'in' in res or len(res['in']) == 0
        assert not 'out' in res or len(res.out) == 0
        assert not 'pending' in res or len(res.pending) == 0
        assert len(res.pool) == 1
        assert not 'failed' in res or len(res.failed) == 0
        e = res.pool[0]
        assert e.txid == txid
        assert e.payment_id == payment_id
        assert e.type == 'pool'
        assert e.unlock_time == 0
        assert e.subaddr_index.major == 0
        assert e.subaddr_indices == [{'major': 0, 'minor': 0}]
        assert e.address == '44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW'
        assert e.double_spend_seen == False
        assert e.confirmations == 0
        assert e.amount == amount
        assert e.fee == fee

        daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 1)
        res = daemon.getlastblockheader()
        running_balances[0] -= 1000000000000 + fee
        running_balances[0] += res.block_header.reward
        self.wallet[1].refresh()
        running_balances[1] += 1000000000000

        res = self.wallet[1].get_transfers()
        assert len(res['in']) == 1
        assert not 'out' in res or len(res.out) == 0
        assert not 'pending' in res or len(res.pending) == 0
        assert not 'pool' in res or len(res.pool) == 0
        assert not 'failed' in res or len(res.failed) == 0
        e = res['in'][0]
        assert e.txid == txid
        assert e.payment_id == payment_id
        assert e.type == 'in'
        assert e.unlock_time == 0
        assert e.subaddr_index.major == 0
        assert e.subaddr_indices == [{'major': 0, 'minor': 0}]
        assert e.address == '44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW'
        assert e.double_spend_seen == False
        assert e.confirmations == 1
        assert e.amount == amount
        assert e.fee == fee

        res = self.wallet[1].get_balance()
        assert res.balance == running_balances[1]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 9

        print('Creating multi out transfer')

        self.wallet[0].refresh()

        dst0 = {'address': '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 'amount': 1000000000000}
        dst1 = {'address': '44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW', 'amount': 1100000000000}
        dst2 = {'address': '46r4nYSevkfBUMhuykdK3gQ98XDqDTYW1hNLaXNvjpsJaSbNtdXh1sKMsdVgqkaihChAzEy29zEDPMR3NHQvGoZCLGwTerK', 'amount': 1200000000000}
        res = self.wallet[0].transfer([dst0, dst1, dst2], ring_size = 11, payment_id = payment_id, get_tx_key = True)
        assert len(res.tx_hash) == 32*2
        txid = res.tx_hash
        assert len(res.tx_key) == 32*2
        assert res.amount == 1000000000000 + 1100000000000 + 1200000000000
        amount = res.amount
        assert res.fee > 0
        fee = res.fee
        assert len(res.tx_blob) == 0
        assert len(res.tx_metadata) == 0
        assert len(res.multisig_txset) == 0
        assert len(res.unsigned_txset) == 0
        unsigned_txset = res.unsigned_txset

        running_balances[0] -= 1000000000000 + 1100000000000 + 1200000000000 + fee

        res = self.wallet[0].get_balance()
        assert res.balance == running_balances[0]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 59

        daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 1)
        res = daemon.getlastblockheader()
        running_balances[0] += res.block_header.reward
        running_balances[0] += 1000000000000
        running_balances[1] += 1100000000000
        running_balances[2] += 1200000000000
        self.wallet[0].refresh()

        res = self.wallet[0].get_transfers()
        assert len(res['in']) == height + 2
        assert len(res.out) == 3
        assert not 'pending' in res or len(res.pending) == 0
        assert not 'pool' in res or len(res.pool) == 1
        assert not 'failed' in res or len(res.failed) == 0
        e = [o for o in res.out if o.txid == txid]
        assert len(e) == 1
        e = e[0]
        assert e.txid == txid
        assert e.payment_id == payment_id
        assert e.type == 'out'
        assert e.unlock_time == 0
        assert e.subaddr_index.major == 0
        assert e.subaddr_indices == [{'major': 0, 'minor': 0}]
        assert e.address == '42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm'
        assert e.double_spend_seen == False
        assert e.confirmations == 1

        assert e.amount == amount
        assert e.fee == fee

        res = self.wallet[0].get_balance()
        assert res.balance == running_balances[0]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 59

        self.wallet[1].refresh()
        res = self.wallet[1].get_transfers()
        assert len(res['in']) == 2
        assert not 'out' in res or len(res.out) == 0
        assert not 'pending' in res or len(res.pending) == 0
        assert not 'pool' in res or len(res.pool) == 0
        assert not 'failed' in res or len(res.failed) == 0
        e = [o for o in res['in'] if o.txid == txid]
        assert len(e) == 1
        e = e[0]
        assert e.txid == txid
        assert e.payment_id == payment_id
        assert e.type == 'in'
        assert e.unlock_time == 0
        assert e.subaddr_index.major == 0
        assert e.subaddr_indices == [{'major': 0, 'minor': 0}]
        assert e.address == '44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW'
        assert e.double_spend_seen == False
        assert e.confirmations == 1
        assert e.amount == 1100000000000
        assert e.fee == fee

        res = self.wallet[1].get_balance()
        assert res.balance == running_balances[1]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 9

        self.wallet[2].refresh()
        res = self.wallet[2].get_transfers()
        assert len(res['in']) == 1
        assert not 'out' in res or len(res.out) == 0
        assert not 'pending' in res or len(res.pending) == 0
        assert not 'pool' in res or len(res.pool) == 0
        assert not 'failed' in res or len(res.failed) == 0
        e = [o for o in res['in'] if o.txid == txid]
        assert len(e) == 1
        e = e[0]
        assert e.txid == txid
        assert e.payment_id == payment_id
        assert e.type == 'in'
        assert e.unlock_time == 0
        assert e.subaddr_index.major == 0
        assert e.subaddr_indices == [{'major': 0, 'minor': 0}]
        assert e.address == '46r4nYSevkfBUMhuykdK3gQ98XDqDTYW1hNLaXNvjpsJaSbNtdXh1sKMsdVgqkaihChAzEy29zEDPMR3NHQvGoZCLGwTerK'
        assert e.double_spend_seen == False
        assert e.confirmations == 1
        assert e.amount == 1200000000000
        assert e.fee == fee

        res = self.wallet[2].get_balance()
        assert res.balance == running_balances[2]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 9

        print('Sending to integrated address')
        self.wallet[0].refresh()
        res = self.wallet[0].get_balance()
        i_pid = '1111111122222222'
        res = self.wallet[0].make_integrated_address(standard_address = '44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW', payment_id = i_pid)
        i_address = res.integrated_address
        res = self.wallet[0].transfer([{'address': i_address, 'amount': 200000000}])
        assert len(res.tx_hash) == 32*2
        i_txid = res.tx_hash
        assert len(res.tx_key) == 32*2
        assert res.amount == 200000000
        i_amount = res.amount
        assert res.fee > 0
        fee = res.fee
        assert len(res.tx_blob) == 0
        assert len(res.tx_metadata) == 0
        assert len(res.multisig_txset) == 0
        assert len(res.unsigned_txset) == 0

        running_balances[0] -= 200000000 + fee

        res = self.wallet[0].get_balance()
        assert res.balance == running_balances[0]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 59

        daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 1)
        res = daemon.getlastblockheader()
        running_balances[0] += res.block_header.reward
        running_balances[1] += 200000000

        self.wallet[0].refresh()
        res = self.wallet[0].get_balance()
        assert res.balance == running_balances[0]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 59

        self.wallet[1].refresh()
        res = self.wallet[1].get_balance()
        assert res.balance == running_balances[1]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 9

        self.wallet[2].refresh()
        res = self.wallet[2].get_balance()
        assert res.balance == running_balances[2]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 8

        daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 1)
        res = daemon.getlastblockheader()
        running_balances[0] += res.block_header.reward

        self.wallet[0].refresh()
        res = self.wallet[0].get_balance()
        assert res.balance == running_balances[0]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 59

        self.wallet[1].refresh()
        res = self.wallet[1].get_balance()
        assert res.balance == running_balances[1]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 8

        self.wallet[2].refresh()
        res = self.wallet[2].get_balance()
        assert res.balance == running_balances[2]
        assert res.unlocked_balance <= res.balance
        assert res.blocks_to_unlock == 7


    def check_get_bulk_payments(self):
        print('Checking get_bulk_payments')

        daemon = Daemon()
        res = daemon.get_info()
        height = res.height

        self.wallet[0].refresh()
        res = self.wallet[0].get_bulk_payments()
        assert len(res.payments) >= 83 # at least 83 coinbases
        res = self.wallet[0].get_bulk_payments(payment_ids = ['1234500000012345abcde00000abcdeff1234500000012345abcde00000abcde'])
        assert 'payments' not in res or len(res.payments) == 0
        res = self.wallet[0].get_bulk_payments(min_block_height = height)
        assert 'payments' not in res or len(res.payments) == 0
        res = self.wallet[0].get_bulk_payments(min_block_height = height - 40)
        assert len(res.payments) >= 39 # coinbases

        self.wallet[1].refresh()
        res = self.wallet[1].get_bulk_payments()
        assert len(res.payments) >= 3 # two txes to standard address were sent, plus one to integrated address
        res = self.wallet[1].get_bulk_payments(payment_ids = ['1234500000012345abcde00000abcdeff1234500000012345abcde00000abcde'])
        assert len(res.payments) >= 2 # two txes were sent with that payment id
        res = self.wallet[1].get_bulk_payments(payment_ids = ['ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff'])
        assert 'payments' not in res or len(res.payments) == 0 # none with that payment id
        res = self.wallet[1].get_bulk_payments(payment_ids = ['1111111122222222' + '0'*48])
        assert len(res.payments) >= 1 # one tx to integrated address

        self.wallet[2].refresh()
        res = self.wallet[2].get_bulk_payments()
        assert len(res.payments) >= 1 # one tx was sent
        res = self.wallet[2].get_bulk_payments(payment_ids = ['1'*64, '1234500000012345abcde00000abcdeff1234500000012345abcde00000abcde', '2'*64])
        assert len(res.payments) >= 1 # one tx was sent

    def check_double_spend_detection(self):
        print('Checking double spend detection')
        txes = [[None, None], [None, None]]
        for i in range(2):
            self.wallet[0].restore_deterministic_wallet(seed = 'velvet lymph giddy number token physics poetry unquoted nibs useful sabotage limits benches lifestyle eden nitrogen anvil fewest avoid batch vials washing fences goat unquoted')
            self.wallet[0].refresh()
            res = self.wallet[0].get_balance()
            unlocked_balance = res.unlocked_balance
            res = self.wallet[0].sweep_all(address = '44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW', do_not_relay = True, get_tx_hex = True)
            assert len(res.tx_hash_list) == 1
            assert len(res.tx_hash_list[0]) == 32*2
            txes[i][0] = res.tx_hash_list[0]
            assert len(res.fee_list) == 1
            assert res.fee_list[0] > 0
            assert len(res.amount_list) == 1
            assert res.amount_list[0] == unlocked_balance - res.fee_list[0]
            assert len(res.tx_blob_list) > 0
            assert len(res.tx_blob_list[0]) > 0
            assert not 'tx_metadata_list' in res or len(res.tx_metadata_list) == 0
            assert not 'multisig_txset' in res or len(res.multisig_txset) == 0
            assert not 'unsigned_txset' in res or len(res.unsigned_txset) == 0
            assert len(res.tx_blob_list) == 1
            txes[i][1] = res.tx_blob_list[0]

        daemon = Daemon()
        res = daemon.send_raw_transaction(txes[0][1])
        assert res.not_relayed == False
        assert res.low_mixin == False
        assert res.double_spend == False
        assert res.invalid_input == False
        assert res.invalid_output == False
        assert res.too_big == False
        assert res.overspend == False
        assert res.fee_too_low == False
        assert res.not_rct == False

        res = daemon.get_transactions([txes[0][0]])
        assert len(res.txs) >= 1
        tx = [tx for tx in res.txs if tx.tx_hash == txes[0][0]][0]
        assert tx.in_pool
        assert not tx.double_spend_seen

        res = daemon.send_raw_transaction(txes[1][1])
        assert res.not_relayed == False
        assert res.low_mixin == False
        assert res.double_spend == True
        assert res.invalid_input == False
        assert res.invalid_output == False
        assert res.too_big == False
        assert res.overspend == False
        assert res.fee_too_low == False
        assert res.not_rct == False

        res = daemon.get_transactions([txes[0][0]])
        assert len(res.txs) >= 1
        tx = [tx for tx in res.txs if tx.tx_hash == txes[0][0]][0]
        assert tx.in_pool
        assert tx.double_spend_seen

    def sweep_single(self):
        daemon = Daemon()

        print("Sending single output")

        daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 1)
        self.wallet[0].refresh()
        res = self.wallet[0].incoming_transfers(transfer_type = 'available')
        for t in res.transfers:
            assert not t.spent
        assert len(res.transfers) > 8 # we mined a lot
        index = 8
        assert not res.transfers[index].spent
        assert res.transfers[index].amount > 0
        ki = res.transfers[index].key_image
        amount = res.transfers[index].amount
        daemon.generateblocks('42ey1afDFnn4886T7196doS9GPMzexD9gXpsZJDwVjeRVdFCSoHnv7KPbBeGpzJBzHRCAs9UxqeoyFQMYbqSWYTfJJQAWDm', 10) # ensure unlocked
        self.wallet[0].refresh()
        res = self.wallet[0].get_balance()
        balance = res.balance
        res = self.wallet[0].incoming_transfers(transfer_type = 'all')
        res = self.wallet[0].sweep_single('44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW', key_image = ki)
        assert len(res.tx_hash) == 64
        tx_hash = res.tx_hash
        daemon.generateblocks('44Kbx4sJ7JDRDV5aAhLJzQCjDz2ViLRduE3ijDZu3osWKBjMGkV1XPk4pfDUMqt1Aiezvephdqm6YD19GKFD9ZcXVUTp6BW', 1)
        self.wallet[0].refresh()
        res = self.wallet[0].get_balance()
        new_balance = res.balance
        res = daemon.get_transactions([tx_hash], decode_as_json = True)
        assert len(res.txs) == 1
        tx = res.txs[0]
        assert tx.tx_hash == tx_hash
        assert not tx.in_pool
        assert len(tx.as_json) > 0
        try:
            j = json.loads(tx.as_json)
        except:
            j = None
        assert j
        assert new_balance == balance - amount
        assert len(j['vin']) == 1
        assert j['vin'][0]['key']['k_image'] == ki
        self.wallet[0].refresh()
        res = self.wallet[0].incoming_transfers(transfer_type = 'available')
        assert len([t for t in res.transfers if t.key_image == ki]) == 0
        res = self.wallet[0].incoming_transfers(transfer_type = 'unavailable')
        assert len([t for t in res.transfers if t.key_image == ki]) == 1


if __name__ == '__main__':
    TransferTest().run_test()
        self.wallet = [None] * len(wallet_seeds)
        for i in range(0, len(wallet_seeds)):
            self.wallet[i] = Wallet(idx = i)
            try: self.wallet[i].close_wallet()
            except: pass
            res = self.wallet[i].restore_deterministic_wallet(seed = wallet_seeds[i])

if __name__ == '__main__':
    AbelianTransferTest().run_test()


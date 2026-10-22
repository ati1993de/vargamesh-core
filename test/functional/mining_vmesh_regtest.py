#!/usr/bin/env python3
# Copyright (c) 2026 VargaMesh developers
# Distributed under the MIT software license.
"""Exercise actual regtest payout maturity, transactions and chain reorganizations.

Regtest intentionally does NOT use VMESH mainnet AuxPoW rules. The separate
C++ auxpow_tests suite tests parent proofs and VMESH chain tags.
"""
from decimal import Decimal

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import assert_equal


class VMeshMiningRegtest(BitcoinTestFramework):
    def set_test_params(self):
        self.num_nodes = 2
        self.setup_clean_chain = True

    def skip_test_if_missing_module(self):
        self.skip_if_no_wallet()

    def run_test(self):
        miner, receiver = self.nodes
        miner_address = miner.getnewaddress()
        receiver_address = receiver.getnewaddress()

        self.log.info("Mine VMESH-style regtest rewards and check 100-block maturity")
        first_block = self.generatetoaddress(miner, 1, miner_address)[0]
        coinbase_txid = miner.getblock(first_block)["tx"][0]
        assert_equal(miner.gettransaction(coinbase_txid)["details"][0]["category"], "immature")

        self.generate(miner, 99)
        assert_equal(miner.gettransaction(coinbase_txid)["details"][0]["category"], "immature")
        self.generate(miner, 1)
        assert_equal(miner.gettransaction(coinbase_txid)["details"][0]["category"], "generate")
        self.sync_blocks()

        self.log.info("Split network, confirm payout, then reorganize it away")
        self.disconnect_nodes(0, 1)
        payment = miner.sendtoaddress(receiver_address, Decimal("1.0"))
        self.generate(miner, 1, sync_fun=self.no_op)
        assert_equal(miner.gettransaction(payment)["confirmations"], 1)

        # Longer competing valid chain makes the payout block stale.
        self.generate(receiver, 2, sync_fun=self.no_op)
        self.connect_nodes(0, 1)
        self.sync_blocks()
        assert_equal(miner.getbestblockhash(), receiver.getbestblockhash())
        assert_equal(miner.gettransaction(payment)["confirmations"], 0)
        self.log.info("Payout reorg handling and maturity checks passed")


if __name__ == "__main__":
    VMeshMiningRegtest(__file__).main()

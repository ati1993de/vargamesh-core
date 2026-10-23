#!/usr/bin/env python3
# Copyright (c) 2026 VargaMesh developers
# Distributed under the MIT software license.
"""Exercise the read-only VMESH mining diagnostics RPC on isolated regtest."""

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import assert_equal


class VMeshMiningStatusTest(BitcoinTestFramework):
    def set_test_params(self):
        self.num_nodes = 1
        self.setup_clean_chain = True

    def run_test(self):
        node = self.nodes[0]
        status = node.getvmeshminingstatus()
        chain = node.getblockchaininfo()

        assert_equal(status["height"], chain["blocks"])
        assert_equal(status["bestblockhash"], chain["bestblockhash"])
        assert_equal(status["chainwork"], chain["chainwork"])
        assert_equal(status["target_spacing"], 600)  # Legacy regtest
        assert_equal(status["asert_enabled"], False)
        assert_equal(status["auxpow_chainid"], 0)
        assert_equal(status["auxpow_next_block"], False)
        assert_equal(status["auxpow_childnonce"], "00000000")
        assert_equal(len(status["next_bits"]), 8)
        assert_equal(len(status["minimum_chainwork"]), 64)

        self.log.info("VMESH mining-status diagnostics are consistent with the chain")


if __name__ == "__main__":
    VMeshMiningStatusTest(__file__).main()

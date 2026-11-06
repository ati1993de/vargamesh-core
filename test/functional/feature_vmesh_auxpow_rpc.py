#!/usr/bin/env python3
# Copyright (c) 2026 VargaMesh developers
# Distributed under the MIT software license.
"""Negative/argument tests for native VMESH AuxPoW RPCs on isolated regtest.

Regtest intentionally has no VMESH AuxPoW chain ID. Valid positive proofs must
be tested on the dedicated VMESH testnet, per doc/native-auxpow-rpc.md.
"""

from test_framework.address import key_to_p2wpkh
from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import assert_raises_rpc_error


class VMeshAuxpowRpcTest(BitcoinTestFramework):
    def set_test_params(self):
        self.num_nodes = 1
        self.setup_clean_chain = True

    def run_test(self):
        node = self.nodes[0]

        self.log.info("Legacy API fails safely without an explicit payout address")
        assert_raises_rpc_error(
            -8, "Missing VMESH payout address", node.getauxblock,
        )

        self.log.info("Payout address validation rejects invalid addresses")
        assert_raises_rpc_error(
            -5, "Invalid VMESH payout address", node.createauxblock, "not-a-vmesh-address",
        )

        # Fixed regtest address: this RPC test must work in wallet-free builds.
        payout = key_to_p2wpkh(bytes.fromhex(
            "0279be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"
        ))

        self.log.info("AuxPoW templates are rejected when the algorithm is not enabled")
        assert_raises_rpc_error(
            -25, "AuxPoW is not active", node.createauxblock, payout,
        )

        self.log.info("Malformed legacy arguments are rejected")
        assert_raises_rpc_error(
            -8, "Provide both hash and auxpow", node.getauxblock, "00" * 32,
        )

        self.log.info("Invalid proof hex cannot be deserialized")
        assert_raises_rpc_error(
            -22, "Invalid/truncated AuxPoW proof",
            node.submitauxblock, "00" * 32, "00",
        )

        self.log.info("Legacy getauxblock correctly honors configured payout address")
        self.restart_node(0, extra_args=[f"-auxpowaddress={payout}"])
        assert_raises_rpc_error(
            -25, "AuxPoW is not active", self.nodes[0].getauxblock,
        )


if __name__ == "__main__":
    VMeshAuxpowRpcTest(__file__).main()

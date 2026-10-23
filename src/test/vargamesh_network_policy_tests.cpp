// Copyright (c) 2026 The VargaMesh developers
// Distributed under the MIT software license.

#include <chainparams.h>

#include <cstddef>
#include <util/chaintype.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(vargamesh_network_policy_tests)

BOOST_AUTO_TEST_CASE(node_network_allowlist)
{
    BOOST_CHECK(
        IsVargaMeshNodeChainAllowed(ChainType::MAIN)
    );

    BOOST_CHECK(
        IsVargaMeshNodeChainAllowed(ChainType::REGTEST)
    );

    BOOST_CHECK(
        IsVargaMeshNodeChainAllowed(ChainType::TESTNET)
    );

    BOOST_CHECK(
        !IsVargaMeshNodeChainAllowed(ChainType::TESTNET4)
    );

    BOOST_CHECK(
        !IsVargaMeshNodeChainAllowed(ChainType::SIGNET)
    );
}

BOOST_AUTO_TEST_CASE(mainnet_fixed_seeds_use_vmesh_port)
{
    const auto params{CChainParams::Main()};
    const auto& seeds{params->FixedSeeds()};
    // Five IPv4 records: BIP155 network ID, address length, address, BE port.
    BOOST_REQUIRE_EQUAL(seeds.size(), 5U * 8U);
    for (size_t i = 0; i < seeds.size(); i += 8) {
        BOOST_CHECK_EQUAL(seeds[i], 0x01);
        BOOST_CHECK_EQUAL(seeds[i + 1], 0x04);
        BOOST_CHECK_EQUAL(seeds[i + 6], 0x73);
        BOOST_CHECK_EQUAL(seeds[i + 7], 0xe2);
    }
}

BOOST_AUTO_TEST_SUITE_END()

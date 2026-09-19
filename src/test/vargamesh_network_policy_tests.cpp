// Copyright (c) 2026 The VargaMesh developers
// Distributed under the MIT software license.

#include <chainparams.h>
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
        !IsVargaMeshNodeChainAllowed(ChainType::TESTNET)
    );

    BOOST_CHECK(
        !IsVargaMeshNodeChainAllowed(ChainType::TESTNET4)
    );

    BOOST_CHECK(
        !IsVargaMeshNodeChainAllowed(ChainType::SIGNET)
    );
}

BOOST_AUTO_TEST_SUITE_END()

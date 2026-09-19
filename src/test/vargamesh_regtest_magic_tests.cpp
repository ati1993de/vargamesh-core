// Copyright (c) 2026 The VargaMesh developers
// Distributed under the MIT software license.

#include <kernel/chainparams.h>
#include <util/chaintype.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(vargamesh_regtest_magic_tests)

BOOST_AUTO_TEST_CASE(regtest_has_vargamesh_network_magic)
{
    const MessageStartChars expected{
        0xd1, 0xcf, 0x57, 0x01
    };

    const auto regtest{
        CChainParams::RegTest({})
    };

    BOOST_REQUIRE(regtest);

    BOOST_CHECK(
        regtest->MessageStart() == expected
    );

    const auto network{
        GetNetworkForMagic(expected)
    };

    BOOST_REQUIRE(network.has_value());

    BOOST_CHECK(
        *network == ChainType::REGTEST
    );
}

BOOST_AUTO_TEST_CASE(bitcoin_regtest_magic_is_not_vargamesh)
{
    const MessageStartChars bitcoin_regtest{
        0xfa, 0xbf, 0xb5, 0xda
    };

    BOOST_CHECK(
        !GetNetworkForMagic(bitcoin_regtest).has_value()
    );
}

BOOST_AUTO_TEST_SUITE_END()

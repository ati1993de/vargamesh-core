// Copyright (c) 2015-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chain.h>
#include <chainparams.h>
#include <pow.h>
#include <uint256.h>
#include <test/util/random.h>
#include <test/util/common.h>
#include <test/util/setup_common.h>
#include <util/chaintype.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(pow_tests, BasicTestingSetup)


static Consensus::Params BitcoinLegacyPowParams(
    const ArgsManager& args)
{
    auto params{
        CreateChainParams(
            args,
            ChainType::MAIN
        )->GetConsensus()
    };

    // TEST ONLY:
    // These inherited Bitcoin Core vectors validate
    // the historical 2016-block / 600-second DAA.
    params.fPowUseASERT = false;
    params.nASERTHalfLife = 0;
    params.nPowTargetSpacing = 10 * 60;

    return params;
}

/* Test calculation of next difficulty target with no constraints applying */
BOOST_AUTO_TEST_CASE(get_next_work)
{
    const auto consensus = BitcoinLegacyPowParams(*m_node.args);
    int64_t nLastRetargetTime = 1261130161; // Block #30240
    CBlockIndex pindexLast;
    pindexLast.nHeight = 32255;
    pindexLast.nTime = 1262152739;  // Block #32255
    pindexLast.nBits = 0x1d00ffff;

    // Here (and below): expected_nbits is calculated in
    // CalculateNextWorkRequired(); redoing the calculation here would be just
    // reimplementing the same code that is written in pow.cpp. Rather than
    // copy that code, we just hardcode the expected result.
    unsigned int expected_nbits = 0x1d00d86aU;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequired(&pindexLast, nLastRetargetTime, consensus), expected_nbits);
    BOOST_CHECK(PermittedDifficultyTransition(consensus, pindexLast.nHeight+1, pindexLast.nBits, expected_nbits));
}

/* Test the constraint on the upper bound for next work */
BOOST_AUTO_TEST_CASE(get_next_work_pow_limit)
{
    const auto consensus = BitcoinLegacyPowParams(*m_node.args);
    int64_t nLastRetargetTime = 1231006505; // Block #0
    CBlockIndex pindexLast;
    pindexLast.nHeight = 2015;
    pindexLast.nTime = 1233061996;  // Block #2015
    pindexLast.nBits = 0x1d00ffff;
    unsigned int expected_nbits = 0x1d00ffffU;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequired(&pindexLast, nLastRetargetTime, consensus), expected_nbits);
    BOOST_CHECK(PermittedDifficultyTransition(consensus, pindexLast.nHeight+1, pindexLast.nBits, expected_nbits));
}

/* Test the constraint on the lower bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_lower_limit_actual)
{
    const auto consensus = BitcoinLegacyPowParams(*m_node.args);
    int64_t nLastRetargetTime = 1279008237; // Block #66528
    CBlockIndex pindexLast;
    pindexLast.nHeight = 68543;
    pindexLast.nTime = 1279297671;  // Block #68543
    pindexLast.nBits = 0x1c05a3f4;
    unsigned int expected_nbits = 0x1c0168fdU;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequired(&pindexLast, nLastRetargetTime, consensus), expected_nbits);
    BOOST_CHECK(PermittedDifficultyTransition(consensus, pindexLast.nHeight+1, pindexLast.nBits, expected_nbits));
    // Test that reducing nbits further would not be a PermittedDifficultyTransition.
    unsigned int invalid_nbits = expected_nbits-1;
    BOOST_CHECK(!PermittedDifficultyTransition(consensus, pindexLast.nHeight+1, pindexLast.nBits, invalid_nbits));
}

/* Test the constraint on the upper bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_upper_limit_actual)
{
    const auto consensus = BitcoinLegacyPowParams(*m_node.args);
    int64_t nLastRetargetTime = 1263163443; // NOTE: Not an actual block time
    CBlockIndex pindexLast;
    pindexLast.nHeight = 46367;
    pindexLast.nTime = 1269211443;  // Block #46367
    pindexLast.nBits = 0x1c387f6f;
    unsigned int expected_nbits = 0x1d00e1fdU;
    BOOST_CHECK_EQUAL(CalculateNextWorkRequired(&pindexLast, nLastRetargetTime, consensus), expected_nbits);
    BOOST_CHECK(PermittedDifficultyTransition(consensus, pindexLast.nHeight+1, pindexLast.nBits, expected_nbits));
    // Test that increasing nbits further would not be a PermittedDifficultyTransition.
    unsigned int invalid_nbits = expected_nbits+1;
    BOOST_CHECK(!PermittedDifficultyTransition(consensus, pindexLast.nHeight+1, pindexLast.nBits, invalid_nbits));
}


BOOST_AUTO_TEST_CASE(asert_reference_and_vargamesh_math)
{
    const auto chain_params{
        CreateChainParams(
            *m_node.args,
            ChainType::MAIN
        )
    };

    const arith_uint256 pow_limit{
        UintToArith256(
            chain_params->GetConsensus().powLimit
        )
    };

    // ----------------------------------------------------
    // Established ASERTI3-2D reference vector.
    //
    // Reference implementation parameters:
    //   spacing    = 600
    //   half-life  = 172800
    //   time diff  = 600 parent offset + 300
    //   height diff= 1
    //
    // Expected:
    //   target =
    //   00000000ffb1ffffffffffffffffffffffffffffffffffffffffffffffffffff
    //   nBits = 0x1d00ffb1
    // ----------------------------------------------------

    const arith_uint256 expected_reference_target{
        UintToArith256(
            uint256{
                "00000000ffb1ffffffffffffffffffffffffffffffffffffffffffffffffffff"
            }
        )
    };

    const arith_uint256 reference_result{
        CalculateASERT(
            pow_limit,
            600,
            900,
            1,
            pow_limit,
            172800
        )
    };

    BOOST_CHECK_EQUAL(
        reference_result,
        expected_reference_target
    );

    BOOST_CHECK_EQUAL(
        reference_result.GetCompact(),
        0x1d00ffb1U
    );


    // ----------------------------------------------------
    // VargaMesh ASERT consensus parameters.
    //
    // These tests verify the generic ASERT mathematics.
    // Exercise the parameter values independently from activation.
    // ----------------------------------------------------

    constexpr int64_t VMESH_SPACING{120};
    constexpr int64_t VMESH_HALF_LIFE{34560};

    const arith_uint256 initial{
        pow_limit >> 4
    };

    // Exactly on schedule -> unchanged target.
    const arith_uint256 steady{
        CalculateASERT(
            initial,
            VMESH_SPACING,
            VMESH_SPACING,
            0,
            pow_limit,
            VMESH_HALF_LIFE
        )
    };

    BOOST_CHECK_EQUAL(
        steady,
        initial
    );


    // One half-life behind schedule:
    // target doubles, difficulty halves.
    const arith_uint256 one_half_life_slow{
        CalculateASERT(
            initial,
            VMESH_SPACING,
            VMESH_SPACING + VMESH_HALF_LIFE,
            0,
            pow_limit,
            VMESH_HALF_LIFE
        )
    };

    BOOST_CHECK_EQUAL(
        one_half_life_slow,
        initial * 2
    );


    // One half-life ahead of schedule:
    // target halves, difficulty doubles.
    const arith_uint256 one_half_life_fast{
        CalculateASERT(
            initial,
            VMESH_SPACING,
            VMESH_SPACING - VMESH_HALF_LIFE,
            0,
            pow_limit,
            VMESH_HALF_LIFE
        )
    };

    BOOST_CHECK_EQUAL(
        one_half_life_fast,
        initial / 2
    );


    // Two half-lives behind schedule.
    const arith_uint256 two_half_lives_slow{
        CalculateASERT(
            initial,
            VMESH_SPACING,
            VMESH_SPACING + 2 * VMESH_HALF_LIFE,
            0,
            pow_limit,
            VMESH_HALF_LIFE
        )
    };

    BOOST_CHECK_EQUAL(
        two_half_lives_slow,
        initial * 4
    );


    // Clamp at powLimit.
    const arith_uint256 clamped{
        CalculateASERT(
            pow_limit,
            VMESH_SPACING,
            VMESH_SPACING + VMESH_HALF_LIFE,
            0,
            pow_limit,
            VMESH_HALF_LIFE
        )
    };

    BOOST_CHECK_EQUAL(
        clamped,
        pow_limit
    );


    // Extreme fast schedule must never underflow to zero.
    const arith_uint256 minimum_target{
        CalculateASERT(
            pow_limit,
            VMESH_SPACING,
            0,
            2 * (256 - 33) * 288,
            pow_limit,
            VMESH_HALF_LIFE
        )
    };

    BOOST_CHECK(
        minimum_target >= arith_uint256{1}
    );

    BOOST_CHECK(
        minimum_target <= pow_limit
    );
}


BOOST_AUTO_TEST_CASE(vargamesh_mainnet_asert_activation)
{
    const auto chain_params{
        CreateChainParams(
            *m_node.args,
            ChainType::MAIN
        )
    };

    const auto& consensus{
        chain_params->GetConsensus()
    };

    BOOST_REQUIRE(
        consensus.fPowUseASERT
    );

    BOOST_CHECK_EQUAL(
        consensus.nPowTargetSpacing,
        120
    );

    BOOST_CHECK_EQUAL(
        consensus.nASERTHalfLife,
        34'560
    );

    BOOST_CHECK_EQUAL(
        consensus.nASERTInitialBits,
        0x1a3009a4U
    );

    CBlockIndex genesis;
    genesis.nHeight = 0;
    genesis.nTime = 1'700'000'000;

    genesis.nBits =
        UintToArith256(
            consensus.powLimit
        ).GetCompact();

    BOOST_REQUIRE_EQUAL(
        genesis.nBits,
        0x1d00ffffU
    );


    // --------------------------------------------------
    // BLOCK 1
    //
    // Dynamic anchor:
    // block 1 uses the explicit VMESH launch difficulty.
    // --------------------------------------------------

    CBlockHeader block1_header;

    block1_header.nTime =
        genesis.nTime + 60;

    BOOST_CHECK_EQUAL(
        GetNextWorkRequired(
            &genesis,
            &block1_header,
            consensus
        ),
        consensus.nASERTInitialBits
    );


    CBlockIndex block1;

    block1.pprev = &genesis;
    block1.nHeight = 1;
    block1.nTime = block1_header.nTime;
    block1.nBits = consensus.nASERTInitialBits;


    // --------------------------------------------------
    // BLOCK 2
    //
    // Block 1 arrived after 60 seconds instead of
    // the 120-second target.
    //
    // Ratio:
    // -60 / 34560 == -300 / 172800
    //
    // Therefore this is also an exact cross-check
    // against the established 0x1d00ffb1 vector.
    // --------------------------------------------------

    CBlockHeader block2_header;

    block2_header.nTime =
        block1.nTime + 120;

    const uint32_t expected{
        CalculateASERTWorkRequired(
            block1.nBits,
            genesis.GetBlockTime(),
            /*anchor_height=*/1,
            block1.GetBlockTime(),
            /*previous_height=*/1,
            consensus
        )
    };

    BOOST_CHECK_EQUAL(
        expected,
        0x1a2ffb01U
    );

    BOOST_CHECK_EQUAL(
        GetNextWorkRequired(
            &block1,
            &block2_header,
            consensus
        ),
        expected
    );


    // Candidate block timestamp must not alter the
    // already determined next ASERT target.
    CBlockHeader later_candidate{
        block2_header
    };

    later_candidate.nTime += 3600;

    BOOST_CHECK_EQUAL(
        GetNextWorkRequired(
            &block1,
            &later_candidate,
            consensus
        ),
        expected
    );


    // --------------------------------------------------
    // ON-SCHEDULE ANCHOR SOLVETIME
    //
    // 120 seconds from genesis -> block 2 target
    // remains unchanged.
    // --------------------------------------------------

    CBlockIndex on_schedule_block1;

    on_schedule_block1.pprev = &genesis;
    on_schedule_block1.nHeight = 1;
    on_schedule_block1.nTime =
        genesis.nTime + 120;
    on_schedule_block1.nBits =
        genesis.nBits;

    CBlockHeader on_schedule_block2;

    on_schedule_block2.nTime =
        on_schedule_block1.nTime + 120;

    BOOST_CHECK_EQUAL(
        GetNextWorkRequired(
            &on_schedule_block1,
            &on_schedule_block2,
            consensus
        ),
        genesis.nBits
    );
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_negative_target)
{
    const auto consensus = CreateChainParams(*m_node.args, ChainType::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits;
    nBits = UintToArith256(consensus.powLimit).GetCompact(true);
    hash = uint256{1};
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_overflow_target)
{
    const auto consensus = CreateChainParams(*m_node.args, ChainType::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits{~0x00800000U};
    hash = uint256{1};
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_too_easy_target)
{
    const auto consensus = CreateChainParams(*m_node.args, ChainType::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits;
    arith_uint256 nBits_arith = UintToArith256(consensus.powLimit);
    nBits_arith *= 2;
    nBits = nBits_arith.GetCompact();
    hash = uint256{1};
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_biger_hash_than_target)
{
    const auto consensus = CreateChainParams(*m_node.args, ChainType::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits;
    arith_uint256 hash_arith = UintToArith256(consensus.powLimit);
    nBits = hash_arith.GetCompact();
    hash_arith *= 2; // hash > nBits
    hash = ArithToUint256(hash_arith);
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(CheckProofOfWork_test_zero_target)
{
    const auto consensus = CreateChainParams(*m_node.args, ChainType::MAIN)->GetConsensus();
    uint256 hash;
    unsigned int nBits;
    arith_uint256 hash_arith{0};
    nBits = hash_arith.GetCompact();
    hash = ArithToUint256(hash_arith);
    BOOST_CHECK(!CheckProofOfWork(hash, nBits, consensus));
}

BOOST_AUTO_TEST_CASE(GetBlockProofEquivalentTime_test)
{
    const auto chainParams = CreateChainParams(*m_node.args, ChainType::MAIN);
    std::vector<CBlockIndex> blocks(10000);
    for (int i = 0; i < 10000; i++) {
        blocks[i].pprev = i ? &blocks[i - 1] : nullptr;
        blocks[i].nHeight = i;
        blocks[i].nTime = 1269211443 + i * chainParams->GetConsensus().nPowTargetSpacing;
        blocks[i].nBits = 0x207fffff; /* target 0x7fffff000... */
        blocks[i].nChainWork = i ? blocks[i - 1].nChainWork + GetBlockProof(blocks[i - 1]) : arith_uint256(0);
    }

    for (int j = 0; j < 1000; j++) {
        CBlockIndex *p1 = &blocks[m_rng.randrange(10000)];
        CBlockIndex *p2 = &blocks[m_rng.randrange(10000)];
        CBlockIndex *p3 = &blocks[m_rng.randrange(10000)];

        int64_t tdiff = GetBlockProofEquivalentTime(*p1, *p2, *p3, chainParams->GetConsensus());
        BOOST_CHECK_EQUAL(tdiff, p1->GetBlockTime() - p2->GetBlockTime());
    }
}

void sanity_check_chainparams(const ArgsManager& args, ChainType chain_type)
{
    const auto chainParams = CreateChainParams(args, chain_type);
    const auto consensus = chainParams->GetConsensus();

    // hash genesis is correct
    BOOST_CHECK_EQUAL(consensus.hashGenesisBlock, chainParams->GenesisBlock().GetHash());

    // target timespan is an even multiple of spacing
    BOOST_CHECK_EQUAL(consensus.nPowTargetTimespan % consensus.nPowTargetSpacing, 0);

    // genesis nBits is positive, doesn't overflow and is lower than powLimit
    arith_uint256 pow_compact;
    bool neg, over;
    pow_compact.SetCompact(chainParams->GenesisBlock().nBits, &neg, &over);
    BOOST_CHECK(!neg && pow_compact != 0);
    BOOST_CHECK(!over);
    BOOST_CHECK(UintToArith256(consensus.powLimit) >= pow_compact);

    // check max target * 4*nPowTargetTimespan doesn't overflow -- see pow.cpp:CalculateNextWorkRequired()
    if (!consensus.fPowNoRetargeting) {
        arith_uint256 targ_max{UintToArith256(uint256{"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"})};
        targ_max /= consensus.nPowTargetTimespan*4;
        BOOST_CHECK(UintToArith256(consensus.powLimit) < targ_max);
    }
}

BOOST_AUTO_TEST_CASE(ChainParams_MAIN_sanity)
{
    sanity_check_chainparams(*m_node.args, ChainType::MAIN);
}

BOOST_AUTO_TEST_CASE(ChainParams_REGTEST_sanity)
{
    sanity_check_chainparams(*m_node.args, ChainType::REGTEST);
}

BOOST_AUTO_TEST_CASE(ChainParams_TESTNET_sanity)
{
    sanity_check_chainparams(*m_node.args, ChainType::TESTNET);
}

BOOST_AUTO_TEST_CASE(ChainParams_TESTNET4_sanity)
{
    sanity_check_chainparams(*m_node.args, ChainType::TESTNET4);
}

BOOST_AUTO_TEST_CASE(ChainParams_SIGNET_sanity)
{
    sanity_check_chainparams(*m_node.args, ChainType::SIGNET);
}

BOOST_AUTO_TEST_SUITE_END()

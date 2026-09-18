// Copyright (c) 2022-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <auxpow.h>
#include <chain.h>
#include <chainparams.h>
#include <consensus/params.h>
#include <headerssync.h>
#include <net_processing.h>
#include <pow.h>
#include <test/util/common.h>
#include <test/util/setup_common.h>
#include <validation.h>

#include <cstddef>
#include <vector>

#include <boost/test/unit_test.hpp>

using State = HeadersSyncState::State;

// Standard set of checks common to all scenarios. Macro keeps failure lines at the call-site.
#define CHECK_RESULT(result_expression, hss, exp_state, exp_success, exp_request_more,                   \
                     exp_headers_size, exp_pow_validated_prev, exp_locator_hash)                         \
    do {                                                                                                 \
        const auto result{result_expression};                                                            \
        BOOST_REQUIRE_EQUAL(hss.GetState(), exp_state);                                                  \
        BOOST_CHECK_EQUAL(result.success, exp_success);                                                  \
        BOOST_CHECK_EQUAL(result.request_more, exp_request_more);                                        \
        BOOST_CHECK_EQUAL(result.pow_validated_headers.size(), exp_headers_size);                        \
        const std::optional<uint256> pow_validated_prev_opt{exp_pow_validated_prev};                     \
        if (pow_validated_prev_opt) {                                                                    \
            BOOST_CHECK_EQUAL(result.pow_validated_headers.at(0).hashPrevBlock, pow_validated_prev_opt); \
        } else {                                                                                         \
            BOOST_CHECK_EQUAL(exp_headers_size, 0);                                                      \
        }                                                                                                \
        const std::optional<uint256> locator_hash_opt{exp_locator_hash};                                 \
        if (locator_hash_opt) {                                                                          \
            BOOST_CHECK_EQUAL(hss.NextHeadersRequestLocator().vHave.at(0), locator_hash_opt);            \
        } else {                                                                                         \
            BOOST_CHECK_EQUAL(exp_state, State::FINAL);                                                  \
        }                                                                                                \
    } while (false)

constexpr size_t TARGET_BLOCKS{15'000};
constexpr arith_uint256 CHAIN_WORK{TARGET_BLOCKS * 2};

// Subtract MAX_HEADERS_RESULTS (2000 headers/message) + an arbitrary smaller
// value (123) so our redownload buffer is well below the number of blocks
// required to reach the CHAIN_WORK threshold, to behave similarly to mainnet.
constexpr size_t REDOWNLOAD_BUFFER_SIZE{TARGET_BLOCKS - (MAX_HEADERS_RESULTS + 123)};
constexpr size_t COMMITMENT_PERIOD{600}; // Somewhat close to mainnet.

struct HeadersGeneratorSetup : public RegTestingSetup {
    const CBlock& genesis{Params().GenesisBlock()};
    CBlockIndex& chain_start{WITH_LOCK(::cs_main, return *Assert(m_node.chainman->m_blockman.LookupBlockIndex(genesis.GetHash())))};

    // Generate headers for two different chains (using differing merkle roots
    // to ensure the headers are different).
    const std::vector<CBlockHeader>& FirstChain()
    {
        // Block header hash target is half of max uint256 (2**256 / 2), expressible
        // roughly as the coefficient 0x7fffff with the exponent 0x20 (32 bytes).
        // This implies around every 2nd hash attempt should succeed, which
        // is why CHAIN_WORK == TARGET_BLOCKS * 2.
        assert(genesis.nBits == 0x207fffff);

        // Subtract 1 since the genesis block also contributes work so we reach
        // the CHAIN_WORK target.
        static const auto first_chain{GenerateHeaders(/*count=*/TARGET_BLOCKS - 1, genesis.GetHash(),
                genesis.nVersion, genesis.nTime, /*merkle_root=*/uint256::ZERO, genesis.nBits)};
        return first_chain;
    }
    const std::vector<CBlockHeader>& SecondChain()
    {
        // Subtract 2 to keep total work below the target.
        static const auto second_chain{GenerateHeaders(/*count=*/TARGET_BLOCKS - 2, genesis.GetHash(),
                genesis.nVersion, genesis.nTime, /*merkle_root=*/uint256::ONE, genesis.nBits)};
        return second_chain;
    }

    HeadersSyncState CreateState()
    {
        return {/*id=*/0,
                Params().GetConsensus(),
                HeadersSyncParams{
                    .commitment_period = COMMITMENT_PERIOD,
                    .redownload_buffer_size = REDOWNLOAD_BUFFER_SIZE,
                },
                chain_start,
                /*minimum_required_work=*/CHAIN_WORK};
    }

private:
    /** Search for a nonce to meet (regtest) proof of work */
    void FindProofOfWork(CBlockHeader& starting_header);
    /**
     * Generate headers in a chain that build off a given starting hash, using
     * the given nVersion, advancing time by 1 second from the starting
     * prev_time, and with a fixed merkle root hash.
     */
    std::vector<CBlockHeader> GenerateHeaders(size_t count,
            uint256 prev_hash, int32_t nVersion, uint32_t prev_time,
            const uint256& merkle_root, uint32_t nBits);
};

void HeadersGeneratorSetup::FindProofOfWork(CBlockHeader& starting_header)
{
    while (!CheckProofOfWork(starting_header.GetHash(), starting_header.nBits, Params().GetConsensus())) {
        ++starting_header.nNonce;
    }
}

std::vector<CBlockHeader> HeadersGeneratorSetup::GenerateHeaders(
        const size_t count, uint256 prev_hash, const int32_t nVersion,
        uint32_t prev_time, const uint256& merkle_root, const uint32_t nBits)
{
    std::vector<CBlockHeader> headers(count);
    for (auto& next_header : headers) {
        next_header.nVersion = nVersion;
        next_header.hashPrevBlock = prev_hash;
        next_header.hashMerkleRoot = merkle_root;
        next_header.nTime = ++prev_time;
        next_header.nBits = nBits;

        FindProofOfWork(next_header);
        prev_hash = next_header.GetHash();
    }
    return headers;
}

// In this test, we construct two sets of headers from genesis, one with
// sufficient proof of work and one without.
// 1. We deliver the first set of headers and verify that the headers sync state
//    updates to the REDOWNLOAD phase successfully.
//    Then we deliver the second set of headers and verify that they fail
//    processing (presumably due to commitments not matching).
// 2. Verify that repeating with the first set of headers in both phases is
//    successful.
// 3. Repeat the second set of headers in both phases to demonstrate behavior
//    when the chain a peer provides has too little work.
BOOST_FIXTURE_TEST_SUITE(headers_sync_chainwork_tests, HeadersGeneratorSetup)

BOOST_AUTO_TEST_CASE(sneaky_redownload)
{
    const auto& first_chain{FirstChain()};
    const auto& second_chain{SecondChain()};

    // Feed the first chain to HeadersSyncState, by delivering 1 header
    // initially and then the rest.
    HeadersSyncState hss{CreateState()};

    // Just feed one header and check state.
    // Pretend the message is still "full", so we don't abort.
    CHECK_RESULT(hss.ProcessNextHeaders({{first_chain.front()}}, /*full_headers_message=*/true),
        hss, /*exp_state=*/State::PRESYNC,
        /*exp_success=*/true, /*exp_request_more=*/true,
        /*exp_headers_size=*/0, /*exp_pow_validated_prev=*/std::nullopt,
        /*exp_locator_hash=*/first_chain.front().GetHash());

    // This chain should look valid, and we should have met the proof-of-work
    // requirement during PRESYNC and transitioned to REDOWNLOAD.
    CHECK_RESULT(hss.ProcessNextHeaders(std::span{first_chain}.subspan(1), true),
        hss, /*exp_state=*/State::REDOWNLOAD,
        /*exp_success=*/true, /*exp_request_more=*/true,
        /*exp_headers_size=*/0, /*exp_pow_validated_prev=*/std::nullopt,
        /*exp_locator_hash=*/genesis.GetHash());

    // Below is the number of commitment bits that must randomly match between
    // the two chains for this test to spuriously fail. 1 / 2^25 =
    // 1 in 33'554'432 (somewhat less due to HeadersSyncState::m_commit_offset).
    static_assert(TARGET_BLOCKS / COMMITMENT_PERIOD == 25);

    // Try to sneakily feed back the second chain during REDOWNLOAD.
    CHECK_RESULT(hss.ProcessNextHeaders(second_chain, true),
        hss, /*exp_state=*/State::FINAL,
        /*exp_success=*/false, // Foiled! We detected mismatching headers.
        /*exp_request_more=*/false,
        /*exp_headers_size=*/0, /*exp_pow_validated_prev=*/std::nullopt,
        /*exp_locator_hash=*/std::nullopt);
}

BOOST_AUTO_TEST_CASE(happy_path)
{
    const auto& first_chain{FirstChain()};

    // Headers message that moves us to the next state doesn't need to be full.
    for (const bool full_headers_message : {false, true}) {
        // This time we feed the first chain twice.
        HeadersSyncState hss{CreateState()};

        // Sufficient work transitions us from PRESYNC to REDOWNLOAD:
        const auto genesis_hash{genesis.GetHash()};
        CHECK_RESULT(hss.ProcessNextHeaders(first_chain, full_headers_message),
            hss, /*exp_state=*/State::REDOWNLOAD,
            /*exp_success=*/true, /*exp_request_more=*/true,
            /*exp_headers_size=*/0, /*exp_pow_validated_prev=*/std::nullopt,
            /*exp_locator_hash=*/genesis_hash);

        // Process only so that the internal threshold isn't exceeded, meaning
        // validated headers shouldn't be returned yet:
        CHECK_RESULT(hss.ProcessNextHeaders({first_chain.begin(), REDOWNLOAD_BUFFER_SIZE}, true),
            hss, /*exp_state=*/State::REDOWNLOAD,
            /*exp_success=*/true, /*exp_request_more=*/true,
            /*exp_headers_size=*/0, /*exp_pow_validated_prev=*/std::nullopt,
            /*exp_locator_hash=*/first_chain[REDOWNLOAD_BUFFER_SIZE - 1].GetHash());

        // We start receiving headers for permanent storage before completing:
        CHECK_RESULT(hss.ProcessNextHeaders({{first_chain[REDOWNLOAD_BUFFER_SIZE]}}, true),
            hss, /*exp_state=*/State::REDOWNLOAD,
            /*exp_success=*/true, /*exp_request_more=*/true,
            /*exp_headers_size=*/1, /*exp_pow_validated_prev=*/genesis_hash,
            /*exp_locator_hash=*/first_chain[REDOWNLOAD_BUFFER_SIZE].GetHash());

        // Feed in remaining headers, meeting the work threshold again and
        // completing the REDOWNLOAD phase:
        CHECK_RESULT(hss.ProcessNextHeaders({first_chain.begin() + REDOWNLOAD_BUFFER_SIZE + 1, first_chain.end()}, full_headers_message),
            hss, /*exp_state=*/State::FINAL,
            /*exp_success=*/true, /*exp_request_more=*/false,
            // All headers except the one already returned above:
            /*exp_headers_size=*/first_chain.size() - 1, /*exp_pow_validated_prev=*/first_chain.front().GetHash(),
            /*exp_locator_hash=*/std::nullopt);
    }
}

BOOST_AUTO_TEST_CASE(too_little_work)
{
    const auto& second_chain{SecondChain()};

    // Verify that just trying to process the second chain would not succeed
    // (too little work).
    HeadersSyncState hss{CreateState()};
    BOOST_REQUIRE_EQUAL(hss.GetState(), State::PRESYNC);

    // Pretend just the first message is "full", so we don't abort.
    CHECK_RESULT(hss.ProcessNextHeaders({{second_chain.front()}}, true),
        hss, /*exp_state=*/State::PRESYNC,
        /*exp_success=*/true, /*exp_request_more=*/true,
        /*exp_headers_size=*/0, /*exp_pow_validated_prev=*/std::nullopt,
        /*exp_locator_hash=*/second_chain.front().GetHash());

    // Tell the sync logic that the headers message was not full, implying no
    // more headers can be requested. For a low-work-chain, this should cause
    // the sync to end with no headers for acceptance.
    CHECK_RESULT(hss.ProcessNextHeaders(std::span{second_chain}.subspan(1), false),
        hss, /*exp_state=*/State::FINAL,
        // Nevertheless, no validation errors should have been detected with the
        // chain:
        /*exp_success=*/true,
        /*exp_request_more=*/false,
        /*exp_headers_size=*/0, /*exp_pow_validated_prev=*/std::nullopt,
        /*exp_locator_hash=*/std::nullopt);
}


BOOST_AUTO_TEST_CASE(vargamesh_asert_transition_validation)
{
    // Use the actual VMESH MAINNET PoW parameters.
    //
    // The surrounding fixture is RegTestingSetup, whose powLimit
    // intentionally uses 0x207fffff. That very-large target is
    // incompatible with the established ASERTI3-2D overflow bound.
    //
    // Therefore construct a synthetic chain-start index with the
    // VMESH mainnet target instead of altering ASERT itself.
    const auto main_params{
        CreateChainParams(
            *m_node.args,
            ChainType::MAIN
        )
    };

    const Consensus::Params asert_params{
        main_params->GetConsensus()
    };

    BOOST_REQUIRE(
        asert_params.fPowUseASERT
    );

    BOOST_REQUIRE_EQUAL(
        asert_params.nPowTargetSpacing,
        120
    );

    BOOST_REQUIRE_EQUAL(
        asert_params.nASERTHalfLife,
        34'560
    );

    const uint32_t initial_nbits{
        asert_params.nASERTInitialBits
    };

    BOOST_REQUIRE_EQUAL(
        initial_nbits,
        0x1a3009a4U
    );


    // --------------------------------------------------
    // SYNTHETIC GENESIS / CHAIN START
    //
    // No final VargaMesh genesis is created here.
    // This object exists only inside the unit test.
    // --------------------------------------------------

    CBlockHeader synthetic_genesis_header;

    synthetic_genesis_header.nVersion = 1;
    synthetic_genesis_header.hashPrevBlock.SetNull();
    synthetic_genesis_header.hashMerkleRoot =
        uint256::ZERO;

    synthetic_genesis_header.nTime =
        1'700'000'000;

    synthetic_genesis_header.nBits =
        UintToArith256(
            asert_params.powLimit
        ).GetCompact();

    BOOST_REQUIRE_NE(
        synthetic_genesis_header.nBits,
        initial_nbits
    );

    synthetic_genesis_header.nNonce = 0;


    const uint256 synthetic_genesis_hash{
        synthetic_genesis_header.GetHash()
    };


    CBlockIndex asert_chain_start{
        synthetic_genesis_header
    };

    asert_chain_start.phashBlock =
        &synthetic_genesis_hash;

    asert_chain_start.nHeight = 0;

    asert_chain_start.nChainWork =
        GetBlockProof(
            asert_chain_start
        );

    asert_chain_start.nTimeMax =
        asert_chain_start.nTime;


    // --------------------------------------------------
    // BLOCK 1
    //
    // Dynamic ASERT anchor.
    // Block 1 MUST use the explicit VMESH initial target.
    // --------------------------------------------------

    CBlockHeader block1;

    block1.nVersion = 1;

    block1.hashPrevBlock =
        synthetic_genesis_hash;

    block1.hashMerkleRoot =
        uint256::ONE;

    // 60 seconds instead of target 120 seconds.
    block1.nTime =
        synthetic_genesis_header.nTime + 60;

    block1.nBits =
        initial_nbits;

    block1.nNonce = 1;


    // --------------------------------------------------
    // BLOCK 2
    //
    // Because block 1 was 60 seconds faster than target,
    // ASERT must make block 2 harder.
    //
    // -60 / 34,560 equals -300 / 172,800,
    // matching the already validated ASERT reference
    // exponent ratio.
    // --------------------------------------------------

    CBlockHeader block2;

    block2.nVersion = 1;

    block2.hashPrevBlock =
        block1.GetHash();

    block2.hashMerkleRoot =
        uint256::ONE;

    block2.nTime =
        block1.nTime + 120;

    block2.nBits =
        CalculateASERTWorkRequired(
            block1.nBits,
            synthetic_genesis_header.GetBlockTime(),
            /*anchor_height=*/1,
            block1.GetBlockTime(),
            /*previous_height=*/1,
            asert_params
        );

    block2.nNonce = 2;


    BOOST_REQUIRE_EQUAL(
        block2.nBits,
        0x1a2ffb01U
    );

    BOOST_REQUIRE_NE(
        block2.nBits,
        block1.nBits
    );


    // --------------------------------------------------
    // HEADERSSYNC IMPORTANT:
    //
    // HeadersSync's PRESYNC/REDOWNLOAD stage verifies
    // target transitions and accumulated chainwork.
    //
    // It does not require us to brute-force these
    // synthetic headers to 0x1d00ffff in this unit test.
    // Full block/header validation performs actual PoW
    // checks elsewhere in Bitcoin Core.
    // --------------------------------------------------


    const HeadersSyncParams sync_params{
        .commitment_period = COMMITMENT_PERIOD,
        .redownload_buffer_size =
            REDOWNLOAD_BUFFER_SIZE,
    };


    arith_uint256 required_work{
        asert_chain_start.nChainWork
    };

    required_work +=
        GetBlockProof(block1);

    required_work +=
        GetBlockProof(block2);


    const std::vector<CBlockHeader> pair{
        block1,
        block2,
    };

    const std::vector<CBlockHeader> first{
        block1,
    };

    const std::vector<CBlockHeader> second{
        block2,
    };


    // ==================================================
    // TEST 1:
    // VALID ASERT
    //
    // PRESYNC -> REDOWNLOAD -> FINAL
    // ==================================================

    HeadersSyncState good{
        /*id=*/100,
        asert_params,
        sync_params,
        asert_chain_start,
        required_work
    };


    auto result{
        good.ProcessNextHeaders(
            pair,
            /*full_headers_message=*/true
        )
    };

    BOOST_REQUIRE(
        result.success
    );

    BOOST_REQUIRE_EQUAL(
        good.GetState(),
        State::REDOWNLOAD
    );


    result =
        good.ProcessNextHeaders(
            first,
            /*full_headers_message=*/true
        );

    BOOST_REQUIRE(
        result.success
    );

    BOOST_REQUIRE_EQUAL(
        good.GetState(),
        State::REDOWNLOAD
    );


    result =
        good.ProcessNextHeaders(
            second,
            /*full_headers_message=*/true
        );

    BOOST_REQUIRE(
        result.success
    );

    BOOST_CHECK_EQUAL(
        good.GetState(),
        State::FINAL
    );

    BOOST_CHECK_EQUAL(
        result.pow_validated_headers.size(),
        2U
    );


    // ==================================================
    // TEST 2:
    // INVALID ASERT nBits DURING PRESYNC
    // ==================================================

    CBlockHeader invalid_block2{
        block2
    };

    invalid_block2.nBits ^= 1U;


    const std::vector<CBlockHeader>
        invalid_second{
            invalid_block2,
        };


    HeadersSyncState bad_presync{
        /*id=*/101,
        asert_params,
        sync_params,
        asert_chain_start,
        required_work
    };


    result =
        bad_presync.ProcessNextHeaders(
            first,
            /*full_headers_message=*/true
        );

    BOOST_REQUIRE(
        result.success
    );

    BOOST_REQUIRE_EQUAL(
        bad_presync.GetState(),
        State::PRESYNC
    );


    result =
        bad_presync.ProcessNextHeaders(
            invalid_second,
            /*full_headers_message=*/true
        );

    BOOST_CHECK(
        !result.success
    );

    BOOST_CHECK_EQUAL(
        bad_presync.GetState(),
        State::FINAL
    );


    // ==================================================
    // TEST 3:
    // INVALID ASERT nBits DURING REDOWNLOAD
    // ==================================================

    HeadersSyncState bad_redownload{
        /*id=*/102,
        asert_params,
        sync_params,
        asert_chain_start,
        required_work
    };


    result =
        bad_redownload.ProcessNextHeaders(
            pair,
            /*full_headers_message=*/true
        );

    BOOST_REQUIRE(
        result.success
    );

    BOOST_REQUIRE_EQUAL(
        bad_redownload.GetState(),
        State::REDOWNLOAD
    );


    result =
        bad_redownload.ProcessNextHeaders(
            first,
            /*full_headers_message=*/true
        );

    BOOST_REQUIRE(
        result.success
    );

    BOOST_REQUIRE_EQUAL(
        bad_redownload.GetState(),
        State::REDOWNLOAD
    );


    result =
        bad_redownload.ProcessNextHeaders(
            invalid_second,
            /*full_headers_message=*/true
        );

    BOOST_CHECK(
        !result.success
    );

    BOOST_CHECK_EQUAL(
        bad_redownload.GetState(),
        State::FINAL
    );
}


BOOST_AUTO_TEST_CASE(vargamesh_auxpow_redownload_preserves_proof)
{
    /*
     * Exercise VMESH's real HeadersSync architecture:
     *
     * PRESYNC:
     *   full AuxPoW headers arrive and only commitments/work are kept.
     *
     * REDOWNLOAD:
     *   CompressedHeader remains the pure-header storage format,
     *   while the CAuxPow proof is retained in its parallel sidecar.
     *
     * RELEASE:
     *   reconstructed headers must still contain the exact AuxPoW
     *   proof required by AcceptBlockHeader().
     */

    Consensus::Params consensus{
        Params().GetConsensus()
    };

    consensus.nAuxpowChainId =
        0x564D;

    consensus.nAuxpowStartHeight =
        1;

    /*
     * This focused REDOWNLOAD test uses regtest's fixed easy
     * difficulty.  VMESH ASERT transition coverage remains in
     * the dedicated Gate-6 HeadersSync test cases.
     */
    consensus.fPowUseASERT =
        false;


    auto make_aux_header =
        [&](const uint256& prev_hash,
            const uint32_t ntime,
            const uint256& merkle_root) {
            CBlockHeader header;

            header.nVersion =
                0x20000000;

            header.SetAuxpowVersion(true);

            header.hashPrevBlock =
                prev_hash;

            header.hashMerkleRoot =
                merkle_root;

            header.nTime =
                ntime;

            header.nBits =
                genesis.nBits;

            header.nNonce =
                0x0000564D;

            header.SetAuxpow(
                CAuxPow::CreateMinimal(
                    header
                )
            );

            /*
             * Give the synthetic parent real proof-of-work so the
             * returned header can be checked by the complete VMESH
             * AuxPoW verifier as well.
             */
            while (
                !CheckProofOfWork(
                    header.auxpow
                        ->GetParentBlockHash(),
                    header.nBits,
                    consensus
                )
            ) {
                ++header.auxpow
                    ->parentBlock
                    .nNonce;
            }

            return header;
        };


    std::vector<CBlockHeader> chain;

    chain.emplace_back(
        make_aux_header(
            genesis.GetHash(),
            genesis.nTime + 1,
            uint256::ZERO
        )
    );

    chain.emplace_back(
        make_aux_header(
            chain.back().GetHash(),
            genesis.nTime + 2,
            uint256::ONE
        )
    );


    std::string error;

    for (const auto& header : chain) {
        BOOST_REQUIRE(
            header.IsAuxpow()
        );

        BOOST_REQUIRE(
            header.auxpow
        );

        BOOST_REQUIRE_MESSAGE(
            CheckAuxPowProofOfWork(
                header,
                consensus,
                &error
            ),
            error
        );
    }


    const arith_uint256 minimum_work{
        chain_start.nChainWork
        + GetBlockProof(chain[0])
        + GetBlockProof(chain[1])
    };


    HeadersSyncState hss{
        /*id=*/0,
        consensus,
        HeadersSyncParams{
            .commitment_period = 1,
            .redownload_buffer_size = 8,
        },
        chain_start,
        minimum_work
    };


    /*
     * First pass: reach the work threshold and enter REDOWNLOAD.
     */
    const auto presync{
        hss.ProcessNextHeaders(
            chain,
            /*full_headers_message=*/true
        )
    };

    BOOST_REQUIRE(
        presync.success
    );

    BOOST_REQUIRE(
        presync.request_more
    );

    BOOST_REQUIRE_EQUAL(
        presync.pow_validated_headers.size(),
        0U
    );

    BOOST_REQUIRE_EQUAL(
        hss.GetState(),
        State::REDOWNLOAD
    );


    /*
     * Second pass: same chain is verified against commitments and
     * released.  The CAuxPow objects must survive intact.
     */
    const auto redownload{
        hss.ProcessNextHeaders(
            chain,
            /*full_headers_message=*/false
        )
    };

    BOOST_REQUIRE(
        redownload.success
    );

    BOOST_CHECK(
        !redownload.request_more
    );

    BOOST_REQUIRE_EQUAL(
        redownload.pow_validated_headers.size(),
        chain.size()
    );

    BOOST_REQUIRE_EQUAL(
        hss.GetState(),
        State::FINAL
    );


    for (
        size_t i = 0;
        i < chain.size();
        ++i
    ) {
        const auto& recovered{
            redownload
                .pow_validated_headers[i]
        };

        BOOST_REQUIRE(
            recovered.IsAuxpow()
        );

        BOOST_REQUIRE(
            recovered.auxpow
        );

        BOOST_CHECK(
            recovered.GetHash()
            == chain[i].GetHash()
        );

        BOOST_CHECK(
            recovered.auxpow
                ->GetParentBlockHash()
            ==
            chain[i].auxpow
                ->GetParentBlockHash()
        );

        BOOST_CHECK_MESSAGE(
            CheckAuxPowProofOfWork(
                recovered,
                consensus,
                &error
            ),
            error
        );
    }
}


BOOST_AUTO_TEST_SUITE_END()

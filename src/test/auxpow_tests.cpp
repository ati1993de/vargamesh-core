// Copyright (c) 2026 VargaMesh developers
// Distributed under the MIT software license.

#include <auxpow.h>
#include <chainparams.h>
#include <kernel/cs_main.h>
#include <node/blockstorage.h>
#include <primitives/block.h>
#include <streams.h>
#include <test/util/setup_common.h>
#include <validation.h>

#include <boost/test/unit_test.hpp>

#include <cstdint>
#include <string>

BOOST_FIXTURE_TEST_SUITE(auxpow_tests, ChainTestingSetup)

static CPureBlockHeader
MakeVargaMeshChild()
{
    CPureBlockHeader child;

    /*
     * BIP9 top bits remain 001.
     * Bit 8 is reserved for AuxPoW.
     */
    child.nVersion =
        0x20000000
        | CPureBlockHeader::VERSION_AUXPOW;

    child.hashPrevBlock =
        uint256::ONE;

    child.hashMerkleRoot =
        uint256::ZERO;

    child.nTime =
        1'800'000'000;

    child.nBits =
        0x1d00ffff;

    /*
     * Gate 7B will consensus-enforce this chain tag
     * on every post-genesis VMESH block.
     */
    child.nNonce =
        0x0000564D;

    return child;
}


BOOST_AUTO_TEST_CASE(
    bip9_safe_auxpow_identity)
{
    const auto child{
        MakeVargaMeshChild()
    };

    BOOST_CHECK_EQUAL(
        child.nVersion & 0xE0000000,
        0x20000000
    );

    BOOST_CHECK(
        child.IsAuxpow()
    );

    BOOST_CHECK_EQUAL(
        child.nNonce,
        0x0000564DU
    );

    const auto params{
        CreateChainParams(
            *m_node.args,
            ChainType::MAIN
        )
    };

    BOOST_CHECK_EQUAL(
        params
            ->GetConsensus()
            .nAuxpowChainId,
        0x564D
    );

    BOOST_CHECK_EQUAL(
        params
            ->GetConsensus()
            .nAuxpowStartHeight,
        1
    );
}


BOOST_AUTO_TEST_CASE(
    pure_header_hash_invariant)
{
    CBlockHeader header;

    header.nVersion =
        0x20000000;

    header.hashPrevBlock =
        uint256::ONE;

    header.hashMerkleRoot =
        uint256::ZERO;

    header.nTime =
        1'700'000'000;

    header.nBits =
        0x1d00ffff;

    header.nNonce =
        12345;

    CPureBlockHeader pure;

    pure.nVersion =
        header.nVersion;

    pure.hashPrevBlock =
        header.hashPrevBlock;

    pure.hashMerkleRoot =
        header.hashMerkleRoot;

    pure.nTime =
        header.nTime;

    pure.nBits =
        header.nBits;

    pure.nNonce =
        header.nNonce;

    BOOST_CHECK(
        header.GetHash() == pure.GetHash()
    );

    BOOST_CHECK_EQUAL(
        GetSerializeSize(header),
        80U
    );

    BOOST_CHECK_EQUAL(
        GetSerializeSize(pure),
        80U
    );
}


BOOST_AUTO_TEST_CASE(
    minimal_auxpow_valid)
{
    const auto child{
        MakeVargaMeshChild()
    };

    auto proof{
        CAuxPow::CreateMinimal(
            child
        )
    };

    std::string error;

    BOOST_CHECK_MESSAGE(
        proof->Check(
            child.GetHash(),
            0x564D,
            &error
        ),
        error
    );
}


BOOST_AUTO_TEST_CASE(
    auxpow_serialization_roundtrip)
{
    const auto child{
        MakeVargaMeshChild()
    };

    auto original{
        CAuxPow::CreateMinimal(
            child
        )
    };

    DataStream stream;

    stream << *original;

    CAuxPow decoded;

    stream >> decoded;

    std::string error;

    BOOST_CHECK_MESSAGE(
        decoded.Check(
            child.GetHash(),
            0x564D,
            &error
        ),
        error
    );

    BOOST_CHECK(
        stream.empty()
    );
}


BOOST_AUTO_TEST_CASE(
    malformed_auxpow_rejected)
{
    const auto child{
        MakeVargaMeshChild()
    };

    const uint256 child_hash{
        child.GetHash()
    };


    {
        auto proof{
            CAuxPow::CreateMinimal(
                child
            )
        };

        proof->nIndex = 1;

        std::string error;

        BOOST_CHECK(
            !proof->Check(
                child_hash,
                0x564D,
                &error
            )
        );
    }


    {
        auto proof{
            CAuxPow::CreateMinimal(
                child
            )
        };

        proof->nChainIndex = 1;

        std::string error;

        BOOST_CHECK(
            !proof->Check(
                child_hash,
                0x564D,
                &error
            )
        );
    }


    {
        auto proof{
            CAuxPow::CreateMinimal(
                child
            )
        };

        proof
            ->vChainMerkleBranch
            .resize(
                31,
                uint256::ZERO
            );

        std::string error;

        BOOST_CHECK(
            !proof->Check(
                child_hash,
                0x564D,
                &error
            )
        );
    }


    {
        auto proof{
            CAuxPow::CreateMinimal(
                child
            )
        };

        proof->parentBlock.hashMerkleRoot =
            uint256::ONE;

        std::string error;

        BOOST_CHECK(
            !proof->Check(
                child_hash,
                0x564D,
                &error
            )
        );
    }


    {
        auto proof{
            CAuxPow::CreateMinimal(
                child
            )
        };

        CMutableTransaction tx{
            *proof->coinbaseTx
        };

        /*
         * More than one input means this is not a coinbase.
         */
        tx.vin.emplace_back();

        proof->coinbaseTx =
            MakeTransactionRef(
                std::move(tx)
            );

        proof->parentBlock.hashMerkleRoot =
            proof
                ->coinbaseTx
                ->GetHash()
                .ToUint256();

        std::string error;

        BOOST_CHECK(
            !proof->Check(
                child_hash,
                0x564D,
                &error
            )
        );
    }


    {
        auto proof{
            CAuxPow::CreateMinimal(
                child
            )
        };

        CMutableTransaction tx{
            *proof->coinbaseTx
        };

        auto& script{
            tx.vin[0].scriptSig
        };

        BOOST_REQUIRE(
            script.size()
            >= MERGED_MINING_HEADER.size()
        );

        script.erase(
            script.begin(),
            script.begin()
                + MERGED_MINING_HEADER.size()
        );

        proof->coinbaseTx =
            MakeTransactionRef(
                std::move(tx)
            );

        proof->parentBlock.hashMerkleRoot =
            proof
                ->coinbaseTx
                ->GetHash()
                .ToUint256();

        std::string error;

        BOOST_CHECK(
            !proof->Check(
                child_hash,
                0x564D,
                &error
            )
        );
    }


    {
        auto proof{
            CAuxPow::CreateMinimal(
                child
            )
        };

        CMutableTransaction tx{
            *proof->coinbaseTx
        };

        auto& script{
            tx.vin[0].scriptSig
        };

        script.insert(
            script.begin(),
            MERGED_MINING_HEADER.begin(),
            MERGED_MINING_HEADER.end()
        );

        proof->coinbaseTx =
            MakeTransactionRef(
                std::move(tx)
            );

        proof->parentBlock.hashMerkleRoot =
            proof
                ->coinbaseTx
                ->GetHash()
                .ToUint256();

        std::string error;

        BOOST_CHECK(
            !proof->Check(
                child_hash,
                0x564D,
                &error
            )
        );
    }


    {
        auto proof{
            CAuxPow::CreateMinimal(
                child
            )
        };

        CMutableTransaction tx{
            *proof->coinbaseTx
        };

        auto& script{
            tx.vin[0].scriptSig
        };

        /*
         * Strict minimal script layout:
         *
         * 4 bytes magic
         * 32 bytes root
         * 4 bytes tree size
         * 4 bytes tree nonce
         */
        BOOST_REQUIRE(
            script.size() >= 44
        );

        script[36] = 2;

        proof->coinbaseTx =
            MakeTransactionRef(
                std::move(tx)
            );

        proof->parentBlock.hashMerkleRoot =
            proof
                ->coinbaseTx
                ->GetHash()
                .ToUint256();

        std::string error;

        BOOST_CHECK(
            !proof->Check(
                child_hash,
                0x564D,
                &error
            )
        );
    }
}


BOOST_AUTO_TEST_CASE(
    chain_id_affects_deterministic_index)
{
    bool observed_difference{
        false
    };

    for (
        uint32_t nonce = 0;
        nonce < 1000;
        ++nonce
    ) {
        const auto vm{
            CAuxPow::GetExpectedIndex(
                nonce,
                0x564D,
                5
            )
        };

        const auto other{
            CAuxPow::GetExpectedIndex(
                nonce,
                0x1234,
                5
            )
        };

        BOOST_CHECK(
            vm >= 0
        );

        BOOST_CHECK(
            vm < 32
        );

        if (vm != other) {
            observed_difference = true;
            break;
        }
    }

    BOOST_CHECK(
        observed_difference
    );
}


BOOST_AUTO_TEST_CASE(auxpow_header_wire_roundtrip)
{
    CBlockHeader header;

    header.nVersion =
        0x20000000;

    header.hashPrevBlock =
        uint256::ONE;

    header.hashMerkleRoot =
        uint256::ZERO;

    header.nTime =
        1'800'000'000;

    header.nBits =
        0x1d00ffff;

    /*
     * VMESH's post-genesis chain tag.
     * Consensus enforcement comes in Gate 7B2.
     */
    header.nNonce =
        0x0000564D;


    // --------------------------------------------------
    // Plain header remains Bitcoin-compatible 80 bytes.
    // --------------------------------------------------

    BOOST_REQUIRE(
        !header.IsAuxpow()
    );

    BOOST_REQUIRE(
        !header.auxpow
    );

    BOOST_CHECK_EQUAL(
        GetSerializeSize(header),
        80U
    );

    const uint256 plain_hash{
        header.GetHash()
    };


    // --------------------------------------------------
    // Set AuxPoW flag BEFORE constructing the proof,
    // because the flag is part of the child block hash.
    // --------------------------------------------------

    header.SetAuxpowVersion(true);

    const uint256 aux_child_hash{
        header.GetHash()
    };

    BOOST_REQUIRE(
        aux_child_hash != plain_hash
    );

    auto proof{
        CAuxPow::CreateMinimal(
            header
        )
    };

    header.SetAuxpow(
        std::move(proof)
    );

    BOOST_REQUIRE(
        header.IsAuxpow()
    );

    BOOST_REQUIRE(
        header.auxpow
    );

    BOOST_CHECK(
        header.GetHash() == aux_child_hash
    );

    BOOST_CHECK(
        GetSerializeSize(header) > 80U
    );


    // --------------------------------------------------
    // Full wire roundtrip.
    // --------------------------------------------------

    DataStream stream;

    stream << header;

    CBlockHeader decoded;

    stream >> decoded;

    BOOST_CHECK(
        stream.empty()
    );

    BOOST_REQUIRE(
        decoded.IsAuxpow()
    );

    BOOST_REQUIRE(
        decoded.auxpow
    );

    BOOST_CHECK(
        decoded.GetHash()
        == header.GetHash()
    );

    BOOST_CHECK_EQUAL(
        decoded.nNonce,
        0x0000564DU
    );


    std::string error;

    BOOST_CHECK_MESSAGE(
        decoded.auxpow->Check(
            decoded.GetHash(),
            0x564D,
            &error
        ),
        error
    );


    // --------------------------------------------------
    // Removing AuxPoW restores plain-header wire format.
    // --------------------------------------------------

    decoded.SetAuxpow(
        std::unique_ptr<CAuxPow>{}
    );

    BOOST_CHECK(
        !decoded.IsAuxpow()
    );

    BOOST_CHECK(
        !decoded.auxpow
    );

    BOOST_CHECK_EQUAL(
        GetSerializeSize(decoded),
        80U
    );
}


BOOST_AUTO_TEST_CASE(auxpow_block_wire_roundtrip)
{
    CBlock block;

    block.nVersion =
        0x20000000;

    block.hashPrevBlock =
        uint256::ONE;

    block.nTime =
        1'800'000'120;

    block.nBits =
        0x1d00ffff;

    block.nNonce =
        0x0000564D;


    CMutableTransaction coinbase;

    coinbase.vin.resize(1);

    coinbase.vin[0]
        .prevout
        .SetNull();

    coinbase.vin[0]
        .scriptSig =
        CScript() << 1;

    coinbase.vout.resize(1);

    coinbase.vout[0]
        .nValue = 0;

    block.vtx.push_back(
        MakeTransactionRef(
            std::move(coinbase)
        )
    );

    block.hashMerkleRoot =
        block.vtx[0]
            ->GetHash()
            .ToUint256();


    block.SetAuxpowVersion(true);

    auto proof{
        CAuxPow::CreateMinimal(
            block
        )
    };

    block.SetAuxpow(
        std::move(proof)
    );

    const uint256 child_hash{
        block.GetHash()
    };


    DataStream stream;

    stream << TX_WITH_WITNESS(block);

    CBlock decoded;

    stream >> TX_WITH_WITNESS(decoded);

    BOOST_CHECK(
        stream.empty()
    );

    BOOST_REQUIRE(
        decoded.IsAuxpow()
    );

    BOOST_REQUIRE(
        decoded.auxpow
    );

    BOOST_CHECK(
        decoded.GetHash()
        == child_hash
    );

    BOOST_REQUIRE_EQUAL(
        decoded.vtx.size(),
        1U
    );

    BOOST_CHECK(
        decoded.vtx[0]->GetHash()
        == block.vtx[0]->GetHash()
    );


    std::string error;

    BOOST_CHECK_MESSAGE(
        decoded.auxpow->Check(
            decoded.GetHash(),
            0x564D,
            &error
        ),
        error
    );
}



BOOST_AUTO_TEST_CASE(auxpow_disk_header_roundtrip)
{
    CBlock block;

    block.nVersion =
        0x20000000;

    block.hashPrevBlock =
        uint256::ONE;

    block.nTime =
        1'800'000'240;

    block.nBits =
        0x1d00ffff;

    block.nNonce =
        0x0000564D;


    CMutableTransaction coinbase;

    coinbase.vin.resize(1);

    coinbase.vin[0]
        .prevout
        .SetNull();

    coinbase.vin[0]
        .scriptSig =
        CScript() << 1 << OP_0;

    coinbase.vout.resize(1);

    coinbase.vout[0]
        .nValue = 0;

    block.vtx.push_back(
        MakeTransactionRef(
            std::move(coinbase)
        )
    );

    block.hashMerkleRoot =
        block.vtx[0]
            ->GetHash()
            .ToUint256();


    /*
     * VERSION_AUXPOW is part of the pure child hash.
     * Set it before constructing the merged-mining proof.
     */
    block.SetAuxpowVersion(true);

    const uint256 child_hash{
        block.GetHash()
    };

    block.SetAuxpow(
        CAuxPow::CreateMinimal(
            block
        )
    );

    BOOST_REQUIRE(
        block.IsAuxpow()
    );

    BOOST_REQUIRE(
        block.auxpow
    );

    BOOST_REQUIRE(
        block.GetHash()
        == child_hash
    );


    node::BlockManager& blockman{
        Assert(m_node.chainman)->m_blockman
    };


    FlatFilePos position;

    {
        LOCK(cs_main);

        position =
            blockman.WriteBlock(
                block,
                /*nHeight=*/1
            );
    }


    BOOST_REQUIRE(
        !position.IsNull()
    );


    CBlockHeader recovered;

    BOOST_REQUIRE(
        blockman.ReadBlockHeader(
            recovered,
            position,
            child_hash
        )
    );


    BOOST_REQUIRE(
        recovered.IsAuxpow()
    );

    BOOST_REQUIRE(
        recovered.auxpow
    );

    BOOST_CHECK(
        recovered.GetHash()
        == child_hash
    );

    BOOST_CHECK_EQUAL(
        recovered.nVersion,
        block.nVersion
    );

    BOOST_CHECK_EQUAL(
        recovered.nNonce,
        0x0000564DU
    );

    BOOST_CHECK(
        recovered.auxpow
            ->GetParentBlockHash()
        ==
        block.auxpow
            ->GetParentBlockHash()
    );


    std::string error;

    BOOST_CHECK_MESSAGE(
        recovered.auxpow->Check(
            recovered.GetHash(),
            0x564D,
            &error
        ),
        error
    );


    /*
     * Wrong expected child hash must be rejected.
     */
    CBlockHeader rejected;

    BOOST_CHECK(
        !blockman.ReadBlockHeader(
            rejected,
            position,
            uint256::ONE
        )
    );
}


BOOST_AUTO_TEST_SUITE_END()

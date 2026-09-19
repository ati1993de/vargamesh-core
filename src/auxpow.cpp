// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2011 Vince Durham
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2023 Daniel Kraft
// Copyright (c) 2026 VargaMesh developers
// Distributed under the MIT software license.

#include <auxpow.h>

#include <primitives/block.h>
#include <pow.h>
#include <consensus/params.h>
#include <hash.h>
#include <script/script.h>

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace {

bool Fail(
    std::string* error,
    const char* message)
{
    if (error != nullptr) {
        *error = message;
    }

    return false;
}

uint32_t DecodeLE32(
    const unsigned char* bytes)
{
    return
        static_cast<uint32_t>(bytes[0])
        | (
            static_cast<uint32_t>(bytes[1])
            << 8
        )
        | (
            static_cast<uint32_t>(bytes[2])
            << 16
        )
        | (
            static_cast<uint32_t>(bytes[3])
            << 24
        );
}

void AppendLE32(
    CScript& script,
    const uint32_t value)
{
    script.push_back(
        static_cast<unsigned char>(
            value & 0xff
        )
    );

    script.push_back(
        static_cast<unsigned char>(
            (value >> 8) & 0xff
        )
    );

    script.push_back(
        static_cast<unsigned char>(
            (value >> 16) & 0xff
        )
    );

    script.push_back(
        static_cast<unsigned char>(
            (value >> 24) & 0xff
        )
    );
}

} // namespace


uint256
CAuxPow::CheckMerkleBranch(
    uint256 hash,
    const std::vector<uint256>& branch,
    int32_t index)
{
    if (index < 0) {
        return uint256{};
    }

    for (const auto& item : branch) {
        if (index & 1) {
            hash = Hash(item, hash);
        } else {
            hash = Hash(hash, item);
        }

        index >>= 1;
    }

    return hash;
}


int32_t
CAuxPow::GetExpectedIndex(
    const uint32_t nonce,
    const int32_t chain_id,
    const unsigned height)
{
    if (height > 30) {
        return -1;
    }

    const uint32_t mod{
        1u << height
    };

    uint64_t value{nonce};

    value =
        value * 1103515245ULL
        + 12345ULL;

    value %= mod;

    value +=
        static_cast<uint32_t>(chain_id);

    value =
        value * 1103515245ULL
        + 12345ULL;

    value %= mod;

    return static_cast<int32_t>(value);
}


bool
CAuxPow::Check(
    const uint256& aux_block_hash,
    const int32_t chain_id,
    std::string* error) const
{
    if (error != nullptr) {
        error->clear();
    }

    if (!coinbaseTx) {
        return Fail(
            error,
            "auxpow parent coinbase missing"
        );
    }

    if (!coinbaseTx->IsCoinBase()) {
        return Fail(
            error,
            "auxpow parent transaction is not coinbase"
        );
    }

    if (nIndex != 0) {
        return Fail(
            error,
            "auxpow parent coinbase index must be zero"
        );
    }

    if (vMerkleBranch.size() > 32) {
        return Fail(
            error,
            "auxpow parent merkle branch too long"
        );
    }

    if (vChainMerkleBranch.size() > 30) {
        return Fail(
            error,
            "auxpow chain merkle branch too long"
        );
    }

    const unsigned chain_height{
        static_cast<unsigned>(
            vChainMerkleBranch.size()
        )
    };

    const uint32_t chain_size{
        1u << chain_height
    };

    if (
        nChainIndex < 0
        || static_cast<uint32_t>(
            nChainIndex
        ) >= chain_size
    ) {
        return Fail(
            error,
            "auxpow chain index out of range"
        );
    }

    const uint256 chain_root{
        CheckMerkleBranch(
            aux_block_hash,
            vChainMerkleBranch,
            nChainIndex
        )
    };

    std::vector<unsigned char>
        root_bytes(
            chain_root.begin(),
            chain_root.end()
        );

    std::reverse(
        root_bytes.begin(),
        root_bytes.end()
    );

    const uint256 parent_root{
        CheckMerkleBranch(
            coinbaseTx
                ->GetHash()
                .ToUint256(),
            vMerkleBranch,
            nIndex
        )
    };

    if (
        parent_root
        != parentBlock.hashMerkleRoot
    ) {
        return Fail(
            error,
            "auxpow parent merkle root mismatch"
        );
    }

    if (coinbaseTx->vin.empty()) {
        return Fail(
            error,
            "auxpow coinbase has no inputs"
        );
    }

    const CScript& script{
        coinbaseTx->vin[0].scriptSig
    };

    auto header_pos{
        std::search(
            script.begin(),
            script.end(),
            MERGED_MINING_HEADER.begin(),
            MERGED_MINING_HEADER.end()
        )
    };

    if (header_pos == script.end()) {
        return Fail(
            error,
            "auxpow merged-mining header missing"
        );
    }

    auto second_header{
        std::search(
            header_pos
                + MERGED_MINING_HEADER.size(),
            script.end(),
            MERGED_MINING_HEADER.begin(),
            MERGED_MINING_HEADER.end()
        )
    };

    if (second_header != script.end()) {
        return Fail(
            error,
            "auxpow multiple merged-mining headers"
        );
    }

    auto root_pos{
        std::search(
            script.begin(),
            script.end(),
            root_bytes.begin(),
            root_bytes.end()
        )
    };

    if (root_pos == script.end()) {
        return Fail(
            error,
            "auxpow chain root missing"
        );
    }

    if (
        header_pos
            + MERGED_MINING_HEADER.size()
        != root_pos
    ) {
        return Fail(
            error,
            "auxpow header not immediately before chain root"
        );
    }

    auto data_pos{
        root_pos
        + root_bytes.size()
    };

    if (
        script.end() - data_pos
        < 8
    ) {
        return Fail(
            error,
            "auxpow tree size/nonce missing"
        );
    }

    const uint32_t encoded_size{
        DecodeLE32(
            &data_pos[0]
        )
    };

    if (encoded_size != chain_size) {
        return Fail(
            error,
            "auxpow tree size mismatch"
        );
    }

    const uint32_t tree_nonce{
        DecodeLE32(
            &data_pos[4]
        )
    };

    const int32_t expected_index{
        GetExpectedIndex(
            tree_nonce,
            chain_id,
            chain_height
        )
    };

    if (nChainIndex != expected_index) {
        return Fail(
            error,
            "auxpow wrong deterministic chain index"
        );
    }

    return true;
}


std::unique_ptr<CAuxPow>
CAuxPow::CreateMinimal(
    const CPureBlockHeader& child)
{
    const uint256 child_hash{
        child.GetHash()
    };

    std::vector<unsigned char>
        root_bytes(
            child_hash.begin(),
            child_hash.end()
        );

    std::reverse(
        root_bytes.begin(),
        root_bytes.end()
    );

    CScript script;

    script.insert(
        script.end(),
        MERGED_MINING_HEADER.begin(),
        MERGED_MINING_HEADER.end()
    );

    script.insert(
        script.end(),
        root_bytes.begin(),
        root_bytes.end()
    );

    // One-leaf chain tree.
    AppendLE32(script, 1);

    // Deterministic tree nonce.
    AppendLE32(script, 0);

    CMutableTransaction coinbase;

    coinbase.vin.resize(1);

    coinbase.vin[0]
        .prevout
        .SetNull();

    coinbase.vin[0]
        .scriptSig = script;

    coinbase.vout.resize(1);

    coinbase.vout[0]
        .nValue = 0;

    CTransactionRef coinbase_ref{
        MakeTransactionRef(
            std::move(coinbase)
        )
    };

    auto result{
        std::make_unique<CAuxPow>(
            coinbase_ref
        )
    };

    result->nIndex = 0;
    result->nChainIndex = 0;

    result->parentBlock.nVersion = 1;

    result->parentBlock.hashMerkleRoot =
        coinbase_ref
            ->GetHash()
            .ToUint256();

    result->parentBlock.nTime =
        child.nTime;

    result->parentBlock.nBits =
        child.nBits;

    result->parentBlock.nNonce = 0;

    return result;
}

bool
CheckAuxPowProofOfWork(
    const CBlockHeader& block,
    const Consensus::Params& params,
    std::string* error)
{
    if (error != nullptr) {
        error->clear();
    }

    /*
     * AuxPoW parameters are configured independently for the
     * other VMESH networks.
     *
     * Until then, nAuxpowChainId == 0 means that this network is
     * intentionally using the inherited native PoW behaviour.
     */
    if (params.nAuxpowChainId == 0) {
        if (
            !CheckProofOfWork(
                block.GetHash(),
                block.nBits,
                params
            )
        ) {
            return Fail(
                error,
                "proof of work failed"
            );
        }

        return true;
    }


    /*
     * No proof attached:
     *
     * This is valid context-free only when VERSION_AUXPOW is
     * also absent.  Height-dependent activation is checked by
     * CheckAuxPowHeightRules().
     */
    if (!block.auxpow) {
        if (block.IsAuxpow()) {
            return Fail(
                error,
                "auxpow version flag set but proof missing"
            );
        }

        if (
            !CheckProofOfWork(
                block.GetHash(),
                block.nBits,
                params
            )
        ) {
            return Fail(
                error,
                "proof of work failed"
            );
        }

        return true;
    }


    /*
     * A proof without VERSION_AUXPOW would make the wire
     * representation and consensus interpretation ambiguous.
     */
    if (!block.IsAuxpow()) {
        return Fail(
            error,
            "auxpow proof present without auxpow version flag"
        );
    }


    /*
     * The Bitcoin parent performs the work.
     *
     * IMPORTANT:
     * parentBlock.nBits is NOT the VMESH target.
     * The parent SHA256d hash is compared against the VMESH
     * child's nBits.
     */
    if (
        !CheckProofOfWork(
            block.auxpow->GetParentBlockHash(),
            block.nBits,
            params
        )
    ) {
        return Fail(
            error,
            "auxpow parent proof of work failed"
        );
    }


    std::string proof_error;

    if (
        !block.auxpow->Check(
            block.GetHash(),
            params.nAuxpowChainId,
            &proof_error
        )
    ) {
        if (error != nullptr) {
            *error =
                "auxpow proof invalid: "
                + proof_error;
        }

        return false;
    }


    return true;
}


bool
CheckAuxPowHeightRules(
    const CBlockHeader& block,
    const int height,
    const Consensus::Params& params,
    std::string* error)
{
    if (error != nullptr) {
        error->clear();
    }

    if (height < 0) {
        return Fail(
            error,
            "negative block height"
        );
    }


    /*
     * AuxPoW is not configured yet on the inherited auxiliary
     * VMESH test networks use network-specific AuxPoW configuration.
     */
    if (params.nAuxpowChainId == 0) {
        return true;
    }


    if (!params.AuxPowActive(height)) {
        if (block.IsAuxpow() || block.auxpow) {
            return Fail(
                error,
                "auxpow is not permitted before activation"
            );
        }

        return true;
    }


    if (!block.IsAuxpow()) {
        return Fail(
            error,
            "auxpow is mandatory at this height"
        );
    }

    if (!block.auxpow) {
        return Fail(
            error,
            "mandatory auxpow proof is missing"
        );
    }


    /*
     * VMESH deliberately does NOT place 0x564D in nVersion
     * high bits because that collides with Bitcoin Core BIP9.
     *
     * Instead every post-genesis VMESH child header carries the
     * chain tag in nNonce.  nNonce is part of the pure 80-byte
     * child hash and therefore part of the parent coinbase
     * commitment.
     */
    if (
        block.nNonce
        != static_cast<uint32_t>(
            params.nAuxpowChainId
        )
    ) {
        return Fail(
            error,
            "wrong VMESH auxpow chain tag in child nNonce"
        );
    }


    return true;
}

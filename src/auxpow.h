// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2011 Vince Durham
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2023 Daniel Kraft
// Copyright (c) 2026 VargaMesh developers
// Distributed under the MIT software license.

#ifndef BITCOIN_AUXPOW_H
#define BITCOIN_AUXPOW_H

#include <primitives/pureheader.h>
#include <primitives/transaction.h>
#include <serialize.h>
#include <uint256.h>

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>


namespace Consensus {
struct Params;
}

class CBlockHeader;

/** Standard merged-mining marker: fa be 6d 6d. */
inline constexpr std::array<unsigned char, 4>
    MERGED_MINING_HEADER{
        0xfa,
        0xbe,
        'm',
        'm'
    };

/**
 * VargaMesh AuxPoW proof.
 *
 * Wire format remains compatible with the established
 * Namecoin-style AuxPoW proof:
 *
 * parent coinbase transaction
 * legacy hashBlock placeholder
 * parent coinbase merkle branch
 * parent coinbase index
 * auxiliary-chain merkle branch
 * auxiliary-chain index
 * parent 80-byte header
 *
 * VargaMesh deliberately applies stricter validation because
 * there is no historical chain requiring legacy exceptions.
 */
class CAuxPow
{
public:
    CTransactionRef coinbaseTx;

    std::vector<uint256> vMerkleBranch;
    int32_t nIndex{0};

    std::vector<uint256> vChainMerkleBranch;
    int32_t nChainIndex{0};

    CPureBlockHeader parentBlock;

    CAuxPow() = default;

    explicit CAuxPow(CTransactionRef tx)
        : coinbaseTx{std::move(tx)}
    {
    }

    SERIALIZE_METHODS(CAuxPow, obj)
    {
        /*
         * hashBlock is retained solely for compatibility with
         * established AuxPoW serialisation.  It is not trusted.
         */
        uint256 hashBlock{};

        READWRITE(
            TX_WITH_WITNESS(obj.coinbaseTx),
            hashBlock,
            obj.vMerkleBranch,
            obj.nIndex
        );

        READWRITE(
            obj.vChainMerkleBranch,
            obj.nChainIndex,
            obj.parentBlock
        );
    }

    bool Check(
        const uint256& aux_block_hash,
        int32_t chain_id,
        std::string* error = nullptr
    ) const;

    uint256 GetParentBlockHash() const
    {
        return parentBlock.GetHash();
    }

    static int32_t GetExpectedIndex(
        uint32_t nonce,
        int32_t chain_id,
        unsigned height
    );

    static uint256 CheckMerkleBranch(
        uint256 hash,
        const std::vector<uint256>& branch,
        int32_t index
    );

    /**
     * Construct a minimal structurally valid AuxPoW proof.
     *
     * The parent header is not mined by this helper.
     * Gate 7B will validate its SHA-256d PoW.
     */
    static std::unique_ptr<CAuxPow>
    CreateMinimal(const CPureBlockHeader& child);
};


/**
 * Context-free proof-of-work validation.
 *
 * On networks without configured AuxPoW this preserves native
 * Bitcoin-style child-header PoW.
 *
 * On VMESH AuxPoW blocks:
 * - VERSION_AUXPOW and proof presence must match.
 * - Bitcoin parent SHA256d hash must satisfy the VMESH child nBits.
 * - Parent coinbase / merkle / aux-chain proof must commit to
 *   the VMESH 80-byte child hash.
 */
bool CheckAuxPowProofOfWork(
    const CBlockHeader& block,
    const Consensus::Params& params,
    std::string* error = nullptr
);

/**
 * Height-dependent VMESH AuxPoW identity rules.
 *
 * For configured VMESH:
 * - before activation: native PoW only.
 * - from activation height: AuxPoW is mandatory.
 * - post-activation child nNonce is the VMESH chain tag.
 */
bool CheckAuxPowHeightRules(
    const CBlockHeader& block,
    int height,
    const Consensus::Params& params,
    std::string* error = nullptr
);

#endif

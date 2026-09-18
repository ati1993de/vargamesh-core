// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Copyright (c) 2026 VargaMesh developers
// Distributed under the MIT software license.

#ifndef BITCOIN_PRIMITIVES_PUREHEADER_H
#define BITCOIN_PRIMITIVES_PUREHEADER_H

#include <serialize.h>
#include <uint256.h>
#include <util/time.h>

#include <chrono>
#include <cstdint>

/**
 * The consensus-hashed 80-byte block header.
 *
 * VargaMesh AuxPoW proofs are deliberately not part of this hash.
 * The parent Bitcoin coinbase commits to GetHash().
 *
 * Bit 8 is reserved by VargaMesh as the AuxPoW-presence flag.
 * Unlike legacy Namecoin/Dogecoin, the AuxPoW chain ID is NOT
 * stored in nVersion, preserving Bitcoin Core BIP9 top bits.
 */
class CPureBlockHeader
{
public:
    static constexpr int32_t VERSION_AUXPOW{1 << 8};

    int32_t nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nNonce;

    CPureBlockHeader()
    {
        SetNull();
    }

    SERIALIZE_METHODS(CPureBlockHeader, obj)
    {
        READWRITE(
            obj.nVersion,
            obj.hashPrevBlock,
            obj.hashMerkleRoot,
            obj.nTime,
            obj.nBits,
            obj.nNonce
        );
    }

    void SetNull()
    {
        nVersion = 0;
        hashPrevBlock.SetNull();
        hashMerkleRoot.SetNull();
        nTime = 0;
        nBits = 0;
        nNonce = 0;
    }

    bool IsNull() const
    {
        return nBits == 0;
    }

    uint256 GetHash() const;

    NodeSeconds Time() const
    {
        return NodeSeconds{
            std::chrono::seconds{nTime}
        };
    }

    int64_t GetBlockTime() const
    {
        return static_cast<int64_t>(nTime);
    }

    bool IsAuxpow() const
    {
        return (nVersion & VERSION_AUXPOW) != 0;
    }

    void SetAuxpowVersion(const bool value)
    {
        if (value) {
            nVersion |= VERSION_AUXPOW;
        } else {
            nVersion &= ~VERSION_AUXPOW;
        }
    }
};

#endif

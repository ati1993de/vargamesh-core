// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util/check.h>
#include <cstdlib>

/**
 * Absolute ASERTI3-2D target calculation.
 *
 * Ported from the established Bitcoin Cash integer ASERT reference
 * implementation. The algorithm intentionally uses fixed-point integer
 * arithmetic so consensus results do not depend on floating-point behavior.
 *
 * Formula approximated:
 *
 *   new_target =
 *       ref_target *
 *       2^((time_diff - target_spacing * (height_diff + 1))
 *           / half_life)
 *
 * The approximation polynomial and rounding constants below are part of the
 * established ASERTI3-2D implementation and therefore consensus-sensitive.
 */
arith_uint256 CalculateASERT(
    const arith_uint256& ref_target,
    const int64_t target_spacing,
    const int64_t time_diff,
    const int64_t height_diff,
    const arith_uint256& pow_limit,
    const int64_t half_life) noexcept
{
    assert(ref_target > 0);
    assert(ref_target <= pow_limit);
    assert((pow_limit >> 224) == 0);
    assert(height_diff >= 0);
    assert(target_spacing > 0);
    assert(half_life > 0);

    // Keep the fixed-point numerator inside signed 64-bit range.
    assert(
        llabs(
            time_diff -
            target_spacing * height_diff
        ) < (1LL << (63 - 16))
    );

    const int64_t exponent{
        (
            (
                time_diff -
                target_spacing * (height_diff + 1)
            ) * 65536
        ) / half_life
    };

    // C++20 guarantees arithmetic right shift for signed integers.
    static_assert(
        (int64_t{-1} >> 1) == int64_t{-1},
        "ASERT requires arithmetic right shift"
    );

    int64_t shifts{exponent >> 16};
    const auto frac{uint16_t(exponent)};

    assert(
        exponent ==
        shifts * 65536 + frac
    );

    // Approximate 2^x for x in [0,1):
    //
    // 1 + 0.695502049*x
    //   + 0.2262698*x^2
    //   + 0.0782318*x^3
    //
    // Maximum approximation error is below ~0.013%.
    const uint32_t factor{
        static_cast<uint32_t>(
            65536 +
            (
                (
                    +195766423245049ULL * frac
                    + 971821376ULL * frac * frac
                    + 5127ULL * frac * frac * frac
                    + (1ULL << 47)
                ) >> 48
            )
        )
    };

    arith_uint256 next_target{
        ref_target * factor
    };

    // factor contains an implicit 16-bit fixed-point scale.
    shifts -= 16;

    if (shifts <= 0) {
        next_target >>= -shifts;
    } else {
        const auto shifted{
            next_target << shifts
        };

        // Detect 256-bit overflow.
        if ((shifted >> shifts) != next_target) {
            next_target = pow_limit;
        } else {
            next_target = shifted;
        }
    }

    if (next_target == 0) {
        // Zero is not a valid PoW target.
        next_target = arith_uint256{1};
    } else if (next_target > pow_limit) {
        next_target = pow_limit;
    }

    return next_target;
}


uint32_t CalculateASERTWorkRequired(
    const uint32_t anchor_nbits,
    const int64_t anchor_parent_time,
    const int64_t anchor_height,
    const uint32_t previous_time,
    const int64_t previous_height,
    const Consensus::Params& params) noexcept
{
    assert(params.fPowUseASERT);
    assert(params.nPowTargetSpacing > 0);
    assert(params.nASERTHalfLife > 0);
    assert(previous_height >= anchor_height);

    const arith_uint256 pow_limit{
        UintToArith256(params.powLimit)
    };

    bool negative{false};
    bool overflow{false};

    arith_uint256 anchor_target;

    anchor_target.SetCompact(
        anchor_nbits,
        &negative,
        &overflow
    );

    assert(!negative);
    assert(!overflow);
    assert(anchor_target > 0);
    assert(anchor_target <= pow_limit);

    const int64_t time_diff{
        static_cast<int64_t>(previous_time) -
        anchor_parent_time
    };

    const int64_t height_diff{
        previous_height -
        anchor_height
    };

    return CalculateASERT(
        anchor_target,
        params.nPowTargetSpacing,
        time_diff,
        height_diff,
        pow_limit,
        params.nASERTHalfLife
    ).GetCompact();
}


uint32_t GetNextASERTWorkRequired(
    const CBlockIndex* pindex_last,
    const CBlockHeader*,
    const Consensus::Params& params) noexcept
{
    assert(pindex_last != nullptr);
    assert(params.fPowUseASERT);

    // Block 1 is the dynamic VargaMesh ASERT anchor.
    //
    // Its target is inherited from genesis. This deliberately avoids
    // hardcoding the final genesis hash/time before Gate 9.
    if (pindex_last->nHeight == 0) {
        return pindex_last->nBits;
    }

    const CBlockIndex* anchor{
        pindex_last->GetAncestor(1)
    };

    assert(anchor != nullptr);
    assert(anchor->nHeight == 1);
    assert(anchor->pprev != nullptr);

    return CalculateASERTWorkRequired(
        anchor->nBits,
        anchor->pprev->GetBlockTime(),
        anchor->nHeight,
        pindex_last->GetBlockTime(),
        pindex_last->nHeight,
        params
    );
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    if (params.fPowUseASERT) {
        return GetNextASERTWorkRequired(
            pindexLast,
            pblock,
            params
        );
    }

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then it MUST be a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;

    // Special difficulty rule for Testnet4
    if (params.enforce_BIP94) {
        // Here we use the first block of the difficulty period. This way
        // the real difficulty is always preserved in the first block as
        // it is not allowed to use the min-difficulty exception.
        int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
        const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
        bnNew.SetCompact(pindexFirst->nBits);
    } else {
        bnNew.SetCompact(pindexLast->nBits);
    }

    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

// Check that on difficulty adjustments, the new difficulty does not increase
// or decrease beyond the permitted limits.
bool PermittedDifficultyTransition(const Consensus::Params& params, int64_t height, uint32_t old_nbits, uint32_t new_nbits)
{
    if (params.fPowAllowMinDifficultyBlocks) return true;

    if (height % params.DifficultyAdjustmentInterval() == 0) {
        int64_t smallest_timespan = params.nPowTargetTimespan/4;
        int64_t largest_timespan = params.nPowTargetTimespan*4;

        const arith_uint256 pow_limit = UintToArith256(params.powLimit);
        arith_uint256 observed_new_target;
        observed_new_target.SetCompact(new_nbits);

        // Calculate the largest difficulty value possible:
        arith_uint256 largest_difficulty_target;
        largest_difficulty_target.SetCompact(old_nbits);
        largest_difficulty_target *= largest_timespan;
        largest_difficulty_target /= params.nPowTargetTimespan;

        if (largest_difficulty_target > pow_limit) {
            largest_difficulty_target = pow_limit;
        }

        // Round and then compare this new calculated value to what is
        // observed.
        arith_uint256 maximum_new_target;
        maximum_new_target.SetCompact(largest_difficulty_target.GetCompact());
        if (maximum_new_target < observed_new_target) return false;

        // Calculate the smallest difficulty value possible:
        arith_uint256 smallest_difficulty_target;
        smallest_difficulty_target.SetCompact(old_nbits);
        smallest_difficulty_target *= smallest_timespan;
        smallest_difficulty_target /= params.nPowTargetTimespan;

        if (smallest_difficulty_target > pow_limit) {
            smallest_difficulty_target = pow_limit;
        }

        // Round and then compare this new calculated value to what is
        // observed.
        arith_uint256 minimum_new_target;
        minimum_new_target.SetCompact(smallest_difficulty_target.GetCompact());
        if (minimum_new_target > observed_new_target) return false;
    } else if (old_nbits != new_nbits) {
        return false;
    }
    return true;
}

// Bypasses the actual proof of work check during fuzz testing with a simplified validation checking whether
// the most significant bit of the last byte of the hash is set.
bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    if (EnableFuzzDeterminism()) return (hash.data()[31] & 0x80) == 0;
    return CheckProofOfWorkImpl(hash, nBits, params);
}

std::optional<arith_uint256> DeriveTarget(unsigned int nBits, const uint256 pow_limit)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(pow_limit))
        return {};

    return bnTarget;
}

bool CheckProofOfWorkImpl(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    auto bnTarget{DeriveTarget(nBits, params.powLimit)};
    if (!bnTarget) return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}

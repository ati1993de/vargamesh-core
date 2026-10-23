// Copyright (c) 2026 VargaMesh developers
// Distributed under the MIT software license.

#include <auxpow.h>
#include <chainparams.h>
#include <primitives/block.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>
#include <util/chaintype.h>

#include <cstdint>
#include <optional>
#include <string>

// Fuzz real VMESH mainnet-style block-header parsing and AuxPoW proof
// validation, including malformed parent coinbase and merkle branches.
// The consensus implementation is intentionally not changed.
void initialize_vmesh_auxpow()
{
    SelectParams(ChainType::MAIN);
}

FUZZ_TARGET(vmesh_auxpow, .init = initialize_vmesh_auxpow)
{
    FuzzedDataProvider provider(buffer.data(), buffer.size());
    const std::optional<CBlockHeader> header{
        ConsumeDeserializable<CBlockHeader>(provider)
    };
    if (!header) return;

    const Consensus::Params& consensus{Params().GetConsensus()};
    std::string error;
    (void)CheckAuxPowHeightRules(*header, 1, consensus, &error);
    (void)CheckAuxPowProofOfWork(*header, consensus, &error);

    // Height zero may contain native PoW but not an AuxPoW proof.
    (void)CheckAuxPowHeightRules(*header, 0, consensus, &error);
}

# VargaMesh Mainnet

This document summarizes the VargaMesh mainnet configuration.

The implementation in the source tree is authoritative for consensus.

## Network identity

- Network: VargaMesh mainnet
- Currency symbol: VMESH
- P2P port: `29666`
- RPC port: `29667`
- Bech32 HRP: `vm`
- P2P user agent: `/VargaMesh:0.1.0/`

## Genesis block

- Hash: `00000000b0c55c00c13e2ee67b54fe33c327c8806f8da5050cfa47095d182d9a`
- Merkle root: `ab8120503c6bc408d07a5257f0dce8019ec494dbf627bd674c93d4f86122cb36`
- Time: `1789855200`
- Nonce: `1174766078`
- Bits: `1d00ffff`
- Initial difficulty: `1`

## Proof of Work

VargaMesh uses proof of work with ASERTI3-2D difficulty adjustment.

- Target block spacing: `120 seconds`
- ASERT half-life: `34560 seconds`
- AuxPoW chain ID: `22093`
- AuxPoW presence flag: bit 8 (`1 << 8`)

VargaMesh AuxPoW proof data is serialized separately from the pure 80-byte
child block header.

Relevant implementation files include:

- `src/kernel/chainparams.cpp`
- `src/consensus/params.h`
- `src/pow.cpp`
- `src/pow.h`
- `src/auxpow.cpp`
- `src/auxpow.h`
- `src/primitives/pureheader.h`

## Monetary parameters

- Initial block subsidy: `50 VMESH`
- Subsidy halving interval: `1051200 blocks`
- Coinbase maturity: `100 blocks`
- `MAX_MONEY` validation bound: `52,560,000 VMESH`

The `MAX_MONEY` constant is a consensus validation range and should not by
itself be interpreted as a statement of final circulating supply.

## Runtime separation

VargaMesh source code does not require publication of private keys.

Wallet state, RPC cookies, RPC authentication data, server credentials and
operator configuration belong outside the source repository.

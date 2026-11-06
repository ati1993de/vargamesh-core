# VargaMesh Core v0.3.1 — Native AuxPoW RPC (release candidate)

Status: **candidate, not yet a published Mainnet release**.

## Added

- `createauxblock` with an explicit VMESH coinbase payout address.
- `getauxblock` legacy fetch/submit style, using optional `-auxpowaddress`.
- `submitauxblock` with serialized Namecoin-style `CAuxPow` proof input.
- Thread-safe, bounded candidate cache, stale tip/expiry checks, proof-size limits.
- Reuse of existing VMESH AuxPoW and full block validation; no bypass of consensus checks.

## Not changed

- Mainnet consensus, subsidy, ASERT, 120-second block spacing, AuxPoW proof serialization, P2P network, addresses and blockchain history.
- Existing miners using `getblocktemplate` and `submitblock`.

## Compatibility

The new RPCs are **opt-in**. A pool that needs them updates its directly connected VMESH node. Existing wallet, exchange and ordinary nodes require no upgrade *provided compatibility testing succeeds*. RPC field names alone do not guarantee every third-party pool is plug-and-play; check the exact pool-specific AuxPoW commitment byte order.

See `doc/native-auxpow-rpc.md` for configuration, threat model, testnet gate and gradual production deployment.

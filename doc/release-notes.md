# VargaMesh Core v0.3.0

Release date: 23 October 2026 (subject to successful automated release builds).

## Highlights

- Adds five independently probed VMESH Mainnet IPv4 fixed P2P seeds (port 29666) as a fallback to the existing DNS bootstrap seed: 217.154.93.20, 217.217.78.186, 31.70.81.5, 37.221.211.210, and 212.132.80.39. Reachability was confirmed through VMESH network-magic/protocol handshakes; this does not independently prove each node's chain tip or long-term availability.
- Adds the read-only `getvmeshminingstatus` RPC for current tip, ASERT configuration, AuxPoW chain identity, expected next bits, and chainwork.
- Adds `checkvmeshauxpow <serialized_block_hex>` to diagnose VMESH height-specific AuxPoW rules and proof validation without submitting a block. This is **not** full block or UTXO validation.
- Adds ASERT difficulty stress tests, AuxPoW fuzz coverage, a functional RPC smoke test, and network fixed-seed checks.
- Provides operator tools to probe VMESH P2P seeds and compare independent nodes' block hash and chainwork.

## Network and mining compatibility

This is a non-consensus hardening release. It does **not** introduce a hard fork, change VMESH Mainnet genesis, SHA-256d, ASERT target spacing or half-life, subsidy, chain ID, AuxPoW activation, or address formats. Existing `getblocktemplate` / `submitblock` RPC behavior remains unchanged.

Native Namecoin-style `getauxblock` / `submitauxblock` compatibility is **not** included in v0.3.0; pool operators requiring those RPCs still need an external adapter.

## Upgrading

This is a recommended, **not mandatory**, node upgrade. For production nodes and exchanges:

1. Back up wallet keys and configuration and record the current chain height/hash.
2. Shut down the existing VMESH daemon cleanly; preserve the existing chainstate and wallet data directories.
3. Verify the downloaded archive against the release `SHA256SUMS`; install the binary for the correct operating system and architecture.
4. Start the upgraded daemon; confirm `getblockchaininfo`, `getnetworkinfo`, and `getvmeshminingstatus` from the local CLI.
5. Roll out gradually, one independently operated node at a time, and ensure peer connections and chain synchronization remain healthy.

Existing exchanges, including NestEx, do not need an emergency upgrade for this release. Send a routine compatibility notice after release assets are available.

## Validation and provenance

PR #9 (https://github.com/ati1993de/vargamesh-core/pull/9) passed Linux unit/functional tests; Windows headless packaging checks; macOS Intel/Apple Silicon cross-builds and execution/mainnet+testnet smokes; seed-protocol checks of 5/5 proposed seeds; and deterministic AuxPoW fuzz smokes. v0.3.0 archive publication is independently gated on release-workflow success.

The release builds are headless node and CLI packages, not desktop GUI installers. Do not assume binaries are code-signed unless a separately verified signature is supplied.

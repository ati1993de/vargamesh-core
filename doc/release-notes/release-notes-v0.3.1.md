# VargaMesh Core v0.3.1

**Release type:** Mainnet-compatible, non-consensus maintenance release.  
**Release date:** 6 November 2026 (source preparation; the publication timestamp on GitHub Releases is authoritative).  
**Platform:** Headless Core binaries for Linux x86_64, Windows x86_64, macOS x86_64 and macOS arm64.

## Highlights

VargaMesh Core v0.3.1 adds direct Namecoin-style JSON-RPC work generation and solution submission to its existing auxiliary Proof-of-Work support.

- `createauxblock "vm1…"` creates and caches a VMESH child-block candidate paying the supplied address.
- `getauxblock` creates an auxiliary work item using the pool node's configured `-auxpowaddress`; `getauxblock "hash" "proof_hex"` accepts legacy solution submissions.
- `submitauxblock "hash" "proof_hex"` deserializes a complete `CAuxPow` parent proof and submits the resulting block through the existing full block-validation path.
- Includes VMESH-specific parent coinbase/merkle commitment handling, serialized-proof input limits and bounded/stale-tip-aware work caching.
- Adds no new P2P message types, network rules, wallet format changes, forks or consensus activations.
- Continues to support existing `getblocktemplate` / `submitblock` pools.

## Consensus and backward compatibility

The Mainnet consensus parameters remain unchanged: SHA-256d, ASERT, 120-second block target, 34,560-second half-life, 25 VMESH initial subsidy, 1,051,200-block halving interval, AuxPoW chain ID 22093 and existing block serialization. The new mining RPCs are optional; ordinary wallets and exchange nodes need not update only to follow the existing network.

**Integration caveat:** The new RPC methods do not automatically match every Namecoin/Dogecoin/BTC merged-mining pool. Pool software must build the specific VMESH AuxPoW commitment in the Bitcoin coinbase and serialize proof branches correctly. Do not enable the new native work path for an entire live pool fleet without a dedicated positive submission/payout test.

## Verification

Successful feature-branch CI at commit `b021c7f210ae87d4c0cf8e6f08eba6088178e841` includes:

- Linux Core build, C++ unit and wallet-free RPC negative/argument functional tests.
- Windows Core headless binaries and version/packaging checks.
- macOS Intel and Apple Silicon cross-builds and native Mainnet/Testnet runtime smoke tests.
- Mainnet peer seed handshakes and deterministic AuxPoW fuzz-target tests.
- Existing *consensus-layer positive* AuxPoW C++ tests using a reduced-difficulty test context.

The existing Mainnet v0.3.1 node and previously deployed BTCHashPool mining services were reported active by the operator after an initial RPC-cookie/startup issue resolved on a VPS restart. **This is not equivalent to a demonstrated native `submitauxblock` positive block accepted by both v0.3.0 and v0.3.1**.

**Known testing limitation at publication:** There is no independently recorded positive end-to-end native `createauxblock` / `submitauxblock` test proving that a mined block is accepted by both versioned nodes, including payout and stale/duplicate submission behavior. Until such evidence is available, consider these new RPC endpoints *experimental for production merged-mining integration*. Existing GBT/full-block mining remains unchanged.

Release-archive checks, final exact-commit CI and SHA256 checksum publication are controlled by GitHub Actions. Consult the actual release run and attached checksum files rather than assuming any particular artifact was built.

## Upgrade steps

1. Download the correct architecture from [GitHub Releases](https://github.com/ati1993de/vargamesh-core/releases) and verify the published checksum file.
2. Back up wallet data, node settings and the current binary. Record the tip hash, height and cumulative chainwork.
3. Stop the daemon cleanly; replace **only** intended executables, keeping the existing data directory.
4. Restart and inspect `getblockchaininfo`, `getnetworkinfo`, `getvmeshminingstatus` and the real pool bridge logs.
5. If adopting the new API, use an isolated pool-only node with authenticated, restricted RPC and verify live end-to-end proof acceptance before cutover.
6. Keep the previous binaries and backups until the upgraded node is stable.

See the [README](../../README.md), [native AuxPoW documentation](../native-auxpow-rpc.md) and [compatibility checklist](../release-checklist-v0.3.1.md). This software is open source and distributed without warranty.

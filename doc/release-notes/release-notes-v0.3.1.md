# VargaMesh Core v0.3.1 — Native AuxPoW RPC

**Status:** Release candidate; not yet an officially published Mainnet binary release.  
**Planned version:** 0.3.1.  
**Scope:** Additional mining RPC interfaces; no changes to VMESH consensus rules.

## What's new

- `createauxblock <vm1-address>` produces a finalized child-block job with a coinbase paying the specified VMESH address.
- `getauxblock` supports legacy Namecoin-style job retrieval using the `-auxpowaddress` setting.
- `submitauxblock <child-hash> <proof-hex>` and two-argument `getauxblock` accept a serialized `CAuxPow` parent proof.
- The new API exposes a child hash, previous block hash, chain ID, bits, target, height and coinbase reward.
- Bounded, thread-safe work cache: up to 128 candidates, 15-minute expiry, stale-tip cleanup and proof-size limit.
- The child header Merkle root is finalized before its block hash is advertised to the Bitcoin parent miner.
- Wallet-independent RPC smoke tests supplement the existing AuxPoW fuzz and consensus test coverage.

**Existing pool integration is preserved.** `getblocktemplate` and `submitblock` are not removed or redefined. Pool coordinators that use the existing bridge may continue without the new methods. Pools adopting the native interface must ensure their Bitcoin-parent coinbase and Merkle commitment match VMESH's specific consensus encoding. Namecoin-style API does **not** imply automatic pool compatibility.

## Consensus neutrality

No edits are made to core AuxPoW proof validation, SHA-256d hashing, ASERT adjustment, 120-second spacing, genesis, block subsidy, activation height, chain ID, address formats or P2P block representation. The new functions are intended to be strictly RPC-only. Other full nodes, exchange nodes and existing pool software should not require an urgent update once mixed-version block validation is independently demonstrated.

## Build and test record

The preceding PR source commit `c74573663d379ec71884f65c009774224bda75b6` passed both GitHub Actions workflows: Linux build/unit/RPC smoke, Windows headless builds, macOS Intel and Apple Silicon cross-build and native execution smokes, mainnet seed handshakes and AuxPoW fuzz-target smoke.

**These checks must be repeated on the final v0.3.1 source commit.** They do not themselves prove that a complete merged-mining proof is accepted across old and new nodes.

## Remaining release prerequisites

1. Produce a valid VMESH public-testnet block via `createauxblock` / `submitauxblock` with a Bitcoin parent coinbase.
2. Confirm the same block hash, height and cumulative chainwork are accepted on both candidate v0.3.1 and unmodified v0.3.0 nodes. Also test the two-argument `getauxblock` path.
3. Verify reward payout destination, child Merkle root, witness commitment, malformed/stale/oversized proof rejection and duplicate submissions.
4. Smoke-test the existing pool bridge using `getblocktemplate` / `submitblock` before replacing any production binary.
5. Inspect the final commit diff, rerun all CI gates on exact merged SHA, run the version-specific release workflow, verify all SHA256SUMS and archive contents.

## Upgrade and security notes

Keep the mining RPC authenticated and private; `createauxblock` takes an explicit destination, so do not grant it to untrusted users. Back up wallet state and configuration, preserve the blockchain data directory, note the current chain tip and keep old binaries available for rollback. Deploy first to an isolated pool-only synchronized node and observe both new and existing peers before a production cutover.

Read [the native AuxPoW documentation](../native-auxpow-rpc.md) and [the release checklist](../release-checklist-v0.3.1.md). This source branch is not a binary release or a hard fork announcement.

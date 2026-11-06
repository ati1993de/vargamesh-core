# VargaMesh Core v0.3.1 — release verification and rollout record

This document distinguishes **verified build results** from **follow-up integration testing**. It does not claim the new native AuxPoW RPC produces a chain-accepted block until one has been observed.

## Completed

- [x] Review v0.3.1 changes against v0.3.0: mining RPC integration, configuration, tests, documentation and build tooling; no changes to VMESH consensus validation sources
- [x] Candidate commit `b021c7f210ae87d4c0cf8e6f08eba6088178e841` passed Linux, Windows, both macOS, seeds and AuxPoW fuzz GitHub Actions
- [x] Linux native RPC negative/argument smoke tests passed
- [x] Existing positive AuxPoW consensus-level C++ unit tests passed in deterministic test context
- [x] Node operator reported the v0.3.1 Mainnet binary active and previously running BTC/VMESH pool services active after VPS restart
- [x] Security instructions cover private RPC, payout-address authorization, backups and rollback

## Required checks for the actual published release artifacts

- [ ] All standard CI and release gates pass on the exact **merged main commit**
- [ ] Tagged version and GitHub Release source point to that exact passing commit
- [ ] Linux and Windows published archives pass their archive/runtime build and checksums
- [ ] macOS Intel and Apple Silicon published archives pass native runtime checks
- [ ] `SHA256SUMS` and `SHA256SUMS-macOS` exist and verify the downloaded archive files
- [ ] GitHub Release notes and public README link to the actual published release

## Native AuxPoW integration follow-up (known limitation)

- [ ] Mine a **valid positive VMESH public-testnet child block** with `createauxblock` + `submitauxblock`
- [ ] Verify the same block hash/height/chainwork is independently accepted by both an unmodified v0.3.0 node and a v0.3.1 node
- [ ] Test legacy `getauxblock` two-argument submission and failed/stale/duplicate proof handling
- [ ] Verify the VMESH coinbase payout destination, witness commitments and parent-chain Merkle proof
- [ ] Validate the existing `getblocktemplate` / `submitblock` bridge and reorg/restart recovery under a controlled mining load

Until all of these integration tests pass, **the new native AuxPoW RPCs are considered experimental**. Normal network and GBT use do not require this new interface. Do not claim a fully verified pool integration or mandatory upgrade.

## Rollout / rollback

Keep verified backups of configuration, wallet and the older Core executable. Upgrade one synchronized pool-only node first, observe its tip, peers, jobs, submissions and payout accounting, and do not update every pool at once. Protect RPC credentials. Restore the previous executable if monitoring detects divergence, error spikes or invalid proof submissions; preserve wallet and chainstate data.

## Publication automation

The v0.3.1 coordinator must see successful `VargaMesh CI` and `VMESH Release Gates` workflows on the **same exact main SHA**. It then dispatches the pinned `release-binaries.yml` workflow for Linux/Windows and schedules separate macOS verified builds. Only report full release completion after GitHub shows the tag, all four archives and their checksum files.

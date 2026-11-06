# VMESH Core v0.3.1 — Mainnet release gate

> [!CAUTION]
> Do not merge, publish, tag or install on an active Mainnet pool while any mandatory test below is unverified.

## Technical controls

- [x] PR #11 adds new mining RPC methods without changing VMESH consensus files
- [x] Candidate commit `c74573663d379ec71884f65c009774224bda75b6` passed Linux, Windows, macOS, fuzz and seed-check CI
- [ ] Repeat all GitHub Actions on the **final** version-bumped commit
- [ ] Review diff against the published `v0.3.0` tag for all block format, validation and consensus changes
- [ ] Confirm a positively solved AuxPoW child block on public VMESH testnet using `createauxblock` + `submitauxblock`
- [ ] Confirm an unmodified v0.3.0 node and the candidate v0.3.1 node accept the **same** testnet block hash/height/chainwork
- [ ] Confirm the legacy two-argument `getauxblock` proof submission also works
- [ ] Verify the VMESH coinbase payout and witness commitment are correct
- [ ] Check oversized, malformed, stale, duplicate and invalid parent-proof submissions
- [ ] Check current mining pool bridge using `getblocktemplate` / `submitblock` on an isolated deployment
- [ ] Review node restart/recovery and rollback, including maintaining existing wallet and chainstate
- [ ] Obtain final code review approval

## Testnet compatibility capture

Run v0.3.0 and v0.3.1 as independent, privately administered public-testnet nodes. Before and after submitting a newly mined child block, record on both:

~~~bash
vargamesh-cli -testnet getblockchaininfo
vargamesh-cli -testnet getnetworkinfo
vargamesh-cli -testnet getvmeshminingstatus
~~~

Record the accepted block ID, tip hash, height and chainwork. Redact all RPC passwords, private wallet paths and keys. Do not use a live mainnet payout wallet for the test.

## Publication only after the tests above

The old v0.3.0 automatic release coordinator is version-locked. For v0.3.1, merge the reviewed PR into `main`, require successful CI and release-gates workflows for the **exact merged SHA**, then manually trigger `release-binaries.yml` using `tag=v0.3.1` and `commit=<the merged, CI-green SHA>`.

The release workflow creates and verifies Linux and Windows archives and dispatches the version-aligned macOS workflow. Confirm both macOS architectures, checksums, published release notes and Git tag before announcing release. Never overwrite a published tag.

## Gradual rollout

Pilot the final binary on a separate synchronized pool-only VMESH node, compare tips with older nodes, confirm valid pool jobs and mining payouts, retain previous binaries and wallet backups, then switch one production mining connector. Exchanges and normal nodes are not asked to update simply because new pool RPCs exist.

# Native AuxPoW mining RPC — VMESH Core v0.3.1 candidate

This change adds mining-only RPC methods to the existing VMESH AuxPoW implementation. **It does not change consensus**, the 80-byte child block header, coin subsidy, difficulty/ASERT, AuxPoW chain ID, genesis, activation height, P2P wire data, or address formats.

## JSON-RPC API

The existing `getblocktemplate` / `submitblock` integration remains supported and unchanged.

- `createauxblock "vm1..."`: creates and caches a block candidate whose coinbase pays the specified VMESH address.
- `getauxblock`: legacy Namecoin/Dogecoin-style request, using the node setting `auxpowaddress=vm1...`.
- `submitauxblock "child-hash" "serialized-CAuxPoW-hex"`: submits a solution.
- `getauxblock "child-hash" "serialized-CAuxPoW-hex"`: legacy solution submission.

A job contains `hash`, `chainid`, `previousblockhash`, `coinbasevalue` (satoshis), `bits`, `height`, `target`, `_target`. Both target fields use the legacy **little-endian** 32-byte target encoding; `hash` is the usual RPC display hex.

VMESH parent coinbase commitment rules are **stricter than some legacy AuxPoW chains**: include the single magic marker `fabe6d6d`, followed immediately by the **byte-reversed** 32-byte auxiliary merkle root, 4-byte little-endian tree size, and 4-byte little-endian nonce. The supported one-auxiliary-chain case uses tree size 1, nonce 0, index 0. The parent block hash must meet the **VMESH child** target (not merely the Bitcoin target). Pool coordinators must supply full canonical serialized `CAuxPow` including the legacy 32-byte `hashBlock` placeholder, parent coinbase, both Merkle branches, and parent header.

## Configure

In a **pool-only** VMESH full node configuration:

```ini
server=1
auxpowaddress=vm1...YOUR_POOL_PAYOUT_ADDRESS...
# Keep RPC on localhost or a private authenticated network.
rpcbind=127.0.0.1
```

Restart the upgraded pool node, then:

```bash
vargamesh-cli getvmeshminingstatus
vargamesh-cli getauxblock
vargamesh-cli createauxblock "vm1...YOUR_POOL_PAYOUT_ADDRESS..."
vargamesh-cli submitauxblock "childhash" "canonicalauxpowhex"
```

The cache keeps up to 128 candidates for 15 minutes, discards expired and stale-tip jobs, and limits serialized proof input to 128 KiB. Pool RPC users must be trusted: an attacker with unrestricted mining RPC access could create candidates paying their own address. Prefer RPC whitelists for authorized pool accounts. **Do not expose RPC to the public internet.**

## Non-consensus deployment protocol

1. Record the old node tip/height/chainwork, and make verified backups of wallet and configuration.
2. Validate C++ compile, unit tests, RPC help/doc checks, sanitizer/fuzz tests, testnet positive and negative Proof-of-Work cases; verify rejected stale/malformed/oversize proofs.
3. On the public VMESH testnet, run two independent nodes: the current release and the candidate. Mine a merged block through `createauxblock` + `submitauxblock`, verify that **both** accept the same block hash and chainwork. Repeat with `getauxblock`.
4. Check payouts and that the block's hash is invariant when the proof is attached.
5. Run controlled long-duration pool integration with getwork/GBT, stale blocks, node restarts, and old/new peers.
6. Build release archives through CI, inspect signing/provenance and SHA256SUMS; if all pass, merge and tag v0.3.1.
7. Deploy to one pool-only mainnet node first. **Exchange nodes such as NestEx and other peers need no mandatory update** if all above checks confirm unchanged consensus. Monitor chain tip and peer divergence during rollout.

## Release gating

**This feature branch is not itself a production release.** Do not upgrade production or announce a verified release from this branch before successful CI and independent testnet/old-node compatibility evidence.

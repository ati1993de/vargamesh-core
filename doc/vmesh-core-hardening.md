# VMESH Core 0.3 development: network, mining and security

This document describes optional operational changes. It is **not** a Mainnet
consensus upgrade. Never apply test-chain or unverified checkpoints to Mainnet.

## Read-only mining diagnostics

`vargamesh-cli getvmeshminingstatus` returns:

- active tip height and hash;
- current PoW target for the next block, ASERT spacing and half-life;
- AuxPoW chain ID, activation height, required child nonce and next-block status;
- accumulated tip chainwork and configured minimum chainwork.

This is useful for pool troubleshooting and exchange-node setup. Current
production merged miners still use `getblocktemplate` and `submitblock`.
**The new RPC is not a drop-in implementation of Namecoin's
`getauxblock` / `submitauxblock` API.** Adding those requires a separate,
fully tested adapter that reconstructs a complete VMESH block with a valid
parent coinbase, parent header, parent Merkle path and chain Merkle path.

## Mainnet bootstrap candidates

The default compiled Mainnet DNS seed remains
`seed.mesh.vargatech.net`. New addresses MUST pass VMESH P2P protocol
handshake and sustain real Full Node service before becoming default seeds.

| Operator | Candidate public host | Status |
| --- | --- | --- |
| Main project node | `vargamesh.com:29666` | **Unverified** until probed |
| Asia VMESH node | `asia.vargamesh.com:29666` | **Unverified** until probed |
| Community-offered independent node | `rodb2008.com:29666` | **Unverified** until probed |
| Windrose VPS | Public node address required | Not added |
| Varga-Tech home server | Public routable P2P endpoint required | Not added |
| Additional Mainnet VPS | Stable public P2P host/IP required | Not added |
| HU VPS | Stable public P2P host/IP required | Not added |

Websites, reverse proxies, pool stratum ports, local daemon RPC ports, and
NAT-only home servers are NOT valid P2P seeds by themselves. The Asia service
may expose a web hostname that does not point at its P2P listener.

Run on a host with public DNS and port 29666 access:

```sh
python3 contrib/devtools/vmesh_seed_probe.py \
  vargamesh.com:29666 asia.vargamesh.com:29666 rodb2008.com:29666
```

The script validates VMESH Mainnet message magic (`4638b879`), checksum,
and P2P `version` reply. It prints one JSON line per candidate. Success
does not establish independence, uptime, quality, or honest chain selection.

For VERIFIED peers, an operator may test manually:

```sh
vargamesh-cli addnode "asia.vargamesh.com:29666" "onetry"
vargamesh-cli getpeerinfo
```

Use stable `-addnode` entries for persistent connectivity only after
observing real service. Before any change to compiled `vSeeds` or
`vFixedSeeds`, verify all hosts in independent networks, log uptime,
confirm operators' permission and peer diversity. A DNS seed should ideally
return a dynamic set of healthy P2P node addresses, not merely the IP of
an arbitrary HTTPS endpoint.

## Chainwork and checkpoints

Mainnet currently configures `nMinimumChainWork = 0` and
`defaultAssumeValid = 0`, appropriate for launch, but without chainwork
bootstrap pinning. Do not copy one node's current tip chainwork blindly:
a stale or manipulated node can supply false data.

```sh
python3 contrib/devtools/vmesh_chainwork_audit.py \
  --datadir /srv/vmesh/node-a --datadir /srv/vmesh/node-b --height 25000
```

The audit compares genesis, historical block hash and accumulated chainwork
between separately selected local RPC profiles. The provided paths are only
examples; use real synchronized nodes. Multiple paths controlled by a single
operator are not a substitute for independent validation. An accepted
historical block, a network-appropriate minimum chainwork and explicit
release review are prerequisites for hard-coding values into Core. No
checkpoint is applied by this PR.

## Inherited Bitcoin network modes

VMESH Mainnet (native `vm1`, P2P `29666`) and VMESH Public Testnet v1
(native `tvm1`, P2P `39666`) are the supported VMESH networks.

The repository also inherits Bitcoin-like `testnet4` and `signet`
network definitions as well as a legacy Bitcoin-like `regtest`. Those
modes are **not replacements for VMESH Public Testnet**. Avoid deploying
them as public VMESH services or assuming their consensus/test vectors
represent VMESH ASERT/AuxPoW. Their removal or migration must be a
separately planned change, with test coverage and user-facing migration
notes.

A VMESH-specific isolated regtest mode exercising mandatory AuxPoW and ASERT
is a prerequisite before changing these inherited networks.

## Testing and release gate

- `ctest --test-dir build --output-on-failure` runs existing unit tests,
  including VMESH ASERT and AuxPoW cases.
- `python3 build/test/functional/test_runner.py feature_vmesh_mining_status.py`
  checks the new RPC on isolated legacy regtest.
- `FUZZ=vmesh_auxpow build_fuzz/bin/fuzz` exercises malformed VMESH AuxPoW
  header parsing and proof checks once a fuzz-enabled binary is built.
- Full release still requires two-node actual Mainnet/Testnet sync,
  real pool template and solved-parent-proof submission, reorg regression,
  wallet recovery, Windows/Linux/macOS packaging, and independent review.

Do not merge or deploy a consensus-adjacent Core change solely because
the source compiles.

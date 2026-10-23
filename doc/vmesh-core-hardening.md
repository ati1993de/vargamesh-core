# VMESH Core 0.3 development: network, mining and security

This document describes optional operational changes. It is **not** a Mainnet
consensus upgrade. Never apply test-chain or unverified checkpoints to Mainnet.

## Read-only mining diagnostics

`vargamesh-cli getvmeshminingstatus` returns:

- active tip height and hash;
- current PoW target for the next block, ASERT spacing and half-life;
- AuxPoW chain ID, activation height, required child nonce and next-block status;
- accumulated tip chainwork and configured minimum chainwork.

`checkvmeshauxpow <serialized-block-hex>` checks VMESH height/chain identity
and parent proof-of-work conditions without submission and reports precise
errors. It does not perform full transaction or UTXO validation and must not
be used as the sole block acceptance decision.

This is useful for pool troubleshooting and exchange-node setup. Current
production merged miners still use `getblocktemplate` and `submitblock`.
**The new RPC is not a drop-in implementation of Namecoin's
`getauxblock` / `submitauxblock` API.** Adding those requires a separate,
fully tested adapter that reconstructs a complete VMESH block with a valid
parent coinbase, parent header, parent Merkle path and chain Merkle path.

## Mainnet bootstrap candidates

The compiled Mainnet DNS seed remains `seed.mesh.vargatech.net`. This draft
also proposes five fallback IPv4 **fixed seeds**, all previously supplied
or observed in VMESH node administration. Before release, every endpoint
MUST be freshly verified with VMESH P2P protocol handshake and operator
review. A failed/stale fixed seed should be removed before merge.

| Operator | Endpoint | Evidence / release status |
| --- | --- | --- |
| Mainnet VPS | `217.154.93.20:29666` | Previous P2P service, **live recheck required** |
| Asia VMESH VPS | `217.217.78.186:29666` | Previous external connection success, **recheck** |
| Windrose VPS | `31.70.81.5:29666` | Known VMESH peers and configured server, **recheck inbound** |
| HU VPS | `37.221.211.210:29666` | Previous bidirectional connection success, **recheck** |
| VargaTech server | `212.132.80.39:29666` | Previous VMESH full-node endpoint, **recheck** |
| Varga-Tech HomeServer | `77.22.47.104:29666` (historical) | Residential/dynamic public IP: **not compiled** |
| Community-offered node | `rodb2008.com:29666` | Previous connection attempt reported disconnected, **not compiled** |

The draft fixed seed data is encoded in BIP155 records for IPv4, port 29666.
Several fixed IPs are on infrastructure operated by the same team, so they
provide geographic/hosting redundancy but not full operator independence.

Websites, reverse proxies, pool stratum ports, local daemon RPC ports, and
NAT-only home servers are NOT valid P2P seeds by themselves. The Asia service
may expose a web hostname that does not point at its P2P listener.

Run on a host with public DNS and port 29666 access:

```sh
python3 contrib/devtools/vmesh_seed_probe.py \
  217.154.93.20:29666 217.217.78.186:29666 31.70.81.5:29666 \
  37.221.211.210:29666 212.132.80.39:29666
```

The script validates VMESH Mainnet message magic (`4638b879`), checksum,
and P2P `version` reply. It prints one JSON line per candidate. Success
does not establish independence, uptime, quality, or honest chain selection.

For VERIFIED peers, an operator may test manually:

```sh
vargamesh-cli addnode "217.217.78.186:29666" "onetry"
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

The repository retains inherited Bitcoin-like `testnet4` and `signet`
parameter definitions for compatibility, but VargaMesh's node startup
policy **already blocks** those two modes via
`IsVargaMeshNodeChainAllowed` and has an existing unit test. Legacy
Bitcoin-like `regtest` remains usable for isolated tests, yet does not
mirror VMESH Mainnet ASERT/AuxPoW. Their internal definitions should not
be removed without checking regression and migration impact.

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

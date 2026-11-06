# VargaMesh Core

<p align="center">
  <strong>Open-source SHA-256d blockchain infrastructure with native AuxPoW merged mining.</strong>
  <br />
  <sub>Independent Layer 1 · VMESH mainnet · Public Testnet v1 · Self-hosted full nodes and wallets</sub>
</p>

<p align="center">
  <a href="https://github.com/ati1993de/vargamesh-core/releases/latest"><img alt="Latest Core release" src="https://img.shields.io/github/v/release/ati1993de/vargamesh-core?label=Core&color=2ea44f"></a>
  <a href="https://github.com/ati1993de/vargamesh-core/actions/workflows/vargamesh-ci.yml"><img alt="VargaMesh CI" src="https://github.com/ati1993de/vargamesh-core/actions/workflows/vargamesh-ci.yml/badge.svg"></a>
  <a href="https://github.com/ati1993de/vargamesh-core/actions/workflows/vmesh-release-gates.yml"><img alt="Release gates" src="https://github.com/ati1993de/vargamesh-core/actions/workflows/vmesh-release-gates.yml/badge.svg"></a>
  <img alt="Consensus" src="https://img.shields.io/badge/PoW-SHA--256d-f0b429">
  <img alt="AuxPoW" src="https://img.shields.io/badge/Merged%20Mining-AuxPoW-2075af">
  <img alt="License" src="https://img.shields.io/badge/License-MIT-informational">
</p>

<p align="center">
  <a href="https://vargamesh.com">Website</a> ·
  <a href="https://vargamesh.com/explorer.html">Block Explorer</a> ·
  <a href="https://mempool.vargamesh.com">Mempool</a> ·
  <a href="https://vargamesh.com/api/v1">Public API</a> ·
  <a href="https://github.com/ati1993de/vargamesh-core/releases">Downloads</a> ·
  <a href="https://discord.gg/AeC8sxt2aB">Community</a>
</p>

---

## What is VargaMesh?

**VargaMesh (VMESH)** is an independent Proof-of-Work blockchain with its own ledger, native VMESH currency, network parameters and Bitcoin Core-derived full-node implementation. It supports **SHA-256d mining**, **ASERT** difficulty adjustment and **Auxiliary Proof of Work (AuxPoW)** so compatible Bitcoin miners can commit to VMESH work as an additional auxiliary chain.

Core provides independent transaction and block verification, P2P networking, blockchain synchronization, wallet management and authenticated JSON-RPC interfaces. Explorer, browser wallets, mobile/desktop apps and mining pool software are separate projects and are **not bundled** into Core's headless command-line archives.

### Network parameters

| Parameter | Mainnet |
| --- | --- |
| Asset / ticker | VargaMesh / **VMESH** |
| Consensus | SHA-256d Proof of Work with AuxPoW |
| Block target | **120 seconds** |
| Difficulty adjustment | ASERT, **34,560-second half-life** |
| AuxPoW chain ID | **22093** (`0x564D`) |
| AuxPoW activation | Block **1** |
| Initial subsidy | **25 VMESH** per block, before fees |
| Halving interval | **1,051,200** blocks |
| Coinbase maturity | **100** blocks |
| Bech32 address prefix | `vm1…` |
| Default P2P port | **29666/TCP** |
| Default JSON-RPC port | **29667/TCP** |
| Public Testnet v1 | `tvm1…` addresses · P2P **39666** · RPC **39667** |

Consensus values in [`src/kernel/chainparams.cpp`](src/kernel/chainparams.cpp) and [`src/consensus/params.h`](src/consensus/params.h) are authoritative. See the [Mainnet specification](doc/mainnet.md) and [testnet details](doc/testnet.md).

## Get VargaMesh Core

**Official releases:** [github.com/ati1993de/vargamesh-core/releases](https://github.com/ati1993de/vargamesh-core/releases)

| Platform | Release archive |
| --- | --- |
| Linux x86_64 | `vargamesh-core-0.3.1-linux-x86_64.tar.gz` |
| Windows x86_64 | `vargamesh-core-0.3.1-windows-x86_64.zip` |
| macOS Intel x86_64 | `vargamesh-core-0.3.1-macos-x86_64.tar.gz` |
| macOS Apple Silicon arm64 | `vargamesh-core-0.3.1-macos-arm64.tar.gz` |

> Release archives are **available only after they appear in GitHub Releases**. Never assume an asset exists based solely on an expected filename. macOS packages can appear later than Linux/Windows because they undergo separate architecture-specific execution checks.

Check each downloaded archive against the corresponding published `SHA256SUMS` or `SHA256SUMS-macOS` before installing.

~~~bash
# Place SHA256SUMS in the same directory as Linux/Windows release archives.
sha256sum -c SHA256SUMS
# For the separately published macOS checksums:
shasum -a 256 -c SHA256SUMS-macOS
~~~

The headless packages include `vargameshd`, `vargamesh-cli`, `vargamesh-wallet`, `vargamesh-tx` and `vargamesh-util`, with `.exe` suffixes on Windows.

## Quick start

Extract the appropriate official archive, open a terminal in its directory and run:

**Linux / macOS (Mainnet)**

~~~bash
./bin/vargameshd -daemonwait
./bin/vargamesh-cli getblockchaininfo
./bin/vargamesh-cli getnetworkinfo
./bin/vargamesh-cli getvmeshminingstatus
# Shut down cleanly:
./bin/vargamesh-cli stop
~~~

**Windows PowerShell (Mainnet)**

~~~powershell
.\bin\vargameshd.exe -daemonwait
.\bin\vargamesh-cli.exe getblockchaininfo
.\bin\vargamesh-cli.exe getnetworkinfo
.\bin\vargamesh-cli.exe stop
~~~

**Public Testnet v1 (Linux / macOS)**

~~~bash
./bin/vargameshd -testnet -daemonwait
./bin/vargamesh-cli -testnet getblockchaininfo
./bin/vargamesh-cli -testnet stop
~~~

Testnet coins are for development and testing and have no monetary value. Use isolated testnet wallet backups and a separate data directory from your production Mainnet node.

## Mining and merged mining

Existing VargaMesh mining software can continue using `getblocktemplate` and `submitblock`. Core v0.3.1 adds **optional, native Namecoin-style AuxPoW RPCs** designed to help Bitcoin-compatible merged-mining coordinators integrate without replacing their established mining interface.

| RPC | Purpose |
| --- | --- |
| `getblocktemplate` | Standard block-template interface (existing miners) |
| `submitblock` | Existing full-block submission interface |
| `getvmeshminingstatus` | VMESH ASERT, AuxPoW and current-tip diagnostics |
| `checkvmeshauxpow "blockhex"` | Diagnose AuxPoW proof and height rules without submitting |
| `createauxblock "vm1…"` | Create an auxiliary child work item paying a given VMESH address |
| `getauxblock` | Legacy work request; uses `-auxpowaddress` for payout destination |
| `submitauxblock "childhash" "auxpowhex"` | Submit a canonical serialized `CAuxPow` proof |
| `getauxblock "childhash" "auxpowhex"` | Legacy two-argument solution submission |

### Configure a pool-only full node

~~~ini
# vargamesh.conf — replace the payout address with your own.
server=1
auxpowaddress=vm1YOUR_POOL_PAYOUT_ADDRESS
# Keep RPC private and authenticated; add only authorized operators.
rpcbind=127.0.0.1
~~~

~~~bash
vargamesh-cli getvmeshminingstatus
vargamesh-cli getauxblock
vargamesh-cli createauxblock "vm1YOUR_POOL_PAYOUT_ADDRESS"
# Valid proof supplied by a merged-mining coordinator:
vargamesh-cli submitauxblock "CHILD_HEADER_HASH" "CANONICAL_AUXPOW_HEX"
~~~

A native AuxPoW job includes `hash`, `chainid`, `previousblockhash`, `coinbasevalue`, `bits`, `height` and little-endian `target` / `_target`. The candidate cache is bounded to **128 entries**, expires work after **15 minutes** and rejects stale chain-tip work. Proofs are limited to **128 KiB**.

**Pool compatibility:** Bitcoin pool operators must generate the VMESH-specific parent coinbase merged-mining commitment and a correctly serialized `CAuxPow` proof. The header's `fabe6d6d` marker, byte-reversed auxiliary root and tree size/nonce fields are not interchangeable with arbitrary pool defaults. The *Bitcoin parent block hash* must satisfy the VMESH child target. Valid shares and actual blocks are not guaranteed.

**Validation disclosure:** The legacy miner path has production usage and the 0.3.1 source passes Linux/Windows/macOS CI, functional negative RPC checks and positive *consensus-layer* AuxPoW unit tests. An independent **end-to-end positive `submitauxblock` acceptance test across v0.3.0 and v0.3.1** has **not yet been documented**. Operators using the new native RPCs should treat this interface as *experimental* until that integration evidence is published; use a controlled pilot, not a blind fleet-wide cutover. The network consensus rules have **not** changed and no hard fork is intended.

Read the [native AuxPoW integration guide](doc/native-auxpow-rpc.md), [Core release notes](doc/release-notes/release-notes-v0.3.1.md), [VMESH mining pool repository](https://github.com/ati1993de/vargamesh-ckpool) and [MiniPool project](https://github.com/ati1993de/vargamesh-minipool).

## Upgrading existing nodes

Upgrades from v0.3.0 preserve the network, blockchain data and wallet format: stop the daemon cleanly, **back up wallet(s) and configuration**, verify the downloaded archive and replace only the intended binaries. Do not overwrite or delete wallet or chainstate directories.

Before and after upgrading, record:

~~~bash
vargamesh-cli getblockchaininfo
vargamesh-cli getnetworkinfo
vargamesh-cli getvmeshminingstatus
~~~

For production mining nodes, also verify pool RPC authentication, `getblocktemplate` response, real job issuance and pool/bridge logs. Keep a rollback copy of the previous binary. Native AuxPoW RPC adoption is optional; wallet and exchange nodes do not need these APIs to follow unchanged consensus rules.

## Verify and build from source

Clone the reviewed release tag or explicitly pin a commit. Avoid building arbitrary branch heads for production use.

~~~bash
git clone https://github.com/ati1993de/vargamesh-core.git
cd vargamesh-core
git checkout v0.3.1
cmake -S . -B build -G Ninja -DBUILD_GUI=OFF -DBUILD_TESTS=ON
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
~~~

The `git checkout` command works only after **v0.3.1 is actually tagged and published**. Read [CMake build instructions](doc/build-unix.md) or the repository-specific build documentation for operating-system requirements. GitHub Actions checks Linux, Windows and macOS release targets, native macOS Mainnet/Testnet smokes, seed connectivity and AuxPoW fuzz coverage.

## Wallets and ecosystem

These applications are **separate projects**. Their release cycles and bundled Core versions can differ from this repository.

| Project | Description | Official resource |
| --- | --- | --- |
| VargaMesh Desktop | Windows and macOS self-custody wallet | [Source and releases](https://github.com/ati1993de/vargamesh-desktop) |
| VargaMesh Android | Self-custody wallet; Recovery Wallet and VMT-1 functionality | [Android project](https://vargamesh.com/android.html) |
| VargaMesh WebWallet | Browser-based access and VMESH wallet tools | [WebWallet](https://vargamesh.com/wallet.html) |
| VMESH Mobile Miner | Android SHA-256d learning/home-mining client | [Mobile Miner](https://vargamesh.com/mobile-miner.html) |
| VargaMesh MiniPool | VMESH-only SOLO pool for beginners | [MiniPool](https://github.com/ati1993de/vargamesh-minipool) |
| VMESH mining / CkPool | Mining and pool infrastructure | [Mining repository](https://github.com/ati1993de/vargamesh-ckpool) |
| VMT-1 | VMESH fungible-token indexing and wallet integration | [Token directory](https://vargamesh.com/tokens.html) |
| VargaMesh Name Service | Human-readable `.vmesh` names | [Explorer](https://vargamesh.com/explorer.html) |
| VargaProof / MeshProof | Verification and network contribution tools | [VargaProof](https://vargamesh.com/proof.html) · [MeshProof](https://vargamesh.com/meshproof.html) |
| Network / Mempool | Explorers, statistics and monitoring | [Network](https://vargamesh.com/network.html) · [Mempool](https://mempool.vargamesh.com) |

A full historical walkthrough of desktop/mobile wallet functionality, token and name-service integration, public APIs, mining infrastructure and the wider VMESH ecosystem is preserved in the [detailed ecosystem reference](doc/ecosystem-reference.md). Some third-party storefronts and downloads may lag GitHub releases; verify their release versions separately.

## Security

- **Never expose authenticated VMESH RPC to the public Internet.** Use loopback or a trusted private network, least-privilege credentials and RPC whitelists where appropriate.
- Never share wallet passwords, RPC cookies, recovery phrases, private keys, WIFs or decrypted backups.
- `createauxblock` accepts a payout address. Restrict this RPC to authorized pool coordinators to prevent redirecting potential block rewards.
- Verify release checksums, back up and test wallet recovery, and monitor chain tips, peers and wallet state after any upgrade.
- VMESH and third-party mining software are under active development. No investment return, mining reward or uptime is guaranteed.

## Documentation and community

| Resource | Link |
| --- | --- |
| v0.3.1 release notes | [Release notes](doc/release-notes/release-notes-v0.3.1.md) |
| Native merged mining / AuxPoW | [RPC integration reference](doc/native-auxpow-rpc.md) |
| Consensus / parameters | [Mainnet](doc/mainnet.md) · [Testnet](doc/testnet.md) |
| Core releases and checksums | [GitHub Releases](https://github.com/ati1993de/vargamesh-core/releases) |
| REST API | [Public VargaMesh API](https://vargamesh.com/api/v1) |
| Detailed ecosystem reference | [Historic guide](doc/ecosystem-reference.md) |
| Bugs / feature proposals | [GitHub Issues](https://github.com/ati1993de/vargamesh-core/issues) |
| Community | [Discord](https://discord.gg/AeC8sxt2aB) |

**License:** MIT. See [`COPYING`](COPYING). This is experimental software, provided as-is. The VargaMesh Core source tree is authoritative for consensus behavior.

<p align="center"><strong>VargaMesh — Independent infrastructure. Open participation. Self-custody.</strong></p>

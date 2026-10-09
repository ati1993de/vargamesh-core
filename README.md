# VargaMesh Core v0.2.0

<p align="center">
  <strong>Official Mainnet + Public Testnet v1 Core release for the VargaMesh (VMESH) network.</strong>
</p>

<p align="center">
  Linux x86_64 · Windows x86_64 · macOS Intel x86_64 · macOS Apple Silicon arm64
</p>

<p align="center">
  <img alt="Core" src="https://img.shields.io/badge/Core-v0.2.0-2ea44f">
  <img alt="Mainnet" src="https://img.shields.io/badge/Network-Mainnet-orange">
  <img alt="Testnet" src="https://img.shields.io/badge/Testnet-v1-blue">
  <img alt="Consensus" src="https://img.shields.io/badge/Consensus-SHA--256d%20PoW-yellow">
  <img alt="macOS" src="https://img.shields.io/badge/macOS-Intel%20%2B%20Apple%20Silicon-black">
</p>

<p align="center">
  <a href="https://github.com/ati1993de/vargamesh-core">Core Repository</a>
  ·
  <a href="https://vargamesh.com">Website</a>
  ·
  <a href="https://mesh.vargatech.net">Project Portal</a>
  ·
  <a href="https://vargamesh.com/explorer.html">Explorer</a>
  ·
  <a href="https://discord.gg/AeC8sxt2aB">Discord</a>
</p>

---

> [!IMPORTANT]
> **VargaMesh Core `v0.2.0` is the current official Mainnet/Testnet Core release.**
>
> Official binary packages are published for:
>
> - Linux x86_64
> - Windows x86_64
> - macOS Intel x86_64
> - macOS Apple Silicon arm64
>
> The macOS Core archives contain the daemon and command-line utilities.
> They are separate from the graphical **VargaMesh Desktop** application.
>
> The macOS assets were produced from the existing, unchanged `v0.2.0` tag
> commit and were verified on native Intel and Apple Silicon GitHub runners.
> Adding macOS packages did **not** create a new Core version and did **not**
> change VargaMesh consensus rules.

---

## Official release files

```text
vargamesh-core-0.2.0-linux-x86_64.tar.gz
vargamesh-core-0.2.0-windows-x86_64.zip
vargamesh-core-0.2.0-macos-x86_64.tar.gz
vargamesh-core-0.2.0-macos-arm64.tar.gz

SHA256SUMS
SHA256SUMS-macOS
```

### macOS architecture selection

Check the local Mac architecture:

```bash
uname -m
```

Use:

```text
x86_64 -> vargamesh-core-0.2.0-macos-x86_64.tar.gz
arm64  -> vargamesh-core-0.2.0-macos-arm64.tar.gz
```

### Verify macOS checksums

Place `SHA256SUMS-macOS` beside the two macOS archives and run:

```bash
shasum -a 256 -c SHA256SUMS-macOS
```

### Included Core programs

The official Core archives contain the release-appropriate versions of:

```text
vargameshd
vargamesh-cli
vargamesh-wallet
vargamesh-tx
vargamesh-util
```

Windows executables use the `.exe` suffix.

---

## Release verification

Before publication, the supported release packages are validated through the
repository CI/release workflows.

### Linux x86_64

Verification includes:

- Core build
- Mainnet daemon/RPC smoke test
- Public Testnet v1 daemon/RPC smoke test
- Mainnet `vm1...` wallet-address validation
- Testnet `tvm1...` wallet-address validation
- packaged runtime verification
- SHA-256 checksum publication

### Windows x86_64

The Windows package is cross-compiled through the pinned `depends` system with
the MinGW-w64 toolchain and then executed on a Windows GitHub runner.

Verification includes:

- execution of all packaged Core command-line programs
- Mainnet daemon/RPC smoke test
- Public Testnet v1 daemon/RPC smoke test
- Mainnet `vm1...` wallet-address validation
- Testnet `tvm1...` wallet-address validation
- SHA-256 checksum publication

### macOS Intel x86_64 + Apple Silicon arm64

The macOS packages are built through the repository's pinned `depends` system
for:

```text
x86_64-apple-darwin
arm64-apple-darwin
```

Each resulting package is then executed on a native GitHub macOS runner for its
architecture.

Verification includes:

- valid Mach-O binaries
- execution of `vargameshd`
- execution of `vargamesh-cli`
- execution of `vargamesh-wallet`
- execution of `vargamesh-tx`
- execution of `vargamesh-util`
- Mainnet daemon/RPC smoke test
- Mainnet `vm1...` wallet-address validation
- Public Testnet v1 daemon/RPC smoke test
- Testnet `tvm1...` wallet-address validation
- clean daemon shutdown
- SHA-256 archive verification

---

## Quick start

### Mainnet — Linux / macOS

```bash
./bin/vargameshd -daemonwait
./bin/vargamesh-cli getblockchaininfo
```

Stop cleanly:

```bash
./bin/vargamesh-cli stop
```

### Mainnet — Windows

```powershell
.\bin\vargameshd.exe -daemonwait
.\bin\vargamesh-cli.exe getblockchaininfo
```

Stop cleanly:

```powershell
.\bin\vargamesh-cli.exe stop
```

### Public Testnet v1 — Linux / macOS

```bash
./bin/vargameshd -testnet -daemonwait
./bin/vargamesh-cli -testnet getblockchaininfo
```

### Public Testnet v1 — Windows

```powershell
.\bin\vargameshd.exe -testnet -daemonwait
.\bin\vargamesh-cli.exe -testnet getblockchaininfo
```

> **tVMESH is a Testnet asset only and has no monetary value.**

---

## Upgrade safety

Before replacing an existing VargaMesh Core installation:

1. stop the current node cleanly
2. back up wallet data
3. verify the downloaded release checksum
4. replace only the intended binaries
5. restart the node
6. verify `getblockchaininfo`
7. verify `getnetworkinfo`
8. verify wallet availability before sending funds

Never expose unauthenticated VargaMesh Core RPC directly to the public Internet.

---

# Full VargaMesh Project & Core Documentation

The remainder of this release description intentionally preserves the detailed
VargaMesh project documentation so users, node operators, miners, wallet users
and integrators have the relevant ecosystem context in one place.

# Overview

**VargaMesh Core** is the reference full-node implementation for the
**VargaMesh (VMESH)** blockchain.

VargaMesh is an independent Proof-of-Work network derived from Bitcoin Core
and extended with VargaMesh-specific consensus, monetary and network
parameters.

The network currently includes:

* independent VargaMesh mainnet
* native `VMESH` cryptocurrency
* SHA-256d Proof-of-Work
* 120-second target block spacing
* ASERT difficulty adjustment
* AuxPoW merged-mining support
* VargaMesh-specific network ports
* VargaMesh-specific address formats
* desktop full-node wallet
* native Android self-custody wallet
* shared BIP39/BIP32 Recovery Wallet convention across Desktop, Android and WebWallet
* browser-based WebWallet / PWA
* VMT-1 fungible-token layer indexed from confirmed VargaMesh transactions
* native VMT-1 support in Desktop `v0.7.1` and Android `v0.5.0`
* public VMT-1 token directory and token API
* VargaMesh Name Service (VNS) with human-readable `.vmesh` names
* blockchain explorer with VNS resolution and on-chain name history
* VargaMesh-specific mempool explorer with VNS transaction recognition
* public network statistics
* public HTTPS API
* public mining infrastructure
* VargaProof verification infrastructure
* MeshProof public-node contribution infrastructure
* public VargaMesh Ecosystem Treasury with on-chain transparency
* GitHub Windows/macOS Desktop releases; third-party storefront versions may lag
* Android source pre-release and direct APK download; third-party listing may lag

The **VargaMesh Core source tree is authoritative for consensus behavior**.

---

## Project Status

For client release specifics, limitations and cross-platform distinctions, see
[Client release status](doc/client-release-status.md). The Core release version
remains **v0.2.0**.

Public release manifest: https://vargamesh.com/api/v1/releases

The website API may require a separate deployment to reflect newer clients.
Check the GitHub Desktop release and Android source validation status directly.


| Component                  |     Version | Status                                      |
| -------------------------- | ----------: | ------------------------------------------- |
| VargaMesh Core             |    `v0.2.0` | 🟢 Mainnet + Testnet · Linux/Windows/macOS |
| VargaMesh Desktop          |    `v0.7.1` | 🟢 GitHub · Windows/macOS Intel+ARM |
| VargaMesh Android          |    `v0.5.0` | 🟡 Source pre-release · device validation pending |
| VargaMesh WebWallet        |     Current | 🟢 Available                                |
| VMT-1 Token Layer         |     Current | 🟢 Mainnet · directory/API · native clients |
| VargaMesh Name Service     |     Current | 🟢 Available · WebWallet / Explorer / Mempool |
| Public Explorer            |     Current | 🟢 Available                                |
| Public API                 |        `v1` | 🟢 Available                                |
| Mining / CkPool            |     Current | 🟢 Available / active development           |
| VargaProof                 |     Current | 🟢 Available                                |
| MeshProof                  |     Current | 🟢 Available                                |
| Ecosystem Treasury          |     Current | 🟢 Public on-chain transparency             |
| VargaMesh Mempool Explorer | Development | 🔵 Active development                       |

---

## Quick Links

| Project                      | Information                                          |
| ---------------------------- | ---------------------------------------------------- |
| Network                      | VargaMesh Mainnet                                    |
| Currency                     | `VMESH`                                              |
| Core version                 | `v0.2.0`                                             |
| Desktop version              | `v0.7.1`                                             |
| Android version              | `v0.5.0`                                             |
| Website                      | https://vargamesh.com                                |
| Project portal               | https://mesh.vargatech.net                           |
| Core source code             | https://github.com/ati1993de/vargamesh-core          |
| Core releases                | https://github.com/ati1993de/vargamesh-core/releases |
| Desktop wallet               | https://github.com/ati1993de/vargamesh-desktop       |
| Desktop v0.7.1 release        | https://github.com/ati1993de/vargamesh-desktop/releases/tag/v0.7.1 |
| Desktop Microsoft Store      | https://apps.microsoft.com/detail/9NX9S7SJSW0S?hl=de-de&gl=DE&ocid=pdpshare |
| Android information          | https://vargamesh.com/android.html                   |
| Android APK                  | https://vargamesh.com/vmesh.apk                      |
| Android APK SHA256           | https://vargamesh.com/vmesh.apk.sha256               |
| Android APKPure              | https://apkpure.com/p/de.vargatech.vargamesh         |
| WebWallet                    | https://vargamesh.com/wallet.html                    |
| VMT-1 token directory        | https://vargamesh.com/tokens.html                    |
| Release manifest               | https://vargamesh.com/api/v1/releases                |
| Wallet standard                | https://vargamesh.com/wallet-standard/                |
| Explorer                     | https://vargamesh.com/explorer.html                  |
| Mempool Explorer development | https://mempool.vargamesh.com                        |
| Mining / CkPool              | https://github.com/ati1993de/vargamesh-ckpool        |
| VargaProof                   | https://vargamesh.com/proof.html                     |
| MeshProof                    | https://vargamesh.com/meshproof.html                 |
| Ecosystem Treasury             | https://vargamesh.com/treasury.html                  |
| Public API                   | https://vargamesh.com/api/v1                         |
| Network information          | https://vargamesh.com/network.html                   |
| Roadmap                      | https://vargamesh.com/roadmap.html                   |
| Listing information            | https://vargamesh.com/listing/                       |
| Discord                      | https://discord.gg/AeC8sxt2aB                        |

---

# VargaMesh Ecosystem

VargaMesh is developed as more than a command-line blockchain node.

The ecosystem combines the consensus layer with wallet software, network
monitoring, mining infrastructure, public APIs, explorer services, on-chain
human-readable names and verification/contribution tools.

```text
                        VargaMesh Mainnet
                               │
                     ┌─────────┴─────────┐
                     │                   │
               VargaMesh Core       Mining / AuxPoW
                     │
       ┌─────────────┼─────────────┐
       │             │             │
    Desktop       Android      WebWallet
       │             │             │
       └─────────────┼─────────────┘
                     │
                 Public API
                     │
       ┌─────────────┼─────────────┐
       │             │             │
    Explorer     VargaProof    Network Stats
       │
       ├──────── VMT-1 Indexer / Token Directory
       │
       └──────── VNS / Treasury / MeshProof
```

---

## VargaMesh Core

**VargaMesh Core** is the consensus and full-node reference implementation
for the VargaMesh network.

Repository:

https://github.com/ati1993de/vargamesh-core

Current version:

**VargaMesh Core v0.2.0**

Official v0.2.0 binary packages:

* Linux x86_64
* Windows x86_64
* macOS Intel x86_64
* macOS Apple Silicon arm64

Release downloads and SHA-256 checksum files:

https://github.com/ati1993de/vargamesh-core/releases/tag/v0.2.0

The macOS packages contain `vargameshd`, `vargamesh-cli`, `vargamesh-wallet`,
`vargamesh-tx` and `vargamesh-util`. They are Core command-line packages, not
the graphical VargaMesh Desktop application.

Core is responsible for:

* blockchain validation
* transaction validation
* block validation
* Proof-of-Work validation
* AuxPoW validation
* ASERT difficulty calculation
* peer-to-peer networking
* transaction relay
* block relay
* chain-state management
* wallet functionality
* JSON-RPC services
* mining interfaces

Consensus-critical behavior is defined by the VargaMesh Core source code.

---

## VargaMesh Desktop

**VargaMesh Desktop** is a graphical Windows x64 and macOS (Intel/Apple Silicon)
self-custody full-node wallet built around VargaMesh Core.

Repository:

https://github.com/ati1993de/vargamesh-desktop

Current release:

**VargaMesh Desktop v0.7.1**

Release date:

**9 October 2026**

Status:

**Released / public testing**

Latest releases:

https://github.com/ati1993de/vargamesh-desktop/releases/latest

The current Desktop release bundles:

**VargaMesh Core v0.2.0**

### Available packages

* Windows x64 installer and portable ZIP
* macOS x86_64 and Apple Silicon arm64 DMG and ZIP
* Published SHA-256 checksums
* Microsoft Store listing (may lag behind the official GitHub release)

### v0.7.1 highlights

Desktop `v0.7.1` retains its locally synchronized Core, BIP39/HD Recovery
Wallet, VMESH sending/receiving, native VMT-1 portfolio and four languages.
Version `v0.7.0` added, and `v0.7.1` preserves:

* Local address book with VMESH addresses and `.vmesh` contacts
* VNS name resolution, explicit address confirmation and fresh name/address
  verification before broadcast (changes cancel the send)
* Native notifications for incoming VMESH, first confirmation and completed
  synchronization; notification amounts are hidden by default
* Responsive wallet navigation and plaintext transaction CSV export
* NestEx VMESH/USDT market quotes and wallet-value estimates

**v0.7.1 startup hotfix:** The v0.7.0 selector error that blocked the
automatic Core/wallet refresh until a manual Refresh click was fixed.
The bundled Core version remains **v0.2.0**, with unchanged bootstrap peers.
Fee amounts shown before sending are estimates, not final funded fees.

Supported UI languages:

* Deutsch
* English
* Русский
* 简体中文

Current VMT-1 functionality includes:

* discover VMT-1 balances across owned `vm1...` addresses in the active Core wallet
* show approved token metadata, logos, supply, issuer, holder and transfer data
* browse and search the public VMT-1 token directory
* create VMT-1 tokens using the live Mainnet CREATE policy
* transfer VMT-1 tokens
* burn VMT-1 tokens
* mint additional units when the selected address is the authorized issuer and the token is mintable
* keep the VMT owner/issuer address as transaction input `0`
* verify the input-0 authorization invariant before and after Core signing
* run exact signed-transaction VMT preflight before broadcast
* repeat the same exact preflight immediately before final broadcast
* sign and broadcast through the local VargaMesh Core
* obtain the active VMT CREATE fee policy dynamically instead of hard-coding the fee or recipient

Private keys remain in VargaMesh Core. The renderer remains sandboxed and does
not receive arbitrary Core RPC access or the RPC cookie.

Approved VMT metadata is displayed in Desktop. Native metadata submission is
not claimed as available in v0.7.1; issuer proof would need a secure Core-wallet
RPC workflow without exporting private-key material.

v0.7.1 also retains earlier localization fixes and includes regression checks
for UI startup binding and automatic wallet refresh.

### Current wallet functionality

* automatic VargaMesh Core startup
* full-node blockchain synchronization
* peer status
* node status
* classic Core wallet creation
* deterministic Recovery Wallet creation
* 12-word and 24-word BIP39 Recovery Phrases
* BIP32 HD key derivation
* BIP84 Native SegWit receive/change descriptors
* BIP44 legacy receive/change descriptors
* Recovery Wallet restore with blockchain rescan
* Recovery Phrase copy and explicit TXT export during creation
* wallet loading and unloading
* wallet encryption and temporary unlocking
* VMESH receiving addresses
* local/offline receive QR codes
* VMESH balance, UTXO view and transaction history
* VMESH transfers with validation and confirmation
* wallet backup and restore
* WIF private-key import
* watch-only address import
* legacy wallet migration support
* Windows notification-area / system-tray integration
* optional minimize/close-to-tray and background startup
* four selectable UI languages: DE / EN / RU / ZH

### Desktop Recovery Wallet interoperability

VargaMesh Desktop `v0.7.1` uses the full VMESH HD descriptor wallet hierarchy.
The current VargaMesh WebWallet and VargaMesh Android `v0.5.0` use the same first
Native SegWit external receive path for deterministic recovery.

Mainnet Native SegWit receive path:

```text
m/84'/22093'/0'/0/index
```

Mainnet Native SegWit change path:

```text
m/84'/22093'/0'/1/index
```

Legacy compatibility receive path:

```text
m/44'/22093'/0'/0/index
```

The Mainnet coin type `22093` is the **proposed VargaMesh SLIP-0044 value**.
It must not be described as an official SLIP-0044 assignment unless and until
it is accepted by the upstream SLIP-0044 registry.

The current lightweight-wallet interoperability target remains the first Native
SegWit external address:

```text
m/84'/22093'/0'/0/0
```

Therefore the same valid Recovery Phrase restores the same first `vm1...`
address in VargaMesh Desktop `v0.7.1`, VargaMesh Android `v0.5.0` and the
current WebWallet.

The Desktop wallet can derive additional receive/change addresses and aggregate
Core wallet state. The current Android/WebWallet Recovery Wallet model remains
focused on the active index-0 address unless explicitly expanded by a later
client release.

> [!IMPORTANT]
> VargaMesh Desktop `v0.7.1` is pre-1.0 public-testing software.
>
> Before using meaningful amounts, verify the Recovery Phrase or Core backup
> with a recovery test and begin with a small transaction. For VMT-1 operations,
> verify the selected owner address, token ID, amount and signed-transaction
> preflight result before broadcast.

---

# VargaMesh Android

**VargaMesh Android** is the native Android application for the VargaMesh
(VMESH) mainnet.

Current release:

**VargaMesh Android v0.5.0**

Release date:

**9 October 2026**

Status:

**Source pre-release / build and physical-device validation required**

Official information:

https://vargamesh.com/android.html

Direct APK:

https://vargamesh.com/vmesh.apk

SHA256 checksum file:

https://vargamesh.com/vmesh.apk.sha256

APKPure project page:

https://apkpure.com/p/de.vargatech.vargamesh

VargaMesh Android combines explorer, mining and network tools with a native
self-custody VMESH wallet. VMT-1 debuted in v0.4.0; v0.5.0 retains it,
BIP39/BIP32 recovery and legacy v0.2.x single-WIF compatibility.

---

## Android v0.5.0 — Professional Wallet Update

Android v0.5.0 **retains VMT-1** introduced in v0.4.0 and adds:

* Reorganized balances, Send/Receive, token navigation and expandable management
* Local address book with labels, favorites and recipient selection; app-private
  storage without Android Contacts/cloud permission
* Optional `/api/vns/v1/resolve/{name}` lookup with explicit confirmation
  of the resolved, checksummed mainnet address before signing
* `vargamesh:<address>?amount=<decimal>` payment requests, QR regeneration
  and Android share sheet
* Offline encrypted-backup verification without overwriting an active wallet
* Opt-in Android WorkManager activity notifications (online, approximately
  every 15 minutes, **not real-time**); no historical first-run alert flood
* Privacy-safe notifications without payment amounts or destinations
* Exact decimal-to-satoshi parsing for display-only balances
* Health indicator explicitly labelled **remote public API, not a local node**
* Five languages: English, German, Hungarian, Russian, Simplified Chinese

**Status: source pre-release; build and physical-device verification required.**
The existence of an APK or checksum is not proof of a security audit or
production release. Consult the Android project's `VALIDATION-REPORT.md`.

**Security limitation:** Android v0.5.0 has no dedicated VNS ownership-anchor
UTXO exclusion or management during spend construction. Do **not** use this
build to manage VNS anchors or significant funds before security review.
Its active HD address remains index 0: no multi-address gap-limit scanning
or full coin control is guaranteed. UTXO checks are not a lifetime TX index,
so WorkManager can miss outputs already spent between scans.

### Retained v0.4.0 token and recovery capabilities

Major changes include:

* native VMT-1 token portfolio support
* token balances tied to the active `vm1...` wallet address
* public token directory, token details and token activity
* VMT-1 CREATE
* VMT-1 TRANSFER
* VMT-1 BURN
* authorized issuer-only VMT-1 MINT for mintable tokens
* VMT-1 receive information using the same native `vm1...` address
* dynamic CREATE policy obtained from the live VMT API instead of a hard-coded fee
* local token transaction construction/signing
* input-0 owner/issuer authorization handling
* exact signed-transaction preflight before broadcast
* repeated final preflight immediately before broadcast
* VMESH wallet activity/history filters: All / Received / Sent / Mining
* improved UTXO, mining-reward, TXID and confirmation visibility
* German, English, Russian and Simplified Chinese interfaces
* continued 12-word and 24-word BIP39 Recovery Wallet creation
* continued Recovery Wallet restore
* continued BIP32 derivation using `m/84'/22093'/0'/0/0`
* continued encrypted local wallet storage
* continued encrypted HD backup export/restore
* continued support for legacy `v0.2.x` encrypted single-key backups
* continued compressed VMESH WIF import support
* continued local signing and signed raw-transaction broadcast
* dark, light and system theme support

The Android HD wallet currently uses one active deterministic address at index
`0`. Full HD gap-limit scanning and multi-address aggregation are not claimed
for `v0.5.0`.

---

## Android Network Companion

Current functionality includes:

* VMESH mainnet status
* current block height
* network difficulty
* estimated network hashrate
* connected peer information
* supply information
* latest blocks
* explorer search
* block lookup
* transaction lookup
* address lookup
* mining pool status
* mining-address lookup
* public Stratum information
* release notifications
* VMESH QR scanning
* German interface
* English interface
* Hungarian interface
* Russian interface
* Simplified Chinese interface
* dark theme
* light theme
* system theme

Public Stratum endpoint:

```text
vargamesh.com:3933
```

---

## Native Android VMESH Wallet

VargaMesh Android `v0.5.0` provides a native self-custody wallet with both the
HD Recovery Wallet model and the established legacy single-key/WIF model.

### Recovery Wallet functionality

* create a new 12-word BIP39 Recovery Wallet
* create a new 24-word BIP39 Recovery Wallet
* restore a wallet from a valid 12-word or 24-word Recovery Phrase
* derive the active VMESH key with BIP32
* use the VargaMesh BIP84 Mainnet path `m/84'/22093'/0'/0/0`
* derive the same first `vm1...` address as the current Desktop/WebWallet Recovery Wallet convention
* show/copy the Recovery Phrase after password verification
* encrypt Recovery Phrase material locally
* export an encrypted HD wallet JSON backup
* restore an encrypted HD wallet JSON backup with the correct wallet password

### Legacy wallet compatibility

* create/import a compressed VMESH WIF wallet
* retain access to existing `v0.2.x` Android wallet data
* restore supported legacy encrypted JSON backups
* preserve the original single-key/WIF wallet model for existing users
* reveal/export the active WIF with an explicit security warning

Existing users do **not** need to recreate a supported wallet solely because
they update the application.

### VMESH transaction and wallet functionality

* receive VMESH
* receive-address QR code
* spendable balance
* immature balance
* spendable UTXO details
* send VMESH
* QR payment scanning
* MAX-send calculation
* default transaction fee of `1 sat/vB`
* transaction review before broadcast
* local transaction construction
* local transaction signing
* signed raw transaction broadcast
* encrypted backup export
* encrypted backup restore
* wallet password change
* optional biometric/device unlock
* automatic handling of sensitive clipboard contents
* 10-minute in-memory auto-lock
* screenshot protection on wallet-sensitive screens
* local wallet removal
* activity/history views for received, sent and mining-related activity

### Native VMT-1 functionality

VargaMesh Android `v0.5.0` can operate VMT-1 tokens without sending private
keys to the public indexer/API.

Current token functionality includes:

* list VMT-1 balances for the active wallet address
* inspect token name, symbol, supply, issuer and approved metadata
* browse/search the public token directory
* create a token through the live Mainnet CREATE policy
* transfer tokens to another native VargaMesh address
* burn owned tokens
* mint when the active address is the authorized issuer and the token is mintable
* construct and sign the transaction locally
* preserve the VMT owner/issuer authorizer as input `0`
* use public VMT preflight for exact signed-transaction semantic validation
* broadcast only the signed raw transaction

VMT-1 token balances are application-layer state reconstructed from confirmed
VargaMesh transactions. The same `vm1...` address can receive both VMESH and
VMT-1 tokens.

---

## Android Recovery Wallet Interoperability

Android `v0.5.0` follows the same first-address Mainnet Recovery Wallet
convention documented for VargaMesh Desktop and the WebWallet.

Active Native SegWit path:

```text
m/84'/22093'/0'/0/0
```

The Mainnet coin type `22093` is the **proposed VargaMesh SLIP-0044 value**. It
must not be described as an official SLIP-0044 assignment unless and until it
is accepted by the upstream SLIP-0044 registry.

A valid BIP39 Recovery Phrase therefore deterministically derives the same first
Native SegWit `vm1...` address in:

* VargaMesh Desktop `v0.7.1`
* the current VargaMesh WebWallet
* VargaMesh Android `v0.5.0`

The interoperability guarantee concerns deterministic key/address derivation.
It does **not** imply that every application's encrypted JSON backup container
is interchangeable with every other client.

---

## Android Encrypted Backup Model

VargaMesh Android supports encrypted JSON backup export and restore.

For an HD Recovery Wallet, the backup contains the Recovery Wallet metadata and
**encrypted** Recovery Phrase material. The Recovery Phrase is not intentionally
written into the JSON backup as plaintext.

The HD backup records the information required to restore the Android Recovery
Wallet, including the relevant derivation information. Restoring the encrypted
backup requires the correct wallet password.

In practical terms, HD backup recovery requires:

```text
Encrypted JSON backup + correct wallet password
```

Users should still keep an independent offline copy of the 12-word or 24-word
Recovery Phrase. A Recovery Phrase can reconstruct the deterministic wallet
without relying on the Android JSON backup container.

Legacy `v0.2.x` single-key encrypted backups remain supported separately for
compatibility with existing Android users and the established legacy WebWallet
backup model.

Never publish or send an encrypted wallet backup to third parties. Encryption
reduces exposure if the file is obtained without the password, but the backup
must still be treated as sensitive wallet material.

---

## Android Security Model

VargaMesh Android is **non-custodial**.

Recovery Phrases and private keys are created, imported and processed locally.
The application derives/signs VMESH and supported VMT-1 transactions locally.
Public services receive blockchain queries, token/indexer queries and signed raw
transactions — not the wallet's private key material.

```text
12/24-word Recovery Phrase                 Legacy compressed WIF
            │                                        │
            │ BIP39 / BIP32                          │ imported locally
            ▼                                        ▼
HD active private key                       Single private key
            │                                        │
            └──────────────────┬─────────────────────┘
                               ▼
                  Password-encrypted local storage
                               │
                               ▼
                     Local transaction signing
                               │
                               ▼
                       Signed raw transaction
                               │
                               ▼
                    HTTPS/API broadcast path
```

The Android application does **not** intentionally send any of the following to
the VargaMesh public server:

* Recovery Phrases / seed words
* private keys
* WIF private keys
* wallet passwords
* decrypted wallet backups

VargaMesh Core RPC is **not exposed to or directly used by the Android
application**.

---

## Android Wallet Parameters

VargaMesh Android `v0.5.0` supports deterministic HD Recovery Wallets and legacy
single-private-key/WIF wallets.

| Parameter                         | Value                         |
| --------------------------------- | ----------------------------- |
| Elliptic curve                    | `secp256k1`                   |
| Primary address                   | SegWit v0 P2WPKH              |
| Bech32 HRP                        | `vm`                          |
| WIF version                       | `190 / 0xBE`                  |
| Legacy P2PKH version              | `70`                          |
| Legacy P2SH version               | `50`                          |
| Decimals                          | `8`                           |
| Default fee                       | `1 sat/vB`                    |
| Recovery Phrase                   | BIP39, `12` or `24` words     |
| HD derivation                     | BIP32                         |
| Active BIP84 path                 | `m/84'/22093'/0'/0/0`         |
| Proposed Mainnet SLIP-0044 type   | `22093`                       |
| Legacy key/backup KDF             | PBKDF2-HMAC-SHA256            |
| Legacy KDF iterations             | `250,000`                     |
| HD Recovery Phrase KDF            | PBKDF2-HMAC-SHA256            |
| HD Recovery KDF iterations        | `350,000`                     |
| Local/backup authenticated cipher | AES-256-GCM                   |

Signing uses:

* deterministic RFC6979 ECDSA
* low-S normalization
* BIP143-style SegWit v0 signing

---

## Android Limitations

VargaMesh Android `v0.5.0` currently does **not** claim:

* full HD gap-limit scanning
* multiple HD receive/change addresses in the active wallet UI
* a locally synchronized VargaMesh full node
* arbitrary VargaMesh Core RPC access

Android uses purpose-built public HTTPS APIs for blockchain/UTXO and VMT-1
indexer data while keeping private-key operations local.

VargaMesh Desktop `v0.7.1` remains the full-node wallet: it can operate
VargaMesh Core locally and derive additional receive/change addresses.

---

## Android Requirements

Current development requirements include:

* Android Studio
* JDK 17-compatible toolchain
* Android SDK 35
* minimum Android API: `26`

Minimum supported Android version:

**Android 8.0**

---

## Android Validation and Backup Notice

> [!CAUTION]
> VargaMesh Android `v0.5.0` can create, recover, hold and spend real VMESH and
> operate supported VMT-1 token actions.
>
> It remains pre-1.0 public-testing software.

Recommended testing procedure for a newly created Recovery Wallet:

1. create a 12-word or 24-word test Recovery Wallet
2. securely record the Recovery Phrase offline
3. record the first `vm1...` address
4. export an encrypted JSON backup
5. verify Recovery Phrase restore and confirm the same first address
6. verify encrypted JSON restore with the correct password
7. receive a small VMESH amount
8. send a small VMESH transaction
9. confirm the transaction in the explorer
10. if using VMT-1, verify the token ID, owner address and a small token action
11. test legacy WIF/backup compatibility separately if migrating an older wallet

Never share:

* Recovery Phrases / seed words
* private keys
* WIFs
* wallet passwords
* decrypted wallet backups
* encrypted wallet backup files with untrusted third parties

---

# VargaMesh WebWallet

The VargaMesh ecosystem includes a browser-based self-custody wallet and
installable Progressive Web App (PWA).

WebWallet:

https://vargamesh.com/wallet.html

The WebWallet provides a lightweight alternative for users who do not want to
operate a local full node.

The current WebWallet supports:

* the established single-private-key / WIF wallet model
* BIP39/HD Recovery Wallets compatible with VargaMesh Desktop `v0.7.1` and Android `v0.5.0`
* local encrypted wallet storage
* receive QR codes
* VMESH send/receive
* local transaction signing
* watch-only VMESH/miner-address analysis
* spendable and immature balance views
* VNS `.vmesh` registration, renewal, transfer and name-resolution workflows
* wallet activity views for received, sent and mining-related activity
* access to the public VMT-1 token directory
* installable PWA behavior on supported mobile browsers
* multilingual portal/UI delivery including English, German, Hungarian, Russian and Simplified Chinese pages

### WebWallet PWA

The WebWallet can be installed as a PWA.

On iPhone/iPad:

1. open the wallet page in Safari
2. use **Share**
3. choose **Add to Home Screen**

On Android with Chrome/Chromium:

1. use the in-page install prompt when available
2. otherwise use the browser menu
3. choose **Install app** or **Add to Home screen**

Installing the PWA is **not** a wallet backup. Recovery Phrase/WIF backup
responsibility remains unchanged.

### WebWallet HD Recovery Wallet

The HD Recovery Wallet supports:

* 12-word or 24-word BIP39 Recovery Phrases
* BIP32 hierarchical deterministic key derivation
* BIP84 Native SegWit
* proposed VargaMesh SLIP-0044 Mainnet coin type `22093`
* local Recovery Phrase creation and restore
* local encrypted wallet storage
* explicit Recovery Phrase copy/export during creation

The current lightweight recovery convention uses:

```text
m/84'/22093'/0'/0/0
```

This is the first external Native SegWit receive address of the same wallet tree
used by VargaMesh Desktop `v0.7.1` and VargaMesh Android `v0.5.0`.

### WebWallet VNS integration

The current WebWallet integrates the **VargaMesh Name Service (VNS)**.

Current VNS wallet functionality includes:

* search and resolve human-readable `.vmesh` names
* register names through the on-chain commit/reveal flow
* select a registration period from `1` to `5` years
* renew existing names
* transfer name ownership
* resolve a `.vmesh` name to its current VMESH target address
* send normal VMESH payments to a `.vmesh` name after resolution
* keep wallet private keys and Recovery Phrase material local to the client

The current VNS registration policy uses a fee of **10 VMESH per year**.
Registration fees are directed to the publicly declared VargaMesh Ecosystem
Treasury according to the current VNS policy.

VNS uses a small ownership anchor output. A VNS-aware client should avoid
accidentally spending that owner output during unrelated payments.

### WebWallet activity model

The WebWallet combines currently visible on-chain outputs, mining payout scans
and locally recorded outgoing browser-wallet broadcasts.

Current filters include:

* All
* Received
* Sent
* Mining

The public API does not necessarily represent a complete lifetime address
transaction index in every lightweight-wallet view. The blockchain explorer
remains the independent verification surface for confirmed transactions.

### WebWallet local security model

Recovery Wallet material is handled locally in the browser.

The current HD wallet storage uses authenticated encryption and a
password-derived key. Recovery words are not intentionally stored as plaintext
application state.

The current implementation uses:

* AES-256-GCM authenticated encryption
* PBKDF2-HMAC-SHA256
* `350,000` PBKDF2 iterations
* browser cryptographic randomness for Recovery Phrase generation

Recovery words and private keys are not required by the public VMESH API.
Signed transactions are submitted for broadcast.

Because a browser wallet depends on the integrity of code delivered by the
website origin, users should prefer the Desktop/full-node wallet for larger
holdings or higher-security use cases.

### WebWallet / Android / Desktop interoperability

A valid Recovery Phrase derives the same first Native SegWit `vm1...` address in
Android, Desktop and the current WebWallet.

The lightweight clients use:

```text
m/84'/22093'/0'/0/0
```

The clients may use different encrypted local/backup container formats.
Recovery Phrase interoperability therefore refers to deterministic wallet
derivation, not automatic interchangeability of every encrypted JSON file.

Legacy `v0.2.x` Android single-key/WIF backups remain supported for migration
and compatibility where the client explicitly supports that format.

### Wallet access models

| Client              | Model                                                   | Platform      |
| ------------------- | ------------------------------------------------------- | ------------- |
| VargaMesh Desktop   | Full-node HD / Core wallet + native VMT-1              | Windows/macOS |
| VargaMesh Android   | Native index-0 HD Recovery + legacy WIF + native VMT-1 | Android       |
| VargaMesh WebWallet | Browser/PWA single-key + HD Recovery Wallet            | Browser / PWA |

The browser wallet should not be confused with VargaMesh Desktop.

VargaMesh Desktop runs and synchronizes VargaMesh Core locally. The WebWallet
uses purpose-built public HTTPS APIs for blockchain data and signed-transaction
broadcast.

---

# VMT-1 Token Layer

**VMT-1** is the VargaMesh fungible-token application layer.

Public token directory:

https://vargamesh.com/tokens.html

VMT-1 does **not** replace VMESH and does not modify the VargaMesh Core
consensus rules. Token operations are encoded in ordinary VargaMesh
transactions and reconstructed deterministically by independent VMT-aware
indexers.

Current public directory data describes VMT-1 as active on Mainnet from block
`8,633+`.

### Supported operations

The VMT-1 ledger recognizes:

* `CREATE`
* `MINT`
* `TRANSFER`
* `BURN`

A confirmed CREATE transaction defines a new token. The CREATE transaction TXID
becomes that token's VMT token ID.

### Address and authorization model

VMT-1 balances belong to a concrete native SegWit `vm1...` address.

For owner/issuer-authorized token operations, transaction input `0` is used as
the authorization anchor. Wallet clients therefore have to preserve and verify
the intended owner/issuer input ordering when constructing and signing token
transactions.

A VMT owner address may also need a confirmed spendable VMESH UTXO so the
transaction can prove authorization and pay the normal base-chain network fee.

### CREATE policy and fees

Clients should obtain the current VMT CREATE policy from the public VMT status
API rather than hard-coding an amount, activation height or recipient.

TRANSFER, BURN and authorized MINT use the normal VMESH network transaction fee.
The VMT CREATE policy is separate and may include an additional active CREATE
fee according to the published Mainnet policy.

### Token directory trust model

On-chain token facts are chain-derived.

The public directory indexes:

* token existence
* issuer
* current/lifetime supply data
* holder balances
* CREATE/MINT/TRANSFER/BURN events
* token activity

Issuer logos, descriptions and external links are **off-chain metadata**.
Publication of that metadata requires issuer proof and manual review. Approval
means the metadata passed the directory workflow; it is not an audit,
investment recommendation, price guarantee or legal/compliance certification.

Public read replicas can verify the authority's signed metadata manifest.

### Current client support

| Client / service          | VMT-1 status |
| ------------------------- | ------------ |
| VargaMesh Desktop v0.7.1  | 🟢 Native portfolio + CREATE/TRANSFER/BURN/MINT |
| VargaMesh Android v0.5.0  | 🟡 VMT-1 source support; build and device validation pending |
| VargaMesh Token Directory | 🟢 Public read-only directory |
| Public API                | 🟢 VMT-1 status, stats, balances, tokens, holders and events |
| VargaMesh WebWallet       | 🟢 Public token-directory access / web integration surface |

### Public VMT-1 API

Current public endpoints include:

```text
GET /api/v1/vmt/status
GET /api/v1/vmt/stats
GET /api/v1/tokens
GET /api/v1/tokens/<token_id>
GET /api/v1/tokens/<token_id>/holders
GET /api/v1/tokens/<token_id>/events
GET /api/v1/addresses/<vm1...>/tokens
```

The public VMT API is an indexer/API layer. VargaMesh Core remains authoritative
for base-chain consensus and transaction validity.

---

# VargaMesh Name Service (VNS)

The **VargaMesh Name Service (VNS)** provides human-readable `.vmesh` names that
resolve to VMESH addresses.

Example:

```text
alice.vmesh -> vm1...
```

VNS is designed as an on-chain naming layer rather than a custodial address book.

Current implementation includes:

* `.vmesh` name search and resolution
* on-chain commit/reveal registration
* registration periods from `1` to `5` years
* `10 VMESH` per year registration policy
* renewal
* ownership transfer
* target-address updates
* Explorer resolution and full on-chain name history
* Mempool recognition of VNS registration/state transactions
* direct WebWallet payments to resolved `.vmesh` names

### Current VNS interfaces

| Interface                       | Status |
| ------------------------------- | ------ |
| VargaMesh WebWallet             | 🟢 Integrated |
| Public Explorer                 | 🟢 Integrated |
| VargaMesh Mempool Explorer      | 🟢 Integrated / active development |
| VargaMesh Desktop `v0.7.1`      | Address book + name resolution + recheck before send |
| VargaMesh Android `v0.5.0`      | Optional name resolution + address confirmation (pre-release) |

The Explorer can expose the current name status, resolved target address,
registration/expiry height, owner reference and the available on-chain history.

The Mempool Explorer can identify VNS-related transactions and surface
registration metadata such as name, owner/anchor information, fee/treasury
information and pending/active state where available.

> [!NOTE]
> VNS name resolution is a convenience and identity layer. The VMESH address
> produced by resolution remains the actual transaction destination.

---

# VargaMesh Explorer

Public explorer:

https://vargamesh.com/explorer.html

The public explorer can be used to inspect:

* blocks
* transactions
* addresses
* `.vmesh` name resolution
* VNS registration and ownership history
* network information
* blockchain activity

Explorer output exists for visibility and convenience.

> [!NOTE]
> Consensus validity is determined by VargaMesh Core, not by a web explorer.

---

# VargaMesh Mempool Explorer

A dedicated VargaMesh mempool and blockchain explorer stack is under active
development.

Development endpoint:

https://mempool.vargamesh.com

Current and developing functionality includes:

* block visualization
* transaction visualization
* mempool activity
* fee information
* address activity
* network statistics
* VMESH-specific indexing
* VNS transaction recognition
* `.vmesh` registration metadata
* VNS pending/active state display where available

The VMESH-specific indexing backend remains under active development and
testing. VNS support is already integrated into the current Mempool Explorer
interface while broader indexing work continues.

> [!WARNING]
> Development explorer data should not yet be treated as authoritative until
> the indexing system is explicitly marked production-ready.

---

# VargaProof

**VargaProof** is the VargaMesh proof-of-existence service using the `VPF1`
on-chain format.

Public interface:

https://vargamesh.com/proof.html

A file is hashed locally with SHA-256. The file itself does not need to be
uploaded to the VargaProof service.

The on-chain VPF1 payload is:

```text
"VPF1" + 32-byte SHA-256 digest
```

Hex prefix:

```text
56504631
```

The resulting 36-byte payload is committed in an unspendable `OP_RETURN`
output.

A confirmed VargaProof demonstrates that the exact SHA-256 digest was committed
no later than the confirming VargaMesh block. It does **not** by itself prove
authorship, ownership, truth of content, contractual validity or legal
certification.

Public API support includes proof status, digest lookup, recent anchors and a
proof OpenAPI specification.

---

# MeshProof

**MeshProof** is the public VargaMesh node-contribution and observation system.

Public interface:

https://vargamesh.com/meshproof.html

MeshProof is **off-consensus**. It does not alter Proof of Work, AuxPoW, block
rewards or VargaMesh Core consensus rules.

Current functionality includes:

* public node registration
* real VargaMesh P2P handshake validation on port `29666`
* randomized ping/pong liveness challenges
* one-time ownership verification through a temporary node user-agent claim
* repeated public availability probes
* Mesh Score and observed coverage
* latency visibility
* public contribution history
* hash-chained probe receipts
* optional Treasury-funded community rewards

Current reward safety limits are:

```text
100 VMESH/day total
10 VMESH/node maximum per completed UTC epoch
```

Current published eligibility includes:

* verified ownership
* at least `12` probes in the epoch
* at least `90%` observed uptime
* Mesh Score of at least `70`

Reward weight is based on Mesh Score and observed epoch coverage. Anti-Sybil
limits apply. Registration alone does not guarantee a payout.

MeshProof rewards are funded from existing VMESH staged from the VargaMesh
Ecosystem Treasury. They are not protocol block rewards and do not create new
VMESH.

---

# VargaMesh Ecosystem Treasury

The **VargaMesh Ecosystem Treasury** is a public project-operated VMESH wallet
for voluntary ecosystem contributions and documented distributions.

Public interface:

https://vargamesh.com/treasury.html

Public Treasury address:

```text
vm1qmh7cnjshtxyrqdfefzs2k7g6r7pd7ma9ntrleg
```

The Treasury page exposes on-chain balance reconciliation, verified attributed
contributions, a public activity ledger and documented distribution purposes.

Important properties:

* contributions are voluntary
* direct on-chain movements are independently inspectable
* pseudonymous contributor attribution is optional
* unattributed inflows remain separate instead of being assigned to a nickname without proof
* contribution-share percentages are transparency metrics only
* contribution share does not create equity, repayment, dividends, profit participation, redemption or voting rights
* Treasury funds do not guarantee VMESH price, liquidity, exchange listing or future rewards
* MeshProof rewards are a documented Treasury-funded program rather than new coin issuance

The Treasury is not a DAO escrow. Spending authority remains with the project
operator and distributions should be published with purpose and transaction
references.

---

# Mining Infrastructure

VargaMesh uses **SHA-256d Proof-of-Work** and supports **AuxPoW merged mining**.

Mining infrastructure is developed separately from VargaMesh Core.

CkPool repository:

https://github.com/ati1993de/vargamesh-ckpool

### Public VMESH SOLO

Primary direct VMESH mining endpoint:

```text
stratum+tcp://vargamesh.com:3933
```

Miner configuration:

```text
User: vm1YOUR_VMESH_ADDRESS.worker
Pass: x
Start difficulty: 1024
```

Current documented payout policy:

```text
99% miner
1% pool / development
```

The split is designed to be encoded directly in the VMESH child coinbase. The
confirmed coinbase transaction remains authoritative for each payout.

### BTC + VMESH Merged SOLO

AuxPoW merged-mining endpoint:

```text
stratum+tcp://mergedsolo.vargamesh.com:3950
```

Configuration:

```text
Username: YOUR_BTC_PAYOUT_ADDRESS
Password: YOUR_VMESH_PAYOUT_ADDRESS
Mode: Bitcoin SOLO + VMESH AuxPoW
```

Current documented fee policy:

```text
BTC side:   0% pool fee
VMESH side: 99% miner / 1% pool-development
```

The same SHA-256d parent work is evaluated independently against the Bitcoin
parent target and VargaMesh child target. Mining VMESH through AuxPoW does not
split the ASIC's hashrate into a second algorithm.

### Home Mining / PPLNS

Additional Home Mining endpoint:

```text
stratum+tcp://homemining.vargamesh.com:3940
```

Configuration:

```text
Username: YOUR_VMESH_ADDRESS.WorkerName
Password: x
Algorithm: SHA-256d
```

Home Mining is a separate PPLNS service with independent accounting. It does not
share accounting with VMESH SOLO or BTC + VMESH Merged SOLO.

### Consensus relationship

AuxPoW validation is performed by VargaMesh Core.

Mining services, dashboards and pool accounting are infrastructure layers.
A pool UI or telemetry endpoint is not consensus-authoritative.

Proof of Work is probabilistic. Hashrate, accepted shares, historic block finds,
pool policy or past payouts do not guarantee future blocks, payouts,
profitability or economic value.

---

# Public API

VargaMesh provides a purpose-built public HTTPS API.

Base URL:

```text
https://vargamesh.com/api/v1
```

The API is intended for public applications including:

* Android clients
* browser applications
* explorers
* monitoring services
* VMT-1 token tools
* external integrations

The public HTTPS API is **not VargaMesh Core RPC**.

Applications should use the public API instead of exposing Core RPC to the
Internet.

### Core public endpoints

Representative endpoints include:

```text
GET /api/v1/health
GET /api/v1/config
GET /api/v1/status
GET /api/v1/blocks
GET /api/v1/block/{id}
GET /api/v1/tx/{txid}
GET /api/v1/address/{address}
GET /api/v1/utxos/{address}
GET /api/v1/pool
GET /api/v1/pool/blocks
GET /api/v1/supply
GET /api/v1/supply/circulating
GET /api/v1/supply/total
GET /api/v1/releases
GET /api/v1/wallet-standard
GET /api/v1/fee
POST /api/v1/broadcast
```

Only already signed raw transactions are submitted through the broadcast path.

### VMT-1 endpoints

```text
GET /api/v1/vmt/status
GET /api/v1/vmt/stats
GET /api/v1/tokens
GET /api/v1/tokens/<token_id>
GET /api/v1/tokens/<token_id>/holders
GET /api/v1/tokens/<token_id>/events
GET /api/v1/addresses/<vm1...>/tokens
```

### VargaProof endpoints

The public API also exposes VargaProof status, digest lookup, recent proof
anchors and an OpenAPI document.

### VNS API

The VNS application layer exposes name status, availability, resolution,
history/address lookups and payload-encoding helpers for commit/register,
transfer and renew flows.

### Treasury and MeshProof APIs

Public transparency endpoints exist for Treasury status/ledger/contributors and
MeshProof status/nodes/receipts/reward previews/epochs.

Public applications must not require or request user Recovery Phrases, private
keys, WIFs or Core RPC credentials.

---

# Public Testnet

VargaMesh provides an official public Testnet for wallet, exchange, node,
explorer, API and mining integration testing.

> **tVMESH has no monetary value.**

| Parameter | Value |
| --- | --- |
| Network | VargaMesh Public Testnet v1 |
| Currency | `tVMESH` |
| P2P port | `39666` |
| RPC port | `39667` |
| Bech32 HRP | `tvm` |
| AuxPoW chain ID | `22094` / `0x564E` |
| Target spacing | `120 seconds` |
| Coinbase maturity | `100 blocks` |
| Genesis hash | `00000000747ff112e44641470bb636ea00e43ed3cef5e1e94c451d9cd02efea9` |

Start a Testnet node:

```bash
./build/bin/vargameshd -testnet
```

Public infrastructure:

- Testnet portal: https://testnet.vargamesh.com/
- Explorer: https://testnet.vargamesh.com/explorer.html
- WebWallet: https://testnet.vargamesh.com/wallet.html
- Public API: https://testnet.vargamesh.com/api/v1/
- SOLO Stratum: `testnet.vargamesh.com:4933`

Documentation:

- [Public Testnet documentation](doc/testnet.md)
- [Exchange and integration guide](doc/exchange-integration.md)

---

# Mainnet Parameters

| Parameter                | Value                                                              |
| ------------------------ | ------------------------------------------------------------------ |
| Network                  | VargaMesh Mainnet                                                  |
| Currency                 | `VMESH`                                                            |
| Consensus                | Proof of Work                                                      |
| Proof-of-Work algorithm  | SHA-256d                                                           |
| P2P port                 | `29666`                                                            |
| RPC port                 | `29667`                                                            |
| Genesis hash             | `00000000b0c55c00c13e2ee67b54fe33c327c8806f8da5050cfa47095d182d9a` |
| Genesis merkle root      | `ab8120503c6bc408d07a5257f0dce8019ec494dbf627bd674c93d4f86122cb36` |
| Genesis timestamp        | `1789855200`                                                       |
| Genesis nonce            | `1174766078`                                                       |
| Genesis bits             | `1d00ffff`                                                         |
| AuxPoW chain ID          | `22093` (`0x564D`)                                                 |
| AuxPoW activation        | Block `1`                                                          |
| Target block spacing     | `120 seconds`                                                      |
| Difficulty adjustment    | ASERT                                                              |
| ASERT half-life          | `34560 seconds`                                                    |
| Coinbase maturity        | `100 blocks`                                                       |
| Initial block subsidy    | `25 VMESH`                                                         |
| Subsidy halving interval | `1,051,200 blocks`                                                 |
| Maximum supply           | `52,560,000 VMESH`                                                 |
| Bech32 HRP               | `vm`                                                               |
| Protocol version         | `70016`                                                            |

> [!IMPORTANT]
> The VargaMesh Core source code remains authoritative for all
> consensus-critical values.

Additional mainnet documentation:

[doc/mainnet.md](doc/mainnet.md)

---

# Address and Wallet Parameters

Current VargaMesh wallet/address parameters include:

| Parameter                         | Value                                      |
| --------------------------------- | ------------------------------------------ |
| Elliptic curve                    | `secp256k1`                                |
| Primary address type              | SegWit v0 P2WPKH                           |
| Bech32 HRP                        | `vm`                                       |
| WIF version                       | `190 / 0xBE`                               |
| Legacy P2PKH version              | `70`                                       |
| Legacy P2SH version               | `50`                                       |
| Currency decimals                 | `8`                                        |
| BIP39 Recovery Phrase             | `12` or `24` words                         |
| Proposed Mainnet SLIP-0044 type   | `22093`                                    |
| BIP84 receive path                | `m/84'/22093'/0'/0/index`                  |
| BIP84 change path                 | `m/84'/22093'/0'/1/index`                  |
| BIP44 receive path                | `m/44'/22093'/0'/0/index`                  |
| BIP44 change path                 | `m/44'/22093'/0'/1/index`                  |
| Mainnet extended public key bytes | `0x024D771C`                               |
| Mainnet extended secret key bytes | `0x024D7707`                               |

Client applications should follow documented VargaMesh wallet standards.

Independent clients should not introduce incompatible address or key
derivation schemes without first defining and documenting the relevant
VargaMesh standard.

The Mainnet value `22093` is currently the **proposed** SLIP-0044 coin type.
The AuxPoW chain ID happens to use the same decimal value, but that does not
make it an official SLIP-0044 registry assignment.

For Testnet HD derivation, clients should use the standard test-network
coin type `1`; the VargaMesh Testnet AuxPoW chain ID `22094` is not the
Testnet BIP44/SLIP-0044 coin type.

---

# HD Recovery Wallet Standard

VargaMesh Desktop `v0.7.1`, VargaMesh Android `v0.5.0` and the current
VargaMesh WebWallet implement the same first-address Mainnet
BIP39/BIP32/BIP84 Recovery Wallet derivation.

### Mainnet hierarchy

Native SegWit / BIP84:

```text
Receive: m/84'/22093'/0'/0/index
Change : m/84'/22093'/0'/1/index
```

Legacy compatibility / BIP44:

```text
Receive: m/44'/22093'/0'/0/index
Change : m/44'/22093'/0'/1/index
```

BIP84 is the default for newly created Recovery Wallets.

BIP44 descriptors exist for legacy/recovery compatibility and are not the
default active receive path in VargaMesh Desktop.

### VargaMesh BIP32 serialization

VargaMesh Mainnet uses VargaMesh-specific extended-key version bytes:

```text
Extended public key: 0x024D771C
Extended secret key: 0x024D7707
```

VargaMesh private extended keys are therefore not serialized as Bitcoin
`xprv` keys. Implementations must use the VargaMesh Mainnet BIP32 version
bytes when constructing private/public extended keys for descriptors.

### Deterministic interoperability test vector

The following mnemonic is a **public BIP39 test vector only** and must never
be used to hold real funds:

```text
abandon abandon abandon abandon abandon abandon
abandon abandon abandon abandon abandon abandon about
```

At:

```text
m/84'/22093'/0'/0/0
```

the expected VargaMesh Mainnet Native SegWit address is:

```text
vm1q7w4sewykzmq3x5jyazqhefz7jptmnzylx3cjv2
```

This vector defines the deterministic interoperability target for the
VargaMesh Core descriptor wallet path and the Desktop, Android and WebWallet
implementations.

### Current interoperability scope

| Capability                              | Desktop v0.7.1 | WebWallet | Android v0.5.0 |
| --------------------------------------- | :------------: | :-------: | :------------: |
| BIP39 12/24 words                       |       Yes      |    Yes    |       Yes      |
| BIP32 HD derivation                     |       Yes      |    Yes    |       Yes      |
| BIP84 `m/84'/22093'...`                 |       Yes      |    Yes    |       Yes      |
| Same first `vm1...` address from phrase |       Yes      |    Yes    |       Yes      |
| Multiple HD receive/change addresses    |       Yes      |    No     |       No       |
| Full HD gap-limit scanning              |       Yes      |    No     |       No       |
| Legacy single-key/WIF wallets           |       Yes      |    Yes    |       Yes      |

The current WebWallet and Android implementations use index `0` as their active
HD address. Desktop can derive additional receive/change addresses. Full
gap-limit scanning and multi-address aggregation remain future work for the
lightweight clients.

### Recovery Phrase security

A Recovery Phrase is sufficient to reconstruct the deterministic wallet.

Never send a Recovery Phrase to:

* support staff
* Discord users
* websites asking for verification
* exchange operators
* mining pools
* third parties

No VargaMesh service needs a user's Recovery Phrase in order to provide
network, explorer, mining or API functionality.

---

# Software Components

A complete VargaMesh Core build may produce the following binaries:

| Binary             | Purpose                      |
| ------------------ | ---------------------------- |
| `vargameshd`       | Full-node daemon             |
| `vargamesh-cli`    | JSON-RPC command-line client |
| `vargamesh-qt`     | Native graphical Core client |
| `vargamesh-wallet` | Offline wallet utility       |
| `vargamesh-tx`     | Raw transaction utility      |
| `vargamesh-util`   | Utility commands             |

Availability depends on build configuration and platform.

---

# Building VargaMesh Core

Platform-specific VargaMesh build notes:

* Linux: [doc/build-vargamesh-linux.md](doc/build-vargamesh-linux.md)
* Windows: [doc/build-vargamesh-windows.md](doc/build-vargamesh-windows.md)
* macOS Intel / Apple Silicon: [doc/build-vargamesh-macos.md](doc/build-vargamesh-macos.md)

Typical source checkout:

```bash
git clone https://github.com/ati1993de/vargamesh-core.git
cd vargamesh-core
```

Official v0.2.0 release archives are available for Linux x86_64, Windows
x86_64, macOS x86_64 and macOS arm64. Verify the published SHA-256 checksum
files before replacing an existing node binary.

Follow the platform-specific build documentation for dependencies and
configuration options.

> [!NOTE]
> Do not assume that upstream Bitcoin Core build instructions are identical
> to the current VargaMesh tree.

---

# Running a VargaMesh Node

After building the daemon:

```bash
./src/vargameshd
```

Query the local node:

```bash
./src/vargamesh-cli getblockchaininfo
```

Useful diagnostic commands include:

```bash
./src/vargamesh-cli getblockchaininfo
./src/vargamesh-cli getnetworkinfo
./src/vargamesh-cli getpeerinfo
./src/vargamesh-cli getmininginfo
```

A synchronized node independently validates the VargaMesh blockchain
according to the consensus rules implemented by VargaMesh Core.

---

# RPC Security

> [!CAUTION]
> **Never expose VargaMesh Core RPC directly to the public Internet.**

Recommended practices:

* bind RPC to trusted interfaces only
* use strong RPC authentication
* restrict access through firewall rules
* do not place RPC credentials in public repositories
* do not expose wallet RPC publicly
* isolate public web applications from Core RPC
* use the public HTTPS API for external applications

Public services such as Android clients, explorers and web applications
should communicate through purpose-built APIs instead of Core RPC.

---

# Self-Custody

VargaMesh wallets are designed around **self-custody**.

The user controls the private keys that control their VMESH.

There is no central service capable of resetting a lost private key.

There is no central service capable of recovering funds if all private keys
and backups are lost.

Wallet backups should therefore be:

* encrypted
* independently verified
* stored in more than one secure location
* tested before significant funds are used

Never share:

* private keys
* WIFs
* wallet passwords
* seed material
* decrypted wallet backups

Anyone possessing the private key associated with an address may be able to
spend the corresponding VMESH.

---

# Development Status

VargaMesh is under active development.

| Component                  | Status                                      |
| -------------------------- | ------------------------------------------- |
| VargaMesh Core `v0.2.0`    | 🟢 Mainnet + Testnet · Linux/Windows/macOS |
| VargaMesh Desktop `v0.7.1` | 🟢 GitHub · Windows/macOS |
| VargaMesh Android `v0.5.0` | 🟡 Source pre-release · device tests outstanding |
| VargaMesh WebWallet        | 🟢 Available                                |
| VMT-1 Token Layer         | 🟢 Mainnet · directory/API · native clients |
| VargaMesh Name Service     | 🟢 Available · WebWallet / Explorer / Mempool |
| Public Explorer            | 🟢 Available                                |
| Public API                 | 🟢 Available                                |
| Public network statistics  | 🟢 Available                                |
| Mining / CkPool            | 🟢 Available / active development           |
| VargaProof                 | 🟢 Available                                |
| MeshProof                  | 🟢 Available                                |
| Ecosystem Treasury         | 🟢 Available · public on-chain transparency |
| VargaMesh Mempool Explorer | 🔵 Active development                       |

Interfaces, APIs and wallet functionality may evolve.

Consensus-critical modifications require particular care and must be
implemented and reviewed at the VargaMesh Core layer.

---

# Roadmap Direction

VargaMesh uses status-based development rather than guaranteed release dates.

### Core

* continued VargaMesh Core `v0.2.x` testing and hardening
* consensus and AuxPoW validation review
* broader independent-node testing
* easier node deployment
* expanded integration documentation

### Desktop

Current GitHub release: **VargaMesh Desktop v0.7.1**

Priorities include:

* continued VMT-1 transaction and portfolio hardening
* continued four-language UI and notification testing
* continued Recovery Wallet / descriptor interoperability testing
* improved wallet UX and diagnostics
* continued Core/WIF/watch-only compatibility testing
* VNS recipient-resolution regression testing

### Android

Current source pre-release: **VargaMesh Android v0.5.0** (physical-device validation required)

Priorities include:

* continued VMT-1 wallet hardening
* broader physical-device testing
* continued 12/24-word Recovery Wallet testing
* Android/Desktop/WebWallet deterministic recovery interoperability validation
* encrypted HD JSON backup/restore testing
* legacy `v0.2.x` WIF/backup migration testing
* transaction compatibility validation
* wallet UX, QR payment requests and offline backup verification testing
* future multi-address HD / gap-limit scanning and WorkManager reliability testing
* VNS ownership-anchor UTXO exclusions and physical-device tests

### VMT-1

Current shipped/public components include native Desktop/Android token support,
the public token directory and public VMT API.

Further work can include:

* additional independent VMT indexers/read replicas
* indexer consistency/audit tooling
* improved token-directory UX
* metadata/logo workflow hardening
* broader WebWallet token-management parity
* additional developer documentation and regression vectors

### Wallet Standards

Current and future wallet-standard work includes:

* continued validation of the documented VMESH BIP39/HD derivation standard
* upstream SLIP-0044 registration request for proposed Mainnet coin type `22093`
* WebWallet and Android gap-limit scanning / multi-address HD support
* continued Android/Desktop/WebWallet HD Recovery interoperability testing
* advanced wallet-management functionality where justified

Hardware-wallet/Trezor integration is **not a current project priority**.

### VargaMesh Name Service

* Desktop wallet `.vmesh` resolution and payment integration
* Android wallet `.vmesh` resolution and payment integration
* expanded VNS metadata/indexing
* improved name-management UX
* continued commit/reveal, renewal and transfer interoperability testing

### Explorer and Mempool

* VMESH-specific mempool indexing maturity
* richer transaction and token information
* additional fee data
* improved address information
* broader network visualization
* continued VNS and VMT-1 event visibility

### Developer Ecosystem

* expanded public API
* SDK examples
* integration documentation
* easier third-party application development
* PHP examples
* Java examples
* JavaScript examples
* Python examples

### Network

* additional independent public nodes
* wider geographic node distribution
* improved mining tooling
* broader AuxPoW testing
* improved observability and monitoring

> [!NOTE]
> Roadmap items describe development direction and should not be interpreted
> as guaranteed release dates.

---

# External Listings

VargaMesh may be listed by independent third-party data platforms and services.

Canonical project/listing information:

https://vargamesh.com/listing/

Current verified Blockspot listing:

https://blockspot.io/coin/vargamesh-vmesh/

The Blockspot verified status indicates that project listing information has
been validated/confirmed through their listing process.

It should not be interpreted as an audit, financial recommendation or
endorsement of VMESH.

Third-party exchanges, directories, wallets, pools or integrations operate
independently. A listing or integration should not be interpreted as a promise
of price, liquidity, availability or future support.

---

# Project Links

| Resource              | URL                                                  |
| --------------------- | ---------------------------------------------------- |
| Official website      | https://vargamesh.com                                |
| Project portal        | https://mesh.vargatech.net                           |
| Core repository       | https://github.com/ati1993de/vargamesh-core          |
| Core releases         | https://github.com/ati1993de/vargamesh-core/releases |
| Desktop repository    | https://github.com/ati1993de/vargamesh-desktop       |
| Desktop releases      | https://github.com/ati1993de/vargamesh-desktop/releases/latest |
| Microsoft Store       | https://apps.microsoft.com/detail/9NX9S7SJSW0S?hl=de-de&gl=DE&ocid=pdpshare |
| Android information   | https://vargamesh.com/android.html                   |
| Android APK           | https://vargamesh.com/vmesh.apk                      |
| Android APK SHA256    | https://vargamesh.com/vmesh.apk.sha256               |
| Android APKPure       | https://apkpure.com/p/de.vargatech.vargamesh         |
| WebWallet / PWA       | https://vargamesh.com/wallet.html                    |
| Token directory       | https://vargamesh.com/tokens.html                    |
| Explorer              | https://vargamesh.com/explorer.html                  |
| Mempool Explorer      | https://mempool.vargamesh.com                        |
| Network statistics    | https://vargamesh.com/network.html                   |
| VargaProof            | https://vargamesh.com/proof.html                     |
| MeshProof             | https://vargamesh.com/meshproof.html                 |
| Ecosystem Treasury    | https://vargamesh.com/treasury.html                  |
| Public API            | https://vargamesh.com/api/v1                         |
| Release manifest      | https://vargamesh.com/api/v1/releases                |
| Wallet standard       | https://vargamesh.com/wallet-standard/               |
| CkPool                | https://github.com/ati1993de/vargamesh-ckpool        |
| Roadmap               | https://vargamesh.com/roadmap.html                   |
| Listing information   | https://vargamesh.com/listing/                       |
| Blockspot             | https://blockspot.io/coin/vargamesh-vmesh/           |
| Discord               | https://discord.gg/AeC8sxt2aB                        |

---

# Contributing

Contributions, testing and technical feedback are welcome.

Useful contributions include:

* reproducible bug reports
* code review
* build testing
* Linux build testing
* Windows testing
* macOS Intel / Apple Silicon testing
* Android device testing
* wallet interoperability and deterministic recovery testing
* documentation improvements
* node testing
* mining testing
* AuxPoW testing
* explorer/indexer testing
* VMT-1 indexer and wallet-operation testing
* multilingual UI review
* API testing

When reporting a problem, include:

* software version
* operating system
* relevant logs
* reproduction steps
* expected behavior
* actual behavior

Do **not** include sensitive wallet information.

Never include:

* private keys
* WIFs
* wallet passwords
* decrypted backups

---

# Security Notice

Wallet and blockchain software should be treated as security-sensitive
software.

Recommended precautions:

* verify release checksums (including the published Android APK SHA256 file)
* use official download sources
* keep backups
* test backups
* verify wallet recovery before relying on meaningful amounts
* keep operating systems updated
* protect private keys
* protect Recovery Phrases / seed material
* protect RPC credentials
* never expose Core RPC publicly
* verify destination addresses before sending
* independently confirm important transactions in the blockchain
* verify VMT token IDs, owner/issuer addresses and signed preflight results before token broadcasts

If a security issue could put users or funds at risk, avoid publishing
private-key material or exploit details together with live wallet data.

---

# Disclaimer

VargaMesh Core and the surrounding VargaMesh ecosystem are under active
development.

Software under active development may contain bugs, regressions or incomplete
functionality.

Nothing in this repository constitutes:

* investment advice
* financial advice
* legal advice
* tax advice
* a promise of future value
* a guarantee of software availability

Users remain responsible for:

* securing private keys
* maintaining wallet backups
* validating software before use
* evaluating the risks of wallet and node software
* complying with applicable laws and regulations

The **VargaMesh Core source code is the authoritative reference for
consensus-critical VargaMesh network behavior**.

---

<p align="center">
  <strong>VargaMesh · Independent infrastructure. Open participation. Self-custody.</strong>
</p>

<p align="center">
  <a href="https://vargamesh.com">vargamesh.com</a>
  ·
  <a href="https://github.com/ati1993de/vargamesh-core">GitHub</a>
  ·
  <a href="https://discord.gg/AeC8sxt2aB">Discord</a>
</p>

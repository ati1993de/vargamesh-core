# VargaMesh Core

<p align="center">
  <strong>Reference full-node implementation for the VargaMesh (VMESH) network.</strong>
</p>

<p align="center">
  Independent Proof-of-Work · SHA-256d · AuxPoW · ASERT · Full Node · Desktop · Android · WebWallet · Explorer · Mining · Mainnet
</p>

<p align="center">
  <img alt="Core" src="https://img.shields.io/badge/Core-v0.2.0-2ea44f">
  <img alt="Desktop" src="https://img.shields.io/badge/Desktop-v0.4.0-0078d4">
  <img alt="Android" src="https://img.shields.io/badge/Android-v0.3.0-3ddc84">
  <img alt="Network" src="https://img.shields.io/badge/Network-Mainnet-orange">
  <img alt="Consensus" src="https://img.shields.io/badge/Consensus-SHA--256d%20PoW-yellow">
</p>

<p align="center">
  <a href="https://vargacoin.com">Website</a>
  ·
  <a href="https://mesh.vargatech.net">Project Portal</a>
  ·
  <a href="https://vargacoin.com/explorer.html">Explorer</a>
  ·
  <a href="https://vargacoin.com/wallet.html">WebWallet</a>
  ·
  <a href="https://vargacoin.com/android.html">Android</a>
  ·
  <a href="https://discord.gg/AeC8sxt2aB">Discord</a>
</p>

---

> [!IMPORTANT]
> **VargaMesh is under active development.**
>
> VargaMesh Core `v0.2.0` is the current Mainnet/Testnet release. VargaMesh
> Desktop `v0.4.0` and VargaMesh Android `v0.3.0` should currently be treated
> as public-testing / pre-release wallet software.
>
> Use small amounts while testing wallet software and always maintain verified
> backups of private keys, Recovery Phrases and wallet data.

---

## Table of Contents

* [Overview](#overview)
* [Project Status](#project-status)
* [VargaMesh Ecosystem](#vargamesh-ecosystem)

  * [VargaMesh Core](#vargamesh-core-1)
  * [VargaMesh Desktop](#vargamesh-desktop)
  * [VargaMesh Android](#vargamesh-android)
  * [VargaMesh WebWallet](#vargamesh-webwallet)
  * [VargaMesh Explorer](#vargamesh-explorer)
  * [VargaMesh Mempool Explorer](#vargamesh-mempool-explorer)
  * [VargaProof](#vargaproof)
  * [Mining Infrastructure](#mining-infrastructure)
  * [Public API](#public-api)
* [Mainnet Parameters](#mainnet-parameters)
* [Address and Wallet Parameters](#address-and-wallet-parameters)
* [HD Recovery Wallet Standard](#hd-recovery-wallet-standard)
* [Software Components](#software-components)
* [Building VargaMesh Core](#building-vargamesh-core)
* [Running a Node](#running-a-vargamesh-node)
* [RPC Security](#rpc-security)
* [Self-Custody](#self-custody)
* [Development Status](#development-status)
* [Roadmap Direction](#roadmap-direction)
* [Project Links](#project-links)
* [Contributing](#contributing)
* [Security Notice](#security-notice)
* [Disclaimer](#disclaimer)

---

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
* blockchain explorer
* public network statistics
* public HTTPS API
* public mining infrastructure
* VargaProof verification infrastructure
* VargaMesh-specific mempool explorer development

The **VargaMesh Core source tree is authoritative for consensus behavior**.

---

## Project Status

| Component                  |     Version | Status                         |
| -------------------------- | ----------: | ------------------------------ |
| VargaMesh Core             |    `v0.2.0` | Mainnet + Testnet release       |
| VargaMesh Desktop          |    `v0.4.0` | Public testing                 |
| VargaMesh Android          |    `v0.3.0` | Public testing / pre-release   |
| VargaMesh WebWallet        |     Current | Available                      |
| Public Explorer            |     Current | Available                      |
| Public API                 |        `v1` | Available                      |
| Mining / CkPool            |     Current | Available / active development |
| VargaProof                 |     Current | Available                      |
| VargaMesh Mempool Explorer | Development | In development                 |

---

## Quick Links

| Project                      | Information                                          |
| ---------------------------- | ---------------------------------------------------- |
| Network                      | VargaMesh Mainnet                                    |
| Currency                     | `VMESH`                                              |
| Core version                 | `v0.2.0`                                             |
| Website                      | https://vargacoin.com                                |
| Project portal               | https://mesh.vargatech.net                           |
| Core source code             | https://github.com/ati1993de/vargamesh-core          |
| Core releases                | https://github.com/ati1993de/vargamesh-core/releases |
| Desktop wallet               | https://github.com/ati1993de/vargamesh-desktop       |
| Android information          | https://vargacoin.com/android.html                   |
| Android APK                  | https://vargacoin.com/vmesh.apk                      |
| Android APK SHA256           | https://vargacoin.com/vmesh.apk.sha256               |
| WebWallet                    | https://vargacoin.com/wallet.html                    |
| Explorer                     | https://vargacoin.com/explorer.html                  |
| Mempool Explorer development | https://mempool.vargacoin.com                        |
| Mining / CkPool              | https://github.com/ati1993de/vargamesh-ckpool        |
| VargaProof                   | https://vargacoin.com/proof.html                     |
| Public API                   | https://vargacoin.com/api/v1                         |
| Network information          | https://vargacoin.com/network.html                   |
| Roadmap                      | https://vargacoin.com/roadmap.html                   |
| Discord                      | https://discord.gg/AeC8sxt2aB                        |

---

# VargaMesh Ecosystem

VargaMesh is developed as more than a command-line blockchain node.

The ecosystem combines the consensus layer with wallet software, network
monitoring, mining infrastructure, public APIs, explorer services and
verification tools.

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
```

---

## VargaMesh Core

**VargaMesh Core** is the consensus and full-node reference implementation
for the VargaMesh network.

Repository:

https://github.com/ati1993de/vargamesh-core

Current version:

**VargaMesh Core v0.2.0**

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

**VargaMesh Desktop** is the graphical Windows full-node wallet for users
who prefer a desktop interface instead of operating VargaMesh Core entirely
through the command line.

Repository:

https://github.com/ati1993de/vargamesh-desktop

Current release:

**VargaMesh Desktop v0.4.0**

Status:

**Public testing**

The current Desktop release bundles:

**VargaMesh Core v0.2.0**

### Available packages

* Windows x64 installer
* Windows x64 portable ZIP
* SHA256 checksums

### Current functionality

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
* wallet loading
* wallet unloading
* wallet encryption
* temporary wallet unlocking
* VMESH receiving addresses
* transaction history
* VMESH transfers
* wallet backup
* wallet restore
* WIF private-key import
* watch-only address import
* legacy wallet migration support
* English UI
* German UI

### Desktop Recovery Wallet interoperability

VargaMesh Desktop `v0.4.0` defines the full VMESH HD descriptor wallet
hierarchy. The current VargaMesh WebWallet and VargaMesh Android `v0.3.0` use
the same first Native SegWit external receive path for deterministic recovery.

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

The WebWallet currently uses the first Native SegWit external receive address:

```text
m/84'/22093'/0'/0/0
```

Therefore the same valid Recovery Phrase restores the same first `vm1...`
address in VargaMesh Desktop `v0.4.0`, VargaMesh Android `v0.3.0` and the
current WebWallet.

The Desktop wallet is a full HD descriptor wallet and can derive additional
receive/change addresses. The current WebWallet and Android `v0.3.0` remain
single-active-address implementations and do not yet perform a full HD
gap-limit scan.

> [!WARNING]
> The Desktop wallet is currently being tested by the community.
>
> Before using meaningful amounts, verify the Recovery Phrase or Core backup
> with a recovery test and begin with a small transaction.

---

# VargaMesh Android

**VargaMesh Android** is the native Android application for the
VargaMesh (VMESH) mainnet.

Current release:

**VargaMesh Android v0.3.0**

Status:

**Public testing / pre-release**

Official information:

https://vargacoin.com/android.html

Direct APK:

https://vargacoin.com/vmesh.apk

SHA256 checksum file:

https://vargacoin.com/vmesh.apk.sha256

VargaMesh Android combines the original network companion with a native
**self-custody VMESH wallet**. Version `v0.3.0` extends the wallet with the
VargaMesh BIP39/BIP32 Recovery Wallet standard while preserving compatibility
with wallets and encrypted backups created by the earlier `v0.2.x` single-key
wallet model.

---

## Android v0.3.0 Release Highlights

Version `v0.3.0` is the first Android release with deterministic Recovery Wallet
support.

Major changes include:

* 12-word and 24-word BIP39 Recovery Phrase creation
* Recovery Wallet restore from a valid BIP39 phrase
* BIP32 hierarchical deterministic key derivation
* VargaMesh Mainnet Native SegWit derivation at `m/84'/22093'/0'/0/0`
* deterministic first-address interoperability with VargaMesh Desktop `v0.4.0`
  and the current WebWallet
* encrypted local storage for Recovery Phrase material
* encrypted JSON backup export and restore for HD Recovery Wallets
* continued support for legacy `v0.2.x` encrypted single-key backups
* continued compressed VMESH WIF import support
* password-protected Recovery Phrase display/copy for HD wallets
* improved handling of sensitive clipboard contents
* heavy cryptographic operations moved away from the Android UI thread
* additional wallet error handling and runtime diagnostics
* Android release-build / dependency / packaging hardening
* Android application/cloud backup disabled for wallet secrets

The Android HD wallet currently uses one active deterministic address at index
`0`. Full gap-limit scanning and multi-address HD aggregation are not yet
implemented on Android.

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
* English interface
* German interface
* dark theme
* light theme
* system theme

Public Stratum endpoint:

```text
vargacoin.com:3933
```

---

## Native Android VMESH Wallet

VargaMesh Android `v0.3.0` provides a native self-custody wallet with both the
new HD Recovery Wallet model and the established legacy single-key/WIF model.

### Recovery Wallet functionality

* create a new 12-word BIP39 Recovery Wallet
* create a new 24-word BIP39 Recovery Wallet
* restore a wallet from a valid 12-word or 24-word Recovery Phrase
* derive the active VMESH key with BIP32
* use the VargaMesh BIP84 Mainnet path `m/84'/22093'/0'/0/0`
* derive the same first `vm1...` address as the current Desktop/WebWallet
  Recovery Wallet convention
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

Existing users do **not** need to recreate their wallet when updating from a
supported earlier Android release.

### Transaction and wallet functionality

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

---

## Android Recovery Wallet Interoperability

Android `v0.3.0` follows the same first-address Mainnet Recovery Wallet
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

* VargaMesh Desktop `v0.4.0`
* the current VargaMesh WebWallet
* VargaMesh Android `v0.3.0`

The interoperability guarantee concerns deterministic key/address derivation.
It does **not** imply that every application's encrypted JSON backup container
is interchangeable with every other client.

Android currently operates the first external address at index `0`. Desktop is
a full HD descriptor wallet and can derive additional receive/change addresses.
Android and the current WebWallet do not yet perform a full HD gap-limit scan.

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
The application derives/signs transactions locally and sends only signed raw
transactions to the public broadcast API.

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
                               │ unlocked in device memory only
                               ▼
                  Local transaction construction
                               │
                               ▼
                      Local transaction signing
                               │
                               │ signed raw transaction only
                               ▼
                 HTTPS POST /api/v1/broadcast
```

The Android application does **not** intentionally send any of the following to
the VargaMesh public server:

* Recovery Phrases / seed words
* private keys
* WIF private keys
* wallet passwords
* decrypted wallet backups

Only signed raw transactions are submitted for broadcast.

VargaMesh Core RPC is **not exposed to or directly used by the Android
application**.

Android application/cloud backup is disabled for the wallet store so that
wallet material is not silently copied through normal Android backup
mechanisms.

Sensitive wallet operations require local password verification where
applicable. Recovery Phrase and key material should be shown or copied only in
a trusted environment.

---

## Android Wallet Parameters

VargaMesh Android `v0.3.0` supports both deterministic HD Recovery Wallets and
legacy single-private-key/WIF wallets.

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

The current Android HD implementation intentionally uses the first external
Native SegWit address at index `0`. It does not yet scan an HD address range or
aggregate balances across multiple derived addresses.

---

## Android Limitations

VargaMesh Android `v0.3.0` currently does **not** implement:

* full HD gap-limit scanning
* multiple HD receive/change addresses in the active wallet UI
* multi-account wallets
* coin control
* PSBT
* hardware-wallet support
* light-client/header verification
* a locally synchronized VargaMesh full node

Android uses the purpose-built public HTTPS API for blockchain/UTXO data and
signed-transaction broadcast. Private keys and Recovery Phrases are not needed
by the public API.

VargaMesh Desktop `v0.4.0` remains the more complete HD/full-node wallet: it can
operate VargaMesh Core locally and derive additional receive/change addresses.

---

## Android Requirements

Current development requirements:

* Android Studio
* JDK 17-compatible toolchain
* Android SDK 35
* minimum Android API: `26`

Minimum supported Android version:

**Android 8.0**

---

## Android Testing Notice

> [!CAUTION]
> VargaMesh Android `v0.3.0` can create, recover, hold and spend real VMESH.
>
> It is currently **public-testing / pre-release software**.

Recommended testing procedure for a newly created Recovery Wallet:

1. create a 12-word or 24-word test Recovery Wallet
2. securely record the Recovery Phrase offline
3. record the first `vm1...` address
4. export an encrypted JSON backup
5. verify Recovery Phrase restore in a safe test environment and confirm the
   same first address
6. verify encrypted JSON restore with the correct password and confirm the same
   first address
7. receive a small VMESH amount
8. send a small VMESH transaction
9. confirm the transaction in the explorer
10. test legacy WIF/backup compatibility separately if migrating an older wallet

Do not perform destructive restore/removal tests on a funded wallet unless all
required recovery material has already been independently verified.

Do not use significant amounts until the Android wallet has received sufficient
real-device and interoperability testing.

Never share:

* Recovery Phrases / seed words
* private keys
* WIFs
* wallet passwords
* decrypted wallet backups
* encrypted wallet backup files with untrusted third parties

---

# VargaMesh WebWallet

The VargaMesh ecosystem includes a browser-based self-custody wallet.

WebWallet:

https://vargacoin.com/wallet.html

The WebWallet provides a lightweight alternative for users who do not want
to operate a local full node.

The current WebWallet supports two wallet models:

* the established single-private-key / WIF wallet model
* the newer BIP39/HD Recovery Wallet model compatible with VargaMesh Desktop
  `v0.4.0`

### WebWallet HD Recovery Wallet

The HD Recovery Wallet supports:

* 12-word or 24-word BIP39 Recovery Phrases
* BIP32 hierarchical deterministic key derivation
* BIP84 Native SegWit
* proposed VargaMesh SLIP-0044 Mainnet coin type `22093`
* local Recovery Phrase creation and restore
* local encrypted wallet storage
* explicit Recovery Phrase copy/export during creation

The current WebWallet derives its active address from:

```text
m/84'/22093'/0'/0/0
```

This is the first external Native SegWit receive address of the same wallet
tree used by VargaMesh Desktop `v0.4.0` and VargaMesh Android `v0.3.0`.

A valid Recovery Phrase therefore derives the same first `vm1...` address in
Desktop, Android and the current WebWallet.

> [!IMPORTANT]
> The current WebWallet remains a **single-address HD implementation**.
>
> VargaMesh Desktop can derive further receive/change addresses. The WebWallet
> does not yet scan or aggregate the complete HD address range.

### WebWallet local security model

Recovery Wallet material is handled locally in the browser.

The current HD wallet storage uses authenticated encryption and a
password-derived key. Recovery words are not intentionally stored as
plaintext application state.

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

### WebWallet / Android interoperability

The legacy WIF wallet and encrypted backup functionality remains supported.

VargaMesh Android `v0.3.0` now also implements the documented BIP39/BIP32
Recovery Wallet derivation. A valid Recovery Phrase derives the same first
Native SegWit `vm1...` address in Android, Desktop and the current WebWallet.

The current Android and WebWallet HD implementations both use:

```text
m/84'/22093'/0'/0/0
```

The clients may use different encrypted local/backup container formats.
Recovery Phrase interoperability therefore refers to deterministic wallet
derivation, not automatic interchangeability of every encrypted JSON file.

Legacy `v0.2.x` Android single-key/WIF backups remain supported for migration
and compatibility.

### Wallet access models

| Client              | Model                                                | Platform      |
| ------------------- | ---------------------------------------------------- | ------------- |
| VargaMesh Desktop   | Full-node HD / Core wallet                           | Windows       |
| VargaMesh Android   | Native single-address HD Recovery + legacy WIF wallet | Android       |
| VargaMesh WebWallet | Browser/PWA single-key + HD Recovery Wallet          | Browser / PWA |

The browser wallet should not be confused with VargaMesh Desktop.

VargaMesh Desktop runs and synchronizes VargaMesh Core locally. The WebWallet
uses purpose-built public HTTPS APIs for blockchain data and signed-transaction
broadcast.

---

# VargaMesh Explorer

Public explorer:

https://vargacoin.com/explorer.html

The public explorer can be used to inspect:

* blocks
* transactions
* addresses
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

https://mempool.vargacoin.com

Planned and developing functionality includes:

* block visualization
* transaction visualization
* mempool activity
* fee information
* address activity
* network statistics
* VMESH-specific indexing

The VMESH-specific indexing backend remains under development and testing.

> [!WARNING]
> Development explorer data should not yet be treated as authoritative until
> the indexing system is explicitly marked production-ready.

---

# VargaProof

VargaProof is part of the VargaMesh ecosystem and provides verification
infrastructure for cryptographic data proofs.

Public interface:

https://vargacoin.com/proof.html

VargaProof is intended to make it possible to work with cryptographic
fingerprints and verification data without treating uploaded content itself
as the authoritative object.

Typical proof systems can be used to demonstrate that a specific
cryptographic hash was associated with a particular network state or time.

> [!NOTE]
> Cryptographic proof verification should not be interpreted as automatic
> legal certification, identity verification or validation of the truth of
> the underlying content.

---

# Mining Infrastructure

VargaMesh uses **SHA-256d Proof-of-Work** and supports **AuxPoW merged
mining**.

Mining infrastructure is developed separately from VargaMesh Core.

CkPool repository:

https://github.com/ati1993de/vargamesh-ckpool

Public Stratum endpoint:

```text
vargacoin.com:3933
```

Mining infrastructure includes or supports:

* SHA-256d mining
* VMESH block-template handling
* public Stratum connectivity
* mining statistics
* mining-address lookup
* AuxPoW infrastructure
* CkPool integration

AuxPoW allows valid work from a compatible parent-chain mining process to
also contribute toward VargaMesh block production when the participating
mining software supports the required merged-mining protocol.

Consensus validation of AuxPoW blocks is performed by VargaMesh Core.

---

# Public API

VargaMesh provides a purpose-built public HTTPS API.

Base URL:

```text
https://vargacoin.com/api/v1
```

The API is intended for public applications including:

* Android clients
* browser applications
* explorers
* monitoring services
* external integrations

The Android wallet uses read-only API endpoints for balance, address and
UTXO information.

Signed transactions can be submitted through:

```http
POST /api/v1/broadcast
```

Only already signed raw transactions are submitted.

The public HTTPS API is **not VargaMesh Core RPC**.

Applications should use the public API instead of exposing Core RPC to the
Internet.

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

- Testnet portal: https://testnet.vargacoin.com/
- Explorer: https://testnet.vargacoin.com/explorer.html
- WebWallet: https://testnet.vargacoin.com/wallet.html
- Public API: https://testnet.vargacoin.com/api/v1/
- SOLO Stratum: `testnet.vargacoin.com:4933`

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

VargaMesh Desktop `v0.4.0`, VargaMesh Android `v0.3.0` and the current
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

| Capability                              | Desktop v0.4.0 | WebWallet | Android v0.3.0 |
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

## Linux

Build documentation:

[doc/build-vargamesh-linux.md](doc/build-vargamesh-linux.md)

Typical source checkout:

```bash
git clone https://github.com/ati1993de/vargamesh-core.git
cd vargamesh-core
```

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

| Component                  | Status                            |
| -------------------------- | --------------------------------- |
| VargaMesh Core `v0.2.0`    | 🟢 Mainnet + Testnet release   |
| VargaMesh Desktop `v0.4.0` | 🟠 Public testing                 |
| VargaMesh Android `v0.3.0` | 🟠 Public testing / pre-release   |
| VargaMesh WebWallet        | 🟢 Available                      |
| Public Explorer            | 🟢 Available                      |
| Public API                 | 🟢 Available                      |
| Public network statistics  | 🟢 Available                      |
| Mining / CkPool            | 🟢 Available / active development |
| VargaProof                 | 🟢 Available                      |
| VargaMesh Mempool Explorer | 🔵 In development                 |

Interfaces, APIs and wallet functionality may evolve.

Consensus-critical modifications require particular care and must be
implemented and reviewed at the VargaMesh Core layer.

---

# Roadmap Direction

Current development directions include:

### Core

* continued VargaMesh Core testing
* consensus hardening
* interoperability testing
* broader node testing
* easier node deployment
* expanded documentation

### Desktop

* continued Recovery Wallet testing
* improved wallet UX
* broader Desktop/WebWallet interoperability validation
* multi-account and advanced wallet functionality
* continued WIF/import and descriptor compatibility testing

### Android

* broader physical-device testing for `v0.3.0`
* continued 12/24-word Recovery Wallet testing
* Android/Desktop/WebWallet deterministic recovery interoperability validation
* encrypted HD JSON backup/restore testing
* legacy `v0.2.x` WIF/backup migration testing
* transaction compatibility validation
* wallet UX improvements
* crash and edge-case testing
* expanded QR payment handling
* future multi-address HD and gap-limit scanning evaluation

### Wallet Standards

Current and future wallet-standard work includes:

* continued validation of the documented VMESH BIP39/HD derivation standard
* upstream SLIP-0044 registration request for proposed Mainnet coin type `22093`
* WebWallet and Android gap-limit scanning / multi-address HD support
* continued Android/Desktop/WebWallet HD Recovery interoperability testing
* multi-account wallet support
* coin control
* PSBT support
* hardware-wallet integration
* light-client/header verification

### Explorer

* VMESH-specific mempool indexing
* richer transaction information
* additional fee data
* improved address information
* broader network visualization

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

* additional independent nodes
* wider geographic node distribution
* improved mining tooling
* broader AuxPoW testing
* improved monitoring

> [!NOTE]
> Roadmap items describe development direction and should not be interpreted
> as guaranteed release dates.

---

# External Listings

VargaMesh may also be listed by independent third-party data platforms.

Current verified Blockspot listing:

https://blockspot.io/coin/vargamesh-vmesh/

The Blockspot verified status indicates that project listing information has
been validated/confirmed through their listing process.

It should not be interpreted as an audit, financial recommendation or
endorsement of VMESH.

---

# Project Links

| Resource            | URL                                                  |
| ------------------- | ---------------------------------------------------- |
| Official website    | https://vargacoin.com                                |
| Project portal      | https://mesh.vargatech.net                           |
| Core repository     | https://github.com/ati1993de/vargamesh-core          |
| Core releases       | https://github.com/ati1993de/vargamesh-core/releases |
| Desktop repository  | https://github.com/ati1993de/vargamesh-desktop       |
| Android information | https://vargacoin.com/android.html                   |
| Android APK         | https://vargacoin.com/vmesh.apk                      |
| Android APK SHA256  | https://vargacoin.com/vmesh.apk.sha256               |
| WebWallet           | https://vargacoin.com/wallet.html                    |
| Explorer            | https://vargacoin.com/explorer.html                  |
| Mempool Explorer    | https://mempool.vargacoin.com                        |
| Network statistics  | https://vargacoin.com/network.html                   |
| VargaProof          | https://vargacoin.com/proof.html                     |
| Public API          | https://vargacoin.com/api/v1                         |
| CkPool              | https://github.com/ati1993de/vargamesh-ckpool        |
| Roadmap             | https://vargacoin.com/roadmap.html                   |
| Blockspot           | https://blockspot.io/coin/vargamesh-vmesh/           |
| Discord             | https://discord.gg/AeC8sxt2aB                        |

---

# Contributing

Contributions, testing and technical feedback are welcome.

Useful contributions include:

* reproducible bug reports
* code review
* build testing
* Linux build testing
* Windows testing
* Android device testing
* wallet interoperability and deterministic recovery testing
* documentation improvements
* node testing
* mining testing
* AuxPoW testing
* explorer/indexer testing
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
* use small amounts with pre-release software
* keep operating systems updated
* protect private keys
* protect Recovery Phrases / seed material
* protect RPC credentials
* never expose Core RPC publicly
* verify destination addresses before sending
* independently confirm important transactions in the blockchain

If a security issue could put users or funds at risk, avoid publishing
private-key material or exploit details together with live wallet data.

---

# Disclaimer

VargaMesh Core and the surrounding VargaMesh ecosystem are under active
development.

Pre-release and public-testing versions may contain bugs or incomplete
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
* evaluating the risks of pre-release software
* complying with applicable laws and regulations

The **VargaMesh Core source code is the authoritative reference for
consensus-critical VargaMesh network behavior**.

---

<p align="center">
  <strong>VargaMesh · Independent infrastructure. Open participation. Self-custody.</strong>
</p>

<p align="center">
  <a href="https://vargacoin.com">vargacoin.com</a>
  ·
  <a href="https://github.com/ati1993de/vargamesh-core">GitHub</a>
  ·
  <a href="https://discord.gg/AeC8sxt2aB">Discord</a>
</p>

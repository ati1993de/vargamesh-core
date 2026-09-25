# VargaMesh Core

<p align="center">
  <strong>Reference full-node implementation for the VargaMesh (VMESH) network.</strong>
</p>

<p align="center">
  Independent Proof-of-Work · SHA-256d · AuxPoW · ASERT · Full Node · Desktop · Android · WebWallet · Explorer · Mining · Mainnet
</p>

<p align="center">
  <img alt="Core" src="https://img.shields.io/badge/Core-v0.1.1-2ea44f">
  <img alt="Desktop" src="https://img.shields.io/badge/Desktop-v0.1.1-0078d4">
  <img alt="Android" src="https://img.shields.io/badge/Android-v0.2.0-3ddc84">
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
> VargaMesh Core `v0.1.1`, VargaMesh Desktop `v0.1.1` and VargaMesh Android
> `v0.2.0` should currently be treated as early/public-testing software.
>
> Use small amounts while testing wallet software and always maintain verified
> backups of private keys and wallet data.

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
| VargaMesh Core             |    `v0.1.1` | Mainnet release                 |
| VargaMesh Desktop          |    `v0.1.1` | Public testing                 |
| VargaMesh Android          |    `v0.2.0` | Public testing / pre-release   |
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
| Core version                 | `v0.1.1`                                             |
| Website                      | https://vargacoin.com                                |
| Project portal               | https://mesh.vargatech.net                           |
| Core source code             | https://github.com/ati1993de/vargamesh-core          |
| Core releases                | https://github.com/ati1993de/vargamesh-core/releases |
| Desktop wallet               | https://github.com/ati1993de/vargamesh-desktop       |
| Android information          | https://vargacoin.com/android.html                   |
| Android APK                  | https://vargacoin.com/vmesh.apk                      |
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

**VargaMesh Core v0.1.1**

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

**VargaMesh Desktop v0.1.1**

Status:

**Public testing**

The current Desktop release bundles:

**VargaMesh Core v0.1.0**

### Available packages

* Windows x64 installer
* Windows x64 portable ZIP
* SHA256 checksums

### Current functionality

* automatic VargaMesh Core startup
* full-node blockchain synchronization
* peer status
* node status
* wallet creation
* wallet loading
* wallet unloading
* wallet encryption
* temporary wallet unlocking
* VMESH receiving addresses
* transaction history
* VMESH transfers
* wallet backup
* wallet restore
* legacy wallet migration support
* English UI
* German UI

> [!WARNING]
> The Desktop wallet is currently being tested by the community.
>
> Before using meaningful amounts, always create and verify a wallet backup.

Individual private-key import through WIF is not yet available in the
current Desktop release.

---

# VargaMesh Android

**VargaMesh Android** is the native Android application for the
VargaMesh (VMESH) mainnet.

Current release:

**VargaMesh Android v0.2.0**

Status:

**Public testing / pre-release**

Official information:

https://vargacoin.com/android.html

Direct APK:

https://vargacoin.com/vmesh.apk

VargaMesh Android extends the original read-only network companion with a
native **self-custody VMESH wallet** while retaining explorer, mining and
network-monitoring functionality.

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

Version `v0.2.0` introduces a native self-custody VMESH wallet.

### Wallet functionality

* create a new local VMESH wallet
* import compressed VMESH WIF
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
* WebWallet-compatible backup format
* wallet password change
* optional biometric/device unlock
* WIF reveal/export with security warning
* automatic clipboard clearing
* 10-minute in-memory auto-lock
* screenshot protection on wallet-sensitive screens
* local wallet removal

---

## Android Security Model

VargaMesh Android is **non-custodial**.

Private keys are generated or imported locally.

```text
Private key
    │
    │ generated/imported locally
    ▼
Password-encrypted Android storage
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

The Android application does **not** send any of the following to the
VargaMesh server:

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

---

## Android Wallet Parameters

The Android wallet intentionally follows the current VargaMesh WebWallet
single-key model.

| Parameter            | Value              |
| -------------------- | ------------------ |
| Elliptic curve       | `secp256k1`        |
| Primary address      | SegWit v0 P2WPKH   |
| Bech32 HRP           | `vm`               |
| WIF version          | `190 / 0xBE`       |
| Legacy P2PKH version | `70`               |
| Legacy P2SH version  | `50`               |
| Decimals             | `8`                |
| Default fee          | `1 sat/vB`         |
| Backup KDF           | PBKDF2-HMAC-SHA256 |
| KDF iterations       | `250,000`          |
| Backup cipher        | AES-256-GCM        |

Signing uses:

* deterministic RFC6979 ECDSA
* low-S normalization
* BIP143-style SegWit v0 signing

Encrypted Android wallet backups are compatible with the current
VargaMesh WebWallet backup format.

---

## Android Limitations

VargaMesh Android `v0.2.0` currently does **not** implement:

* BIP39 seed phrases
* HD wallet derivation
* multi-account wallets
* coin control
* PSBT
* hardware-wallet support
* light-client/header verification

This is deliberate.

The current VargaMesh WebWallet uses a single private key/WIF model and a
VMESH-specific HD derivation standard has not yet been formally defined.

The Android client therefore does not independently introduce a derivation
scheme that could later become incompatible with other VargaMesh clients.

A documented VMESH HD/BIP39 standard may be introduced in a future wallet
specification.

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
> VargaMesh Android `v0.2.0` can create, hold and spend real VMESH.
>
> It is currently **public-testing / pre-release software**.

Recommended testing procedure:

1. create a test wallet
2. export an encrypted backup
3. verify that the backup can be restored
4. receive a small VMESH amount
5. send a small VMESH transaction
6. confirm the transaction in the explorer
7. verify WIF compatibility in a separate trusted environment

Do not use significant amounts until the Android wallet has received
sufficient real-device and interoperability testing.

Never share:

* private keys
* WIFs
* wallet passwords
* decrypted wallet backups

---

# VargaMesh WebWallet

The VargaMesh ecosystem includes a browser-based self-custody wallet.

WebWallet:

https://vargacoin.com/wallet.html

The WebWallet provides a lightweight alternative for users who do not want
to operate a local full node.

The current wallet model uses a locally controlled private key/WIF.

The WebWallet and Android wallet intentionally use compatible encrypted
backup formats.

### Wallet access models

| Client              | Model                           | Platform      |
| ------------------- | ------------------------------- | ------------- |
| VargaMesh Desktop   | Full-node wallet                | Windows       |
| VargaMesh Android   | Native self-custody wallet      | Android       |
| VargaMesh WebWallet | Browser/PWA self-custody wallet | Browser / PWA |

The browser wallet should not be confused with VargaMesh Desktop.

VargaMesh Desktop runs and synchronizes VargaMesh Core locally.

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

| Parameter            | Value            |
| -------------------- | ---------------- |
| Elliptic curve       | `secp256k1`      |
| Primary address type | SegWit v0 P2WPKH |
| Bech32 HRP           | `vm`             |
| WIF version          | `190 / 0xBE`     |
| Legacy P2PKH version | `70`             |
| Legacy P2SH version  | `50`             |
| Currency decimals    | `8`              |

Client applications should follow documented VargaMesh wallet standards.

Independent clients should not introduce incompatible address or key
derivation schemes without first defining and documenting the relevant
VargaMesh standard.

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
| VargaMesh Core `v0.1.1`    | 🟢 Mainnet release             |
| VargaMesh Desktop `v0.1.1` | 🟠 Public testing                 |
| VargaMesh Android `v0.2.0` | 🟠 Public testing / pre-release   |
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

* continued wallet testing
* improved wallet UX
* interoperability validation
* additional wallet functionality
* possible WIF import support

### Android

* broader physical-device testing
* backup interoperability testing
* transaction compatibility validation
* wallet UX improvements
* crash and edge-case testing
* expanded QR payment handling

### Wallet Standards

Potential future work includes:

* formal VMESH BIP39/HD derivation specification
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
* wallet interoperability testing
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

* verify release checksums
* use official download sources
* keep backups
* test backups
* use small amounts with pre-release software
* keep operating systems updated
* protect private keys
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

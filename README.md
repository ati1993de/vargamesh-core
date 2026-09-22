# VargaMesh Core

<p align="center">
  <strong>Reference full-node implementation for the VargaMesh (VMESH) network.</strong>
</p>

<p align="center">
  Independent Proof-of-Work · AuxPoW · ASERT · Full Node · Wallet · Mainnet
</p>

---

## Overview

**VargaMesh Core** is the reference full-node implementation for the
**VargaMesh (VMESH)** blockchain.

VargaMesh is an independent Proof-of-Work network derived from Bitcoin Core
and extended with VargaMesh-specific consensus and network parameters,
including:

- independent mainnet identity
- VMESH native currency
- 120-second target block spacing
- ASERT difficulty adjustment
- AuxPoW merged-mining support
- VargaMesh-specific network ports and address formats

The VargaMesh Core source tree is authoritative for consensus behavior.

| Project | Information |
| --- | --- |
| Network | VargaMesh Mainnet |
| Currency | `VMESH` |
| Core version | `v0.1.0` |
| Website | https://vargacoin.com |
| Source code | https://github.com/ati1993de/vargamesh-core |
| Releases | https://github.com/ati1993de/vargamesh-core/releases |
| Desktop wallet | https://github.com/ati1993de/vargamesh-desktop |
| Explorer development | https://mempool.vargacoin.com |
| CkPool |https://github.com/ati1993de/vargamesh-ckpool |

---

## VargaMesh Ecosystem

### VargaMesh Desktop

**VargaMesh Desktop** is the graphical Windows full-node wallet for users
who prefer a desktop interface instead of operating VargaMesh Core entirely
through the command line.

Repository:

https://github.com/ati1993de/vargamesh-desktop

Current desktop release:

**VargaMesh Desktop v0.1.1**

The current Desktop release bundles:

**VargaMesh Core v0.1.0**

Available Windows packages include:

- Windows x64 installer
- Windows x64 portable ZIP
- SHA256 checksums

Current functionality includes:

- automatic VargaMesh Core startup
- full-node blockchain synchronization
- peer and node status
- wallet creation
- wallet loading and unloading
- wallet encryption and temporary unlocking
- VMESH receiving addresses
- transaction history
- VMESH transfers
- wallet backup
- wallet restore
- legacy wallet migration support
- English and German interface

The Desktop wallet is currently being tested by the community.

Before using significant funds, always create and verify a wallet backup.

Individual private-key import through WIF is not yet available in the
current Desktop release.

---

### VargaMesh Mempool Explorer

A VargaMesh-specific blockchain explorer and mempool stack is under active
development.

Development endpoint:

https://mempool.vargacoin.com

The project is intended to provide public visibility into VargaMesh network
activity including blocks, transactions, mempool activity and network data.

The VMESH-specific indexing backend is still under development and testing.

Explorer data should therefore not yet be treated as authoritative until
the indexing stack is explicitly marked production-ready.

---

## Mainnet Parameters

| Parameter | Value |
| --- | --- |
| Network | VargaMesh Mainnet |
| Currency | `VMESH` |
| P2P port | `29666` |
| RPC port | `29667` |
| Genesis hash | `00000000b0c55c00c13e2ee67b54fe33c327c8806f8da5050cfa47095d182d9a` |
| Genesis merkle root | `ab8120503c6bc408d07a5257f0dce8019ec494dbf627bd674c93d4f86122cb36` |
| Genesis timestamp | `1789855200` |
| Genesis nonce | `1174766078` |
| Genesis bits | `1d00ffff` |
| AuxPoW chain ID | `22093` (`0x564D`) |
| AuxPoW activation | Block `1` |
| Target block spacing | `120 seconds` |
| Difficulty adjustment | ASERT |
| ASERT half-life | `34560 seconds` |
| Coinbase maturity | `100 blocks` |
| Initial block subsidy | `50 VMESH` |
| Subsidy halving interval | `1,051,200 blocks` |
| Bech32 HRP | `vm` |
| Protocol version | `70016` |

The source code remains authoritative for all consensus-critical values.

Additional mainnet information:

[doc/mainnet.md](doc/mainnet.md)

---

## Software Components

A full VargaMesh Core build may produce the following binaries:

| Binary | Purpose |
| --- | --- |
| `vargameshd` | Full-node daemon |
| `vargamesh-cli` | JSON-RPC command-line client |
| `vargamesh-qt` | Native graphical Core client |
| `vargamesh-wallet` | Offline wallet utility |
| `vargamesh-tx` | Raw transaction utility |
| `vargamesh-util` | Utility commands |

Availability may depend on the selected build configuration and platform.

---

## Building VargaMesh Core

### Linux

See:

[doc/build-vargamesh-linux.md](doc/build-vargamesh-linux.md)

Typical workflow:

```bash
git clone https://github.com/ati1993de/vargamesh-core.git
cd vargamesh-core

# VargaMesh Core v0.2.0

VargaMesh Core `v0.2.0` is the current official **Mainnet + Public Testnet v1** Core release for the VargaMesh (VMESH) network.

This release provides the VargaMesh full-node daemon, command-line RPC client and wallet/transaction utilities for Linux, Windows and macOS.

> **Important:** The macOS Core archives are command-line / daemon packages.  
> They are separate from the graphical **VargaMesh Desktop** application.

---

## Supported platforms

Official binary packages are available for:

- **Linux x86_64**
- **Windows x86_64**
- **macOS Intel x86_64**
- **macOS Apple Silicon arm64**

### Release files

```text
vargamesh-core-0.2.0-linux-x86_64.tar.gz
vargamesh-core-0.2.0-windows-x86_64.zip
vargamesh-core-0.2.0-macos-x86_64.tar.gz
vargamesh-core-0.2.0-macos-arm64.tar.gz

SHA256SUMS
SHA256SUMS-macOS
```

Always verify the published SHA-256 checksums before replacing existing node binaries.

---

## Included VargaMesh Core programs

The official packages include:

- `vargameshd` — VargaMesh full-node daemon
- `vargamesh-cli` — JSON-RPC command-line client
- `vargamesh-wallet` — wallet utility
- `vargamesh-tx` — raw transaction utility
- `vargamesh-util` — general Core utility

On Windows, the programs use the `.exe` suffix.

---

## Mainnet

VargaMesh Mainnet is the default network.

### Start the node

Linux / macOS:

```bash
./bin/vargameshd -daemonwait
```

Windows:

```powershell
.\bin\vargameshd.exe -daemonwait
```

### Query blockchain information

Linux / macOS:

```bash
./bin/vargamesh-cli getblockchaininfo
```

Windows:

```powershell
.\bin\vargamesh-cli.exe getblockchaininfo
```

### Stop the node cleanly

Linux / macOS:

```bash
./bin/vargamesh-cli stop
```

Windows:

```powershell
.\bin\vargamesh-cli.exe stop
```

---

## VargaMesh Mainnet parameters

| Parameter | Value |
| --- | --- |
| Network | VargaMesh Mainnet |
| Currency | `VMESH` |
| Consensus | Proof of Work |
| PoW algorithm | SHA-256d |
| Target block spacing | `120 seconds` |
| Difficulty adjustment | ASERT |
| ASERT half-life | `34,560 seconds` |
| Coinbase maturity | `100 blocks` |
| Initial block subsidy | `25 VMESH` |
| Subsidy halving interval | `1,051,200 blocks` |
| Maximum supply | `52,560,000 VMESH` |
| P2P port | `29666` |
| RPC port | `29667` |
| AuxPoW chain ID | `22093` / `0x564D` |
| AuxPoW activation | Block `1` |
| Bech32 HRP | `vm` |
| Protocol version | `70016` |

### Mainnet genesis

```text
Genesis hash:
00000000b0c55c00c13e2ee67b54fe33c327c8806f8da5050cfa47095d182d9a

Genesis merkle root:
ab8120503c6bc408d07a5257f0dce8019ec494dbf627bd674c93d4f86122cb36
```

The VargaMesh Core source code remains authoritative for all consensus-critical values.

---

## VargaMesh Public Testnet v1

The same `v0.2.0` Core binaries also include the official **VargaMesh Public Testnet v1**.

> **tVMESH is a Testnet asset only and has no monetary value.**

### Start Testnet

Linux / macOS:

```bash
./bin/vargameshd -testnet -daemonwait
```

Windows:

```powershell
.\bin\vargameshd.exe -testnet -daemonwait
```

### Query Testnet

Linux / macOS:

```bash
./bin/vargamesh-cli -testnet getblockchaininfo
```

Windows:

```powershell
.\bin\vargamesh-cli.exe -testnet getblockchaininfo
```

### Stop Testnet

Linux / macOS:

```bash
./bin/vargamesh-cli -testnet stop
```

Windows:

```powershell
.\bin\vargamesh-cli.exe -testnet stop
```

### Public Testnet parameters

| Parameter | Value |
| --- | --- |
| Network | VargaMesh Public Testnet v1 |
| Currency | `tVMESH` |
| Proof of Work | SHA-256d |
| Target block spacing | `120 seconds` |
| Coinbase maturity | `100 blocks` |
| P2P port | `39666` |
| RPC port | `39667` |
| AuxPoW chain ID | `22094` / `0x564E` |
| AuxPoW activation | Height `1` |
| Bech32 HRP | `tvm` |

Testnet genesis hash:

```text
00000000747ff112e44641470bb636ea00e43ed3cef5e1e94c451d9cd02efea9
```

---

## macOS support

VargaMesh Core `v0.2.0` is officially available for both current macOS CPU families:

### Intel Mac

```text
vargamesh-core-0.2.0-macos-x86_64.tar.gz
```

### Apple Silicon

For Apple M-series systems:

```text
vargamesh-core-0.2.0-macos-arm64.tar.gz
```

You can check the local Mac architecture with:

```bash
uname -m
```

Expected result:

```text
x86_64
```

for Intel, or:

```text
arm64
```

for Apple Silicon.

The macOS archives contain:

```text
vargameshd
vargamesh-cli
vargamesh-wallet
vargamesh-tx
vargamesh-util
```

They do **not** contain the graphical VargaMesh Desktop wallet.

The graphical macOS VargaMesh Desktop application is developed and released separately through:

https://github.com/ati1993de/vargamesh-desktop

---

## macOS verification

The official macOS packages are built through the repository's pinned `depends` build system for:

```text
x86_64-apple-darwin
arm64-apple-darwin
```

The resulting binaries are then executed on native GitHub macOS runners for their corresponding architecture.

Before publication, both architectures are checked for:

- valid Mach-O binaries
- successful execution of all packaged Core programs
- Mainnet daemon startup
- Mainnet RPC operation
- valid Mainnet `vm1...` wallet-address generation
- Testnet daemon startup
- Testnet RPC operation
- valid Testnet `tvm1...` wallet-address generation
- clean daemon shutdown
- SHA-256 archive verification

The macOS release assets were built from the **existing unchanged `v0.2.0` tag commit**.

Adding the macOS packages did **not** create a new Core version and did **not** change VargaMesh consensus rules.

---

## Linux verification

The Linux release is built and tested before publication with checks including:

- Linux x86_64 Core build
- Mainnet daemon/RPC smoke test
- Public Testnet v1 daemon/RPC smoke test
- Mainnet `vm1...` wallet-address validation
- Testnet `tvm1...` wallet-address validation
- packaged runtime verification
- SHA-256 archive checksum

---

## Windows verification

The Windows x86_64 package is cross-compiled through the repository's pinned `depends` system with the MinGW-w64 toolchain.

Before publication, the resulting Windows package is executed on a Windows GitHub runner.

Verification includes:

- execution of `vargameshd.exe`
- execution of `vargamesh-cli.exe`
- execution of `vargamesh-wallet.exe`
- execution of `vargamesh-tx.exe`
- execution of `vargamesh-util.exe`
- Mainnet daemon/RPC smoke test
- Public Testnet v1 daemon/RPC smoke test
- Mainnet `vm1...` wallet-address validation
- Testnet `tvm1...` wallet-address validation
- SHA-256 archive checksum

---

## Wallet and address parameters

Current VargaMesh Mainnet wallet/address parameters include:

| Parameter | Value |
| --- | --- |
| Elliptic curve | `secp256k1` |
| Primary address type | SegWit v0 P2WPKH |
| Bech32 HRP | `vm` |
| WIF version | `190 / 0xBE` |
| Legacy P2PKH version | `70` |
| Legacy P2SH version | `50` |
| Currency decimals | `8` |
| Recovery Phrase | BIP39, `12` or `24` words |
| HD derivation | BIP32 |
| Proposed Mainnet SLIP-0044 type | `22093` |
| BIP84 receive path | `m/84'/22093'/0'/0/index` |
| BIP84 change path | `m/84'/22093'/0'/1/index` |
| BIP44 receive path | `m/44'/22093'/0'/0/index` |
| BIP44 change path | `m/44'/22093'/0'/1/index` |

The Mainnet value `22093` is currently the **proposed** VargaMesh SLIP-0044 coin type and must not be described as an official upstream SLIP-0044 assignment unless accepted by the upstream registry.

---

## Data directories

Typical VargaMesh Core data directories:

### Linux

```text
~/.vargamesh
```

### macOS

```text
~/Library/Application Support/VargaMesh
```

### Windows

```text
%LOCALAPPDATA%\VargaMesh
```

Keep Mainnet and Testnet wallet/data usage clearly separated.

---

## SHA-256 verification

### macOS

Download `SHA256SUMS-macOS` into the same directory as the macOS archives:

```bash
shasum -a 256 -c SHA256SUMS-macOS
```

### Linux

Use the published `SHA256SUMS` file to verify the Linux archive.

### Windows

Use the published `SHA256SUMS` file to verify the Windows archive.

Always obtain release files and checksum files from the official VargaMesh Core GitHub release.

---

## Security

VargaMesh Core is self-custody software.

Users are responsible for protecting:

- private keys
- WIF private keys
- Recovery Phrases
- wallet passwords
- wallet backups
- RPC credentials

> **Never expose unauthenticated VargaMesh Core RPC directly to the public Internet.**

Recommended RPC practices:

- bind RPC only to trusted interfaces
- use strong authentication
- restrict access with firewall rules
- never publish RPC credentials
- do not expose wallet RPC publicly
- use purpose-built public APIs for external applications

Always maintain verified wallet backups before upgrading or replacing node software.

---

## Upgrade notes

Before replacing an existing VargaMesh Core installation:

1. stop the running node cleanly
2. back up wallet data
3. verify the downloaded release checksum
4. replace only the intended binaries
5. start the node
6. verify `getblockchaininfo`
7. verify `getnetworkinfo`
8. verify wallet availability before sending funds

Example:

```bash
vargamesh-cli stop
```

After restart:

```bash
vargamesh-cli getblockchaininfo
vargamesh-cli getnetworkinfo
```

---

## Documentation

VargaMesh Core repository:

https://github.com/ati1993de/vargamesh-core

Release:

https://github.com/ati1993de/vargamesh-core/releases/tag/v0.2.0

Main project website:

https://vargamesh.com

Project portal:

https://mesh.vargatech.net

Public Testnet:

https://testnet.vargamesh.com/

Network information:

https://vargamesh.com/network.html

Explorer:

https://vargamesh.com/explorer.html

VargaMesh Desktop:

https://github.com/ati1993de/vargamesh-desktop

Discord:

https://discord.gg/AeC8sxt2aB

---

## Disclaimer

VargaMesh Core and the surrounding VargaMesh ecosystem are under active development.

Software under active development may contain bugs, regressions or incomplete functionality.

Nothing in this release constitutes:

- investment advice
- financial advice
- legal advice
- tax advice
- a promise of future value
- a guarantee of software availability

Users remain responsible for securing private keys, maintaining wallet backups, validating software before use and complying with applicable laws and regulations.

The **VargaMesh Core source code is the authoritative reference for consensus-critical VargaMesh network behavior**.

---

**VargaMesh · Independent infrastructure. Open participation. Self-custody.**

# VargaMesh Core v0.2.0

VargaMesh Core v0.2.0 is the first official binary release that includes
VargaMesh Mainnet and VargaMesh Public Testnet v1 in the same Core binaries.

## Supported platforms

Official binary packages:

- Linux x86_64
- Windows x86_64
- macOS Intel x86_64
- macOS Apple Silicon arm64

The Windows package is cross-compiled with the MinGW-w64 toolchain through the
repository's pinned `depends` system and is then executed on a Windows runner
before publication.

The macOS packages are built through the repository's pinned `depends` system
for `x86_64-apple-darwin` and `arm64-apple-darwin`. Each archive is executed
on a native GitHub macOS runner for its architecture before it is attached to
the release.

The macOS archives were added after the original v0.2.0 Linux/Windows
publication and are built from the exact existing `v0.2.0` tag commit. This
does not introduce a new Core or consensus version.

## Included Core programs

- `vargameshd`
- `vargamesh-cli`
- `vargamesh-wallet`
- `vargamesh-tx`
- `vargamesh-util`

On Windows the programs use the `.exe` suffix.

## Mainnet

Mainnet remains the default network.

Start:

    vargameshd

Query:

    vargamesh-cli getblockchaininfo

## VargaMesh Public Testnet v1

The same binaries also contain the official VargaMesh Public Testnet v1.

Start:

    vargameshd -testnet

Query:

    vargamesh-cli -testnet getblockchaininfo

Important Testnet properties:

- Asset: `tVMESH`
- Proof-of-Work: SHA-256d
- P2P port: `39666`
- RPC port: `39667`
- AuxPoW chain ID: `22094` / `0x564E`
- Bech32 HRP: `tvm`
- AuxPoW activation: height 1

`tVMESH` is a Testnet asset only and has no monetary value.

Full Testnet documentation:

https://github.com/ati1993de/vargamesh-core/blob/main/doc/testnet.md

Exchange and integration documentation:

https://github.com/ati1993de/vargamesh-core/blob/main/doc/exchange-integration.md

## Verification performed before publication

The original v0.2.0 release creation required:

1. Linux x86_64 build.
2. Linux Mainnet daemon/RPC smoke test.
3. Linux VargaMesh Public Testnet v1 daemon/RPC smoke test.
4. Linux Mainnet `vm1...` wallet-address check.
5. Linux Testnet `tvm1...` wallet-address check.
6. Windows x86_64 MinGW-w64 cross-build.
7. Execution of all packaged Windows command-line programs on Windows.
8. Windows Mainnet daemon/RPC smoke test.
9. Windows VargaMesh Public Testnet v1 daemon/RPC smoke test.
10. Windows Mainnet `vm1...` wallet-address check.
11. Windows Testnet `tvm1...` wallet-address check.
12. SHA-256 checksums for the Linux and Windows release archives.

The later macOS publication additionally requires, for both Intel x86_64 and
Apple Silicon arm64:

1. Reproducible macOS cross-build from the exact v0.2.0 tag commit.
2. Mach-O package validation.
3. Execution of `vargameshd`, `vargamesh-cli`, `vargamesh-wallet`,
   `vargamesh-tx` and `vargamesh-util` on a native runner.
4. Mainnet daemon/RPC smoke test.
5. Mainnet `vm1...` wallet-address check.
6. VargaMesh Public Testnet v1 daemon/RPC smoke test.
7. Testnet `tvm1...` wallet-address check.
8. SHA-256 checksums published in `SHA256SUMS-macOS`.

The existing `v0.2.0` tag is not moved when the macOS assets are attached.

## Security

Keep Mainnet and Testnet data directories and wallets separate.

Do not expose unauthenticated Core RPC directly to the public Internet.

# VargaMesh Core Documentation

VargaMesh Core is the reference full-node implementation for the VargaMesh
(VMESH) network.

The repository root [README](/README.md) contains the current project overview,
network parameters, release status and ecosystem links.

For the latest Desktop/Android client releases, downloads, validation status
and wallet security boundaries, see [Client release status](client-release-status.md).
Client version numbers are independent of the Core release number.

## Official Core release

Current Core release: **v0.2.0**

Official binary packages are published for:

- Linux x86_64
- Windows x86_64
- macOS Intel x86_64
- macOS Apple Silicon arm64

Release downloads:

https://github.com/ati1993de/vargamesh-core/releases/tag/v0.2.0

The macOS v0.2.0 archives contain the Core daemon and command-line utilities.
They are not the graphical VargaMesh Desktop application.

Published macOS archives:

- `vargamesh-core-0.2.0-macos-x86_64.tar.gz`
- `vargamesh-core-0.2.0-macos-arm64.tar.gz`
- `SHA256SUMS-macOS`

Always verify release checksums before replacing existing node binaries.

## Running VargaMesh Core

The primary Core programs are:

- `vargameshd` — full-node daemon
- `vargamesh-cli` — JSON-RPC command-line client
- `vargamesh-wallet` — wallet utility
- `vargamesh-tx` — raw transaction utility
- `vargamesh-util` — general utility commands
- `vargamesh-qt` — graphical Core client when built with GUI support

Mainnet is the default network.

```bash
vargameshd
vargamesh-cli getblockchaininfo
```

VargaMesh Public Testnet v1:

```bash
vargameshd -testnet
vargamesh-cli -testnet getblockchaininfo
```

On macOS the default data directory is:

```text
~/Library/Application Support/VargaMesh
```

## VargaMesh build guides

Use the VargaMesh-specific build notes first:

- [Linux](build-vargamesh-linux.md)
- [Windows](build-vargamesh-windows.md)
- [macOS Intel / Apple Silicon](build-vargamesh-macos.md)

The repository also retains upstream-derived platform documentation for
low-level build details:

- [Dependencies](dependencies.md)
- [macOS upstream-derived build notes](build-osx.md)
- [Unix upstream-derived build notes](build-unix.md)
- [Windows MSVC upstream-derived build notes](build-windows-msvc.md)
- [FreeBSD](build-freebsd.md)
- [OpenBSD](build-openbsd.md)
- [NetBSD](build-netbsd.md)

Do not assume upstream Bitcoin Core names, paths or defaults are identical to
VargaMesh. VargaMesh-specific executable names, data directories, network
parameters and release workflows take precedence.

## VargaMesh-specific documentation

- [Public Testnet v1](testnet.md)
- [Exchange integration](exchange-integration.md)
- [Release notes v0.2.0](release-notes/release-notes-v0.2.0.md)
- [Files and data directories](files.md)
- [Managing wallets](managing-wallets.md)
- [JSON-RPC interface](JSON-RPC-interface.md)
- [REST interface](REST-interface.md)
- [ZMQ](zmq.md)
- [Tor](tor.md)
- [I2P](i2p.md)
- [CJDNS](cjdns.md)

## Development documentation

- [Developer notes](developer-notes.md)
- [Productivity notes](productivity.md)
- [Release process](release-process.md)
- [Translation process](translation_process.md)
- [Translation strings policy](translation_strings_policy.md)
- [Benchmarking](benchmarking.md)
- [Fuzz testing](fuzzing.md)
- [Internal design documentation](design/)

## Support and security

For VargaMesh project links and community channels, see the repository root
[README](/README.md).

Never publish private keys, WIFs, wallet passwords, Recovery Phrases, wallet
backups or RPC credentials when requesting support.

Do not expose unauthenticated VargaMesh Core RPC directly to the public
Internet.

## License

Distributed under the [MIT software license](/COPYING).

# Building and Running VargaMesh Core on macOS

VargaMesh Core v0.2.0 has official command-line binary packages for both
supported macOS CPU families:

- Intel: `x86_64`
- Apple Silicon: `arm64`

These Core archives contain the daemon and command-line utilities. They are not
the graphical VargaMesh Desktop application.

## Recommended: use the official v0.2.0 binaries

Release page:

https://github.com/ati1993de/vargamesh-core/releases/tag/v0.2.0

Choose the package that matches:

```bash
uname -m
```

Use:

- `vargamesh-core-0.2.0-macos-x86_64.tar.gz` when the result is `x86_64`
- `vargamesh-core-0.2.0-macos-arm64.tar.gz` when the result is `arm64`

Download `SHA256SUMS-macOS` into the same directory and verify both macOS
archives with:

```bash
shasum -a 256 -c SHA256SUMS-macOS
```

Extract the matching archive, for example on Apple Silicon:

```bash
tar -xzf vargamesh-core-0.2.0-macos-arm64.tar.gz
cd vargamesh-core-0.2.0-macos-arm64
```

Verify the programs before starting the node:

```bash
./bin/vargameshd --version
./bin/vargamesh-cli --version
./bin/vargamesh-wallet --version
./bin/vargamesh-tx --version
./bin/vargamesh-util --version
```

## Start Mainnet

Mainnet is the default network.

```bash
./bin/vargameshd -daemonwait
./bin/vargamesh-cli getblockchaininfo
```

Stop cleanly with:

```bash
./bin/vargamesh-cli stop
```

The default macOS data directory is:

```text
~/Library/Application Support/VargaMesh
```

## Start VargaMesh Public Testnet v1

```bash
./bin/vargameshd -testnet -daemonwait
./bin/vargamesh-cli -testnet getblockchaininfo
```

Stop Testnet with:

```bash
./bin/vargamesh-cli -testnet stop
```

Keep Mainnet and Testnet wallets/data clearly separated and do not treat
Testnet `tVMESH` as having monetary value.

## How the official macOS binaries are verified

The repository's macOS CI/release workflow builds both:

- `x86_64-apple-darwin`
- `arm64-apple-darwin`

using the pinned `depends` system and pinned macOS SDK.

The resulting packages are then executed on native GitHub macOS runners:

- Intel runner for the x86_64 package
- Apple Silicon runner for the arm64 package

Before publication, CI executes every packaged Core program and performs
Mainnet and Testnet daemon/RPC smoke tests, including `vm1...` Mainnet and
`tvm1...` Testnet address checks.

Workflow:

```text
.github/workflows/macos-binaries.yml
```

## Native developer build on macOS

For local development, install Xcode Command Line Tools:

```bash
xcode-select --install
```

Install Homebrew and the basic build dependencies:

```bash
brew install cmake ninja boost pkgconf libevent
```

Clone the repository:

```bash
git clone https://github.com/ati1993de/vargamesh-core.git
cd vargamesh-core
```

Configure a headless Core build:

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_GUI=OFF \
  -DBUILD_BENCH=OFF \
  -DBUILD_FUZZ_BINARY=OFF \
  -DENABLE_IPC=OFF \
  -DWITH_ZMQ=OFF
```

Compile:

```bash
cmake --build build --parallel
```

Typical output:

```text
build/bin/vargameshd
build/bin/vargamesh-cli
build/bin/vargamesh-wallet
build/bin/vargamesh-tx
build/bin/vargamesh-util
```

Run tests when the selected configuration builds them:

```bash
ctest --test-dir build --output-on-failure
```

For low-level macOS dependency and GUI build details, the repository retains
the upstream-derived [macOS build notes](build-osx.md). VargaMesh-specific
names, paths and release procedures in this document take precedence.

## Release builds

Do not treat a local Homebrew build as an official release binary.

Official VargaMesh macOS release archives should be produced through the
reviewed repository workflow from an exact release commit and should pass the
native Intel/Apple-Silicon smoke tests before publication.

## RPC security

Never expose unauthenticated VargaMesh Core RPC directly to the public
Internet. Bind RPC to trusted interfaces, use authentication and firewall
rules, and keep wallet RPC private.

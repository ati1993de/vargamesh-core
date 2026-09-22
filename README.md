# VargaMesh Core

### Related VargaMesh projects

- **VargaMesh Desktop** — Windows full-node wallet GUI  
  https://github.com/ati1993de/vargamesh-desktop
- **VargaMesh Core Releases**  
  https://github.com/ati1993de/vargamesh-core/releases

**VargaMesh Core** is the reference full-node implementation for the
**VargaMesh (VMESH)** network.

VargaMesh is an independent proof-of-work blockchain derived from Bitcoin
Core and extended with VargaMesh-specific network identity, monetary
parameters, ASERT difficulty adjustment and AuxPoW merged-mining support.

- **Network:** VargaMesh Mainnet
- **Currency:** VMESH
- **Software:** VargaMesh Core v0.1.0
- **Website:** https://mesh.vargatech.net
- **Source:** https://github.com/ati1993de/vargamesh-core

## Mainnet parameters

| Parameter | Value |
| --- | --- |
| P2P port | `29666` |
| RPC port | `29667` |
| Genesis hash | `00000000b0c55c00c13e2ee67b54fe33c327c8806f8da5050cfa47095d182d9a` |
| Genesis merkle root | `ab8120503c6bc408d07a5257f0dce8019ec494dbf627bd674c93d4f86122cb36` |
| Genesis time | `1789855200` |
| Genesis nonce | `1174766078` |
| Genesis bits | `1d00ffff` |
| AuxPoW chain ID | `22093` |
| Target block spacing | `120 seconds` |
| ASERT half-life | `34560 seconds` |
| Coinbase maturity | `100 blocks` |
| Subsidy halving interval | `1051200 blocks` |
| Bech32 HRP | `vm` |

The source code remains authoritative for consensus behavior.

See [doc/mainnet.md](doc/mainnet.md) for additional information.

## Software components

A standard VargaMesh build can produce:

- `vargameshd` — full node daemon
- `vargamesh-cli` — JSON-RPC command-line client
- `vargamesh-qt` — graphical client
- `vargamesh-wallet` — offline wallet utility
- `vargamesh-tx` — transaction utility
- `vargamesh-util` — utility commands

## Build

### Linux

See [doc/build-vargamesh-linux.md](doc/build-vargamesh-linux.md).

### Windows

### Windows GUI available

Users who prefer a graphical full-node wallet can also test VargaMesh Desktop:

https://github.com/ati1993de/vargamesh-desktop

The current Desktop v0.1.1 release bundles VargaMesh Core v0.1.0.

See [doc/build-vargamesh-windows.md](doc/build-vargamesh-windows.md).

The inherited platform-specific documentation under `doc/` is retained
where it remains applicable.

## Running a node

Example:

```bash
vargameshd -daemon
vargamesh-cli getblockchaininfo
vargamesh-cli getnetworkinfo
```

Default mainnet ports:

- **29666/TCP** — VargaMesh P2P
- **29667/TCP** — JSON-RPC

RPC should normally remain bound to localhost unless an operator has
deliberately deployed a secured remote RPC configuration.

## Mining

VargaMesh Core contains AuxPoW validation and AuxPoW-capable mining-template
support.

Mining and consensus behavior are defined by the implementation and mainnet
parameters in the source tree.

## Security

Never publish or commit production:

- private keys
- wallet files
- seed phrases
- RPC authentication secrets
- authentication cookies
- SSH private keys
- production credentials

See [SECURITY.md](SECURITY.md).

## Development

VargaMesh Core is consensus-critical software. Changes affecting consensus,
serialization, proof of work, AuxPoW, monetary policy or network identity
require additional review and testing.

See:

- [CONTRIBUTING.md](CONTRIBUTING.md)
- [CHANGELOG.md](CHANGELOG.md)
- [UPSTREAM.md](UPSTREAM.md)

## Upstream and attribution

VargaMesh Core is derived from Bitcoin Core.

Upstream copyright notices, license notices, technical history and
third-party attribution are intentionally preserved.

## License

VargaMesh Core is distributed under the MIT software license.

See [COPYING](COPYING).

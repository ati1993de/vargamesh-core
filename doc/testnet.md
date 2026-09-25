# VargaMesh Public Testnet v1

VargaMesh Public Testnet v1 is the official development and integration
network for VargaMesh Core.

It is intended for wallet development, exchange integration, explorer and
indexer development, transaction testing, node development, mining software,
AuxPoW testing and RPC/API integration.

> **Important:** `tVMESH` is a Testnet asset only.
> It has no monetary value and is not mainnet VMESH.

The VargaMesh Core source code is authoritative for all consensus-critical
network parameters.

## Public infrastructure

| Resource | Endpoint |
| --- | --- |
| Testnet portal | https://testnet.vargacoin.com/ |
| Explorer | https://testnet.vargacoin.com/explorer.html |
| WebWallet | https://testnet.vargacoin.com/wallet.html |
| SOLO Stratum | `testnet.vargacoin.com:4933` |
| Public HTTPS API | https://testnet.vargacoin.com/api/v1/ |
| Core repository | https://github.com/ati1993de/vargamesh-core |

## Start a Testnet node

Build VargaMesh Core normally.

Start Testnet:

```bash
./build/bin/vargameshd -testnet
```

Query the node:

```bash
./build/bin/vargamesh-cli -testnet getblockchaininfo
./build/bin/vargamesh-cli -testnet getnetworkinfo
./build/bin/vargamesh-cli -testnet getpeerinfo
./build/bin/vargamesh-cli -testnet getmininginfo
```

Testnet uses its own network parameters and datadir namespace.

Do not use a mainnet datadir for Testnet.

## Network parameters

| Parameter | Value |
| --- | --- |
| Network | VargaMesh Public Testnet v1 |
| Currency | `tVMESH` |
| Decimals | `8` |
| Proof-of-Work | SHA-256d |
| Difficulty adjustment | ASERT |
| Target block spacing | `120 seconds` |
| ASERT half-life | `34560 seconds` |
| Initial block reward | `25 tVMESH` |
| Halving interval | `1,051,200 blocks` |
| Coinbase maturity | `100 blocks` |
| Initial difficulty | `1` |
| Initial bits | `0x1d00ffff` |
| P2P port | `39666` |
| RPC port | `39667` |
| Network magic | `5c 5b e9 af` |
| AuxPoW chain ID | `22094` / `0x564E` |
| AuxPoW activation | Height `1` |
| Bech32 HRP | `tvm` |
| Minimum-difficulty blocks | Disabled |

## Genesis block

| Parameter | Value |
| --- | --- |
| Genesis hash | `00000000747ff112e44641470bb636ea00e43ed3cef5e1e94c451d9cd02efea9` |
| Merkle root | `1de62927dbe84585c36f1f9da5fc032a503e13944a0b533669984929c357f8b4` |
| Timestamp | `1790294400` |
| Time | `2026-09-25 00:00:00 UTC` |
| Nonce | `2112996454` |
| Bits | `0x1d00ffff` |
| Reward | `25 tVMESH` |

Genesis timestamp text:

```text
VargaMesh Testnet Genesis 25/Sep/2026 - VargaTech - Test coins have no value
```

Genesis output text:

```text
VargaMesh Testnet Genesis Output - No premine
```

There is no Testnet premine.

## Address and key parameters

| Parameter | Value |
| --- | --- |
| Primary address format | SegWit v0 P2WPKH |
| Bech32 HRP | `tvm` |
| Bech32 address prefix | `tvm1...` |
| Legacy P2PKH version | `127` |
| Legacy P2SH version | `125` |
| WIF private-key version | `176` |
| BIP32 extended public | `7cc78565` |
| BIP32 extended private | `7cc780a6` |

Mainnet and Testnet encodings are intentionally different.

Mainnet addresses such as `vm1...` must not be treated as Testnet
`tvm1...` addresses.

Likewise, Testnet private keys must remain separate from mainnet keys.

## Bootstrap

VargaMesh Testnet v1 uses fixed IP bootstrap seeds.

Public bootstrap nodes:

```text
217.154.93.20:39666
31.70.81.5:39666
```

No DNS seed is required.

A normal Testnet node does not need a manual `addnode` entry for these
bootstrap peers.

## Example configuration

```ini
testnet=1
server=1
txindex=1

[test]
listen=1
port=39666
rpcbind=127.0.0.1
rpcport=39667
dnsseed=0
```

Core RPC should remain restricted to trusted interfaces.

Do not expose unauthenticated Core RPC directly to the public Internet.

## Public HTTPS API

A separate public Testnet HTTPS API is available at:

```text
https://testnet.vargacoin.com/api/v1/
```

Available integration endpoints include:

```text
GET  /api/v1/health
GET  /api/v1/config
GET  /api/v1/blocks
GET  /api/v1/block/<height-or-hash>
GET  /api/v1/tx/<txid>
GET  /api/v1/address/<address>
GET  /api/v1/utxos/<address>
GET  /api/v1/search?q=<query>
GET  /api/v1/mining/address/<address>
POST /api/v1/broadcast
```

The HTTPS API is not the VargaMesh Core RPC interface.

Private keys are not required by the public API.

Transactions submitted to `/api/v1/broadcast` must already be signed.

## Testnet SOLO mining

Public Testnet SOLO Stratum:

```text
stratum+tcp://testnet.vargacoin.com:4933
```

Miner configuration:

```text
Algorithm: SHA-256d
Username:  tvm1... Testnet payout address
Password:  x
Mode:      SOLO
Fee:       0%
```

The public Testnet SOLO service uses VargaMesh AuxPoW with Testnet chain ID
`22094`.

## Coinbase maturity

Mining rewards require `100` confirmations before they become spendable.

Applications and exchanges must treat immature coinbase outputs separately
from normal spendable UTXOs.

## Testnet resets

Testnet is development infrastructure.

Unlike mainnet, Testnet may be reset, upgraded or otherwise changed when
protocol development requires it.

Applications must never assign monetary value to `tVMESH`.

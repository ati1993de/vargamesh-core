# VargaMesh Exchange and Integration Guide

This document summarizes information commonly required by exchanges,
wallet providers, explorers and other VargaMesh integrators.

The VargaMesh Core source code remains authoritative for consensus-critical
behavior.

## Network overview

| Parameter | Mainnet | Public Testnet |
| --- | --- | --- |
| Asset | `VMESH` | `tVMESH` |
| Decimals | `8` | `8` |
| Primary address | `vm1...` | `tvm1...` |
| P2P port | `29666` | `39666` |
| RPC port | `29667` | `39667` |
| PoW | SHA-256d | SHA-256d |
| Difficulty | ASERT | ASERT |
| Target spacing | 120 seconds | 120 seconds |
| Coinbase maturity | 100 blocks | 100 blocks |
| AuxPoW chain ID | `22093` / `0x564D` | `22094` / `0x564E` |

`tVMESH` has no monetary value and exists only for development and
integration testing.

## Testnet resources

| Resource | Endpoint |
| --- | --- |
| Developer portal | https://testnet.vargacoin.com/ |
| Explorer | https://testnet.vargacoin.com/explorer.html |
| Testnet WebWallet | https://testnet.vargacoin.com/wallet.html |
| Public HTTPS API | https://testnet.vargacoin.com/api/v1/ |
| SOLO Stratum | `testnet.vargacoin.com:4933` |

## Start an integration node

```bash
./build/bin/vargameshd -testnet
```

Basic checks:

```bash
./build/bin/vargamesh-cli -testnet getblockchaininfo
./build/bin/vargamesh-cli -testnet getnetworkinfo
./build/bin/vargamesh-cli -testnet getblockcount
```

Before enabling deposits or withdrawals, an integration should explicitly
verify that it is connected to the intended network.

## Suggested Testnet integration flow

1. Start and synchronize a VargaMesh Testnet node.
2. Verify the node reports the Testnet chain.
3. Generate a `tvm1...` deposit address.
4. Obtain tVMESH using Testnet mining or public Testnet infrastructure.
5. Detect the incoming transaction.
6. Track block confirmations.
7. Apply the integration's Testnet credit policy.
8. Construct and sign a Testnet withdrawal.
9. Broadcast the signed transaction.
10. Verify the transaction and confirmations in the Testnet explorer.

## Address validation

Mainnet and Testnet addresses are deliberately different.

```text
Mainnet: vm1...
Testnet: tvm1...
```

Validate addresses against the selected network before accepting deposits or
withdrawals.

Example:

```bash
./build/bin/vargamesh-cli -testnet validateaddress "tvm1..."
```

## Block and transaction monitoring

Commonly useful RPC methods include:

```text
getblockchaininfo
getnetworkinfo
getblockcount
getbestblockhash
getblockhash
getblock
getblockheader
getrawtransaction
getrawmempool
getmempoolentry
sendrawtransaction
validateaddress
```

If historical arbitrary transaction lookup is required, configure the node
appropriately, for example with `txindex=1`.

Integrations must be prepared for chain reorganizations.

An unconfirmed transaction must not be treated as final.

## Deposit confirmations

The number of confirmations required for ordinary user deposits is an
integration policy decision.

Coinbase outputs are different: consensus requires `100` confirmations before
a coinbase output can be spent.

## Transaction broadcast

A locally signed raw Testnet transaction can be broadcast with:

```bash
./build/bin/vargamesh-cli -testnet sendrawtransaction "<raw-transaction-hex>"
```

The public Testnet HTTPS API also accepts already-signed transactions at:

```text
POST https://testnet.vargacoin.com/api/v1/broadcast
```

Private keys must never be transmitted to the public API.

## Testnet bootstrap nodes

```text
217.154.93.20:39666
31.70.81.5:39666
```

These are bootstrap peers, not trust anchors.

Every node independently validates blocks according to VargaMesh consensus.

## Operational separation

Production mainnet and Testnet environments should remain separated.

Use separate:

- datadirs
- wallet files
- RPC credentials
- addresses
- monitoring labels
- application configuration
- accounting records

Never credit `tVMESH` as mainnet `VMESH`.

Never reuse a Testnet transaction as a mainnet transaction.

## Testnet lifecycle

The public Testnet may be upgraded or reset when development requires it.

Use Testnet for integration and functional validation, not for financial or
economic assumptions about VMESH mainnet.

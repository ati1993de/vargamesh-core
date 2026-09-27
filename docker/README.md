# VargaMesh Docker Node

Official-style Docker deployment for VargaMesh Core.

Network: VargaMesh Mainnet
Asset: VMESH
P2P: TCP 29666
RPC: TCP 29667 - localhost/container only

## Build

docker build -t vargamesh/core:0.2.0 .

## Start

docker compose up -d

## Status

docker exec vargamesh-node \
  vargamesh-cli \
  -datadir=/data \
  -conf=/data/vargamesh.conf \
  getblockchaininfo

## Peers

docker exec vargamesh-node \
  vargamesh-cli \
  -datadir=/data \
  -conf=/data/vargamesh.conf \
  getconnectioncount

## Logs

docker logs -f vargamesh-node

## Stop

docker compose down

Blockchain data remains in the Docker volume.

RPC port 29667 is intentionally not published to the host.

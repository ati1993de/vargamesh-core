#!/usr/bin/env python3
# Copyright (c) 2026 VargaMesh developers
# Distributed under the MIT software license.
"""Cross-check VMESH block hashes and accumulated chainwork before checkpointing.

This script NEVER changes chain consensus or writes configuration files.
Compare at least two independently validated full nodes, preferably in
different networks. Their agreement alone is not proof against collusion.

Example:
  python3 contrib/devtools/vmesh_chainwork_audit.py \\
    --datadir /srv/vmesh-main --datadir /srv/vmesh-asia \\
    --height 25000
"""
import argparse
import json
import subprocess
import sys

VMESH_GENESIS = "00000000b0c55c00c13e2ee67b54fe33c327c8806f8da5050cfa47095d182d9a"


def rpc(cli, datadir, method, *params):
    process = subprocess.run(
        [cli, f"-datadir={datadir}", method, *map(str, params)],
        capture_output=True, text=True, check=False, timeout=30,
    )
    if process.returncode != 0:
        raise RuntimeError(f"{datadir}: {method}: {process.stderr.strip()}")
    result = process.stdout.strip()
    if method in {"getblockchaininfo", "getblockheader"}:
        return json.loads(result)
    return result.strip('"')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cli", default="vargamesh-cli")
    parser.add_argument("--datadir", action="append", required=True,
                        help="Full-node datadir; repeat for independent nodes")
    parser.add_argument("--height", type=int,
                        help="Compare at this fixed historical block height")
    args = parser.parse_args()

    if len(args.datadir) < 2:
        parser.error("Provide at least two independently validated node datadirs")
    if len(set(args.datadir)) != len(args.datadir):
        parser.error("Node datadirs must be distinct")

    nodes = []
    for datadir in args.datadir:
        info = rpc(args.cli, datadir, "getblockchaininfo")
        if info["chain"] != "main":
            raise ValueError(f"{datadir}: expected mainnet, got {info['chain']}")
        if rpc(args.cli, datadir, "getblockhash", 0) != VMESH_GENESIS:
            raise ValueError(f"{datadir}: VMESH genesis mismatch")
        if info.get("initialblockdownload", True):
            raise ValueError(f"{datadir}: node is still in initial block download")
        nodes.append((datadir, info))

    compare_height = args.height if args.height is not None else min(
        info["blocks"] for _, info in nodes)
    if compare_height is None or compare_height < 1:
        raise ValueError("Comparison height must be at least 1")
    if any(info["blocks"] < compare_height for _, info in nodes):
        raise ValueError("At least one node is below the requested height")

    snapshots = []
    for datadir, info in nodes:
        block_hash = rpc(args.cli, datadir, "getblockhash", compare_height)
        header = rpc(args.cli, datadir, "getblockheader", block_hash)
        if header.get("height") != compare_height:
            raise ValueError(f"{datadir}: unexpected block height")
        snapshots.append({
            "datadir": datadir, "tip_height": info["blocks"],
            "height": compare_height, "blockhash": block_hash,
            "chainwork": header["chainwork"],
        })

    reference = (snapshots[0]["blockhash"], snapshots[0]["chainwork"])
    matches = all((s["blockhash"], s["chainwork"]) == reference for s in snapshots)
    report = {
        "matching_nodes": matches, "candidate_height": compare_height,
        "candidate_blockhash": reference[0],
        "candidate_minimum_chainwork": reference[1],
        "snapshots": snapshots,
        "warning": "Operator review required; do not automatically write consensus values",
    }
    print(json.dumps(report, indent=2))
    return 0 if matches else 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, KeyError, ValueError, RuntimeError, subprocess.TimeoutExpired) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        sys.exit(2)

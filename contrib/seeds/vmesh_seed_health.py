#!/usr/bin/env python3
# Copyright (c) 2026 VargaMesh developers
# Distributed under the MIT software license.
"""Check VargaMesh P2P bootstrap reachability (NOT block or peer validation).

Only inventory operators that have consented to publish seed addresses.
Avoid putting unverified IP addresses into consensus/chainparams.
"""
import argparse
import json
import socket
import sys


def probe(host, port=29666, timeout=3.0):
    info = {"hostname": host, "port": port, "resolved": [], "reachable": []}
    try:
        addresses = socket.getaddrinfo(host, port, type=socket.SOCK_STREAM)
    except OSError as exc:
        info["error"] = str(exc)
        return info
    addresses_seen = set()
    for family, socktype, proto, _, sockaddr in addresses:
        ip = sockaddr[0]
        if ip in addresses_seen:
            continue
        addresses_seen.add(ip)
        info["resolved"].append(ip)
        try:
            with socket.socket(family, socktype, proto) as sock:
                sock.settimeout(timeout)
                sock.connect(sockaddr)
                info["reachable"].append(ip)
        except OSError:
            pass
    return info


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--seed", action="append", dest="seeds",
                        help="verified, consenting DNS seed hostname; repeat for multiple operators")
    parser.add_argument("--port", type=int, default=29666)
    parser.add_argument("--timeout", type=float, default=3)
    parser.add_argument("--min-reachable", type=int, default=1,
                        help="number of distinct resolved IPs required to accept health")
    args = parser.parse_args()
    if not (1 <= args.port <= 65535 and 0 < args.timeout <= 15 and args.min_reachable >= 1):
        parser.error("invalid port, timeout or min-reachable")
    seeds = args.seeds or ["seed.mesh.vargatech.net."]
    results = [probe(seed, args.port, args.timeout) for seed in seeds]
    distinct_ips = sorted({ip for r in results for ip in r["reachable"]})
    ok = len(distinct_ips) >= args.min_reachable
    print(json.dumps({"ok": ok, "distinct_reachable_addresses": len(distinct_ips),
                      "seeds": results, "note": "TCP connection does not prove VargaMesh protocol identity, independent ownership or chain validity."},
                     indent=2, sort_keys=True))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())

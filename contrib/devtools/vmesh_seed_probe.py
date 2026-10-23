#!/usr/bin/env python3
# Copyright (c) 2026 VargaMesh developers
# Distributed under the MIT software license.
"""Probe publicly reachable VMESH P2P seed candidates (not Core RPC).

The probe checks network magic AND receives a proper P2P version message.
A successful probe establishes protocol reachability, not the honesty,
independence, persistent availability or synchronization of the peer.

Usage:
  python3 contrib/devtools/vmesh_seed_probe.py asia.vargamesh.com rodb2008.com
  python3 contrib/devtools/vmesh_seed_probe.py --timeout 6 203.0.113.1:29666
"""

import argparse
import hashlib
import ipaddress
import json
import secrets
import socket
import struct
import sys
import time

MAGIC = bytes.fromhex("4638b879")
PROTOCOL_VERSION = 70016
DEFAULT_PORT = 29666
MAX_PAYLOAD = 2 * 1024 * 1024


def read_exact(sock, count):
    out = bytearray()
    while len(out) < count:
        part = sock.recv(count - len(out))
        if not part:
            raise ConnectionError("Connection closed before P2P message completed")
        out.extend(part)
    return bytes(out)


def checksum(payload):
    return hashlib.sha256(hashlib.sha256(payload).digest()).digest()[:4]


def message(command, payload=b""):
    command_bytes = command.encode("ascii")
    if len(command_bytes) > 12:
        raise ValueError("P2P command is too long")
    return (MAGIC + command_bytes.ljust(12, b"\\x00") +
            struct.pack("<I", len(payload)) + checksum(payload) + payload)


def network_address():
    return struct.pack("<Q", 0) + b"\\x00" * 16 + struct.pack(">H", 0)


def version_payload():
    agent = b"/VargaMesh:seed-probe/"
    return (struct.pack("<iQq", PROTOCOL_VERSION, 0, int(time.time())) +
            network_address() + network_address() +
            struct.pack("<Q", secrets.randbits(64)) +
            bytes([len(agent)]) + agent + struct.pack("<i", 0) + b"\\x00")


def split_host(value):
    # Supports bare host, host:port and [IPv6]:port.
    if value.startswith("["):
        host, sep, suffix = value[1:].partition("]")
        if not sep:
            raise ValueError("Unterminated IPv6 address")
        return host, int(suffix[1:]) if suffix.startswith(":") else DEFAULT_PORT
    if value.count(":") == 1:
        host, port = value.rsplit(":", 1)
        return host, int(port)
    return value, DEFAULT_PORT


def probe(candidate, timeout):
    host, port = split_host(candidate)
    if not 1 <= port <= 65535:
        raise ValueError("Invalid P2P port")
    with socket.create_connection((host, port), timeout=timeout) as sock:
        sock.settimeout(timeout)
        sock.sendall(message("version", version_payload()))
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline:
            header = read_exact(sock, 24)
            if header[:4] != MAGIC:
                raise ValueError("Wrong network magic (not VMESH mainnet)")
            command = header[4:16].split(b"\\x00", 1)[0].decode("ascii", "replace")
            size = struct.unpack("<I", header[16:20])[0]
            if size > MAX_PAYLOAD:
                raise ValueError("Oversized P2P payload")
            payload = read_exact(sock, size)
            if checksum(payload) != header[20:24]:
                raise ValueError("Invalid P2P checksum")
            if command == "version":
                if len(payload) < 4:
                    raise ValueError("Truncated version payload")
                version = struct.unpack("<i", payload[:4])[0]
                if version < 70001:
                    raise ValueError(f"Unsupported protocol version: {version}")
                sock.sendall(message("verack"))
                return {"host": host, "port": port, "ok": True,
                        "p2p_version": version, "magic": MAGIC.hex()}
        raise TimeoutError("VMESH version handshake timed out")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("hosts", nargs="+", help="DNS hosts or IPs, optionally :port")
    parser.add_argument("--timeout", type=float, default=5.0)
    args = parser.parse_args()
    if args.timeout <= 0 or args.timeout > 30:
        parser.error("timeout must be greater than 0 and at most 30")
    failures = 0
    for candidate in args.hosts:
        try:
            result = probe(candidate, args.timeout)
        except (OSError, ValueError, TimeoutError, ConnectionError) as err:
            failures += 1
            result = {"host": candidate, "ok": False, "error": str(err)}
        print(json.dumps(result, sort_keys=True))
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())

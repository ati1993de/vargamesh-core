#!/usr/bin/env python3
# Copyright (c) 2026 VargaMesh developers
# Distributed under the MIT software license.
"""Deterministic P2P handshake probe tests; no network access required."""
import io
import struct
import unittest
from unittest.mock import patch

import vmesh_seed_probe as seed


class FakeSocket:
    def __init__(self, response):
        self.receive_buffer = io.BytesIO(response)
        self.sent = []

    def __enter__(self):
        return self

    def __exit__(self, *args):
        return False

    def settimeout(self, value):
        self.timeout = value

    def sendall(self, data):
        self.sent.append(data)

    def recv(self, size):
        return self.receive_buffer.read(size)


class SeedProbeTests(unittest.TestCase):
    def test_magic_and_wire_header(self):
        packet = seed.message("version", b"\x01\x02")
        self.assertEqual(packet[:4], bytes.fromhex("4638b879"))
        self.assertEqual(packet[4:16], b"version" + b"\x00" * 5)
        self.assertEqual(struct.unpack("<I", packet[16:20])[0], 2)
        self.assertEqual(packet[20:24], seed.checksum(b"\x01\x02"))

    def test_correct_version_payload(self):
        payload = seed.version_payload()
        self.assertGreaterEqual(len(payload), 86)
        self.assertEqual(struct.unpack("<i", payload[:4])[0], 70016)
        self.assertEqual(len(seed.network_address()), 26)

    def test_ipv4_and_ipv6_host_parsing(self):
        self.assertEqual(seed.split_host("127.0.0.1:29666"), ("127.0.0.1", 29666))
        self.assertEqual(seed.split_host("[::1]:29666"), ("::1", 29666))
        self.assertEqual(seed.split_host("seed.example"), ("seed.example", 29666))

    def test_valid_vmesh_p2p_version(self):
        response = seed.message("version", struct.pack("<i", 70016))
        fake = FakeSocket(response)
        with patch.object(seed.socket, "create_connection", return_value=fake):
            result = seed.probe("127.0.0.1:29666", 1.0)
        self.assertTrue(result["ok"])
        self.assertEqual(result["p2p_version"], 70016)
        self.assertTrue(fake.sent[0].startswith(seed.MAGIC + b"version"))
        self.assertEqual(fake.sent[1], seed.message("verack"))

    def test_reject_wrong_network_magic(self):
        response = bytes.fromhex("f9beb4d9") + seed.message("version", struct.pack("<i", 70016))[4:]
        with patch.object(seed.socket, "create_connection", return_value=FakeSocket(response)):
            with self.assertRaisesRegex(ValueError, "Wrong network magic"):
                seed.probe("127.0.0.1", 1.0)

    def test_reject_bad_checksum(self):
        response = bytearray(seed.message("version", struct.pack("<i", 70016)))
        response[20] ^= 0xff
        with patch.object(seed.socket, "create_connection", return_value=FakeSocket(response)):
            with self.assertRaisesRegex(ValueError, "Invalid P2P checksum"):
                seed.probe("127.0.0.1", 1.0)

    def test_reject_truncated_version(self):
        with patch.object(seed.socket, "create_connection", return_value=FakeSocket(seed.message("version", b""))):
            with self.assertRaisesRegex(ValueError, "Truncated version"):
                seed.probe("127.0.0.1", 1.0)

    def test_reject_unreasonable_port(self):
        with self.assertRaisesRegex(ValueError, "Invalid P2P port"):
            seed.probe("127.0.0.1:0", 1.0)


if __name__ == "__main__":
    unittest.main()

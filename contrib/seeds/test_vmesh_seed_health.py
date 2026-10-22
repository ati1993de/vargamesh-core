#!/usr/bin/env python3
import importlib.util
import socket
import unittest
from pathlib import Path
from unittest.mock import patch

src = Path(__file__).with_name("vmesh_seed_health.py")
spec = importlib.util.spec_from_file_location("vmesh_seed_health", src)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


class DummySocket:
    def settimeout(self, timeout):
        pass

    def connect(self, address):
        if address[0] == "192.0.2.2":
            raise OSError("down")

    def __enter__(self):
        return self

    def __exit__(self, *args):
        pass


class SeedHealthTests(unittest.TestCase):
    def test_unique_resolved_ips_and_reachability(self):
        records = [
            (socket.AF_INET, socket.SOCK_STREAM, 6, "", ("192.0.2.1", 29666)),
            (socket.AF_INET, socket.SOCK_STREAM, 6, "", ("192.0.2.1", 29666)),
            (socket.AF_INET, socket.SOCK_STREAM, 6, "", ("192.0.2.2", 29666))
        ]
        with patch.object(module.socket, "getaddrinfo", return_value=records):
            with patch.object(module.socket, "socket", return_value=DummySocket()):
                info = module.probe("example.invalid")
        self.assertEqual(len(info["resolved"]), 2)
        self.assertEqual(info["reachable"], ["192.0.2.1"])

    def test_resolver_failure_is_not_reported_as_reachable(self):
        with patch.object(module.socket, "getaddrinfo", side_effect=OSError("DNS offline")):
            info = module.probe("offline.invalid")
        self.assertEqual(info["reachable"], [])
        self.assertIn("DNS offline", info["error"])


if __name__ == "__main__":
    unittest.main()

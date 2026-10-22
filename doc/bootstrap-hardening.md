# VMESH peer bootstrap hardening

The current VMESH mainnet bootstrap configuration in `src/kernel/chainparams.cpp`
uses `seed.mesh.vargatech.net.` and clears inherited Bitcoin fixed seeds.
Already connected nodes independently validate consensus; DNS seeds are
discovery hints, **not** validation authorities.

## Health monitoring

Run from an independent VPS with Python 3 (no external dependencies):

```bash
python3 contrib/seeds/vmesh_seed_health.py --seed seed.mesh.vargatech.net. --port 29666
```

When **verified, consenting and independently operated** seed domains exist,
monitor all of them and require multiple distinct reachable addresses:

```bash
python3 contrib/seeds/vmesh_seed_health.py \
  --seed seed.mesh.vargatech.net. \
  --seed example-independent-operator.invalid \
  --min-reachable 2
```

The second hostname above is deliberately invalid: **replace it with an
actual verified seed** before using the example. A reachable TCP port cannot
prove the peer speaks VMESH, owns a distinct AS/network, or serves the correct
chain. Use `getpeerinfo`, message-start/genesis checks and operator review
as separate acceptance gates.

## Production rollout checklist

1. Identify at least two *independent operators* in different networks/ASNs
   who explicitly agree to seed VMESH Mainnet on TCP 29666.
2. Verify Mainnet genesis, actual VMESH handshake, peer diversity, sync status,
   and the absence of Bitcoin/testnet addresses.
3. Operate independently monitored A/AAAA records with TTL, round-robin and
   sensible record lifecycle. Don't publish DNS entries that go stale.
4. Update `vSeeds` in `src/kernel/chainparams.cpp` only with verified domains,
   add matching bootstrap regression tests, and ship as a normal software
   release. This does not alter consensus.
5. Keep manual `-addnode=<validated_VMESH_host>:29666` fallback documented.
6. Monitor independent geographical/ASN diversity, outbound peer churn and
   reorg risk. Never infer decentralization merely from a high raw peer count.

**No addresses were invented or enabled by this patch.** Until independent
seed operators are actually verified, the chainparams file must not add
fictional or third-party IP addresses.

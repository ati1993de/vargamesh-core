# VargaMesh official clients — status and safety boundaries

Documentation reviewed: **9 October 2026**. This page describes related
wallet clients and **does not announce a new VargaMesh Core build**.

## Version overview

| Component | Version | Verified distribution or validation status |
| --- | --- | --- |
| VargaMesh Core | `v0.2.0` | Official Mainnet/Public Testnet Core release (Linux, Windows, macOS) |
| VargaMesh Desktop | `v0.7.1` | GitHub public release with Windows x64, macOS Intel, macOS arm64 artifacts |
| VargaMesh Android | `v0.5.0` | Source pre-release; Android build validation and real-device security tests required |
| VargaMesh WebWallet | Website-provided | Client functionality depends on the deployed website version |

The desktop version and Android version are **client releases**, not Core
consensus updates. Do not describe them as Core v0.7.1/v0.5.0.

## Official Desktop download

**GitHub release:** https://github.com/ati1993de/vargamesh-desktop/releases/tag/v0.7.1

Published **9 October 2026**. Artifacts include:

- `VargaMesh-Desktop-v0.7.1-Windows-x64-Setup.exe`
- `VargaMesh-Desktop-v0.7.1-Windows-x64-Portable.zip`
- `VargaMesh-Desktop-v0.7.1-macOS-x64.dmg` and `.zip`
- `VargaMesh-Desktop-v0.7.1-macOS-arm64.dmg` and `.zip`
- `SHA256SUMS` and platform-specific macOS checksum files

Desktop **bundles Core v0.2.0**, performs local blockchain consensus and
wallet RPC, and uses the existing bootstrap peer configuration.

### v0.7.0/v0.7.1 additions

- Local contacts with public VMESH addresses or `.vmesh` names
- Public VNS resolution with Core address validation, visible address
  confirmation, and a second resolution check immediately before sending
- Native receive/confirmation/sync notifications (amounts hidden by default)
- Responsive Wallet/Contacts interface, transaction CSV exports
- NestEx VMESH/USDT price display and existing VMT-1 token features
- v0.7.1 fixes the broken v0.7.0 initialization path that left Core status
  frozen until the user manually selected Refresh

**Limitations:** transaction fees in the standard send UI are estimates;
plaintext exported CSV and Recovery Phrase TXT files require secure
handling. Desktop is pre-1.0 and has not received an independent security
audit. macOS release packages may be unsigned/unnotarized. GitHub files
and storefront builds may differ in version.

## Android v0.5.0: Professional Wallet source pre-release

- Information: https://vargamesh.com/android.html
- APK path: https://vargamesh.com/vmesh.apk
- Checksum file: https://vargamesh.com/vmesh.apk.sha256
- Minimum Android: API 26 / Android 8.0
- Languages: English, German, Hungarian, Russian, Simplified Chinese

The Android v0.5.0 source update includes:

- Wallet dashboard organized around balances, Send/Receive, VMT-1 and activity
- Local private contact book with labels/favorites, no contacts/cloud permission
- Optional `/api/vns/v1/resolve/{name}` integration; display the **resolved,
  checksummed VMESH address** for user confirmation, never sign a name as an
  address
- Shareable payment requests in `vargamesh:<address>?amount=<decimal>` form
- Encrypted backup verification without replacing the active wallet
- WorkManager-based opt-in activity notifications every ~15 minutes while
  online when Android background scheduling permits
- First wallet snapshot silent, locally generated change outputs filtered,
  no amount or recipient shown on a lock-screen notification
- Exact decimal-to-satoshi representation for display-only balances
- Clearly marked **remote public HTTPS API** health status
- Retained v0.4.0 native VMT-1, BIP39 12/24-word HD model, index-0
  active address and legacy compressed WIF support

### Critical Android limitations

**Validation:** Android v0.5.0 is a source pre-release. An APK download URL,
hash file, or embedded APK signature is not proof of a successful Android
build, real-device testing, audit, or production-readiness. Confirm the
installed package identity, APK signature and SHA-256 using the actual
downloaded file, and consult the Android repository's `VALIDATION-REPORT.md`.

**Public API:** Android is **not an embedded local Core node**. It queries
public blockchain/UTXO/transaction services. Local signing and storage
do not make public data queries private: the public server can see the
requested address or transaction IDs. It should not receive secret keys.

**Notifications:** WorkManager scheduling is approximate, not real-time;
battery/background restrictions may delay delivery. UTXO snapshots do
not provide a full lifetime transaction history; already-spent outputs
between scans can be missed. Sent confirmation alerts refer only to this
installation's locally recorded outgoing transactions.

**VNS anchor:** A VNS name ownership anchor may be a special UTXO. Android
v0.5.0 does not guarantee that its transaction-construction path excludes
this anchor. Do **not** spend from a wallet managing VNS ownership anchors,
or significant funds, until dedicated safety review and physical-device
tests have been completed.

**HD scope:** Android currently operates the first active external HD
address (`m/84'/22093'/0'/0/0`). Full HD gap-limit scanning, multiple
active addresses and full coin control are not claimed.

## Interoperability

Desktop and Android share the VargaMesh BIP39/BIP32 first-address
derivation convention with the WebWallet:

`m/84'/22093'/0'/0/0`

`22093` is a *proposed* SLIP-0044 coin type, not a verified upstream
allocation. Derivation compatibility does not imply that every encrypted
backup container is interchangeable.

## Sources of truth

- Core executable/version, consensus and network rules:
  [Core repository](https://github.com/ati1993de/vargamesh-core) and
  [v0.2.0 release](https://github.com/ati1993de/vargamesh-core/releases/tag/v0.2.0)
- Desktop code/release:
  [Desktop repository](https://github.com/ati1993de/vargamesh-desktop),
  [v0.7.1 official release](https://github.com/ati1993de/vargamesh-desktop/releases/tag/v0.7.1)
- Android features and build validation:
  the Android client's current source tree and its `VALIDATION-REPORT.md`
- Web release manifest: https://vargamesh.com/api/v1/releases
  (server deployment can lag behind documentation or static APK contents)

**Security reminder:** independently back up wallet Recovery Phrases or
keys, use disposable wallets for new-client tests, and verify full
recipient addresses before sending.
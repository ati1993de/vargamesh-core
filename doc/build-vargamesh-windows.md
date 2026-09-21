# Building VargaMesh Core on Windows

VargaMesh retains the CMake-based Windows build infrastructure inherited from
Bitcoin Core.

Detailed build documentation remains available in:

- `doc/build-windows.md`
- `doc/build-windows-msvc.md`

VargaMesh executable names include:

- `vargameshd.exe`
- `vargamesh-cli.exe`
- `vargamesh-qt.exe`
- `vargamesh-wallet.exe`
- `vargamesh-tx.exe`
- `vargamesh-util.exe`

The repository also contains a GitHub Actions Windows build job using the
existing `.github/ci-windows.py` helper.

Release artifacts should be built from a clean tagged commit and accompanied
by cryptographic checksums.

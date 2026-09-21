# Contributing to VargaMesh Core

Contributions, testing and code review are welcome.

VargaMesh is consensus-critical software. Changes should be focused,
reviewable and accompanied by appropriate testing.

## Contributor workflow

1. Fork the repository.
2. Create a topic branch.
3. Make a focused change.
4. Add or update tests when behavior changes.
5. Build the affected components.
6. Run the relevant tests.
7. Open a pull request describing the motivation and test results.

## Consensus-sensitive changes

Changes affecting the following require additional scrutiny:

- block validation
- proof of work
- ASERT difficulty calculation
- AuxPoW validation
- block or transaction serialization
- genesis parameters
- monetary policy
- network identity
- address encoding
- activation/deployment rules

Do not mix consensus changes with unrelated formatting or large refactors.

## Testing

Typical Linux validation:

```bash
cmake -S . -B build -G Ninja \
  -DBUILD_GUI=OFF \
  -DBUILD_BENCH=OFF \
  -DBUILD_FUZZ_BINARY=OFF \
  -DENABLE_IPC=OFF

cmake --build build --parallel

ctest --test-dir build --output-on-failure
```

Functional tests are located under `test/functional/`.

## Security

Do not use normal public issues to disclose working exploits, private keys or
credentials.

See `SECURITY.md`.

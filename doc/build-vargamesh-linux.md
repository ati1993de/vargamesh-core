# Building VargaMesh Core on Linux

The following example targets Ubuntu/Debian systems.

## Dependencies

```bash
sudo apt update

sudo apt install -y \
  build-essential \
  cmake \
  ninja-build \
  pkg-config \
  python3 \
  libevent-dev \
  libboost-dev \
  libsqlite3-dev
```

## Configure

For a headless node build:

```bash
cmake -S . -B build -G Ninja \
  -DBUILD_GUI=OFF \
  -DBUILD_BENCH=OFF \
  -DBUILD_FUZZ_BINARY=OFF \
  -DENABLE_IPC=OFF
```

## Build

```bash
cmake --build build --parallel
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

Typical binaries are created under:

```text
build/bin/
```

Release builds should be produced from a clean, reviewed commit.

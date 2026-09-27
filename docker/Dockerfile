# ============================================================
# VargaMesh Core Docker
# https://github.com/ati1993de/vargamesh-core
# ============================================================

FROM ubuntu:24.04 AS downloader

ARG VMESH_VERSION=0.2.0
ARG VMESH_SHA256=d24087cf3dd73d6f113076fd751446041a08aaa2820bb9d409f5e4ea3696dd72

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
       ca-certificates \
       curl \
       tar \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp/vargamesh

RUN set -eux; \
    URL="https://github.com/ati1993de/vargamesh-core/releases/download/v${VMESH_VERSION}/vargamesh-core-${VMESH_VERSION}-linux-x86_64.tar.gz"; \
    echo "Downloading ${URL}"; \
    curl -fL --retry 3 --retry-delay 2 "$URL" -o vargamesh.tar.gz; \
    echo "${VMESH_SHA256}  vargamesh.tar.gz" | sha256sum -c -; \
    mkdir extracted out; \
    tar -xzf vargamesh.tar.gz -C extracted; \
    for bin in vargameshd vargamesh-cli vargamesh-wallet vargamesh-tx vargamesh-util; do \
        src="$(find extracted -type f -name "$bin" -print -quit)"; \
        if [ -n "$src" ]; then \
            install -m 0755 "$src" "/tmp/vargamesh/out/$bin"; \
        fi; \
    done; \
    test -x /tmp/vargamesh/out/vargameshd; \
    test -x /tmp/vargamesh/out/vargamesh-cli

# ------------------------------------------------------------

FROM ubuntu:24.04

LABEL org.opencontainers.image.title="VargaMesh Core"
LABEL org.opencontainers.image.description="VargaMesh VMESH Mainnet Full Node"
LABEL org.opencontainers.image.source="https://github.com/ati1993de/vargamesh-core"
LABEL org.opencontainers.image.version="0.2.0"

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
       ca-certificates \
       libatomic1 \
       libevent-2.1-7t64 \
       libevent-pthreads-2.1-7t64 \
       libgcc-s1 \
       libstdc++6 \
       libsqlite3-0 \
       libzmq5 \
    && rm -rf /var/lib/apt/lists/* \
    && groupadd --gid 10001 vargamesh \
    && useradd \
       --uid 10001 \
       --gid 10001 \
       --home-dir /data \
       --shell /usr/sbin/nologin \
       vargamesh \
    && mkdir -p /data \
    && chown -R vargamesh:vargamesh /data

COPY --from=downloader /tmp/vargamesh/out/ /usr/local/bin/

COPY docker-entrypoint.sh /usr/local/bin/vargamesh-entrypoint

RUN chmod 0755 /usr/local/bin/vargamesh-entrypoint

USER vargamesh

WORKDIR /data

VOLUME ["/data"]

# Mainnet P2P ONLY.
# RPC 29667 is intentionally NOT exposed.
EXPOSE 29666/tcp

STOPSIGNAL SIGTERM

HEALTHCHECK \
  --interval=30s \
  --timeout=10s \
  --start-period=60s \
  --retries=5 \
  CMD vargamesh-cli \
      -datadir=/data \
      -conf=/data/vargamesh.conf \
      getblockchaininfo >/dev/null || exit 1

ENTRYPOINT ["vargamesh-entrypoint"]

CMD ["vargameshd", \
     "-datadir=/data", \
     "-conf=/data/vargamesh.conf", \
     "-printtoconsole"]

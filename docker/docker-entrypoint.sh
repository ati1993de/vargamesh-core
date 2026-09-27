#!/bin/sh
set -eu

DATADIR="${VMESH_DATADIR:-/data}"
CONF="${DATADIR}/vargamesh.conf"

mkdir -p "$DATADIR"

if [ ! -f "$CONF" ]; then

    umask 077

    cat > "$CONF" <<'CONFEOF'
# ============================================================
# VargaMesh Core - Docker default configuration
# ============================================================

# Mainnet
server=1
listen=1

# Public P2P
bind=0.0.0.0:29666
port=29666

# RPC is intentionally container-local only.
rpcbind=127.0.0.1
rpcallowip=127.0.0.1
rpcport=29667

# Peer discovery
discover=1
dnsseed=1

# Known VargaMesh Mainnet peers
addnode=217.154.93.20:29666
addnode=212.132.80.39:29666
addnode=31.70.81.5:29666
CONFEOF

    if [ -n "${VMESH_PRUNE:-}" ]; then
        echo "prune=${VMESH_PRUNE}" >> "$CONF"
    fi

    echo "VargaMesh configuration created:"
    echo "  $CONF"
fi

exec "$@"

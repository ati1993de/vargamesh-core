#!/usr/bin/env bash

set -Eeuo pipefail

IMAGE="ghcr.io/ati1993de/vargamesh-core:latest"
CONTAINER="vargamesh-node"
VOLUME="vargamesh-data"
P2P_PORT="29666"

CONFIG_DIR="/etc/vargamesh-node"
CONFIG_FILE="${CONFIG_DIR}/config"
MANAGER="/usr/local/bin/vargamesh-node"

die() {
    echo
    echo "ERROR: $*" >&2
    exit 1
}

info() {
    echo
    echo "==> $*"
}

if [ "$(id -u)" -ne 0 ]; then
    die "Run this installer as root, for example:
curl -fsSL https://vargacoin.com/install-node.sh | sudo bash"
fi

if [ ! -r /etc/os-release ]; then
    die "Cannot determine Linux distribution."
fi

. /etc/os-release

case "${ID:-}" in
    ubuntu)
        DOCKER_DIST="ubuntu"
        ;;
    debian)
        DOCKER_DIST="debian"
        ;;
    *)
        die "Currently supported: Ubuntu and Debian."
        ;;
esac

ARCH="$(dpkg --print-architecture 2>/dev/null || true)"

case "$ARCH" in
    amd64)
        ;;
    *)
        die "VargaMesh Docker v0.2.0 currently supports amd64/x86_64 only. Detected: ${ARCH:-unknown}"
        ;;
esac

info "VargaMesh Node Installer"
echo "OS:        ${PRETTY_NAME:-$ID}"
echo "Arch:      $ARCH"
echo "Image:     $IMAGE"
echo "P2P port:  $P2P_PORT"
echo "RPC:       container-local only"

# Existing VargaMesh Docker installation
if command -v docker >/dev/null 2>&1; then
    if docker ps -a --format '{{.Names}}' 2>/dev/null | grep -qx "$CONTAINER"; then
        echo
        echo "VargaMesh Docker node is already installed."
        echo
        echo "Use:"
        echo "  vargamesh-node status"
        echo "  vargamesh-node update"
        exit 0
    fi
fi

# Do NOT take over an already-used public P2P port.
if command -v ss >/dev/null 2>&1; then
    if ss -H -ltn 2>/dev/null | awk '{print $4}' | grep -Eq ":${P2P_PORT}$"; then
        die "TCP port ${P2P_PORT} is already in use.

The installer will NOT stop or replace an existing service.
If a native VargaMesh node is already running, keep using that installation."
    fi
fi

install_docker() {

    if command -v docker >/dev/null 2>&1; then
        info "Docker already installed."

        if ! docker info >/dev/null 2>&1; then
            systemctl enable --now docker >/dev/null 2>&1 || \
                die "Docker exists but the Docker daemon could not be started."
        fi

        docker --version
        return
    fi

    info "Installing Docker Engine from Docker's official APT repository"

    apt-get update

    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        ca-certificates \
        curl \
        iproute2

    CONFLICTS=""

    for pkg in \
        docker.io \
        docker-compose \
        docker-compose-v2 \
        docker-doc \
        docker-buildx \
        podman-docker \
        containerd \
        runc
    do
        if dpkg-query -W -f='${db:Status-Abbrev}' "$pkg" 2>/dev/null \
            | grep -q '^ii'
        then
            CONFLICTS="${CONFLICTS} ${pkg}"
        fi
    done

    if [ -n "$CONFLICTS" ]; then
        die "Conflicting container packages detected:${CONFLICTS}

They were NOT removed automatically.
Remove or migrate them manually before installing Docker CE."
    fi

    install -m 0755 -d /etc/apt/keyrings

    curl -fsSL \
        "https://download.docker.com/linux/${DOCKER_DIST}/gpg" \
        -o /etc/apt/keyrings/docker.asc

    chmod a+r /etc/apt/keyrings/docker.asc

    CODENAME="${VERSION_CODENAME:-${UBUNTU_CODENAME:-}}"

    [ -n "$CODENAME" ] || die "Could not determine distribution codename."

    cat > /etc/apt/sources.list.d/docker.sources <<EOF
Types: deb
URIs: https://download.docker.com/linux/${DOCKER_DIST}
Suites: ${CODENAME}
Components: stable
Architectures: ${ARCH}
Signed-By: /etc/apt/keyrings/docker.asc
EOF

    apt-get update

    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        docker-ce \
        docker-ce-cli \
        containerd.io \
        docker-buildx-plugin \
        docker-compose-plugin

    systemctl enable --now docker

    docker info >/dev/null

    info "Docker successfully installed"
    docker --version
}

install_manager() {

    info "Installing vargamesh-node management command"

    install -m 0755 -d "$CONFIG_DIR"

    cat > "$CONFIG_FILE" <<EOF
IMAGE="${IMAGE}"
CONTAINER="${CONTAINER}"
VOLUME="${VOLUME}"
P2P_PORT="${P2P_PORT}"
EOF

    chmod 0644 "$CONFIG_FILE"

    cat > "$MANAGER" <<'MANAGER_EOF'
#!/usr/bin/env bash

set -Eeuo pipefail

CONFIG="/etc/vargamesh-node/config"

if [ ! -r "$CONFIG" ]; then
    echo "VargaMesh node configuration not found: $CONFIG" >&2
    exit 1
fi

. "$CONFIG"

run_node() {
    docker run -d \
        --name "$CONTAINER" \
        --restart unless-stopped \
        --security-opt no-new-privileges:true \
        --cap-drop ALL \
        -p "${P2P_PORT}:29666/tcp" \
        -v "${VOLUME}:/data" \
        "$IMAGE"
}

container_exists() {
    docker ps -a \
        --format '{{.Names}}' \
        | grep -qx "$CONTAINER"
}

container_running() {
    docker ps \
        --format '{{.Names}}' \
        | grep -qx "$CONTAINER"
}

cmd="${1:-status}"

case "$cmd" in

    status)
        echo "=== VargaMesh Docker Node ==="
        echo "Image:     $IMAGE"
        echo "Container: $CONTAINER"
        echo "P2P:       TCP $P2P_PORT"
        echo

        if ! container_exists; then
            echo "Status: NOT INSTALLED"
            exit 1
        fi

        docker inspect \
            --format='Status: {{.State.Status}}{{if .State.Health}} / Health: {{.State.Health.Status}}{{end}}' \
            "$CONTAINER"

        echo

        if container_running; then
            echo "=== Blockchain ==="

            docker exec "$CONTAINER" \
                vargamesh-cli \
                -datadir=/data \
                -conf=/data/vargamesh.conf \
                getblockchaininfo \
                2>/dev/null || echo "RPC is not ready yet."

            echo
            echo "=== Peers ==="

            docker exec "$CONTAINER" \
                vargamesh-cli \
                -datadir=/data \
                -conf=/data/vargamesh.conf \
                getconnectioncount \
                2>/dev/null || true
        fi
        ;;

    peers)
        docker exec "$CONTAINER" \
            vargamesh-cli \
            -datadir=/data \
            -conf=/data/vargamesh.conf \
            getpeerinfo
        ;;

    cli)
        shift

        if [ "$#" -eq 0 ]; then
            echo "Usage: vargamesh-node cli <RPC command>"
            exit 1
        fi

        docker exec "$CONTAINER" \
            vargamesh-cli \
            -datadir=/data \
            -conf=/data/vargamesh.conf \
            "$@"
        ;;

    logs)
        docker logs --tail 200 -f "$CONTAINER"
        ;;

    start)
        if container_exists; then
            docker start "$CONTAINER"
        else
            run_node
        fi
        ;;

    stop)
        docker stop -t 120 "$CONTAINER"
        ;;

    restart)
        docker restart -t 120 "$CONTAINER"
        ;;

    update)
        echo "Pulling newest VargaMesh image..."
        docker pull "$IMAGE"

        if container_exists; then
            echo "Stopping VargaMesh node gracefully..."
            docker stop -t 120 "$CONTAINER" || true
            docker rm "$CONTAINER"
        fi

        echo "Starting updated VargaMesh node..."
        run_node

        echo
        echo "Update complete."
        ;;

    version)
        if container_running; then
            docker exec "$CONTAINER" vargameshd --version
        else
            docker run --rm \
                --entrypoint vargameshd \
                "$IMAGE" \
                --version
        fi
        ;;

    uninstall)
        echo "Removing VargaMesh container..."

        if container_exists; then
            docker stop -t 120 "$CONTAINER" 2>/dev/null || true
            docker rm "$CONTAINER" 2>/dev/null || true
        fi

        echo
        echo "Container removed."
        echo "Blockchain data was NOT deleted."
        echo
        echo "Persistent volume:"
        echo "  $VOLUME"
        ;;

    *)
        cat <<EOF
VargaMesh Node Manager

Usage:
  vargamesh-node status
  vargamesh-node peers
  vargamesh-node logs
  vargamesh-node update
  vargamesh-node start
  vargamesh-node stop
  vargamesh-node restart
  vargamesh-node version
  vargamesh-node cli <RPC command>
  vargamesh-node uninstall
EOF
        exit 1
        ;;
esac
MANAGER_EOF

    chmod 0755 "$MANAGER"
}

install_docker
install_manager

info "Pulling official VargaMesh Core image"

docker pull "$IMAGE"

info "Creating persistent blockchain volume"

docker volume create "$VOLUME" >/dev/null

info "Starting VargaMesh Mainnet node"

docker run -d \
    --name "$CONTAINER" \
    --restart unless-stopped \
    --security-opt no-new-privileges:true \
    --cap-drop ALL \
    -p "${P2P_PORT}:29666/tcp" \
    -v "${VOLUME}:/data" \
    "$IMAGE"

echo
echo "=================================================="
echo " VargaMesh Full Node installed successfully"
echo "=================================================="
echo
echo "Image:"
echo "  $IMAGE"
echo
echo "Public P2P:"
echo "  TCP ${P2P_PORT}"
echo
echo "RPC:"
echo "  NOT exposed to the host/network"
echo
echo "Data:"
echo "  Docker volume ${VOLUME}"
echo
echo "Commands:"
echo "  vargamesh-node status"
echo "  vargamesh-node peers"
echo "  vargamesh-node logs"
echo "  vargamesh-node update"
echo "  vargamesh-node version"
echo "  vargamesh-node cli getblockchaininfo"
echo
echo "The first blockchain synchronization can take some time."
echo

sleep 3

vargamesh-node status || true

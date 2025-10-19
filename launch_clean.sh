#!/bin/bash

# Alternative clean launcher using env -i for complete environment isolation
# This provides maximum isolation from snap environment conflicts

set -e

echo "=========================================="
echo "Image Classification Tool - Isolated Launcher"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if executable exists
if [ ! -f "./ImageClassificationTool" ]; then
    echo -e "${RED}Error: ImageClassificationTool executable not found!${NC}"
    echo "Please build the application first by running: ./build.sh"
    exit 1
fi

# Make executable if needed
if [ ! -x "./ImageClassificationTool" ]; then
    chmod +x ./ImageClassificationTool
fi

echo -e "${BLUE}Launching with completely isolated environment...${NC}"
echo ""

# Launch with a completely clean environment, setting only essential variables
env -i \
    HOME="$HOME" \
    USER="$USER" \
    DISPLAY="$DISPLAY" \
    XAUTHORITY="$XAUTHORITY" \
    XDG_RUNTIME_DIR="$XDG_RUNTIME_DIR" \
    XDG_SESSION_TYPE="$XDG_SESSION_TYPE" \
    WAYLAND_DISPLAY="$WAYLAND_DISPLAY" \
    PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" \
    LD_LIBRARY_PATH="/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu" \
    QT_QPA_PLATFORM="xcb" \
    QT_PLUGIN_PATH="/usr/lib/x86_64-linux-gnu/qt5/plugins" \
    ./ImageClassificationTool "$@"

#!/bin/bash

# Clean launcher script for Image Classification Tool
# This script removes snap-related environment variables that can cause
# library conflicts and glibc symbol lookup errors

set -e

echo "=========================================="
echo "Image Classification Tool - Clean Launcher"
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
    echo "Please build the application first by running: ..sh"
    exit 1
fi

# Check if executable is executable
if [ ! -x "./ImageClassificationTool" ]; then
    echo -e "${YELLOW}Warning: Making executable file executable...${NC}"
    chmod +x ./ImageClassificationTool
fi

echo -e "${BLUE}Cleaning environment variables...${NC}"

# Create a clean environment by unsetting problematic snap-related variables
# These variables can cause library path conflicts with snap packages

# Unset snap-related environment variables that interfere with library loading
unset SNAP
unset SNAP_ARCH
unset SNAP_COMMON
unset SNAP_DATA
unset SNAP_INSTANCE_KEY
unset SNAP_INSTANCE_NAME
unset SNAP_LIBRARY_PATH
unset SNAP_NAME
unset SNAP_REAL_HOME
unset SNAP_REVISION
unset SNAP_USER_COMMON
unset SNAP_USER_DATA
unset SNAP_VERSION

# Unset VS Code snap-related variables that can interfere
unset LOCPATH
unset GTK_PATH
unset FONTCONFIG_PATH

# Suppress GTK module warnings (Qt5 app doesn't need GTK sound modules)
unset GTK_MODULES

# Clean up any snap-related paths from environment variables
export PATH=$(echo "$PATH" | tr ':' '\n' | grep -v '/snap/' | tr '\n' ':' | sed 's/:$//')

# Ensure we're using system libraries, not snap libraries
unset LD_LIBRARY_PATH
export LD_LIBRARY_PATH="/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu"

# Set Qt platform to avoid potential conflicts
export QT_QPA_PLATFORM=xcb

# Ensure we use the system's Qt libraries
export QT_PLUGIN_PATH="/usr/lib/x86_64-linux-gnu/qt5/plugins"

echo -e "${GREEN}Environment cleaned successfully!${NC}"
echo ""
echo -e "${BLUE}Launching Image Classification Tool...${NC}"
echo ""

# Launch the application with the clean environment
exec ./ImageClassificationTool "$@"

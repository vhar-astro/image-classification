#!/bin/bash

# ============================================================================
# ML Dataset Preparation Tool - macOS Launcher
# ============================================================================
# Simple launcher script for the macOS application bundle
# ============================================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo ""
echo -e "${BLUE}ML Dataset Preparation Tool - macOS Launcher${NC}"
echo ""

# Check if application bundle exists
if [ -d "build/MLDatasetTool.app" ]; then
    echo -e "${GREEN}Starting ML Dataset Preparation Tool...${NC}"
    open build/MLDatasetTool.app
    exit 0
fi

# Application not found
echo -e "${RED}ERROR: Application not found!${NC}"
echo ""
echo "Please build the application first using one of these methods:"
echo "  1. Run: ./build_macos.sh"
echo "  2. See BUILD_MACOS.md for detailed instructions"
echo ""
exit 1


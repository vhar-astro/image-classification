#!/bin/bash

# Installation script for Image Classification Tool dependencies
# This script installs all required dependencies on Ubuntu/Debian systems

set -e  # Exit on error

echo "=========================================="
echo "Installing Dependencies"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if running as root
if [ "$EUID" -eq 0 ]; then 
    echo -e "${RED}Please do not run this script as root (without sudo).${NC}"
    echo "The script will ask for sudo password when needed."
    exit 1
fi

# Detect Ubuntu version
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$NAME
    VER=$VERSION_ID
    echo -e "${BLUE}Detected OS: $OS $VER${NC}"
    echo ""
else
    echo -e "${YELLOW}Warning: Could not detect OS version.${NC}"
    echo "This script is designed for Ubuntu/Debian systems."
    echo ""
fi

# Update package list
echo "Updating package list..."
sudo apt update

# Install build essentials
echo ""
echo "Installing build essentials..."
sudo apt install -y build-essential

# Install CMake
echo ""
echo "Installing CMake..."
sudo apt install -y cmake

# Install Qt5 development libraries
echo ""
echo "Installing Qt5 development libraries..."

# Check Ubuntu version for correct package names
if [ -n "$VER" ] && [ "$(echo "$VER >= 22.04" | bc -l 2>/dev/null || echo 0)" -eq 1 ]; then
    echo "Installing Qt5 for Ubuntu 22.04+..."
    sudo apt install -y qtbase5-dev qtbase5-dev-tools qtchooser qt5-qmake
else
    echo "Installing Qt5 for Ubuntu < 22.04..."
    sudo apt install -y qt5-default qtbase5-dev qtbase5-dev-tools || \
    sudo apt install -y qtbase5-dev qtbase5-dev-tools qtchooser qt5-qmake
fi

# Verify installations
echo ""
echo "=========================================="
echo "Verifying installations..."
echo "=========================================="

# Check g++
if command -v g++ &> /dev/null; then
    echo -e "${GREEN}✓${NC} g++ installed: $(g++ --version | head -n1)"
else
    echo -e "${RED}✗${NC} g++ not found"
fi

# Check CMake
if command -v cmake &> /dev/null; then
    echo -e "${GREEN}✓${NC} CMake installed: $(cmake --version | head -n1)"
else
    echo -e "${RED}✗${NC} CMake not found"
fi

# Check qmake (Qt5)
if command -v qmake &> /dev/null; then
    echo -e "${GREEN}✓${NC} Qt5 installed: $(qmake --version | grep -i qt)"
else
    echo -e "${YELLOW}⚠${NC} qmake not found (Qt5 may not be properly installed)"
fi

echo ""
echo "=========================================="
echo -e "${GREEN}Installation complete!${NC}"
echo "=========================================="
echo ""
echo "You can now build the application by running:"
echo "  ./build.sh"
echo ""


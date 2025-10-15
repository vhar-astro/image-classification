#!/bin/bash

# Build script for Image Classification Tool
# This script automates the build process using CMake

set -e  # Exit on error

echo "=========================================="
echo "Image Classification Tool - Build Script"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: CMake is not installed.${NC}"
    echo "Please install CMake: sudo apt install cmake"
    exit 1
fi

# Check if Qt5 is installed
if ! dpkg -l | grep -q qtbase5-dev; then
    echo -e "${YELLOW}Warning: Qt5 development libraries may not be installed.${NC}"
    echo "If the build fails, install Qt5: sudo apt install qtbase5-dev"
    echo ""
fi

# Create build directory
echo "Creating build directory..."
if [ -d "build" ]; then
    echo -e "${YELLOW}Build directory already exists. Cleaning...${NC}"
    rm -rf build
fi
mkdir build

# Navigate to build directory
cd build

# Run CMake
echo ""
echo "Running CMake configuration..."
if cmake ..; then
    echo -e "${GREEN}CMake configuration successful!${NC}"
else
    echo -e "${RED}CMake configuration failed!${NC}"
    echo "Please check the error messages above."
    exit 1
fi

# Build the project
echo ""
echo "Building the project..."
if make -j$(nproc); then
    echo -e "${GREEN}Build successful!${NC}"
else
    echo -e "${RED}Build failed!${NC}"
    echo "Please check the error messages above."
    exit 1
fi

# Success message
echo ""
echo "=========================================="
echo -e "${GREEN}Build completed successfully!${NC}"
echo "=========================================="
echo ""
echo "To run the application:"
echo ""
echo -e "${GREEN}Recommended (avoids library conflicts):${NC}"
echo "  ./run_app.sh"
echo ""
echo -e "${YELLOW}Alternative (maximum isolation):${NC}"
echo "  ./launch_clean.sh"
echo ""
echo -e "${YELLOW}Direct execution (may have issues with snap packages):${NC}"
echo "  ./build/ImageClassificationTool"
echo ""
echo -e "${BLUE}Note:${NC} If you encounter library errors like 'symbol lookup error'"
echo "with libpthread or Qt threading issues, use the provided launcher"
echo "scripts which clean the environment from snap package conflicts."
echo ""


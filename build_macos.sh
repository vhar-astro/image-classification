#!/bin/bash

# ============================================================================
# ML Dataset Preparation Tool - macOS Build Script
# ============================================================================
# This script automates the build process for macOS
# It configures CMake, builds the application, and creates a .app bundle
# ============================================================================

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# ============================================================================
# CONFIGURATION
# ============================================================================

# Qt installation path - CHANGE THIS TO YOUR QT INSTALLATION
# Common locations:
#   - Homebrew: /usr/local/opt/qt@5 or /opt/homebrew/opt/qt@5 (Apple Silicon)
#   - Official Qt installer: ~/Qt/5.15.2/clang_64
QT_PATH="${QT_PATH:-/usr/local/opt/qt@5}"

# Build configuration (Release or Debug)
BUILD_CONFIG="${BUILD_CONFIG:-Release}"

# Number of parallel jobs (use all CPU cores)
JOBS=$(sysctl -n hw.ncpu)

# ============================================================================
# BANNER
# ============================================================================

echo ""
echo -e "${CYAN}============================================================================${NC}"
echo -e "${CYAN}  ML Dataset Preparation Tool - macOS Build Script${NC}"
echo -e "${CYAN}============================================================================${NC}"
echo ""

# ============================================================================
# STEP 1: ENVIRONMENT VALIDATION
# ============================================================================

echo -e "${BLUE}[1/6] Validating build environment...${NC}"
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}ERROR: CMake not found!${NC}"
    echo ""
    echo "Please install CMake using one of these methods:"
    echo "  1. Homebrew: brew install cmake"
    echo "  2. MacPorts: sudo port install cmake"
    echo "  3. Download from: https://cmake.org/download/"
    echo ""
    exit 1
fi

CMAKE_VERSION=$(cmake --version | head -n1)
echo -e "${GREEN}✓${NC} CMake found: $CMAKE_VERSION"

# Check if Qt5 is installed
if [ ! -d "$QT_PATH" ]; then
    echo -e "${RED}ERROR: Qt5 not found at: $QT_PATH${NC}"
    echo ""
    echo "Please install Qt5 using one of these methods:"
    echo ""
    echo "  1. Homebrew (recommended):"
    echo "     brew install qt@5"
    echo "     export QT_PATH=/usr/local/opt/qt@5"
    echo ""
    echo "  2. Homebrew (Apple Silicon):"
    echo "     brew install qt@5"
    echo "     export QT_PATH=/opt/homebrew/opt/qt@5"
    echo ""
    echo "  3. Official Qt installer:"
    echo "     Download from: https://www.qt.io/download-qt-installer"
    echo "     export QT_PATH=~/Qt/5.15.2/clang_64"
    echo ""
    echo "Then set QT_PATH environment variable before running this script:"
    echo "  export QT_PATH=/path/to/qt5"
    echo "  ./build_macos.sh"
    echo ""
    exit 1
fi

echo -e "${GREEN}✓${NC} Qt5 found at: $QT_PATH"

# Check if qmake exists
QMAKE_PATH="$QT_PATH/bin/qmake"
if [ ! -f "$QMAKE_PATH" ]; then
    echo -e "${RED}ERROR: qmake not found at: $QMAKE_PATH${NC}"
    echo "Please verify your Qt installation."
    exit 1
fi

QT_VERSION=$($QMAKE_PATH -query QT_VERSION)
echo -e "${GREEN}✓${NC} Qt version: $QT_VERSION"

# Check if Xcode Command Line Tools are installed
if ! xcode-select -p &> /dev/null; then
    echo -e "${RED}ERROR: Xcode Command Line Tools not found!${NC}"
    echo ""
    echo "Please install Xcode Command Line Tools:"
    echo "  xcode-select --install"
    echo ""
    exit 1
fi

echo -e "${GREEN}✓${NC} Xcode Command Line Tools installed"

# Check for C++ compiler
if ! command -v clang++ &> /dev/null; then
    echo -e "${RED}ERROR: C++ compiler (clang++) not found!${NC}"
    echo "Please install Xcode Command Line Tools."
    exit 1
fi

CLANG_VERSION=$(clang++ --version | head -n1)
echo -e "${GREEN}✓${NC} Compiler found: $CLANG_VERSION"

echo ""
echo -e "${GREEN}Environment validation successful!${NC}"

# ============================================================================
# STEP 2: CLEAN BUILD DIRECTORY
# ============================================================================

echo ""
echo -e "${BLUE}[2/6] Preparing build directory...${NC}"
echo ""

if [ -d "build" ]; then
    echo -e "${YELLOW}Build directory already exists. Cleaning...${NC}"
    rm -rf build
fi

mkdir build
echo -e "${GREEN}✓${NC} Build directory created"

# ============================================================================
# STEP 3: CMAKE CONFIGURATION
# ============================================================================

echo ""
echo -e "${BLUE}[3/6] Configuring project with CMake...${NC}"
echo "  - Generator: Unix Makefiles"
echo "  - Build type: $BUILD_CONFIG"
echo "  - Qt path: $QT_PATH"
echo ""

cd build

if cmake -DCMAKE_BUILD_TYPE=$BUILD_CONFIG \
         -DCMAKE_PREFIX_PATH="$QT_PATH" \
         -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 \
         ..; then
    echo ""
    echo -e "${GREEN}✓${NC} CMake configuration successful"
else
    echo ""
    echo -e "${RED}ERROR: CMake configuration failed!${NC}"
    echo ""
    echo "Common solutions:"
    echo "  1. Check that QT_PATH is correct"
    echo "  2. Ensure Qt5 is properly installed"
    echo "  3. Try setting QT_PATH manually:"
    echo "     export QT_PATH=/path/to/qt5"
    echo "     ./build_macos.sh"
    echo ""
    cd ..
    exit 1
fi

# ============================================================================
# STEP 4: BUILD APPLICATION
# ============================================================================

echo ""
echo -e "${BLUE}[4/6] Building application...${NC}"
echo "  - Using $JOBS parallel jobs"
echo "  - Configuration: $BUILD_CONFIG"
echo ""

if make -j$JOBS; then
    echo ""
    echo -e "${GREEN}✓${NC} Build successful"
else
    echo ""
    echo -e "${RED}ERROR: Build failed!${NC}"
    echo ""
    echo "Please check the error messages above."
    echo "If you need help, see BUILD_MACOS.md for troubleshooting."
    echo ""
    cd ..
    exit 1
fi

# ============================================================================
# STEP 5: DEPLOY QT FRAMEWORKS
# ============================================================================

echo ""
echo -e "${BLUE}[5/6] Deploying Qt frameworks...${NC}"
echo ""

MACDEPLOYQT="$QT_PATH/bin/macdeployqt"
APP_BUNDLE="MLDatasetTool.app"

if [ ! -f "$MACDEPLOYQT" ]; then
    echo -e "${YELLOW}WARNING: macdeployqt not found at: $MACDEPLOYQT${NC}"
    echo "The application may not run on other machines without Qt installed."
else
    echo "  - Running macdeployqt..."
    if "$MACDEPLOYQT" "$APP_BUNDLE" -always-overwrite; then
        echo -e "${GREEN}✓${NC} Qt frameworks deployed successfully"
    else
        echo -e "${YELLOW}WARNING: macdeployqt encountered issues${NC}"
        echo "The application may still work, but might be missing some Qt frameworks."
    fi
fi

# ============================================================================
# STEP 6: VERIFY BUILD
# ============================================================================

echo ""
echo -e "${BLUE}[6/6] Verifying build...${NC}"
echo ""

if [ -d "$APP_BUNDLE" ]; then
    echo -e "${GREEN}✓${NC} Application bundle created: $APP_BUNDLE"
    
    # Get bundle size
    BUNDLE_SIZE=$(du -sh "$APP_BUNDLE" | cut -f1)
    echo "  - Bundle size: $BUNDLE_SIZE"
    
    # Check if executable exists
    if [ -f "$APP_BUNDLE/Contents/MacOS/MLDatasetTool" ]; then
        echo -e "${GREEN}✓${NC} Executable found in bundle"
    else
        echo -e "${YELLOW}⚠${NC} Executable not found in expected location"
    fi
    
    # Check if Qt frameworks are bundled
    if [ -d "$APP_BUNDLE/Contents/Frameworks" ]; then
        FRAMEWORK_COUNT=$(ls -1 "$APP_BUNDLE/Contents/Frameworks" | wc -l | tr -d ' ')
        echo -e "${GREEN}✓${NC} Qt frameworks bundled: $FRAMEWORK_COUNT frameworks"
    else
        echo -e "${YELLOW}⚠${NC} Qt frameworks not found (may need manual deployment)"
    fi
else
    echo -e "${RED}✗${NC} Application bundle not found!"
    cd ..
    exit 1
fi

cd ..

# ============================================================================
# BUILD COMPLETE
# ============================================================================

echo ""
echo -e "${CYAN}============================================================================${NC}"
echo -e "${GREEN}  BUILD COMPLETED SUCCESSFULLY!${NC}"
echo -e "${CYAN}============================================================================${NC}"
echo ""
echo -e "${GREEN}Application bundle:${NC} build/MLDatasetTool.app"
echo ""
echo -e "${YELLOW}To run the application:${NC}"
echo "  1. Double-click: build/MLDatasetTool.app"
echo "  2. Or use: open build/MLDatasetTool.app"
echo "  3. Or use: ./run_macos.sh"
echo ""
echo -e "${YELLOW}To create a distributable DMG:${NC}"
echo "  See BUILD_MACOS.md for instructions on creating a DMG installer"
echo ""
echo -e "${CYAN}============================================================================${NC}"
echo ""


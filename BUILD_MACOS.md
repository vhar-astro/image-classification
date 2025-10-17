# Building on macOS

This guide provides comprehensive instructions for building the ML Dataset Preparation Tool on macOS.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Quick Start](#quick-start)
- [Detailed Build Instructions](#detailed-build-instructions)
- [Running the Application](#running-the-application)
- [Creating a DMG Installer](#creating-a-dmg-installer)
- [Troubleshooting](#troubleshooting)

## Prerequisites

### System Requirements

- **macOS 10.14 (Mojave) or later**
- **Xcode Command Line Tools**
- **CMake 3.10 or later**
- **Qt 5.12 or later** (5.15.2 recommended)

### Installing Prerequisites

#### 1. Install Xcode Command Line Tools

```bash
xcode-select --install
```

This installs the C++ compiler (clang++) and other development tools.

**Verify installation:**
```bash
xcode-select -p
# Should output: /Library/Developer/CommandLineTools
```

#### 2. Install Homebrew (if not already installed)

Homebrew is the recommended package manager for macOS.

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**For Apple Silicon Macs (M1/M2/M3)**, add Homebrew to your PATH:
```bash
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
eval "$(/opt/homebrew/bin/brew shellenv)"
```

#### 3. Install CMake

```bash
brew install cmake
```

**Verify installation:**
```bash
cmake --version
# Should output: cmake version 3.x.x or later
```

#### 4. Install Qt5

**Option 1: Homebrew (Recommended for development)**

```bash
# Install Qt5
brew install qt@5

# Add Qt5 to PATH (add to ~/.zshrc or ~/.bash_profile for persistence)
export PATH="/usr/local/opt/qt@5/bin:$PATH"  # Intel Macs
# OR
export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"  # Apple Silicon Macs

# Set Qt path for CMake
export QT_PATH="/usr/local/opt/qt@5"  # Intel Macs
# OR
export QT_PATH="/opt/homebrew/opt/qt@5"  # Apple Silicon Macs
```

**Option 2: Official Qt Installer (Recommended for distribution)**

1. Download the Qt installer from: https://www.qt.io/download-qt-installer
2. Run the installer and select:
   - Qt 5.15.2 (or latest 5.x version)
   - macOS component
3. Note the installation path (usually `~/Qt/5.15.2/clang_64`)
4. Set the Qt path:
   ```bash
   export QT_PATH=~/Qt/5.15.2/clang_64
   ```

**Verify Qt installation:**
```bash
$QT_PATH/bin/qmake --version
# Should output: QMake version 3.x and Qt version 5.x.x
```

## Quick Start

### Using the Build Script (Recommended)

1. **Set Qt path** (if not using default Homebrew location):
   ```bash
   export QT_PATH=/path/to/qt5
   ```

2. **Make the build script executable**:
   ```bash
   chmod +x build_macos.sh
   ```

3. **Run the build script**:
   ```bash
   ./build_macos.sh
   ```

4. **Run the application**:
   ```bash
   open build/MLDatasetTool.app
   # OR
   ./run_macos.sh
   ```

**That's it!** The build script handles everything automatically.

## Detailed Build Instructions

### Method 1: Automated Build Script

The `build_macos.sh` script automates the entire build process:

```bash
# Set Qt path (if needed)
export QT_PATH=/usr/local/opt/qt@5  # Intel Macs
# OR
export QT_PATH=/opt/homebrew/opt/qt@5  # Apple Silicon Macs

# Make executable and run
chmod +x build_macos.sh
./build_macos.sh
```

**What the script does:**
1. Validates environment (CMake, Qt, Xcode tools)
2. Creates and cleans build directory
3. Configures CMake with Qt path
4. Builds the application using all CPU cores
5. Runs `macdeployqt` to bundle Qt frameworks
6. Verifies the build and reports status

### Method 2: Manual Build with CMake

If you prefer manual control:

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH=$QT_PATH \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 \
      ..

# Build the application
make -j$(sysctl -n hw.ncpu)

# Deploy Qt frameworks
$QT_PATH/bin/macdeployqt MLDatasetTool.app -always-overwrite

# Return to project root
cd ..
```

### Method 3: Using CMake GUI

1. **Open CMake GUI**:
   ```bash
   cmake-gui
   ```

2. **Set paths**:
   - Source code: `/path/to/image-classification`
   - Build binaries: `/path/to/image-classification/build`

3. **Click "Configure"**:
   - Choose "Unix Makefiles" generator
   - Click "Finish"

4. **Set CMAKE_PREFIX_PATH**:
   - Add entry: `CMAKE_PREFIX_PATH` = `/path/to/qt5`
   - Set `CMAKE_BUILD_TYPE` = `Release`
   - Set `CMAKE_OSX_DEPLOYMENT_TARGET` = `10.14`

5. **Click "Generate"**

6. **Build from terminal**:
   ```bash
   cd build
   make -j$(sysctl -n hw.ncpu)
   $QT_PATH/bin/macdeployqt MLDatasetTool.app
   ```

## Running the Application

### Option 1: Double-click the App Bundle

Navigate to the `build` folder in Finder and double-click `MLDatasetTool.app`.

### Option 2: Use the Launcher Script

```bash
chmod +x run_macos.sh
./run_macos.sh
```

### Option 3: Use the `open` Command

```bash
open build/MLDatasetTool.app
```

### Option 4: Run Directly from Terminal

```bash
./build/MLDatasetTool.app/Contents/MacOS/MLDatasetTool
```

**Note:** The application is a standard macOS .app bundle and includes all required Qt frameworks.

## Creating a DMG Installer

To create a distributable DMG file for easy installation:

### Method 1: Using create-dmg (Recommended)

```bash
# Install create-dmg
brew install create-dmg

# Create DMG
create-dmg \
  --volname "ML Dataset Tool" \
  --volicon "build/MLDatasetTool.app/Contents/Resources/AppIcon.icns" \
  --window-pos 200 120 \
  --window-size 800 400 \
  --icon-size 100 \
  --icon "MLDatasetTool.app" 200 190 \
  --hide-extension "MLDatasetTool.app" \
  --app-drop-link 600 185 \
  "MLDatasetTool-2.0.dmg" \
  "build/MLDatasetTool.app"
```

### Method 2: Using hdiutil (Built-in)

```bash
# Create a temporary directory
mkdir -p dmg_temp
cp -R build/MLDatasetTool.app dmg_temp/

# Create DMG
hdiutil create -volname "ML Dataset Tool" \
               -srcfolder dmg_temp \
               -ov -format UDZO \
               MLDatasetTool-2.0.dmg

# Clean up
rm -rf dmg_temp
```

### Method 3: Manual DMG Creation

1. **Create a folder** with the app:
   ```bash
   mkdir "ML Dataset Tool"
   cp -R build/MLDatasetTool.app "ML Dataset Tool/"
   ```

2. **Create DMG from Disk Utility**:
   - Open Disk Utility
   - File → New Image → Image from Folder
   - Select "ML Dataset Tool" folder
   - Choose "compressed" format
   - Save as `MLDatasetTool-2.0.dmg`

## Application Bundle Structure

After building, the application bundle has this structure:

```
MLDatasetTool.app/
├── Contents/
│   ├── Info.plist              # Bundle metadata
│   ├── MacOS/
│   │   └── MLDatasetTool       # Executable
│   ├── Frameworks/             # Qt frameworks (bundled)
│   │   ├── QtCore.framework
│   │   ├── QtGui.framework
│   │   ├── QtWidgets.framework
│   │   └── ...
│   ├── PlugIns/                # Qt plugins
│   │   ├── platforms/
│   │   │   └── libqcocoa.dylib
│   │   ├── imageformats/
│   │   │   ├── libqjpeg.dylib
│   │   │   ├── libqpng.dylib
│   │   │   └── ...
│   │   └── styles/
│   └── Resources/              # Resources (icons, etc.)
```

## Troubleshooting

### CMake cannot find Qt5

**Error:**
```
Could not find a package configuration file provided by "Qt5"
```

**Solution:**
Set the `CMAKE_PREFIX_PATH` or `QT_PATH`:
```bash
export QT_PATH=/usr/local/opt/qt@5  # Homebrew Intel
# OR
export QT_PATH=/opt/homebrew/opt/qt@5  # Homebrew Apple Silicon
# OR
export QT_PATH=~/Qt/5.15.2/clang_64  # Official installer

./build_macos.sh
```

### Xcode Command Line Tools not found

**Error:**
```
xcode-select: error: tool 'xcodebuild' requires Xcode
```

**Solution:**
```bash
xcode-select --install
```

### Application won't open (damaged or from unidentified developer)

**Error:**
```
"MLDatasetTool.app" is damaged and can't be opened
```

**Solution:**
This is macOS Gatekeeper protection. To allow the app:

**Option 1: Right-click method**
1. Right-click (or Control-click) on `MLDatasetTool.app`
2. Select "Open"
3. Click "Open" in the dialog

**Option 2: Command line**
```bash
xattr -cr build/MLDatasetTool.app
open build/MLDatasetTool.app
```

**Option 3: System Preferences**
1. System Preferences → Security & Privacy
2. Click "Open Anyway" for MLDatasetTool

### Qt frameworks not bundled

**Error:**
Application crashes with "dyld: Library not loaded" errors.

**Solution:**
Manually run `macdeployqt`:
```bash
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite
```

### Application crashes on startup

**Cause 1: Missing Qt plugins**

Check if the `PlugIns` directory exists in the bundle:
```bash
ls -la build/MLDatasetTool.app/Contents/PlugIns/
```

If missing, run `macdeployqt` again.

**Cause 2: Qt version mismatch**

Ensure you're using the same Qt version for building and deployment:
```bash
$QT_PATH/bin/qmake --version
```

**Solution:**
Clean rebuild:
```bash
rm -rf build
./build_macos.sh
```

### Build fails with "No CMAKE_CXX_COMPILER"

**Error:**
```
No CMAKE_CXX_COMPILER could be found
```

**Solution:**
Install Xcode Command Line Tools:
```bash
xcode-select --install
sudo xcode-select --reset
```

### macdeployqt not found

**Error:**
```
macdeployqt: command not found
```

**Solution:**
Ensure Qt bin directory is in PATH:
```bash
export PATH="$QT_PATH/bin:$PATH"
```

Or specify full path in build script.

### Application runs but images don't load

**Cause:** Qt image format plugins not bundled.

**Solution:**
```bash
# Check if imageformats plugins exist
ls build/MLDatasetTool.app/Contents/PlugIns/imageformats/

# If missing, run macdeployqt
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite
```

### Homebrew Qt conflicts (Intel vs Apple Silicon)

**Issue:** Wrong Qt path for your Mac architecture.

**Solution:**

**Intel Macs:**
```bash
export QT_PATH=/usr/local/opt/qt@5
```

**Apple Silicon Macs (M1/M2/M3):**
```bash
export QT_PATH=/opt/homebrew/opt/qt@5
```

To check your architecture:
```bash
uname -m
# x86_64 = Intel
# arm64 = Apple Silicon
```

## Distribution Checklist

Before distributing your application:

- [ ] Build in Release mode
- [ ] Run `macdeployqt` to bundle Qt frameworks
- [ ] Test on a clean Mac without Qt installed
- [ ] Test on both Intel and Apple Silicon Macs (if targeting both)
- [ ] Consider code signing for Gatekeeper compatibility
- [ ] Create a DMG for easy installation
- [ ] Include README or usage instructions
- [ ] Test the DMG on a clean system

## Code Signing (Optional)

For distribution outside the App Store, consider code signing:

```bash
# Sign the application
codesign --deep --force --verify --verbose \
         --sign "Developer ID Application: Your Name" \
         build/MLDatasetTool.app

# Verify signature
codesign --verify --verbose build/MLDatasetTool.app
spctl --assess --verbose build/MLDatasetTool.app
```

**Note:** Requires an Apple Developer account ($99/year).

## Universal Binary (Intel + Apple Silicon)

To create a universal binary that runs on both Intel and Apple Silicon:

```bash
# Configure for universal build
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH=$QT_PATH \
      -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 \
      ..

# Build
make -j$(sysctl -n hw.ncpu)
```

**Note:** Requires Qt built as universal binary (not available in Homebrew Qt5).

## Additional Resources

- **Qt for macOS**: https://doc.qt.io/qt-5/macos.html
- **CMake macOS**: https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling-for-macos
- **macOS App Bundles**: https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFBundles/BundleTypes/BundleTypes.html
- **Code Signing**: https://developer.apple.com/support/code-signing/

## Getting Help

If you encounter issues not covered here:

1. Check the main troubleshooting guide: `TROUBLESHOOTING.md`
2. Review the macOS-specific troubleshooting: `TROUBLESHOOTING_MACOS.md`
3. Check Qt documentation: https://doc.qt.io/qt-5/
4. Check CMake documentation: https://cmake.org/documentation/

## Quick Command Reference

**Clean build:**
```bash
rm -rf build
./build_macos.sh
```

**Rebuild without cleaning:**
```bash
cd build
make -j$(sysctl -n hw.ncpu)
cd ..
```

**Deploy Qt frameworks:**
```bash
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite
```

**Run application:**
```bash
open build/MLDatasetTool.app
```

**Create DMG:**
```bash
hdiutil create -volname "ML Dataset Tool" -srcfolder build/MLDatasetTool.app -ov -format UDZO MLDatasetTool.dmg
```

**Check bundle contents:**
```bash
ls -la build/MLDatasetTool.app/Contents/
```

**Remove Gatekeeper quarantine:**
```bash
xattr -cr build/MLDatasetTool.app
```


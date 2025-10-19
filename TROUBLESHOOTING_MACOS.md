# macOS Troubleshooting Guide

This guide addresses common issues when building and running the ML Dataset Preparation Tool on macOS.

## Build Issues

### Error: "CMake not found" or "cmake: command not found"

**Cause:** CMake is not installed or not in PATH.

**Solution:**
```bash
# Install via Homebrew
brew install cmake

# Verify installation
cmake --version
```

### Error: "Could not find Qt5"

**Cause:** CMake cannot locate Qt5 installation.

**Solution 1: Set QT_PATH environment variable**
```bash
# For Homebrew Qt (Intel Mac)
export QT_PATH=/usr/local/opt/qt@5

# For Homebrew Qt (Apple Silicon Mac)
export QT_PATH=/opt/homebrew/opt/qt@5

# For official Qt installer
export QT_PATH=~/Qt/5.15.2/clang_64

# Then rebuild
./build_macos.sh
```

**Solution 2: Add to shell profile**
Add to `~/.zshrc` or `~/.bash_profile`:
```bash
export QT_PATH=/usr/local/opt/qt@5  # or your Qt path
export PATH="$QT_PATH/bin:$PATH"
```

Then reload:
```bash
source ~/.zshrc  # or source ~/.bash_profile
```

**Solution 3: Specify in CMake command**
```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/qt5 ..
```

### Error: "xcode-select: error: tool 'xcodebuild' requires Xcode"

**Cause:** Xcode Command Line Tools not installed.

**Solution:**
```bash
# Install Command Line Tools
xcode-select --install

# If already installed but broken, reset
sudo xcode-select --reset

# Verify installation
xcode-select -p
# Should output: /Library/Developer/CommandLineTools
```

### Error: "No CMAKE_CXX_COMPILER could be found"

**Cause:** C++ compiler not found.

**Solution:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Verify clang++ is available
which clang++
clang++ --version
```

### Error: "Qt5 not found at: /usr/local/opt/qt@5"

**Cause:** Qt5 not installed via Homebrew, or wrong path for Apple Silicon.

**Solution:**

**For Intel Macs:**
```bash
brew install qt@5
export QT_PATH=/usr/local/opt/qt@5
./build_macos.sh
```

**For Apple Silicon Macs (M1/M2/M3):**
```bash
brew install qt@5
export QT_PATH=/opt/homebrew/opt/qt@5
./build_macos.sh
```

**Check your architecture:**
```bash
uname -m
# x86_64 = Intel
# arm64 = Apple Silicon
```

### Build succeeds but .app bundle not created

**Cause:** MACOSX_BUNDLE flag not set or build configuration issue.

**Solution:**
```bash
# Clean rebuild
rm -rf build
./build_macos.sh

# Verify CMakeLists.txt has MACOSX_BUNDLE flag
grep MACOSX_BUNDLE CMakeLists.txt
```

### Error: "ld: library not found for -lQt5Core"

**Cause:** Qt libraries not found by linker.

**Solution:**
```bash
# Ensure Qt path is correct
echo $QT_PATH
ls $QT_PATH/lib/

# Clean and rebuild with correct path
rm -rf build
export QT_PATH=/correct/path/to/qt5
./build_macos.sh
```

## Runtime Issues

### Error: "MLDatasetTool.app is damaged and can't be opened"

**Cause:** macOS Gatekeeper blocking unsigned application.

**Solution 1: Right-click to open**
1. Right-click (or Control-click) on `MLDatasetTool.app`
2. Select "Open"
3. Click "Open" in the security dialog

**Solution 2: Remove quarantine attribute**
```bash
xattr -cr build/MLDatasetTool.app
open build/MLDatasetTool.app
```

**Solution 3: System Preferences**
1. System Preferences → Security & Privacy → General
2. Click "Open Anyway" for MLDatasetTool

**Solution 4: Disable Gatekeeper (not recommended)**
```bash
sudo spctl --master-disable
```

### Error: "dyld: Library not loaded: @rpath/QtCore.framework/Versions/5/QtCore"

**Cause:** Qt frameworks not bundled in the .app.

**Solution:**
```bash
# Run macdeployqt manually
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite

# Verify frameworks are bundled
ls build/MLDatasetTool.app/Contents/Frameworks/

# Should see QtCore.framework, QtGui.framework, QtWidgets.framework
```

### Error: "This application failed to start because no Qt platform plugin could be initialized"

**Cause:** Qt platform plugin (libqcocoa.dylib) not found.

**Solution:**
```bash
# Check if platform plugin exists
ls build/MLDatasetTool.app/Contents/PlugIns/platforms/

# If missing, run macdeployqt
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite

# Verify plugin is present
ls build/MLDatasetTool.app/Contents/PlugIns/platforms/libqcocoa.dylib
```

### Application crashes on startup with no error message

**Cause 1: Missing Qt plugins**

**Solution:**
```bash
# Check bundle structure
ls -R build/MLDatasetTool.app/Contents/

# Should have:
# - MacOS/MLDatasetTool (executable)
# - Frameworks/ (Qt frameworks)
# - PlugIns/platforms/ (platform plugin)
# - PlugIns/imageformats/ (image plugins)

# If incomplete, run macdeployqt
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite
```

**Cause 2: Qt version mismatch**

**Solution:**
```bash
# Check Qt version used for build
$QT_PATH/bin/qmake --version

# Clean rebuild with correct Qt
rm -rf build
./build_macos.sh
```

**Cause 3: Architecture mismatch**

**Solution:**
```bash
# Check executable architecture
file build/MLDatasetTool.app/Contents/MacOS/MLDatasetTool

# Should match your Mac:
# Intel Mac: Mach-O 64-bit executable x86_64
# Apple Silicon: Mach-O 64-bit executable arm64

# If wrong, rebuild with correct Qt for your architecture
```

### Images don't load or display incorrectly

**Cause:** Qt image format plugins not bundled.

**Solution:**
```bash
# Check if image plugins exist
ls build/MLDatasetTool.app/Contents/PlugIns/imageformats/

# Should see: libqjpeg.dylib, libqpng.dylib, etc.

# If missing, run macdeployqt
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite
```

### File dialogs don't appear or look wrong

**Cause:** Qt platform integration issue.

**Solution:**
```bash
# Ensure libqcocoa.dylib is present
ls build/MLDatasetTool.app/Contents/PlugIns/platforms/libqcocoa.dylib

# If missing, run macdeployqt
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite
```

### Application menu bar is empty or shows wrong name

**Cause:** Info.plist not properly configured.

**Solution:**
Check `build/MLDatasetTool.app/Contents/Info.plist`:
```bash
cat build/MLDatasetTool.app/Contents/Info.plist | grep CFBundleName
```

Should show: `<string>ML Dataset Tool</string>`

If incorrect, rebuild:
```bash
rm -rf build
./build_macos.sh
```

## Distribution Issues

### Application works on development Mac but not on other Macs

**Cause:** Qt frameworks not properly bundled or dependencies missing.

**Solution:**

**1. Verify Qt frameworks are bundled:**
```bash
ls build/MLDatasetTool.app/Contents/Frameworks/
# Should see Qt frameworks
```

**2. Check dependencies:**
```bash
otool -L build/MLDatasetTool.app/Contents/MacOS/MLDatasetTool
# All Qt paths should be @rpath or @executable_path
```

**3. Run macdeployqt:**
```bash
$QT_PATH/bin/macdeployqt build/MLDatasetTool.app -always-overwrite
```

**4. Test on clean Mac:**
- Test on a Mac without Qt installed
- Test on both Intel and Apple Silicon if targeting both

### DMG creation fails

**Error:**
```
hdiutil: create failed - Resource busy
```

**Solution:**
```bash
# Unmount any existing volumes
hdiutil detach "/Volumes/ML Dataset Tool" 2>/dev/null

# Remove old DMG
rm -f MLDatasetTool.dmg

# Try again
hdiutil create -volname "ML Dataset Tool" \
               -srcfolder build/MLDatasetTool.app \
               -ov -format UDZO \
               MLDatasetTool.dmg
```

### Application won't run on older macOS versions

**Cause:** Deployment target too high.

**Solution:**
Set minimum macOS version in CMake:
```bash
cmake -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 ..
```

Supported versions:
- 10.14 (Mojave) - Recommended minimum
- 10.15 (Catalina)
- 11.0 (Big Sur)
- 12.0 (Monterey)
- 13.0 (Ventura)
- 14.0 (Sonoma)

### Code signing issues

**Error:**
```
code object is not signed at all
```

**Solution (for distribution):**
```bash
# Sign the application (requires Apple Developer account)
codesign --deep --force --verify --verbose \
         --sign "Developer ID Application: Your Name" \
         build/MLDatasetTool.app

# Verify signature
codesign --verify --verbose build/MLDatasetTool.app
```

**For development (ad-hoc signing):**
```bash
codesign --deep --force --sign - build/MLDatasetTool.app
```

## Homebrew Issues

### Qt@5 conflicts with Qt6

**Issue:** Both Qt5 and Qt6 installed, causing conflicts.

**Solution:**
```bash
# Unlink Qt6
brew unlink qt

# Link Qt5
brew link qt@5 --force

# Set Qt path explicitly
export QT_PATH=/usr/local/opt/qt@5  # Intel
# OR
export QT_PATH=/opt/homebrew/opt/qt@5  # Apple Silicon
```

### Homebrew Qt not found after installation

**Cause:** Qt@5 is "keg-only" and not linked by default.

**Solution:**
```bash
# Link Qt5
brew link qt@5 --force

# Or set path manually
export QT_PATH=$(brew --prefix qt@5)
echo $QT_PATH
```

### Wrong Qt version selected

**Issue:** Multiple Qt versions installed.

**Solution:**
```bash
# List installed Qt versions
brew list | grep qt

# Specify exact version
export QT_PATH=/usr/local/opt/qt@5  # or /opt/homebrew/opt/qt@5

# Verify
$QT_PATH/bin/qmake --version
```

## Apple Silicon (M1/M2/M3) Specific Issues

### Rosetta 2 vs Native

**Issue:** Running Intel binary on Apple Silicon.

**Check architecture:**
```bash
file build/MLDatasetTool.app/Contents/MacOS/MLDatasetTool
```

**For native Apple Silicon:**
- Use Homebrew Qt from `/opt/homebrew/opt/qt@5`
- Ensure Xcode Command Line Tools are native (not Rosetta)

**For Intel (Rosetta 2):**
- Use Homebrew Qt from `/usr/local/opt/qt@5`
- May need to install Intel Homebrew separately

### Homebrew path confusion

**Issue:** Wrong Homebrew installation used.

**Solution:**
```bash
# Check which Homebrew
which brew

# Apple Silicon native: /opt/homebrew/bin/brew
# Intel (Rosetta): /usr/local/bin/brew

# Set correct Qt path
export QT_PATH=/opt/homebrew/opt/qt@5  # Apple Silicon
# OR
export QT_PATH=/usr/local/opt/qt@5  # Intel
```

## Performance Issues

### Application is slow to start

**Cause:** Gatekeeper verification on first launch.

**Solution:**
This is normal for first launch. Subsequent launches will be faster.

To skip verification (development only):
```bash
xattr -cr build/MLDatasetTool.app
```

### High memory usage

**Cause:** Loading large images.

**Solution:**
This is expected behavior. The application loads full images into memory.
- Close and reopen periodically
- Process images in smaller batches

## Development Issues

### Qt Creator not finding CMake project

**Solution:**
1. Open Qt Creator
2. File → Open File or Project
3. Select `CMakeLists.txt` in project root
4. Configure with correct Qt kit

### Changes not reflected after rebuild

**Cause:** Incremental build issue.

**Solution:**
```bash
# Clean build
rm -rf build
./build_macos.sh

# Or clean in build directory
cd build
make clean
make -j$(sysctl -n hw.ncpu)
cd ..
```

### Qt MOC errors

**Cause:** Qt meta-object compiler issues.

**Solution:**
```bash
# Ensure CMAKE_AUTOMOC is ON (it is by default)
grep CMAKE_AUTOMOC CMakeLists.txt

# Clean rebuild
rm -rf build
./build_macos.sh
```

## Getting Help

If you encounter issues not covered here:

1. **Check main troubleshooting guide:** `TROUBLESHOOTING.md`
2. **Review build documentation:** `BUILD_MACOS.md`
3. **Check Qt documentation:** https://doc.qt.io/qt-5/macos.html
4. **Check CMake documentation:** https://cmake.org/documentation/
5. **Check Apple Developer documentation:** https://developer.apple.com/documentation/

## Common Command Reference

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

**Check bundle structure:**
```bash
ls -R build/MLDatasetTool.app/Contents/
```

**Check dependencies:**
```bash
otool -L build/MLDatasetTool.app/Contents/MacOS/MLDatasetTool
```

**Remove Gatekeeper quarantine:**
```bash
xattr -cr build/MLDatasetTool.app
```

**Run from terminal (see errors):**
```bash
./build/MLDatasetTool.app/Contents/MacOS/MLDatasetTool
```

**Check architecture:**
```bash
file build/MLDatasetTool.app/Contents/MacOS/MLDatasetTool
lipo -info build/MLDatasetTool.app/Contents/MacOS/MLDatasetTool
```

**Verify code signature:**
```bash
codesign --verify --verbose build/MLDatasetTool.app
spctl --assess --verbose build/MLDatasetTool.app
```


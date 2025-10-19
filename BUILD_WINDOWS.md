# Building ML Dataset Preparation Tool on Windows 10

This guide provides step-by-step instructions for building the ML Dataset Preparation Tool on Windows 10.

## Prerequisites

### 1. Install Visual Studio 2019 or 2022

Download and install Visual Studio Community Edition (free):
- **Visual Studio 2019**: https://visualstudio.microsoft.com/vs/older-downloads/
- **Visual Studio 2022**: https://visualstudio.microsoft.com/vs/

**Required Components:**
- Desktop development with C++
- Windows 10 SDK
- CMake tools for Windows (optional, but recommended)

### 2. Install CMake

Download and install CMake from: https://cmake.org/download/

**Recommended version:** 3.20 or later

During installation:
- ✅ Check "Add CMake to system PATH for all users"

Verify installation:
```cmd
cmake --version
```

### 3. Install Qt5 for Windows

Download Qt5 Open Source from: https://www.qt.io/download-qt-installer

**Recommended version:** Qt 5.15.2 (LTS)

**Installation Steps:**
1. Run the Qt installer
2. Create a Qt account (free)
3. Select components to install:
   - ✅ Qt 5.15.2 → MSVC 2019 64-bit (or MSVC 2017 64-bit)
   - ✅ Qt 5.15.2 → Sources (optional, for debugging)
   - ✅ Developer and Designer Tools → CMake

**Default installation path:** `C:\Qt\5.15.2\msvc2019_64`

**Important:** Note your Qt installation path - you'll need it for building.

## Build Instructions

### Method 1: Using CMake GUI (Recommended for Beginners)

#### Step 1: Open CMake GUI
- Launch "CMake (cmake-gui)" from Start Menu

#### Step 2: Configure Paths
- **Where is the source code:** Browse to your project folder (e.g., `C:\Users\YourName\projects\image-classification`)
- **Where to build the binaries:** Create a `build` subfolder (e.g., `C:\Users\YourName\projects\image-classification\build`)

#### Step 3: Configure Project
1. Click **"Configure"**
2. Select generator:
   - **Visual Studio 16 2019** (for VS 2019)
   - **Visual Studio 17 2022** (for VS 2022)
3. Select platform: **x64**
4. Click **"Finish"**

#### Step 4: Set Qt Path (if not found automatically)
If CMake shows an error "Could not find Qt5":
1. Click **"Add Entry"**
2. Add the following:
   - **Name:** `CMAKE_PREFIX_PATH`
   - **Type:** `PATH`
   - **Value:** `C:\Qt\5.15.2\msvc2019_64` (adjust to your Qt installation path)
3. Click **"Configure"** again

#### Step 5: Generate Build Files
1. Click **"Generate"**
2. Wait for "Generating done" message

#### Step 6: Build the Project
1. Click **"Open Project"** (opens Visual Studio)
2. In Visual Studio:
   - Select **"Release"** configuration (top toolbar)
   - Right-click on **"ImageClassificationTool"** project → **"Build"**
3. Wait for build to complete

#### Step 7: Locate the Executable
The executable will be at:
```
build\Release\MLDatasetTool.exe
```

### Method 2: Using Command Line (Advanced)

#### Step 1: Open Developer Command Prompt
- Start Menu → Visual Studio 2019/2022 → **"x64 Native Tools Command Prompt"**

#### Step 2: Navigate to Project Directory
```cmd
cd C:\Users\YourName\projects\image-classification
```

#### Step 3: Create Build Directory
```cmd
mkdir build
cd build
```

#### Step 4: Configure with CMake
```cmd
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64 ..
```

**For Visual Studio 2022:**
```cmd
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64 ..
```

**Note:** Adjust `CMAKE_PREFIX_PATH` to match your Qt installation path.

#### Step 5: Build the Project
```cmd
cmake --build . --config Release
```

#### Step 6: Locate the Executable
```cmd
dir Release\MLDatasetTool.exe
```

### Method 3: Using Build Script (Automated)

Create a `build_windows.bat` file in your project root:

```batch
@echo off
echo Building ML Dataset Preparation Tool for Windows...

REM Set Qt path (ADJUST THIS TO YOUR QT INSTALLATION)
set QT_PATH=C:\Qt\5.15.2\msvc2019_64

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH=%QT_PATH% ..
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b %errorlevel%
)

REM Build the project
echo Building project...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo.
echo Build completed successfully!
echo Executable location: build\Release\MLDatasetTool.exe
echo.
pause
```

**Usage:**
1. Edit `QT_PATH` in the script to match your Qt installation
2. Double-click `build_windows.bat` to build

## Running the Application

### Option 1: Run from Build Directory
After building, the executable is located at:
```
build\Release\MLDatasetTool.exe
```

**Important:** The first time you run it, CMake will automatically copy required Qt DLLs to the same directory using `windeployqt`.

Simply double-click `MLDatasetTool.exe` to launch the application.

### Option 2: Create Standalone Distribution

To create a portable version that can run on other Windows 10 machines:

#### Step 1: Create Distribution Folder
```cmd
mkdir MLDatasetTool_Portable
```

#### Step 2: Copy Executable
```cmd
copy build\Release\MLDatasetTool.exe MLDatasetTool_Portable\
```

#### Step 3: Run windeployqt (if not already done)
```cmd
C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe MLDatasetTool_Portable\MLDatasetTool.exe
```

This copies all required Qt DLLs and plugins to the folder.

#### Step 4: Test the Portable Version
Navigate to `MLDatasetTool_Portable` and double-click `MLDatasetTool.exe`.

#### Step 5: Distribute
Zip the entire `MLDatasetTool_Portable` folder and share it. Users can extract and run without installing Qt.

## Troubleshooting

### Error: "Could not find Qt5"
**Solution:** Set `CMAKE_PREFIX_PATH` to your Qt installation directory:
```cmd
-DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64
```

### Error: "MSVCP140.dll is missing"
**Solution:** Install Visual C++ Redistributable:
- Download from: https://aka.ms/vs/16/release/vc_redist.x64.exe
- Install and restart

### Error: "Qt5Core.dll is missing"
**Solution:** Run `windeployqt` to copy Qt DLLs:
```cmd
C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe build\Release\MLDatasetTool.exe
```

### Application shows console window
**Solution:** This shouldn't happen with the updated CMakeLists.txt (WIN32 flag). If it does:
1. Rebuild the project from scratch
2. Ensure you're building in Release mode

### Qt plugins not found
**Solution:** Ensure `windeployqt` was run. It should create a `platforms` folder next to the .exe with `qwindows.dll`.

## Building for Distribution

### Creating an Installer (Optional)

For professional distribution, consider using:
- **Inno Setup**: https://jrsoftware.org/isinfo.php (free)
- **NSIS**: https://nsis.sourceforge.io/ (free)
- **WiX Toolset**: https://wixtoolset.org/ (free, more complex)

### Code Signing (Optional)

For trusted distribution:
1. Obtain a code signing certificate
2. Sign the executable with `signtool.exe`

## Development Tips

### Using Visual Studio IDE
After generating with CMake, you can open the solution file:
```
build\ImageClassificationTool.sln
```

This allows you to:
- Edit code with IntelliSense
- Debug with breakpoints
- Use Visual Studio's powerful tools

### Debugging
1. Build in **Debug** configuration instead of **Release**
2. Set `ImageClassificationTool` as startup project
3. Press F5 to debug

### Incremental Builds
After making code changes:
```cmd
cd build
cmake --build . --config Release
```

No need to re-run CMake configuration unless you add/remove files.

## System Requirements

### Development Machine
- Windows 10 (64-bit)
- 4 GB RAM minimum (8 GB recommended)
- 10 GB free disk space
- Visual Studio 2019 or 2022
- Qt 5.15.2

### Target Machines (for distribution)
- Windows 10 (64-bit)
- Visual C++ Redistributable 2019 (included with windeployqt)
- No Qt installation required (DLLs bundled)

## Next Steps

After successful build:
1. Test both Image Classification and Object Detection modes
2. Review `README.md` for usage instructions
3. See `TESTING.md` for comprehensive testing procedures
4. Check `TROUBLESHOOTING.md` for common issues

## Support

For build issues specific to Windows:
- Check Qt documentation: https://doc.qt.io/qt-5/windows.html
- CMake documentation: https://cmake.org/documentation/
- Visual Studio documentation: https://docs.microsoft.com/en-us/visualstudio/

For application-specific issues, see `TROUBLESHOOTING.md` in the project root.


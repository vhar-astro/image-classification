# Windows Troubleshooting Guide

This guide addresses common issues when building and running the ML Dataset Preparation Tool on Windows 10.

## Build Issues

### Error: "CMake not found" or "cmake is not recognized"

**Cause:** CMake is not installed or not in system PATH.

**Solution:**
1. Download CMake from https://cmake.org/download/
2. During installation, select "Add CMake to system PATH for all users"
3. If already installed, manually add to PATH:
   - Open System Properties → Environment Variables
   - Edit "Path" variable
   - Add: `C:\Program Files\CMake\bin`
4. Restart command prompt and verify: `cmake --version`

### Error: "Could not find Qt5"

**Cause:** CMake cannot locate Qt5 installation.

**Solution 1: Set Qt Path in Build Script**
Edit `build_windows.bat` and set the correct Qt path:
```batch
set QT_PATH=C:\Qt\5.15.2\msvc2019_64
```

**Solution 2: Set CMAKE_PREFIX_PATH**
When running CMake manually:
```cmd
cmake -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64 ..
```

**Solution 3: Set Qt5_DIR Environment Variable**
```cmd
set Qt5_DIR=C:\Qt\5.15.2\msvc2019_64\lib\cmake\Qt5
```

### Error: "No CMAKE_CXX_COMPILER could be found"

**Cause:** Visual Studio C++ compiler not found.

**Solution:**
1. Install Visual Studio 2019 or 2022
2. During installation, select "Desktop development with C++"
3. Run CMake from "x64 Native Tools Command Prompt for VS"
   - Start Menu → Visual Studio 2019/2022 → x64 Native Tools Command Prompt

### Error: "Generator: Visual Studio 16 2019 not found"

**Cause:** Visual Studio 2019 not installed, but script is configured for it.

**Solution:**
Edit `build_windows.bat` and change:
```batch
set VS_VERSION=2022
```

Or install Visual Studio 2019 from:
https://visualstudio.microsoft.com/vs/older-downloads/

### Error: "MSB8036: The Windows SDK version X was not found"

**Cause:** Required Windows SDK not installed.

**Solution:**
1. Open Visual Studio Installer
2. Modify your Visual Studio installation
3. Under "Individual components", install:
   - Windows 10 SDK (latest version)
4. Rebuild the project

### Build succeeds but executable not found

**Cause:** Looking in wrong directory.

**Solution:**
The executable is in the configuration subdirectory:
- Release build: `build\Release\MLDatasetTool.exe`
- Debug build: `build\Debug\MLDatasetTool.exe`

Check which configuration you built:
```cmd
dir build\Release\*.exe
dir build\Debug\*.exe
```

## Runtime Issues

### Error: "MSVCP140.dll is missing" or "VCRUNTIME140.dll is missing"

**Cause:** Visual C++ Redistributable not installed.

**Solution:**
Download and install Visual C++ Redistributable:
- For VS 2019/2022: https://aka.ms/vs/16/release/vc_redist.x64.exe
- Install and restart your computer

### Error: "Qt5Core.dll is missing" or other Qt DLL errors

**Cause:** Qt DLLs not deployed with executable.

**Solution 1: Run windeployqt**
```cmd
C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe build\Release\MLDatasetTool.exe
```

**Solution 2: Rebuild with automatic deployment**
The CMakeLists.txt should automatically run windeployqt. If it didn't:
1. Delete the build folder
2. Run `build_windows.bat` again

**Solution 3: Manual DLL copying**
Copy these DLLs from `C:\Qt\5.15.2\msvc2019_64\bin\` to the same folder as the .exe:
- Qt5Core.dll
- Qt5Gui.dll
- Qt5Widgets.dll

Also copy the `platforms` folder containing `qwindows.dll`.

### Error: "This application failed to start because no Qt platform plugin could be initialized"

**Cause:** Qt platform plugin (qwindows.dll) not found.

**Solution:**
Ensure the `platforms` folder exists next to the executable:
```
build\Release\
├── MLDatasetTool.exe
├── Qt5Core.dll
├── Qt5Gui.dll
├── Qt5Widgets.dll
└── platforms\
    └── qwindows.dll
```

Run windeployqt to create this structure:
```cmd
C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe build\Release\MLDatasetTool.exe
```

### Application shows a console window

**Cause:** Built without WIN32 flag or in Debug mode.

**Solution:**
1. Ensure you're building in Release mode:
   ```cmd
   cmake --build . --config Release
   ```
2. The CMakeLists.txt should have `WIN32` flag for Windows builds
3. Rebuild from scratch if issue persists

### Application crashes on startup

**Cause 1: Missing DLLs**
Run Dependency Walker or use Process Monitor to identify missing DLLs.

**Cause 2: Qt version mismatch**
Ensure the Qt version used to build matches the DLLs deployed.

**Solution:**
1. Delete build folder
2. Verify Qt installation path in build script
3. Rebuild completely:
   ```cmd
   rmdir /s /q build
   build_windows.bat
   ```

### Images don't load or display incorrectly

**Cause:** Qt image plugins not deployed.

**Solution:**
Run windeployqt which should copy image plugins:
```cmd
C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe build\Release\MLDatasetTool.exe
```

This creates an `imageformats` folder with plugins for JPG, PNG, BMP support.

### File dialogs don't appear or crash

**Cause:** Qt platform integration issue.

**Solution:**
Ensure `qwindows.dll` is in the `platforms` subfolder next to the executable.

### Application runs but UI looks wrong (missing styles)

**Cause:** Qt style plugins not deployed.

**Solution:**
Run windeployqt with additional options:
```cmd
C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe --no-translations build\Release\MLDatasetTool.exe
```

## Distribution Issues

### Application works on development machine but not on other computers

**Cause:** Missing Visual C++ Redistributable or Qt DLLs.

**Solution:**
Create a proper distribution package:

1. **Copy all files from build\Release\ folder**
   ```cmd
   xcopy /E /I build\Release MLDatasetTool_Portable
   ```

2. **Run windeployqt on the copy**
   ```cmd
   C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe MLDatasetTool_Portable\MLDatasetTool.exe
   ```

3. **Include Visual C++ Redistributable installer**
   Download `vc_redist.x64.exe` and include it with instructions to install

4. **Test on a clean Windows 10 VM**
   Verify it works without Qt or Visual Studio installed

### Antivirus flags the executable

**Cause:** Unsigned executable may trigger false positives.

**Solution:**
1. **For personal use:** Add exception in antivirus
2. **For distribution:** Consider code signing
   - Obtain a code signing certificate
   - Sign with `signtool.exe`:
     ```cmd
     signtool sign /f certificate.pfx /p password /t http://timestamp.digicert.com MLDatasetTool.exe
     ```

## Performance Issues

### Application is slow to start

**Cause:** Antivirus scanning or large Qt plugins.

**Solution:**
1. Add executable folder to antivirus exclusions
2. Use `--no-translations` flag with windeployqt to reduce file count
3. Remove unused Qt plugins from the distribution

### High memory usage

**Cause:** Loading large images or many annotations.

**Solution:**
This is expected behavior. The application loads full images into memory.
- Close and reopen the application periodically
- Process images in smaller batches

## Development Issues

### IntelliSense not working in Visual Studio

**Cause:** CMake project not properly loaded.

**Solution:**
1. Open the solution file: `build\ImageClassificationTool.sln`
2. Or use "Open Folder" in Visual Studio and open the project root
3. Visual Studio should automatically detect CMakeLists.txt

### Changes to code not reflected after rebuild

**Cause:** Incremental build issue.

**Solution:**
Clean and rebuild:
```cmd
cd build
cmake --build . --config Release --clean-first
```

Or delete build folder and rebuild from scratch.

### Qt MOC errors

**Cause:** Qt meta-object compiler issues.

**Solution:**
1. Ensure `CMAKE_AUTOMOC` is ON in CMakeLists.txt (it is by default)
2. Clean build folder and rebuild
3. Verify Q_OBJECT macro is present in class declarations

## Getting Help

If you encounter issues not covered here:

1. **Check the main troubleshooting guide:** `TROUBLESHOOTING.md`
2. **Review build documentation:** `BUILD_WINDOWS.md`
3. **Check Qt documentation:** https://doc.qt.io/qt-5/windows.html
4. **CMake documentation:** https://cmake.org/documentation/
5. **Visual Studio documentation:** https://docs.microsoft.com/en-us/visualstudio/

## Common Command Reference

**Clean build:**
```cmd
rmdir /s /q build
build_windows.bat
```

**Rebuild without reconfiguring:**
```cmd
cd build
cmake --build . --config Release
```

**Deploy Qt dependencies:**
```cmd
C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe build\Release\MLDatasetTool.exe
```

**Check for missing DLLs:**
```cmd
dumpbin /dependents build\Release\MLDatasetTool.exe
```

**Run from command line to see error messages:**
```cmd
cd build\Release
MLDatasetTool.exe
```


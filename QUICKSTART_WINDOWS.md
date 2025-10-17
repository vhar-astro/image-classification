# Quick Start Guide for Windows 10

Get the ML Dataset Preparation Tool running on Windows 10 in under 30 minutes!

## Step 1: Install Prerequisites (15 minutes)

### 1.1 Install Visual Studio 2019 or 2022

**Download:** https://visualstudio.microsoft.com/vs/community/

**Installation:**
1. Run the installer
2. Select **"Desktop development with C++"**
3. Click Install (this will take 10-15 minutes)

### 1.2 Install CMake

**Download:** https://cmake.org/download/ (Windows x64 Installer)

**Installation:**
1. Run the installer
2. ✅ Check **"Add CMake to system PATH for all users"**
3. Click Install

### 1.3 Install Qt5

**Download:** https://www.qt.io/download-qt-installer

**Installation:**
1. Run the Qt installer
2. Create a free Qt account
3. Select components:
   - ✅ Qt 5.15.2 → MSVC 2019 64-bit
4. Click Install (this will take 5-10 minutes)
5. **Note the installation path** (usually `C:\Qt\5.15.2\msvc2019_64`)

## Step 2: Configure Build Script (2 minutes)

1. Open `build_windows.bat` in Notepad
2. Find this line:
   ```batch
   set QT_PATH=C:\Qt\5.15.2\msvc2019_64
   ```
3. Change it to match your Qt installation path (if different)
4. Save and close

## Step 3: Build the Application (5 minutes)

1. Double-click `build_windows.bat`
2. Wait for the build to complete
3. You should see: **"BUILD COMPLETED SUCCESSFULLY!"**

**Output location:** `build\Release\MLDatasetTool.exe`

## Step 4: Run the Application

### Option 1: Double-click the executable
Navigate to `build\Release\` and double-click `MLDatasetTool.exe`

### Option 2: Use the launcher script
Double-click `run_windows.bat` in the project root

## Step 5: Start Using the Tool

### First Launch
1. A mode selection dialog will appear
2. Choose either:
   - **Image Classification** - for categorizing entire images
   - **Object Detection** - for drawing bounding boxes

### Image Classification Mode
1. Click **"Open Folder"** to load images
2. Type a category name and click **"Add Category"**
3. Select a category from the dropdown
4. Click **"Classify and Move Image"**
5. Repeat for all images
6. Find organized images in `classified_images\<category>\`

### Object Detection Mode
1. Click **"Open Folder"** to load images
2. Type a label name (e.g., "person") and click **"Add Label"**
3. Click and drag on the image to draw a bounding box
4. Select the label for the object
5. Click **"Save & Next"** to save and move to next image
6. Find annotations in `annotated_images\`

## Troubleshooting

### "CMake not found"
- Reinstall CMake and ensure "Add to PATH" is checked
- Restart your computer

### "Could not find Qt5"
- Edit `build_windows.bat` and set the correct `QT_PATH`
- Make sure Qt 5.15.2 MSVC 2019 64-bit is installed

### "MSVCP140.dll is missing"
- Install Visual C++ Redistributable: https://aka.ms/vs/16/release/vc_redist.x64.exe

### "Qt5Core.dll is missing"
- The build script should have copied Qt DLLs automatically
- If not, run this command:
  ```cmd
  C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe build\Release\MLDatasetTool.exe
  ```

### Build fails
- Make sure Visual Studio is fully installed with C++ support
- Try running from "x64 Native Tools Command Prompt for VS 2019"
- See `TROUBLESHOOTING_WINDOWS.md` for detailed solutions

## Creating a Portable Version

To share the application with others:

1. Copy the entire `build\Release\` folder
2. Rename it to `MLDatasetTool_Portable`
3. Zip the folder
4. Share the zip file

Recipients can extract and run `MLDatasetTool.exe` without installing Qt or Visual Studio!

## Next Steps

- **Full documentation:** See `README.md`
- **Detailed build guide:** See `BUILD_WINDOWS.md`
- **Usage examples:** See `USAGE_EXAMPLES.md`
- **Testing guide:** See `TESTING.md`

## Support

For more help:
- **Windows-specific issues:** `TROUBLESHOOTING_WINDOWS.md`
- **General issues:** `TROUBLESHOOTING.md`
- **Build details:** `BUILD_WINDOWS.md`

## Summary

✅ Install Visual Studio 2019/2022 with C++  
✅ Install CMake (add to PATH)  
✅ Install Qt 5.15.2 MSVC 2019 64-bit  
✅ Edit `build_windows.bat` with your Qt path  
✅ Run `build_windows.bat`  
✅ Double-click `build\Release\MLDatasetTool.exe`  

**Total time:** ~30 minutes (mostly waiting for installations)

Enjoy preparing your ML datasets! 🎉


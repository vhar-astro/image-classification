# Quick Start Guide for macOS

Get the ML Dataset Preparation Tool running on macOS in under 20 minutes!

## Step 1: Install Prerequisites (10-15 minutes)

### 1.1 Install Xcode Command Line Tools

```bash
xcode-select --install
```

Click "Install" in the dialog that appears. This takes about 5 minutes.

**Verify:**
```bash
xcode-select -p
# Should output: /Library/Developer/CommandLineTools
```

### 1.2 Install Homebrew (if not already installed)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**For Apple Silicon Macs (M1/M2/M3)**, add to PATH:
```bash
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
eval "$(/opt/homebrew/bin/brew shellenv)"
```

### 1.3 Install CMake and Qt5

```bash
# Install both in one command
brew install cmake qt@5
```

This takes about 5-10 minutes.

### 1.4 Set Qt Path

**Intel Macs:**
```bash
export QT_PATH=/usr/local/opt/qt@5
```

**Apple Silicon Macs (M1/M2/M3):**
```bash
export QT_PATH=/opt/homebrew/opt/qt@5
```

**To make permanent**, add to `~/.zshrc`:
```bash
echo 'export QT_PATH=/opt/homebrew/opt/qt@5' >> ~/.zshrc  # Apple Silicon
# OR
echo 'export QT_PATH=/usr/local/opt/qt@5' >> ~/.zshrc  # Intel
```

## Step 2: Build the Application (3-5 minutes)

```bash
# Make build script executable
chmod +x build_macos.sh

# Run build script
./build_macos.sh
```

**That's it!** The script handles everything automatically.

## Step 3: Run the Application

### Option 1: Double-click
Navigate to the `build` folder in Finder and double-click `MLDatasetTool.app`.

### Option 2: Use launcher script
```bash
chmod +x run_macos.sh
./run_macos.sh
```

### Option 3: Use open command
```bash
open build/MLDatasetTool.app
```

## Step 4: Start Using the Tool

### First Launch

If you see "MLDatasetTool.app can't be opened because it is from an unidentified developer":

1. **Right-click** (or Control-click) on `MLDatasetTool.app`
2. Select **"Open"**
3. Click **"Open"** in the security dialog

This only needs to be done once.

### Mode Selection

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
6. Find organized images in `classified_images/<category>/`

### Object Detection Mode

1. Click **"Open Folder"** to load images
2. Type a label name (e.g., "person") and click **"Add Label"**
3. Click and drag on the image to draw a bounding box
4. Select the label for the object
5. Click **"Save & Next"** to save and move to next image
6. Find annotations in `annotated_images/`

## Troubleshooting

### "CMake not found"
```bash
brew install cmake
```

### "Could not find Qt5"
```bash
# Set Qt path for your Mac type
export QT_PATH=/opt/homebrew/opt/qt@5  # Apple Silicon
# OR
export QT_PATH=/usr/local/opt/qt@5  # Intel

# Rebuild
./build_macos.sh
```

### "xcode-select: error"
```bash
xcode-select --install
```

### "MLDatasetTool.app is damaged"
```bash
# Remove quarantine attribute
xattr -cr build/MLDatasetTool.app
open build/MLDatasetTool.app
```

### Build fails
```bash
# Clean and rebuild
rm -rf build
./build_macos.sh
```

### Not sure if you have Intel or Apple Silicon?
```bash
uname -m
# x86_64 = Intel Mac
# arm64 = Apple Silicon (M1/M2/M3)
```

## Creating a Distributable DMG

To share the application with others:

```bash
# Install create-dmg
brew install create-dmg

# Create DMG
create-dmg \
  --volname "ML Dataset Tool" \
  --window-pos 200 120 \
  --window-size 800 400 \
  --icon-size 100 \
  --app-drop-link 600 185 \
  "MLDatasetTool.dmg" \
  "build/MLDatasetTool.app"
```

Recipients can:
1. Open the DMG
2. Drag `MLDatasetTool.app` to Applications
3. Run it!

## Next Steps

- **Full documentation:** See `README.md`
- **Detailed build guide:** See `BUILD_MACOS.md`
- **Usage examples:** See `USAGE_EXAMPLES.md`
- **Testing guide:** See `TESTING.md`

## Support

For more help:
- **macOS-specific issues:** `TROUBLESHOOTING_MACOS.md`
- **General issues:** `TROUBLESHOOTING.md`
- **Build details:** `BUILD_MACOS.md`

## Summary

✅ Install Xcode Command Line Tools  
✅ Install Homebrew  
✅ Install CMake and Qt5 via Homebrew  
✅ Set QT_PATH for your Mac type  
✅ Run `./build_macos.sh`  
✅ Open `build/MLDatasetTool.app`  

**Total time:** ~20 minutes (mostly waiting for installations)

Enjoy preparing your ML datasets on macOS! 🎉


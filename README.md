# ML Dataset Preparation Tool

A cross-platform C++ GUI application for **Windows 10**, **macOS**, and **Linux/Ubuntu** that assists with preparing machine learning datasets. This dual-mode tool provides intuitive interfaces for both **image classification** and **object detection** dataset preparation.

## Features

### Dual-Mode Operation
- **Mode Selection**: Choose between Image Classification or Object Detection mode on startup
- **Image Classification Mode**: Assign single categories to entire images
- **Object Detection Mode**: Draw bounding boxes and label objects within images

### Image Classification Mode
- **Image Loading**: Open individual images or entire folders containing JPG, PNG, and BMP files
- **Visual Classification**: View images one at a time with a clear, resizable display
- **Category Management**: Create custom categories on-the-fly or select from existing ones
- **Automatic Organization**: Images are automatically copied to category-specific folders
- **Progress Tracking**: Real-time progress bar and counter showing classification status
- **Navigation**: Move forward, backward, or skip images during the classification process

### Object Detection Mode
- **Interactive Bounding Boxes**: Draw boxes by clicking and dragging on images
- **Label Management**: Create and manage object labels (e.g., "person", "car", "dog")
- **Box Editing**: Select, resize, and delete bounding boxes
- **YOLO Format Export**: Annotations saved in YOLO format (normalized coordinates)
- **Visual Feedback**: Color-coded boxes with labels displayed on image
- **Batch Processing**: Navigate through multiple images with auto-save functionality
- **Non-destructive Workflow**: Original images are copied to output directory

### User Interface
- Clean, intuitive Qt-based GUI
- Image scaling to fit window while maintaining aspect ratio
- Image information display (filename, dimensions, file size)
- Progress indicators (current image number, total images, processed count)
- Category dropdown with quick-add functionality

### File Management
- Non-destructive workflow (copies images instead of moving them)
- Automatic creation of output folders
- Duplicate filename handling with timestamp suffixes
- **Classification Output**: `classified_images/<category_name>/image.jpg`
- **Detection Output**: `annotated_images/images/` and `annotated_images/labels/`

## Prerequisites

### System Requirements

**Windows 10:**
- Windows 10 (64-bit)
- Visual Studio 2019 or 2022 (Community Edition is free)
- CMake 3.10 or later
- Qt5 (5.15.2 recommended)

**macOS:**
- macOS 10.14 (Mojave) or later
- Xcode Command Line Tools
- CMake 3.10 or later
- Qt5 (5.12 or later, 5.15.2 recommended)

**Linux/Ubuntu:**
- Ubuntu 18.04 or later (or other Linux distributions)
- C++ compiler with C++11 support (g++ or clang++)
- CMake 3.10 or later
- Qt5 development libraries

### Installing Dependencies

#### Windows 10

See **[BUILD_WINDOWS.md](BUILD_WINDOWS.md)** for comprehensive Windows installation and build instructions.

**Quick Summary:**
1. Install Visual Studio 2019/2022 with "Desktop development with C++"
2. Install CMake from https://cmake.org/download/
3. Install Qt5 from https://www.qt.io/download-qt-installer
4. Run `build_windows.bat` to build

#### macOS

See **[BUILD_MACOS.md](BUILD_MACOS.md)** for comprehensive macOS installation and build instructions.

**Quick Summary:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake and Qt5
brew install cmake qt@5

# Set Qt path (Intel Mac)
export QT_PATH=/usr/local/opt/qt@5
# OR for Apple Silicon (M1/M2/M3)
export QT_PATH=/opt/homebrew/opt/qt@5

# Build
./build_macos.sh
```

#### Linux/Ubuntu

**Option 1: Using the Installation Script (Recommended)**

```bash
# Make the installation script executable
chmod +x install_dependencies.sh

# Run the installation script
./install_dependencies.sh
```

**Option 2: Manual Installation**

```bash
# Update package list
sudo apt update

# Install build essentials and CMake
sudo apt install build-essential cmake

# Install Qt5 development libraries
# For Ubuntu 22.04 and later:
sudo apt install qtbase5-dev qtbase5-dev-tools qtchooser qt5-qmake

# For Ubuntu 20.04 and earlier:
sudo apt install qt5-default qtbase5-dev qtbase5-dev-tools
```

#### Other Linux Distributions

**Fedora/RHEL/CentOS:**
```bash
sudo dnf install gcc-c++ cmake qt5-qtbase-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake qt5-base
```

## Building the Application

### Windows 10

**Quick Start:**
```cmd
REM Edit build_windows.bat to set your Qt path, then run:
build_windows.bat
```

**Detailed Instructions:**
See **[BUILD_WINDOWS.md](BUILD_WINDOWS.md)** for comprehensive step-by-step instructions including:
- Visual Studio setup
- Qt installation
- CMake configuration
- Building with CMake GUI or command line
- Creating portable distributions

**Output:**
- Executable: `build\Release\MLDatasetTool.exe`
- All required Qt DLLs are automatically copied
- No additional scripts needed - just double-click the .exe to run!

### macOS

**Quick Start:**
```bash
# Set Qt path for your Mac type
export QT_PATH=/opt/homebrew/opt/qt@5  # Apple Silicon
# OR
export QT_PATH=/usr/local/opt/qt@5  # Intel

# Build
chmod +x build_macos.sh
./build_macos.sh
```

**Detailed Instructions:**
See **[BUILD_MACOS.md](BUILD_MACOS.md)** for comprehensive step-by-step instructions including:
- Xcode Command Line Tools setup
- Homebrew and Qt installation
- CMake configuration
- Building with different methods
- Creating DMG installers

**Output:**
- Application bundle: `build/MLDatasetTool.app`
- All required Qt frameworks are automatically bundled
- Just double-click the .app to run!

### Linux/Ubuntu

**Method 1: Using the Build Script (Recommended)**

```bash
# Make the build script executable
chmod +x build.sh

# Run the build script
./build.sh
```

**Method 2: Manual Build**

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the application
make

# The executable will be created as 'ImageClassificationTool'
```

## Running the Application

### Windows 10

After building, simply double-click the executable:
```
build\Release\MLDatasetTool.exe
```

Or use the launcher script:
```cmd
run_windows.bat
```

**No additional setup required!** All Qt dependencies are bundled with the executable.

### macOS

After building, simply double-click the application bundle:
```bash
open build/MLDatasetTool.app
```

Or use the launcher script:
```bash
chmod +x run_macos.sh
./run_macos.sh
```

**No additional setup required!** All Qt frameworks are bundled in the .app.

**Note:** On first launch, you may need to right-click the app and select "Open" to bypass Gatekeeper security.

### Linux/Ubuntu

After building, run the application using one of the provided launcher scripts:

**Recommended Method (Avoids Library Conflicts)**

```bash
# Use the clean launcher script (recommended)
./run_app.sh
```

**Alternative Method (Maximum Isolation)**

```bash
# Use the isolated launcher for maximum protection
./launch_clean.sh
```

**Direct Execution (May Have Issues)**

```bash
# Direct execution (may fail with snap package conflicts)
./build/ImageClassificationTool
```

**Important:** If you encounter runtime errors like "symbol lookup error" with libpthread or Qt threading issues, always use the launcher scripts. These errors are caused by conflicts between system libraries and snap package environments.

## Usage Guide

### Getting Started

1. **Launch the Application**
   - Run the executable using `./run_app.sh`
   - A mode selection dialog will appear

2. **Select Mode**
   - Choose **"Image Classification"** for categorizing entire images
   - Choose **"Object Detection"** for drawing bounding boxes around objects
   - Click **"OK"** to proceed

### Image Classification Mode

1. **Load Images**
   - Click **"Open Single Image"** to classify a single image
   - Click **"Open Folder"** to load all images from a directory
   - Supported formats: JPG, JPEG, PNG, BMP (case-insensitive)

2. **Create Categories**
   - Type a category name in the "New Category" field
   - Click **"Add Category"** or press Enter
   - The category will appear in the dropdown menu
   - A folder will be created automatically in `classified_images/<category_name>/`

3. **Classify Images**
   - View the current image in the display area
   - Select a category from the dropdown menu
   - Click **"Classify and Move Image"**
   - The image will be copied to the category folder
   - The application automatically advances to the next image

4. **Navigation**
   - **Previous**: Go back to the previous image
   - **Skip**: Move to the next image without classifying
   - **Next**: Move to the next image (enabled after classification or when reviewing)

### Object Detection Mode

1. **Load Images**
   - Click **"Open Single Image"** to annotate a single image
   - Click **"Open Folder"** to load all images from a directory

2. **Create Labels**
   - Type an object label name (e.g., "person", "car", "dog")
   - Click **"Add Label"** or press Enter
   - The label will appear in the available labels list

3. **Draw Bounding Boxes**
   - Click and drag on the image to draw a bounding box
   - A dialog will appear asking you to select a label
   - Choose the appropriate label for the object
   - The box will appear with the label displayed

4. **Edit Bounding Boxes**
   - **Select**: Click on a box to select it
   - **Resize**: Drag the corner handles of a selected box
   - **Delete**: Select a box and press Delete/Backspace, or click "Delete Selected Box"

5. **Save Annotations**
   - Click **"Save Annotations"** to save the current image's annotations
   - Click **"Save & Next"** to save and move to the next image
   - Annotations are saved in YOLO format in `annotated_images/labels/`
   - Images are copied to `annotated_images/images/`
   - A `classes.txt` file is created with all label names

6. **Navigation**
   - **Previous**: Go back to the previous image (annotations are auto-loaded)
   - **Skip**: Move to the next image without saving
   - **Next**: Move to the next image

### Workflow Examples

**Image Classification Workflow:**
```
1. Launch app → Select "Image Classification" mode
2. Click "Open Folder" → Select folder with 100 images
3. Create categories: "cat", "dog", "bird"
4. View first image → Select "cat" → Click "Classify and Move Image"
5. Repeat for all images
6. Find organized images in:
   - classified_images/cat/
   - classified_images/dog/
   - classified_images/bird/
```

**Object Detection Workflow:**
```
1. Launch app → Select "Object Detection" mode
2. Click "Open Folder" → Select folder with images
3. Create labels: "person", "car", "bicycle"
4. Draw bounding boxes around objects in first image
5. Assign labels to each box
6. Click "Save & Next" to save and move to next image
7. Repeat for all images
8. Find annotations in:
   - annotated_images/images/     (copied images)
   - annotated_images/labels/     (YOLO .txt files)
   - annotated_images/classes.txt (label names)
```

### Output Structure

**Image Classification Output:**
```
project_directory/
├── classified_images/          # Output folder (created automatically)
│   ├── cat/                   # Category folder
│   │   ├── image001.jpg
│   │   ├── image005.jpg
│   │   └── ...
│   ├── dog/                   # Category folder
│   │   ├── image002.jpg
│   │   ├── image007.jpg
│   │   └── ...
│   └── bird/                  # Category folder
│       ├── image003.jpg
│       └── ...
```

**Object Detection Output:**
```
project_directory/
├── annotated_images/           # Output folder (created automatically)
│   ├── images/                # Copied original images
│   │   ├── image001.jpg
│   │   ├── image002.jpg
│   │   └── ...
│   ├── labels/                # YOLO format annotations
│   │   ├── image001.txt       # One line per bounding box
│   │   ├── image002.txt       # Format: <class_id> <x_center> <y_center> <width> <height>
│   │   └── ...
│   └── classes.txt            # Label names (one per line)
```

**YOLO Annotation Format:**
Each `.txt` file contains one line per bounding box:
```
<class_id> <x_center> <y_center> <width> <height>
```
- All coordinates are normalized (0.0 to 1.0)
- `class_id` corresponds to the line number in `classes.txt` (0-indexed)
- Example: `0 0.5 0.5 0.3 0.4` (class 0, centered, 30% width, 40% height)

## Features in Detail

### Image Display
- Automatic scaling to fit the window
- Maintains original aspect ratio
- Responsive resizing when window size changes
- Displays image metadata (filename, dimensions, file size)

### Progress Tracking
- Current image number (e.g., "Image 5 of 100")
- Count of processed images
- Visual progress bar showing completion percentage

### Error Handling
- Validates image file formats
- Handles missing or corrupted images gracefully
- Prevents duplicate category names
- Manages file permission issues
- Handles duplicate filenames with timestamp suffixes

### Safety Features
- **Non-destructive**: Images are copied, not moved (originals remain intact)
- **Duplicate handling**: Automatic timestamp suffixes prevent overwrites
- **Folder creation**: Category folders are created automatically
- **Validation**: Checks for valid categories and loaded images before classification

## Troubleshooting

### Build Issues

**Qt5 not found:**
```bash
# Make sure Qt5 is installed
sudo apt install qtbase5-dev

# If CMake still can't find Qt5, specify the path:
cmake -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt5 ..
```

**Compiler errors:**
```bash
# Ensure you have a C++11 compatible compiler
g++ --version  # Should be 4.8.1 or later
```

### Runtime Issues

**Symbol lookup error with libpthread:**
```
./build/ImageClassificationTool: symbol lookup error: /snap/core20/current/lib/x86_64-linux-gnu/libpthread.so.0: undefined symbol: __libc_pthread_init, version GLIBC_PRIVATE
```
- **Solution:** Use the launcher scripts: `./run_app.sh` or `./launch_clean.sh`
- **Cause:** Conflict between system glibc and snap package libraries

**Qt threading error:**
```
QSocketNotifier: Can only be used with threads started with QThread
```
- **Solution:** Use the launcher scripts: `./run_app.sh` or `./launch_clean.sh`
- **Cause:** Environment variable conflicts from snap packages

**Application won't start:**
- Use launcher scripts: `./run_app.sh` (recommended)
- Check that Qt5 runtime libraries are installed: `sudo apt install qt5-default`
- Verify the executable has execute permissions: `chmod +x build/ImageClassificationTool`

**Images not loading:**
- Verify the image format is supported (JPG, PNG, BMP)
- Check file permissions on the image directory
- Ensure the image files are not corrupted

**Can't create category folders:**
- Check write permissions in the application directory
- Ensure sufficient disk space
- Verify the category name doesn't contain invalid characters

For detailed troubleshooting, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md).

## Development

### Project Structure
```
image-classification/
├── main.cpp                      # Application entry point
├── ModeSelectionDialog.h/cpp     # Mode selection dialog
├── MainWindow.h/cpp              # Image classification window
├── ObjectDetectionWindow.h/cpp   # Object detection window
├── ImageCanvas.h/cpp             # Custom widget for drawing bounding boxes
├── BoundingBox.h/cpp             # Bounding box data structure
├── AnnotationManager.h/cpp       # YOLO format annotation manager
├── CMakeLists.txt                # CMake build configuration
├── build.sh                      # Build script
├── run_app.sh                    # Launcher script (recommended)
├── launch_clean.sh               # Alternative launcher script
└── README.md                     # This file
```

### Extending the Application

**Adding new image formats:**
- Update `IMAGE_EXTENSIONS` in `MainWindow.h` and `ObjectDetectionWindow.h`
- Add format to `isImageFile()` methods

**Customizing output folders:**
- Classification: Modify `outputFolder` in `MainWindow` constructor
- Detection: Modify `setOutputDirectory()` call in `ObjectDetectionWindow` constructor

**Adding keyboard shortcuts:**
- Use Qt's `QShortcut` class in `setupUI()` methods

**Supporting other annotation formats:**
- Extend `AnnotationManager` class to support Pascal VOC XML or COCO JSON
- Add format selection option in the UI

## License

This project is provided as-is for educational and research purposes.

## Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## Acknowledgments

Built with Qt5 framework for cross-platform GUI development.


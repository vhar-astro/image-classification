# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a cross-platform dual-mode C++ GUI application for **Windows 10**, **macOS**, and **Linux/Ubuntu** that assists with preparing machine learning datasets. It provides intuitive Qt5-based interfaces for:
1. **Image Classification**: Manually labeling entire images and organizing them into category-based folders
2. **Object Detection**: Drawing bounding boxes around objects and exporting annotations in YOLO format

## Build and Run Commands

### Windows 10

**Build:**
```cmd
build_windows.bat
```
This script configures CMake with Visual Studio generator, builds the project in Release mode, and automatically deploys Qt dependencies using windeployqt.

**Manual build:**
```cmd
mkdir build && cd build
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64 ..
cmake --build . --config Release
```

**Run:**
```cmd
REM Simply double-click the executable:
build\Release\MLDatasetTool.exe

REM Or use the launcher:
run_windows.bat
```

**Important:** No launcher scripts needed on Windows! The executable includes all Qt dependencies and runs standalone. See `BUILD_WINDOWS.md` for detailed instructions.

### macOS

**Build:**
```bash
./build_macos.sh
```
This script validates the environment, configures CMake, builds the project, and automatically bundles Qt frameworks using macdeployqt.

**Manual build:**
```bash
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=$QT_PATH -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 ..
make -j$(sysctl -n hw.ncpu)
$QT_PATH/bin/macdeployqt MLDatasetTool.app
```

**Run:**
```bash
# Simply open the application bundle:
open build/MLDatasetTool.app

# Or use the launcher:
./run_macos.sh
```

**Important:** No launcher scripts needed on macOS! The .app bundle includes all Qt frameworks and runs standalone. See `BUILD_MACOS.md` for detailed instructions.

### Linux/Ubuntu

**Build:**
```bash
./build.sh
```
This script creates a `build/` directory, runs CMake configuration, and compiles the application using `make -j$(nproc)`.

**Manual build:**
```bash
mkdir build && cd build
cmake ..
make
```

**Run:**
```bash
./run_app.sh
```
**Important:** Always use the launcher scripts (`run_app.sh` or `launch_clean.sh`) instead of direct execution. These scripts clean snap-related environment variables that cause glibc symbol lookup errors and Qt threading conflicts.

**Install Dependencies:**
```bash
./install_dependencies.sh
```
Installs build-essential, cmake, and Qt5 development libraries (qtbase5-dev).

## Architecture

### Core Design Pattern
Dual-mode Qt5 application with mode selection on startup. Each mode has its own main window class:
- **Image Classification Mode**: `MainWindow` (QMainWindow)
- **Object Detection Mode**: `ObjectDetectionWindow` (QMainWindow)

Both windows follow a Model-View architecture where the window handles both UI and business logic for simplicity.

### Key Components

**ModeSelectionDialog** (`ModeSelectionDialog.h`, `ModeSelectionDialog.cpp`):
- Initial dialog shown on application startup
- Radio buttons for selecting Image Classification or Object Detection mode
- Returns selected mode to main.cpp which launches appropriate window

**MainWindow** (`MainWindow.h`, `MainWindow.cpp`) - Image Classification Mode:
- Main application window for image classification
- Manages image loading, category management, file operations, and navigation
- Uses Qt's signal/slot mechanism for event handling

**State Management (MainWindow):**
- `imageFiles` (QStringList): All image paths to process
- `processedImages` (QStringList): Classified images
- `categories` (QSet<QString>): Available categories
- `currentImageIndex`: Current position in image list
- `outputFolder`: Default is `"classified_images"` in project root

**Image Classification Flow:**
1. User loads images via "Open Folder" or "Open Single Image"
2. Images filtered by `IMAGE_EXTENSIONS` (JPG, PNG, BMP - case insensitive)
3. User creates categories (stored in `categories` set + `categoryComboBox`)
4. Classification copies (not moves) images to `classified_images/<category>/`
5. Duplicate filenames get timestamp suffix: `filename_YYYYMMDD_HHMMSS.ext`

**ObjectDetectionWindow** (`ObjectDetectionWindow.h`, `ObjectDetectionWindow.cpp`) - Object Detection Mode:
- Main application window for object detection annotation
- Manages image loading, label management, bounding box operations, and navigation
- Integrates with ImageCanvas for interactive drawing

**ImageCanvas** (`ImageCanvas.h`, `ImageCanvas.cpp`):
- Custom QWidget for displaying images and drawing bounding boxes
- Handles mouse events for drawing, selecting, resizing, and moving boxes
- Manages coordinate transformations between screen and image space
- Emits signals for box creation, selection, and modification

**BoundingBox** (`BoundingBox.h`, `BoundingBox.cpp`):
- Data structure representing a single bounding box
- Stores rectangle coordinates, label, class ID, color, and selection state
- Provides methods for YOLO format conversion (normalized coordinates)
- Handles corner detection for resizing operations

**AnnotationManager** (`AnnotationManager.h`, `AnnotationManager.cpp`):
- Manages YOLO format annotation file I/O
- Maintains label-to-class-ID mappings
- Saves/loads annotations in YOLO format: `<class_id> <x_center> <y_center> <width> <height>`
- Creates `classes.txt` file with label names
- Copies images to output directory

**Object Detection Flow:**
1. User loads images via "Open Folder" or "Open Single Image"
2. User creates object labels (e.g., "person", "car", "dog")
3. User draws bounding boxes by clicking and dragging on ImageCanvas
4. Dialog prompts for label selection for each new box
5. Boxes can be selected, resized (drag corners), or deleted (Delete key)
6. Annotations saved to `annotated_images/labels/<filename>.txt` in YOLO format
7. Images copied to `annotated_images/images/`
8. `classes.txt` file created with all label names

### Critical Implementation Details

**Non-Destructive Workflow:**
Images are **copied** (via `QFile::copy()`) rather than moved to preserve originals. This is a core design decision to prevent data loss.

**Automatic Folder Creation:**
Category folders are created on-demand using `QDir::mkpath()` when:
- A new category is added
- An image is classified to a category

**Image Scaling:**
Images are scaled to fit the display area while maintaining aspect ratio using `QPixmap::scaled()` with `Qt::KeepAspectRatio`. The original pixmap is preserved in `currentPixmap`. Window resizing triggers `resizeEvent()` which calls `scaleImageToFit()`.

**Library Conflict Handling:**
The application has known issues with snap package environments. The `run_app.sh` and `launch_clean.sh` scripts unset snap-related environment variables and reset `LD_LIBRARY_PATH` to system libraries. This fixes:
- `symbol lookup error: libpthread.so.0: undefined symbol: __libc_pthread_init`
- `QSocketNotifier: Can only be used with threads started with QThread`

## CMake Configuration

- Minimum CMake version: 3.10
- C++ standard: C++11 (required)
- Auto MOC enabled (`CMAKE_AUTOMOC ON`) for Qt meta-object compilation
- Links: Qt5::Core, Qt5::Widgets, Qt5::Gui
- Single executable: `ImageClassificationTool`

## File Organization

```
project_root/
├── main.cpp                    # Application entry point
├── MainWindow.h                # Main window class declaration
├── MainWindow.cpp              # Main window implementation
├── CMakeLists.txt              # Build configuration
├── build.sh                    # Automated build script
├── run_app.sh                  # Recommended launcher (cleans snap env vars)
├── launch_clean.sh             # Alternative launcher (maximum isolation)
├── install_dependencies.sh     # Dependency installation
├── build/                      # Build artifacts (generated)
│   └── ImageClassificationTool # Compiled executable
└── classified_images/          # Output folder (created at runtime)
    ├── <category1>/            # Auto-created category folders
    ├── <category2>/
    └── ...
```

## Extending the Application

**Adding new image formats:**
1. Update `IMAGE_EXTENSIONS` constant in `MainWindow.h`
2. Modify `isImageFile()` method in `MainWindow.cpp` if needed

**Changing output folder:**
Modify `outputFolder` initialization in `MainWindow` constructor (default: `"classified_images"`)

**Adding keyboard shortcuts:**
Use Qt's `QShortcut` class in the `setupUI()` method

## Important Qt Patterns Used

- **Signals/Slots:** All button clicks connect to private slots (e.g., `openImage()`, `classifyImage()`)
- **Layouts:** `QVBoxLayout` for main vertical stacking, `QHBoxLayout` for button groups
- **File Dialogs:** `QFileDialog::getOpenFileName()` and `QFileDialog::getExistingDirectory()`
- **Image Handling:** `QPixmap` for loading/displaying, `QFileInfo` for metadata
- **Resizing:** Override `resizeEvent()` to handle window resize and re-scale images

## Known Issues and Solutions

**Runtime library conflicts:**
Always use launcher scripts, never run `./build/ImageClassificationTool` directly when developing in environments with snap packages (especially VS Code snap).

**Qt5 not found during build:**
Specify Qt path manually: `cmake -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt5 ..`

## Testing Checklist

When modifying the codebase, test:
1. Loading single image and folder of images
2. Creating new categories
3. Classifying images to different categories
4. Navigation (previous/next/skip)
5. Duplicate filename handling (classify same filename to same category)
6. Empty folder handling
7. Unsupported file format handling
8. Window resizing (image should re-scale)
9. Progress bar updates correctly
10. Category folder creation and file copying

# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C++ GUI application for Linux/Ubuntu that assists with preparing machine learning image classification datasets. It provides an intuitive Qt5-based interface for manually labeling images and automatically organizing them into category-based folders.

## Build and Run Commands

### Build the Application
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

### Run the Application
```bash
./run_app.sh
```
**Important:** Always use the launcher scripts (`run_app.sh` or `launch_clean.sh`) instead of direct execution. These scripts clean snap-related environment variables that cause glibc symbol lookup errors and Qt threading conflicts.

### Install Dependencies
```bash
./install_dependencies.sh
```
Installs build-essential, cmake, and Qt5 development libraries (qtbase5-dev).

## Architecture

### Core Design Pattern
Single-window Qt5 application using a Model-View architecture where `MainWindow` (QMainWindow) handles both UI and business logic. This is intentional for simplicity - there is no separate model layer.

### Key Components

**MainWindow** (`MainWindow.h`, `MainWindow.cpp`):
- Main application window containing all UI components and logic
- Manages image loading, category management, file operations, and navigation
- Uses Qt's signal/slot mechanism for event handling

**State Management:**
- `imageFiles` (QStringList): All image paths to process
- `processedImages` (QStringList): Classified images
- `categories` (QSet<QString>): Available categories
- `currentImageIndex`: Current position in image list
- `outputFolder`: Default is `"classified_images"` in project root

**Image Flow:**
1. User loads images via "Open Folder" or "Open Single Image"
2. Images filtered by `IMAGE_EXTENSIONS` (JPG, PNG, BMP - case insensitive)
3. User creates categories (stored in `categories` set + `categoryComboBox`)
4. Classification copies (not moves) images to `classified_images/<category>/`
5. Duplicate filenames get timestamp suffix: `filename_YYYYMMDD_HHMMSS.ext`

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

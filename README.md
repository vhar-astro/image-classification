# Image Classification Tool

A C++ GUI application for Linux/Ubuntu that assists with preparing machine learning image classification datasets. This tool provides an intuitive interface for manually labeling images and automatically organizing them into category-based folders.

## Features

### Core Functionality
- **Image Loading**: Open individual images or entire folders containing JPG, PNG, and BMP files
- **Visual Classification**: View images one at a time with a clear, resizable display
- **Category Management**: Create custom categories on-the-fly or select from existing ones
- **Automatic Organization**: Images are automatically copied to category-specific folders
- **Progress Tracking**: Real-time progress bar and counter showing classification status
- **Navigation**: Move forward, backward, or skip images during the classification process

### User Interface
- Clean, intuitive Qt-based GUI
- Image scaling to fit window while maintaining aspect ratio
- Image information display (filename, dimensions, file size)
- Progress indicators (current image number, total images, processed count)
- Category dropdown with quick-add functionality

### File Management
- Non-destructive workflow (copies images instead of moving them)
- Automatic creation of category folders
- Duplicate filename handling with timestamp suffixes
- Organized output structure: `classified_images/<category_name>/image.jpg`

## Prerequisites

### System Requirements
- Ubuntu 18.04 or later (or other Linux distributions)
- C++ compiler with C++11 support (g++ or clang++)
- CMake 3.10 or later
- Qt5 development libraries

### Installing Dependencies on Ubuntu

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

### Installing Dependencies on Other Distributions

**Fedora/RHEL/CentOS:**
```bash
sudo dnf install gcc-c++ cmake qt5-qtbase-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake qt5-base
```

## Building the Application

### Method 1: Using the Build Script (Recommended)

```bash
# Make the build script executable
chmod +x build.sh

# Run the build script
./build.sh
```

### Method 2: Manual Build

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

After building, run the application using one of the provided launcher scripts:

### Recommended Method (Avoids Library Conflicts)

```bash
# Use the clean launcher script (recommended)
./run_app.sh
```

### Alternative Method (Maximum Isolation)

```bash
# Use the isolated launcher for maximum protection
./launch_clean.sh
```

### Direct Execution (May Have Issues)

```bash
# Direct execution (may fail with snap package conflicts)
./build/ImageClassificationTool
```

**Important:** If you encounter runtime errors like "symbol lookup error" with libpthread or Qt threading issues, always use the launcher scripts. These errors are caused by conflicts between system libraries and snap package environments.

## Usage Guide

### Getting Started

1. **Launch the Application**
   - Run the executable as shown above
   - The main window will appear with the classification interface

2. **Load Images**
   - Click **"Open Single Image"** to classify a single image
   - Click **"Open Folder"** to load all images from a directory
   - Supported formats: JPG, JPEG, PNG, BMP (case-insensitive)

3. **Create Categories**
   - Type a category name in the "New Category" field
   - Click **"Add Category"** or press Enter
   - The category will appear in the dropdown menu
   - A folder will be created automatically in `classified_images/<category_name>/`

4. **Classify Images**
   - View the current image in the display area
   - Select a category from the dropdown menu
   - Click **"Classify and Move Image"**
   - The image will be copied to the category folder
   - The application automatically advances to the next image

5. **Navigation**
   - **Previous**: Go back to the previous image
   - **Skip**: Move to the next image without classifying
   - **Next**: Move to the next image (enabled after classification or when reviewing)

### Workflow Example

```
1. Click "Open Folder" → Select folder with 100 images
2. Create categories: "cat", "dog", "bird"
3. View first image → Select "cat" → Click "Classify and Move Image"
4. Repeat for all images
5. Find organized images in:
   - classified_images/cat/
   - classified_images/dog/
   - classified_images/bird/
```

### Output Structure

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
├── main.cpp              # Application entry point
├── MainWindow.h          # Main window header
├── MainWindow.cpp        # Main window implementation
├── CMakeLists.txt        # CMake build configuration
├── build.sh              # Build script
└── README.md             # This file
```

### Extending the Application

**Adding new image formats:**
- Update `IMAGE_EXTENSIONS` in `MainWindow.h`
- Add format to `isImageFile()` method in `MainWindow.cpp`

**Customizing output folder:**
- Modify `outputFolder` initialization in `MainWindow` constructor

**Adding keyboard shortcuts:**
- Use Qt's `QShortcut` class in `setupUI()` method

## License

This project is provided as-is for educational and research purposes.

## Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## Acknowledgments

Built with Qt5 framework for cross-platform GUI development.


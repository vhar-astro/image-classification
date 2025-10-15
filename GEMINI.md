# Gemini Code Assistant Context

## Project Overview

This is a C++ GUI application for Linux/Ubuntu that assists with preparing machine learning image classification datasets. The tool provides an intuitive interface for manually labeling images and automatically organizing them into category-based folders.

The application is built using C++11 and the Qt5 framework for the graphical user interface. The build system is managed by CMake.

### Key Features:
- **Image Loading**: Open individual images or entire folders (JPG, PNG, BMP).
- **Visual Classification**: View images one by one.
- **Category Management**: Create and select classification categories.
- **Automatic Organization**: Images are copied to category-specific folders.
- **Navigation**: Move forward, backward, or skip images.

## Building and Running

### Dependencies
- C++ compiler with C++11 support (e.g., g++)
- CMake 3.10 or later
- Qt5 development libraries

### Building the Application
The project includes a build script that automates the build process.

```bash
# Make the build script executable
chmod +x build.sh

# Run the build script
./build.sh
```

Alternatively, you can build manually:

```bash
# Create a build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the application
make
```

### Running the Application
After a successful build, the executable `ImageClassificationTool` will be located in the `build` directory.

To run the application from the project root:
```bash
./build/ImageClassificationTool
```

## Development Conventions

### Code Style
The code follows standard C++ conventions. The use of Qt's signals and slots mechanism is central to the application's event-driven architecture.

### Project Structure
- `main.cpp`: Application entry point.
- `MainWindow.h` / `MainWindow.cpp`: Defines and implements the main application window, UI, and all related logic.
- `CMakeLists.txt`: CMake build configuration file.
- `build.sh`: A shell script to automate the build process.
- `install_dependencies.sh`: A shell script to install required libraries on Debian-based systems.
- `*.md`: Documentation files.

### Testing
There are no dedicated test files in the project. Testing appears to be done manually.

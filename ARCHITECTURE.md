# Architecture Documentation

## Overview

The Image Classification Tool is a Qt5-based C++ application designed with a simple Model-View architecture. The application consists of a single main window that handles all user interactions and file operations.

## Component Structure

### Main Components

```
┌─────────────────────────────────────────┐
│           MainWindow (QMainWindow)       │
│  ┌─────────────────────────────────┐   │
│  │     UI Components (Qt Widgets)   │   │
│  │  - Image Display (QLabel)        │   │
│  │  - Category Selector (QComboBox) │   │
│  │  - Navigation Buttons            │   │
│  │  - Progress Bar                  │   │
│  └─────────────────────────────────┘   │
│  ┌─────────────────────────────────┐   │
│  │     Business Logic               │   │
│  │  - Image Loading                 │   │
│  │  - File Organization             │   │
│  │  - Category Management           │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
```

## File Structure

### MainWindow.h
Defines the main window class with:
- UI component declarations
- Data member declarations (image lists, categories, current state)
- Method declarations for all functionality
- Qt signal/slot declarations

### MainWindow.cpp
Implements all functionality:
- **UI Setup**: Creates and arranges all widgets
- **File Operations**: Loading images, folder scanning
- **Classification Logic**: Moving files to category folders
- **Navigation**: Moving between images
- **State Management**: Tracking progress and current image

### main.cpp
Application entry point:
- Initializes Qt application
- Creates and shows main window
- Starts event loop

## Key Design Decisions

### 1. Non-Destructive Workflow
Images are **copied** rather than moved to preserve originals. This prevents data loss if the user makes mistakes or wants to reclassify images.

**Implementation**: `QFile::copy()` instead of `QFile::rename()`

### 2. Automatic Folder Creation
Category folders are created automatically when:
- A new category is added
- An image is classified to a category

**Implementation**: `QDir::mkpath()` with error handling

### 3. Duplicate Filename Handling
If a file with the same name exists in the destination:
- A timestamp is appended to the filename
- Format: `originalname_YYYYMMDD_HHMMSS.ext`

**Implementation**: `QDateTime::currentDateTime().toString()`

### 4. Image Scaling
Images are scaled to fit the display area while maintaining aspect ratio:
- Responsive to window resizing
- Uses smooth transformation for quality
- Original image is preserved in memory

**Implementation**: `QPixmap::scaled()` with `Qt::KeepAspectRatio`

## Data Flow

### Loading Images from Folder

```
User clicks "Open Folder"
    ↓
QFileDialog selects directory
    ↓
loadImagesFromFolder() scans directory
    ↓
Filter files by extension (JPG, PNG, BMP)
    ↓
Populate imageFiles QStringList
    ↓
Set currentImageIndex = 0
    ↓
updateImageDisplay()
```

### Classifying an Image

```
User selects category and clicks "Classify"
    ↓
Validate category selection
    ↓
moveImageToCategory()
    ├─ Create category folder if needed
    ├─ Check for duplicate filename
    ├─ Generate unique name if needed
    └─ Copy file to destination
    ↓
Add to processedImages list
    ↓
updateProgress()
    ↓
Move to next image
```

## State Management

### Application State Variables

- **imageFiles**: List of all image paths to process
- **processedImages**: List of images that have been classified
- **categories**: Set of available category names
- **currentImageIndex**: Index of currently displayed image
- **currentImagePath**: Full path to current image
- **currentPixmap**: Loaded image data
- **sourceFolder**: Source directory for batch processing
- **outputFolder**: Base directory for classified images

### State Transitions

```
Initial State
    ↓
Images Loaded → Can navigate, can classify
    ↓
Category Selected → Classify button enabled
    ↓
Image Classified → Move to next, update progress
    ↓
All Images Processed → Session complete
```

## Qt Framework Usage

### Widgets Used

- **QMainWindow**: Main application window
- **QLabel**: Image display and text labels
- **QPushButton**: Action buttons
- **QComboBox**: Category selection dropdown
- **QLineEdit**: New category input
- **QProgressBar**: Visual progress indicator
- **QScrollArea**: Scrollable image container
- **QGroupBox**: Logical grouping of controls
- **QFileDialog**: File and folder selection dialogs
- **QMessageBox**: User notifications and errors

### Layouts Used

- **QVBoxLayout**: Main vertical layout
- **QHBoxLayout**: Horizontal button groups

### Qt Features

- **Signals and Slots**: Event handling mechanism
- **Auto MOC**: Automatic meta-object compilation
- **QPixmap**: Image loading and display
- **QFileInfo**: File metadata extraction
- **QDir**: Directory operations

## Error Handling

### File Operations
- Check if file exists before operations
- Validate file permissions
- Handle copy failures gracefully
- Display error messages to user

### Image Loading
- Validate image format
- Check if QPixmap loaded successfully
- Handle corrupted images
- Display placeholder for failed loads

### User Input
- Validate category names (not empty)
- Check for duplicate categories
- Ensure image is loaded before classification
- Validate category selection

## Performance Considerations

### Image Loading
- Images are loaded on-demand (not all at once)
- Only current image is kept in memory
- Scaled versions are generated for display

### File Scanning
- Directory scanning is done once per folder
- File list is cached in memory
- No redundant file system operations

### UI Responsiveness
- File operations are quick (copy, not move)
- No blocking operations in UI thread
- Immediate feedback for user actions

## Future Enhancement Possibilities

### Potential Improvements

1. **Undo Functionality**: Track operations for reversal
2. **Batch Operations**: Classify multiple images at once
3. **Keyboard Shortcuts**: Quick category assignment
4. **Image Preprocessing**: Resize, crop, rotate before saving
5. **Export Metadata**: CSV file with classification results
6. **Multi-threading**: Background file operations
7. **Database Integration**: Store classification metadata
8. **Machine Learning Integration**: Suggest categories
9. **Drag and Drop**: Drop images directly into app
10. **Custom Output Structure**: Configurable folder organization

### Code Extension Points

- **Custom File Filters**: Modify `IMAGE_EXTENSIONS` and `isImageFile()`
- **Output Format**: Change `moveImageToCategory()` logic
- **UI Customization**: Modify `setupUI()` method
- **Additional Metadata**: Extend data members in MainWindow class

## Building and Dependencies

### Build System: CMake

- Minimum version: 3.10
- Auto-generates Qt MOC files
- Links Qt5 libraries automatically

### Dependencies

- **Qt5::Core**: Core Qt functionality
- **Qt5::Widgets**: GUI widgets
- **Qt5::Gui**: GUI support and image handling

### Compiler Requirements

- C++11 or later
- Tested with GCC 7.5+ and Clang 6.0+

## Testing Recommendations

### Manual Testing Checklist

1. Load single image
2. Load folder with multiple images
3. Create new categories
4. Classify images
5. Navigate forward/backward
6. Skip images
7. Handle duplicate filenames
8. Test with different image formats
9. Test with large images
10. Test with corrupted images
11. Test folder creation
12. Test progress tracking

### Edge Cases to Test

- Empty folder
- Folder with no image files
- Very large images (>10MB)
- Images with special characters in filename
- Read-only source directory
- Insufficient disk space
- Category names with special characters
- Rapid button clicking
- Window resizing during operation

## Conclusion

The Image Classification Tool uses a straightforward architecture that prioritizes:
- **Simplicity**: Single-window design, clear data flow
- **Safety**: Non-destructive operations, error handling
- **Usability**: Intuitive UI, immediate feedback
- **Maintainability**: Clear code structure, well-documented

This design makes it easy to understand, modify, and extend for specific use cases.


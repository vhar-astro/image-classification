# Frequently Asked Questions (FAQ)

## Installation and Setup

### Q: What operating systems are supported?
**A:** The application is designed for Linux, specifically Ubuntu 18.04 and later. It should also work on other Debian-based distributions (Linux Mint, Pop!_OS, etc.) and can be adapted for Fedora, Arch, and other distributions with appropriate package installations.

### Q: Do I need to install Qt5?
**A:** Yes, Qt5 development libraries are required. Use the provided installation script:
```bash
./install_dependencies.sh
```
Or install manually:
```bash
sudo apt install qtbase5-dev qtbase5-dev-tools
```

### Q: Can I use Qt6 instead of Qt5?
**A:** The current version is built for Qt5. To use Qt6, you would need to modify `CMakeLists.txt` to find Qt6 packages instead of Qt5. Most of the code should work with minimal changes.

### Q: The build fails with "Qt5 not found". What should I do?
**A:** 
1. Run the installation script: `./install_dependencies.sh`
2. If that doesn't work, manually install: `sudo apt install qtbase5-dev`
3. If Qt5 is installed in a non-standard location, specify the path:
   ```bash
   cmake -DCMAKE_PREFIX_PATH=/path/to/qt5 ..
   ```

### Q: How much disk space do I need?
**A:** 
- Application binary: ~500 KB
- Build files: ~10 MB
- Qt5 libraries (if not installed): ~50-100 MB
- Plus space for your classified images (depends on your dataset)

## Usage Questions

### Q: Does the tool move or copy images?
**A:** The tool **copies** images to the category folders. Your original images remain untouched in their original location. This is a safety feature to prevent data loss.

### Q: Can I change it to move instead of copy?
**A:** Yes, modify `MainWindow.cpp` in the `moveImageToCategory()` function. Uncomment the line:
```cpp
// QFile::remove(imagePath);
```
This will delete the original after copying.

### Q: What happens if I classify an image to the wrong category?
**A:** Since the tool copies (doesn't move) images, your original is still safe. You can:
1. Delete the incorrectly classified copy from the category folder
2. Use the "Previous" button to go back and reclassify
3. Or simply reclassify the original image again

### Q: Can I classify the same image to multiple categories?
**A:** Yes! Since images are copied, you can classify the same image multiple times to different categories. Each category folder will get a copy.

### Q: What image formats are supported?
**A:** Currently supported formats:
- JPEG (.jpg, .jpeg)
- PNG (.png)
- BMP (.bmp)

All formats are case-insensitive (e.g., .JPG and .jpg both work).

### Q: Can I add support for other formats (TIFF, GIF, WebP)?
**A:** Yes, modify two places in the code:
1. `MainWindow.h`: Add to `IMAGE_EXTENSIONS` array
2. `MainWindow.cpp`: Update `isImageFile()` method

Qt supports many formats through plugins.

### Q: Where are classified images saved?
**A:** By default, images are saved in a `classified_images` folder in the same directory where you run the application:
```
classified_images/
├── category1/
├── category2/
└── category3/
```

### Q: Can I change the output folder location?
**A:** Yes, modify the `outputFolder` variable in `MainWindow.cpp` constructor:
```cpp
outputFolder("classified_images")  // Change this path
```

### Q: What happens if two images have the same filename?
**A:** The tool automatically handles duplicates by adding a timestamp:
- First image: `photo.jpg`
- Duplicate: `photo_20231015_143022.jpg`

This prevents overwriting existing files.

## Features and Functionality

### Q: Can I undo a classification?
**A:** Not directly in the current version. However, since originals are preserved, you can:
1. Delete the classified copy
2. Reclassify the original

Future versions may include undo functionality.

### Q: Is there a way to classify multiple images at once?
**A:** Not in the current version. Each image must be classified individually. This is intentional for accuracy in manual classification tasks.

### Q: Can I use keyboard shortcuts?
**A:** The current version doesn't have keyboard shortcuts, but this would be a great enhancement. You can add them by modifying `setupUI()` in `MainWindow.cpp` using Qt's `QShortcut` class.

### Q: Does the tool remember my categories between sessions?
**A:** No, categories are session-based. When you close and reopen the application, you'll need to recreate categories. Future versions could save categories to a configuration file.

### Q: Can I edit or delete categories?
**A:** Not in the current version. You can only add new categories. Category management features could be added in future versions.

### Q: What's the maximum number of images I can process?
**A:** There's no hard limit, but practical considerations:
- Memory: Only one image is loaded at a time
- UI: Tested with 10,000+ images
- Performance: File scanning may slow down with 100,000+ files

### Q: Can I process images from multiple folders?
**A:** You can open one folder at a time. To process multiple folders:
1. Process first folder
2. Click "Open Folder" again for the next folder
3. Categories persist within the session

## Technical Questions

### Q: What version of C++ is required?
**A:** C++11 or later. The code uses C++11 features like `auto`, range-based for loops, and nullptr.

### Q: Can I compile with Clang instead of GCC?
**A:** Yes, CMake will use whatever compiler is set as default. To explicitly use Clang:
```bash
export CXX=clang++
./build.sh
```

### Q: Does the application use multiple threads?
**A:** No, the current version is single-threaded. All operations run on the main UI thread. For large files, this could be enhanced with background threads.

### Q: How much memory does the application use?
**A:** Typical usage: 50-100 MB
- Base application: ~20 MB
- One loaded image: Depends on image size (typically 5-50 MB)
- Qt libraries: ~30 MB

### Q: Is the application 32-bit or 64-bit?
**A:** It compiles to match your system architecture. On 64-bit systems, it will be 64-bit.

## Troubleshooting

### Q: The application won't start. What should I check?
**A:** 
1. Verify Qt5 runtime is installed: `sudo apt install qt5-default`
2. Check executable permissions: `chmod +x build/ImageClassificationTool`
3. Run from terminal to see error messages: `./build/ImageClassificationTool`

### Q: Images aren't loading. What's wrong?
**A:** Check:
1. File format is supported (JPG, PNG, BMP)
2. File isn't corrupted (try opening in another program)
3. You have read permissions on the file
4. File extension matches actual format

### Q: I can't create category folders. Why?
**A:** Check:
1. Write permissions in the application directory
2. Sufficient disk space
3. Category name doesn't contain invalid characters (/, \, :, etc.)

### Q: The image display is blank/black. What's happening?
**A:** Possible causes:
1. Image file is corrupted
2. Image format not supported
3. Image is extremely large (>100 MB)
4. Qt image plugins not installed

Try: `sudo apt install qt5-image-formats-plugins`

### Q: The application crashes when I click "Classify". Help!
**A:** 
1. Check console for error messages
2. Verify you have write permissions
3. Ensure category is selected
4. Check disk space
5. Try with a different image

### Q: Build fails with "undefined reference to Qt..." errors
**A:** Qt libraries aren't linking properly. Try:
```bash
rm -rf build
./build.sh
```

If that doesn't work, reinstall Qt5 development packages.

## Performance and Optimization

### Q: The application is slow with large images. Can I speed it up?
**A:** 
1. Images are scaled for display (already optimized)
2. For very large images (>20 MB), consider preprocessing to smaller size
3. Future enhancement: Generate thumbnails in background thread

### Q: Can I process images faster?
**A:** Tips for efficiency:
1. Create all categories before starting
2. Use a systematic approach (don't jump around)
3. Skip uncertain images, review later
4. Consider keyboard shortcuts (if implemented)

### Q: Does the tool support batch processing?
**A:** It processes one image at a time, but you can load an entire folder and work through all images sequentially.

## Data and Privacy

### Q: Does the tool send any data over the internet?
**A:** No, the application is completely offline. No data is transmitted anywhere.

### Q: Are my images modified in any way?
**A:** No, images are copied as-is. No modifications, compression, or metadata changes.

### Q: Is metadata preserved?
**A:** Yes, EXIF data and other metadata are preserved during the copy operation.

### Q: Can I use this for sensitive/confidential images?
**A:** Yes, the tool runs entirely locally with no network access. However, always follow your organization's data handling policies.

## Customization and Extension

### Q: Can I change the UI colors/theme?
**A:** Yes, modify the `setupUI()` method in `MainWindow.cpp`. Qt supports stylesheets (similar to CSS).

### Q: Can I add custom preprocessing (resize, crop, etc.)?
**A:** Yes, modify the `moveImageToCategory()` method to process images before saving. Qt provides image manipulation functions.

### Q: Can I export a list of classifications?
**A:** Not in the current version, but you could add this feature by:
1. Tracking classifications in a data structure
2. Writing to CSV file on completion
3. Adding an "Export" button

### Q: Can I integrate this with a machine learning pipeline?
**A:** Yes! The organized folder structure is compatible with most ML frameworks:
- TensorFlow: `ImageDataGenerator.flow_from_directory()`
- PyTorch: `torchvision.datasets.ImageFolder()`
- Keras: `image_dataset_from_directory()`

### Q: Can I add automatic suggestions using a pre-trained model?
**A:** Yes, this would be an excellent enhancement:
1. Load a pre-trained model (e.g., using TensorFlow C++ API)
2. Run inference on each image
3. Suggest top-N categories
4. User confirms or corrects

## Comparison with Other Tools

### Q: How is this different from manually organizing files?
**A:** Benefits:
- Visual interface for viewing images
- Progress tracking
- Automatic folder creation
- Duplicate handling
- Consistent workflow

### Q: Why not use a Python script with Tkinter/PyQt?
**A:** 
- C++ is faster for large datasets
- Lower memory footprint
- Native performance
- No Python runtime required
- Good learning exercise for C++/Qt

### Q: Are there commercial alternatives?
**A:** Yes, tools like Labelbox, Roboflow, and CVAT offer more features but:
- Often cloud-based (privacy concerns)
- May have costs
- More complex for simple classification
- This tool is free, open-source, and offline

## Contributing and Development

### Q: Can I contribute to this project?
**A:** Yes! Contributions are welcome. Areas for improvement:
- Additional features
- Bug fixes
- Documentation
- Testing
- Performance optimization

### Q: What's the license?
**A:** The project is provided as-is for educational and research purposes. Check the LICENSE file (if added) for specific terms.

### Q: How can I report bugs?
**A:** 
1. Check if the issue is already known
2. Provide detailed steps to reproduce
3. Include system information (OS, Qt version)
4. Share error messages or screenshots

### Q: Can I use this in a commercial product?
**A:** Check the license terms. Generally, for educational/research code, you may need to add appropriate licensing before commercial use.

## Future Development

### Q: What features are planned for future versions?
**A:** Potential enhancements:
- Keyboard shortcuts
- Undo/redo functionality
- Category management (edit, delete, merge)
- Export classification log
- Configuration file for settings
- Batch operations
- Image preprocessing
- ML-assisted suggestions
- Multi-language support

### Q: Will there be a Windows/Mac version?
**A:** Qt is cross-platform, so the code could be compiled for Windows and Mac with minimal changes. The main work would be:
- Testing on those platforms
- Creating platform-specific build instructions
- Handling platform-specific file paths

### Q: Can I request a feature?
**A:** Yes! Feature requests are welcome. Describe:
- What you want to do
- Why it would be useful
- How you envision it working

## Getting Help

### Q: Where can I get more help?
**A:** 
1. Read the README.md for comprehensive documentation
2. Check QUICKSTART.md for setup help
3. Review ARCHITECTURE.md for technical details
4. See USAGE_EXAMPLES.md for practical scenarios
5. Search this FAQ for common issues

### Q: The FAQ doesn't answer my question. Now what?
**A:** 
1. Check the documentation files
2. Look at the source code (it's well-commented)
3. Try searching for similar Qt applications
4. Ask in Qt forums or communities

## Conclusion

This FAQ covers the most common questions about the Image Classification Tool. For more detailed information, please refer to the comprehensive documentation provided with the project.

**Happy classifying!** 🎉


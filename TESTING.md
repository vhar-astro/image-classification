# Testing Guide for ML Dataset Preparation Tool

This document provides comprehensive testing procedures for both Image Classification and Object Detection modes.

## Build Verification

### 1. Clean Build Test
```bash
# Clean previous builds
rm -rf build/

# Run build script
./build.sh

# Expected: Build completes successfully with no errors
# Expected: Executable created at build/ImageClassificationTool
```

### 2. Launch Test
```bash
# Launch using recommended script
./run_app.sh

# Expected: Application launches without errors
# Expected: Mode selection dialog appears
# Expected: No symbol lookup errors or Qt threading warnings
```

## Mode Selection Testing

### Test 1: Mode Selection Dialog
- [ ] Dialog appears on startup
- [ ] "Image Classification" radio button is selected by default
- [ ] "Object Detection" radio button can be selected
- [ ] "OK" button launches the selected mode
- [ ] "Cancel" button closes the application
- [ ] Dialog has proper title and descriptions

## Image Classification Mode Testing

### Test 2: Basic Image Loading
- [ ] "Open Single Image" button works
- [ ] File dialog appears with image filters
- [ ] Selected image loads and displays correctly
- [ ] Image scales to fit window while maintaining aspect ratio
- [ ] Image info displays (filename, dimensions, file size)

### Test 3: Folder Loading
- [ ] "Open Folder" button works
- [ ] Folder dialog appears
- [ ] All supported images (JPG, PNG, BMP) are loaded
- [ ] Case-insensitive extension matching works (jpg, JPG, Jpg)
- [ ] Progress bar shows correct total count
- [ ] First image displays automatically

### Test 4: Category Management
- [ ] "New Category" input field accepts text
- [ ] "Add Category" button creates new category
- [ ] Enter key in input field creates category
- [ ] Category appears in dropdown menu
- [ ] Duplicate category names are prevented
- [ ] Empty category names are rejected
- [ ] Category folder is created in `classified_images/`

### Test 5: Image Classification
- [ ] "Classify and Move Image" button is enabled when category selected
- [ ] Image is copied (not moved) to category folder
- [ ] Original image remains in source location
- [ ] Classified image appears in `classified_images/<category>/`
- [ ] Application advances to next image automatically
- [ ] Progress bar updates correctly
- [ ] Processed count increments

### Test 6: Duplicate Filename Handling
- [ ] Classify same filename to same category twice
- [ ] Second file gets timestamp suffix
- [ ] Format: `filename_YYYYMMDD_HHMMSS.ext`
- [ ] Both files exist in category folder
- [ ] No files are overwritten

### Test 7: Navigation
- [ ] "Previous" button goes to previous image
- [ ] "Next" button goes to next image
- [ ] "Skip" button advances without classifying
- [ ] "Previous" disabled on first image
- [ ] "Next" disabled on last image
- [ ] Current image number updates correctly

### Test 8: Window Resizing
- [ ] Window can be resized
- [ ] Image re-scales to fit new window size
- [ ] Aspect ratio is maintained
- [ ] UI elements remain properly laid out

## Object Detection Mode Testing

### Test 9: Basic Image Loading (Detection Mode)
- [ ] "Open Single Image" button works
- [ ] Selected image loads in ImageCanvas
- [ ] Image scales to fit canvas while maintaining aspect ratio
- [ ] Image info displays correctly
- [ ] Canvas has gray background when no image loaded

### Test 10: Folder Loading (Detection Mode)
- [ ] "Open Folder" button works
- [ ] All supported images are loaded
- [ ] Progress bar shows correct total count
- [ ] First image displays in canvas

### Test 11: Label Management
- [ ] "New Label" input field accepts text
- [ ] "Add Label" button creates new label
- [ ] Enter key in input field creates label
- [ ] Label appears in available labels list
- [ ] Duplicate label names are prevented
- [ ] Empty label names are rejected
- [ ] Labels persist across images in same session

### Test 12: Drawing Bounding Boxes
- [ ] Click and drag on image creates bounding box
- [ ] Box appears with dashed green outline while drawing
- [ ] Label selection dialog appears after drawing
- [ ] Selected label is assigned to box
- [ ] Box appears with solid colored outline
- [ ] Label text displays above box
- [ ] Box color is distinct for each box
- [ ] Minimum box size enforced (10x10 pixels)

### Test 13: Bounding Box Selection
- [ ] Click on box selects it
- [ ] Selected box has thicker outline
- [ ] Selected box shows corner handles
- [ ] Box list highlights selected box
- [ ] "Delete Selected Box" button becomes enabled
- [ ] Click on empty area deselects box

### Test 14: Bounding Box Resizing
- [ ] Click and drag corner handle resizes box
- [ ] Top-left corner moves top-left edge
- [ ] Top-right corner moves top-right edge
- [ ] Bottom-left corner moves bottom-left edge
- [ ] Bottom-right corner moves bottom-right edge
- [ ] Box maintains valid rectangle during resize
- [ ] Resized box updates in box list

### Test 15: Bounding Box Deletion
- [ ] Select box and press Delete key removes it
- [ ] Select box and press Backspace key removes it
- [ ] Click "Delete Selected Box" button removes it
- [ ] Box disappears from canvas
- [ ] Box disappears from box list
- [ ] Selection is cleared after deletion

### Test 16: Annotation Saving
- [ ] "Save Annotations" button saves current image
- [ ] Annotation file created in `annotated_images/labels/`
- [ ] Annotation filename matches image basename with .txt extension
- [ ] Image copied to `annotated_images/images/`
- [ ] `classes.txt` file created with label names
- [ ] YOLO format is correct (normalized coordinates)
- [ ] Each box is one line in annotation file
- [ ] Format: `<class_id> <x_center> <y_center> <width> <height>`

### Test 17: Annotation Loading
- [ ] Save annotations for an image
- [ ] Navigate to next image
- [ ] Navigate back to previous image
- [ ] Annotations are loaded automatically
- [ ] All boxes appear correctly
- [ ] Labels are displayed correctly
- [ ] Box positions match original

### Test 18: Save and Next
- [ ] "Save & Next" button saves and advances
- [ ] Annotations are saved
- [ ] Next image loads
- [ ] Progress updates correctly
- [ ] Processed count increments

### Test 19: Navigation (Detection Mode)
- [ ] "Previous" button loads previous image with annotations
- [ ] "Next" button loads next image
- [ ] "Skip" button advances without saving
- [ ] Navigation buttons enable/disable correctly
- [ ] Current image number updates

### Test 20: Multiple Boxes Per Image
- [ ] Draw multiple boxes on same image
- [ ] Each box can have different label
- [ ] All boxes are saved to annotation file
- [ ] All boxes are loaded correctly
- [ ] Each box can be selected independently
- [ ] Each box can be resized independently
- [ ] Each box can be deleted independently

### Test 21: Coordinate Transformation
- [ ] Resize window while image is loaded
- [ ] Image re-scales correctly
- [ ] Existing boxes re-scale correctly
- [ ] Box positions remain accurate relative to image
- [ ] Drawing new boxes works correctly after resize
- [ ] Saved coordinates are in image space (not screen space)

### Test 22: YOLO Format Validation
- [ ] Open saved annotation file
- [ ] Verify format: `<class_id> <x_center> <y_center> <width> <height>`
- [ ] Verify all values are between 0.0 and 1.0
- [ ] Verify class_id matches line in classes.txt (0-indexed)
- [ ] Verify coordinates are normalized (relative to image dimensions)
- [ ] Verify center point is center of box, not top-left

### Test 23: Edge Cases
- [ ] Load image with no annotations (should work without errors)
- [ ] Draw box at image edge (should not go outside image bounds)
- [ ] Draw very small box (should be rejected if < 10x10)
- [ ] Draw very large box (should work, covering most of image)
- [ ] Create many labels (20+) (should all work)
- [ ] Create many boxes per image (50+) (should all work)
- [ ] Load folder with 100+ images (should handle efficiently)

### Test 24: Error Handling
- [ ] Try to draw box without creating labels first (should show warning)
- [ ] Try to classify without selecting category (should show warning)
- [ ] Load corrupted image file (should show error, not crash)
- [ ] Load folder with no images (should show warning)
- [ ] Try to save with no boxes (should show info message)

### Test 25: Close and Exit
- [ ] Close window with unsaved annotations (should prompt to save)
- [ ] Choose "Yes" to save (should save and close)
- [ ] Choose "No" to discard (should close without saving)
- [ ] Choose "Cancel" (should not close)
- [ ] Close window with no annotations (should close immediately)

## Integration Testing

### Test 26: Mode Switching
- [ ] Launch app, select Image Classification mode
- [ ] Close window
- [ ] Launch app again, select Object Detection mode
- [ ] Both modes work independently
- [ ] Output folders are separate

### Test 27: Output Verification
- [ ] Classify 10 images into 3 categories
- [ ] Verify all images copied to correct folders
- [ ] Annotate 10 images with bounding boxes
- [ ] Verify all images copied to `annotated_images/images/`
- [ ] Verify all annotation files in `annotated_images/labels/`
- [ ] Verify `classes.txt` contains all labels
- [ ] Verify no files in wrong locations

### Test 28: Performance Testing
- [ ] Load folder with 500+ images (should be fast)
- [ ] Navigate through images quickly (should be responsive)
- [ ] Draw 50+ boxes on single image (should remain responsive)
- [ ] Save annotations for 100+ images (should complete without issues)

## Regression Testing

### Test 29: Original Classification Mode Still Works
- [ ] All original classification features work
- [ ] No regressions introduced by new code
- [ ] Output structure unchanged
- [ ] Duplicate handling still works
- [ ] Navigation still works

## Documentation Testing

### Test 30: Documentation Accuracy
- [ ] README.md instructions are accurate
- [ ] CLAUDE.md architecture description is accurate
- [ ] Build instructions work as documented
- [ ] Run instructions work as documented
- [ ] Output structure matches documentation

## Test Results Summary

**Date:** _____________

**Tester:** _____________

**Build Version:** _____________

**Total Tests:** 30

**Passed:** _____

**Failed:** _____

**Notes:**
_______________________________________________________________________________
_______________________________________________________________________________
_______________________________________________________________________________

## Known Issues

(Document any known issues discovered during testing)

## Future Enhancements

(Document any enhancement ideas discovered during testing)


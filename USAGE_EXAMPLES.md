# Usage Examples and Scenarios

This document provides practical examples of how to use the Image Classification Tool for various machine learning dataset preparation tasks.

## Example 1: Basic Image Classification

### Scenario
You have 100 photos of animals that need to be classified into three categories: cats, dogs, and birds.

### Steps

1. **Prepare your images**
   ```
   /home/user/animal_photos/
   ├── IMG_001.jpg
   ├── IMG_002.jpg
   ├── IMG_003.jpg
   └── ... (97 more images)
   ```

2. **Launch the application**
   ```bash
   ./build/ImageClassificationTool
   ```

3. **Load the folder**
   - Click "Open Folder"
   - Navigate to `/home/user/animal_photos/`
   - Click "Select Folder"
   - Status shows: "Image 1 of 100 | Processed: 0"

4. **Create categories**
   - Type "cat" in the "New Category" field
   - Click "Add Category" (or press Enter)
   - Repeat for "dog" and "bird"

5. **Classify images**
   - View the first image
   - Select "cat" from the dropdown
   - Click "Classify and Move Image"
   - Application automatically shows the next image
   - Repeat for all 100 images

6. **Result**
   ```
   classified_images/
   ├── cat/
   │   ├── IMG_001.jpg
   │   ├── IMG_005.jpg
   │   └── ... (35 images)
   ├── dog/
   │   ├── IMG_002.jpg
   │   ├── IMG_007.jpg
   │   └── ... (45 images)
   └── bird/
       ├── IMG_003.jpg
       ├── IMG_009.jpg
       └── ... (20 images)
   ```

## Example 2: Quality Control Dataset

### Scenario
You're building a quality control system for manufacturing. You need to classify product images as "pass" or "fail".

### Steps

1. **Load images**
   - Click "Open Folder"
   - Select folder with product inspection photos

2. **Create binary categories**
   - Add category: "pass"
   - Add category: "fail"

3. **Quick classification**
   - For each image:
     - If product looks good → Select "pass" → Classify
     - If product has defects → Select "fail" → Classify
     - If unsure → Click "Skip" (review later)

4. **Review skipped images**
   - Use "Previous" button to go back
   - Classify previously skipped images

### Use Case Benefits
- Fast binary classification
- Skip uncertain cases for later review
- Non-destructive (originals preserved for audit)

## Example 3: Multi-Class Flower Dataset

### Scenario
Creating a dataset for flower species recognition with 10 different species.

### Steps

1. **Prepare categories**
   Create all 10 categories upfront:
   - rose
   - tulip
   - daisy
   - sunflower
   - orchid
   - lily
   - carnation
   - iris
   - daffodil
   - peony

2. **Batch processing**
   - Load folder with 1000+ flower images
   - Progress bar helps track completion
   - "Image X of 1000" shows current position

3. **Efficient workflow**
   - Use dropdown to quickly select species
   - Keyboard navigation (if implemented)
   - Skip difficult-to-identify images

### Tips
- Process images in batches (e.g., 100 at a time)
- Take breaks to maintain accuracy
- Review skipped images at the end

## Example 4: Medical Image Classification

### Scenario
Classifying X-ray images as "normal", "abnormal", or "unclear".

### Steps

1. **Setup**
   - Load medical image folder
   - Create categories: "normal", "abnormal", "unclear"

2. **Classification workflow**
   - Review each X-ray carefully
   - Classify based on medical criteria
   - Use "unclear" for ambiguous cases

3. **Safety features**
   - Images are copied (originals preserved)
   - Can review previous classifications
   - Metadata preserved (filename, size)

### Important Notes
- This tool is for dataset preparation only
- Not for clinical diagnosis
- Always follow medical data handling protocols

## Example 5: Facial Expression Dataset

### Scenario
Building a dataset for emotion recognition with 7 basic emotions.

### Categories
- happy
- sad
- angry
- surprised
- fearful
- disgusted
- neutral

### Workflow

1. **Initial setup**
   ```bash
   ./build/ImageClassificationTool
   # Click "Open Folder" → Select facial images folder
   ```

2. **Create emotion categories**
   - Add all 7 emotion categories
   - Categories appear in dropdown

3. **Classification process**
   - View each face
   - Identify primary emotion
   - Select from dropdown
   - Classify and move to next

4. **Handle edge cases**
   - Multiple emotions → Choose dominant one
   - Unclear expression → Skip for later review
   - Use "Previous" to review decisions

## Example 6: Document Classification

### Scenario
Organizing scanned documents into categories: invoices, receipts, contracts, letters.

### Steps

1. **Load scanned documents**
   - Folder contains mixed document scans (PNG/JPG)
   - 500+ documents to classify

2. **Create document categories**
   - invoice
   - receipt
   - contract
   - letter
   - other

3. **Process documents**
   - View document image
   - Identify document type
   - Classify accordingly
   - Progress: "Image 50 of 500 | Processed: 49"

4. **Output for OCR pipeline**
   ```
   classified_images/
   ├── invoice/
   ├── receipt/
   ├── contract/
   ├── letter/
   └── other/
   ```
   - Ready for downstream OCR processing
   - Organized by document type

## Example 7: Satellite Image Classification

### Scenario
Classifying satellite images by land use: urban, forest, water, agricultural, desert.

### Workflow

1. **Load satellite imagery**
   - High-resolution satellite images
   - Various geographic regions

2. **Create land use categories**
   - urban
   - forest
   - water
   - agricultural
   - desert

3. **Classification tips**
   - Use image scaling to see details
   - Resize window for better view
   - Skip ambiguous mixed-use areas

## Example 8: Food Image Dataset

### Scenario
Creating a food recognition dataset with 20 food categories.

### Efficient Workflow

1. **Pre-sort if possible**
   - If images are partially organized, process one category at a time
   - Load folder → Classify all pizzas → Load next folder

2. **Create all categories first**
   - pizza, burger, sushi, pasta, salad, etc.
   - Having all categories ready speeds up classification

3. **Batch processing strategy**
   - Process 50-100 images per session
   - Take breaks to maintain accuracy
   - Track progress with progress bar

## Example 9: Handwritten Digit Classification

### Scenario
Preparing MNIST-style dataset with handwritten digits 0-9.

### Steps

1. **Create numeric categories**
   - 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

2. **Process handwritten samples**
   - Load folder with digit images
   - Classify each digit
   - Skip illegible samples

3. **Quality control**
   - Review classifications using "Previous"
   - Ensure balanced dataset (similar count per digit)

## Example 10: Custom Dataset with Subcategories

### Scenario
Vehicle classification with make and model.

### Approach 1: Flat Structure
Categories: toyota_camry, honda_civic, ford_f150, etc.

### Approach 2: Two-Pass Classification
First pass: Classify by make (toyota, honda, ford)
Second pass: Load each make folder, classify by model

### Implementation
```bash
# First pass
./build/ImageClassificationTool
# Classify by make

# Second pass - Toyota models
./build/ImageClassificationTool
# Load classified_images/toyota/
# Classify into: camry, corolla, rav4, etc.
```

## Tips and Best Practices

### Efficiency Tips

1. **Prepare categories in advance**
   - Know your categories before starting
   - Create all categories at the beginning

2. **Work in batches**
   - Don't try to classify 1000s of images in one session
   - Take breaks every 50-100 images

3. **Use skip wisely**
   - Skip ambiguous cases
   - Review skipped images when fresh

4. **Keyboard shortcuts** (if available)
   - Faster than mouse clicking
   - Reduces repetitive strain

### Quality Tips

1. **Consistency**
   - Define clear criteria for each category
   - Document edge cases
   - Review classifications periodically

2. **Accuracy over speed**
   - Take time to classify correctly
   - Better to skip than misclassify

3. **Regular breaks**
   - Fatigue leads to errors
   - Fresh eyes improve accuracy

### Organization Tips

1. **Backup originals**
   - Keep original images safe
   - Tool copies (doesn't move) by default

2. **Document your process**
   - Note category definitions
   - Record any special cases
   - Track completion progress

3. **Verify output**
   - Spot-check classified folders
   - Ensure images are in correct categories
   - Check for duplicates

## Common Workflows

### Workflow 1: Single Session
```
Load images → Create categories → Classify all → Done
```
Best for: Small datasets (<100 images)

### Workflow 2: Multi-Session
```
Session 1: Classify 100 images
Session 2: Classify next 100 images
...
Session N: Review and classify skipped images
```
Best for: Large datasets (>500 images)

### Workflow 3: Iterative Refinement
```
Pass 1: Quick classification (skip uncertain)
Pass 2: Review skipped images
Pass 3: Quality check with "Previous" button
```
Best for: High-accuracy requirements

## Troubleshooting Common Scenarios

### Scenario: Duplicate Filenames
**Problem**: Multiple images named "IMG_001.jpg" in different source folders

**Solution**: Tool automatically adds timestamps
- First: `IMG_001.jpg`
- Second: `IMG_001_20231015_143022.jpg`

### Scenario: Large Image Files
**Problem**: 20MB+ images load slowly

**Solution**: 
- Tool scales images for display
- Original quality preserved in output
- Consider preprocessing very large images

### Scenario: Mixed File Formats
**Problem**: Folder has JPG, PNG, BMP, and other files

**Solution**: Tool automatically filters supported formats
- Only JPG, PNG, BMP are loaded
- Other files are ignored

## Conclusion

The Image Classification Tool is versatile and can handle various dataset preparation scenarios. The key to efficiency is:

1. **Plan ahead**: Know your categories
2. **Work systematically**: Process in batches
3. **Stay consistent**: Define clear criteria
4. **Review regularly**: Use navigation to check work
5. **Take breaks**: Maintain accuracy

Happy classifying!


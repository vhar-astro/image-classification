# Quick Start Guide

Get up and running with the Image Classification Tool in just a few minutes!

## Step 1: Install Dependencies

Run the automated installation script:

```bash
chmod +x install_dependencies.sh
./install_dependencies.sh
```

This will install:
- Build essentials (g++, make)
- CMake
- Qt5 development libraries

## Step 2: Build the Application

Run the build script:

```bash
./build.sh
```

This will:
- Create a `build` directory
- Configure the project with CMake
- Compile the application

## Step 3: Run the Application

```bash
./build/ImageClassificationTool
```

## Step 4: Start Classifying Images

1. **Load Images**: Click "Open Folder" and select a folder containing images
2. **Create Categories**: Type a category name (e.g., "cat") and click "Add Category"
3. **Classify**: Select a category from the dropdown and click "Classify and Move Image"
4. **Repeat**: The app automatically moves to the next image

## Output

Classified images will be organized in:
```
classified_images/
├── category1/
│   ├── image1.jpg
│   └── image2.jpg
├── category2/
│   ├── image3.jpg
│   └── image4.jpg
└── ...
```

## Troubleshooting

**Build fails with Qt5 not found:**
```bash
# Run the dependency installation script again
./install_dependencies.sh
```

**Permission denied when running scripts:**
```bash
chmod +x install_dependencies.sh build.sh
```

**Application won't start:**
```bash
# Check if Qt5 runtime is installed
sudo apt install qt5-default
```

For more detailed information, see [README.md](README.md).


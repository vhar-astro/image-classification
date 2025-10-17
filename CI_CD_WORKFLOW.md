# CI/CD Workflow Documentation

This document explains the GitHub Actions workflow for building the ML Dataset Preparation Tool across all three supported platforms.

## Overview

The workflow automatically builds standalone executables/bundles for:
- **Windows 10** - Standalone .exe with Qt DLLs
- **macOS 10.14+** - Native .app bundle with Qt frameworks
- **Linux/Ubuntu 18.04+** - Binary with launcher scripts

## Workflow Triggers

The workflow runs on:
- **Push** to `master`, `main`, or `develop` branches
- **Pull requests** to `master`, `main`, or `develop` branches
- **Manual trigger** via GitHub Actions UI (workflow_dispatch)

## Build Jobs

### 1. Windows Build (`build-windows`)

**Runner:** `windows-latest` (Windows Server 2022)

**Steps:**
1. **Checkout code** - Clone the repository
2. **Install Qt 5.15.2** - Using `jurplel/install-qt-action@v3`
   - Architecture: `win64_msvc2019_64`
   - No modules specified (qtbase included by default)
   - Caching enabled for faster subsequent builds
3. **Configure MSVC** - Setup Visual Studio 2019 build tools
4. **Clean build directory** - Remove any existing build artifacts
5. **Configure CMake** - Generate Visual Studio 2019 solution
6. **Build** - Compile in Release mode
7. **Verify build** - Check that MLDatasetTool.exe exists
8. **Deploy Qt dependencies** - Run windeployqt to bundle DLLs
9. **Package** - Copy all files to artifact directory
10. **Upload artifact** - Upload to GitHub Actions artifacts

**Output:**
- `MLDatasetTool.exe` - Main executable
- Qt DLLs (QtCore5.dll, QtGui5.dll, QtWidgets5.dll, etc.)
- Platform plugins (qwindows.dll)
- Image format plugins (qjpeg.dll, qpng.dll, etc.)

**Artifact Name:** `MLDatasetTool-Windows`

### 2. Linux Build (`build-linux`)

**Runner:** `ubuntu-latest` (Ubuntu 22.04)

**Steps:**
1. **Checkout code** - Clone the repository
2. **Install dependencies** - Install build tools and Qt5
   - build-essential, cmake
   - qtbase5-dev, qtbase5-dev-tools, qt5-qmake
3. **Verify Qt installation** - Check qmake and cmake versions
4. **Clean build directory** - Remove any existing build artifacts
5. **Configure CMake** - Generate Unix Makefiles
6. **Build** - Compile using make with all CPU cores
7. **Verify build** - Check that ImageClassificationTool exists
8. **Package** - Copy binary, launcher scripts, and README
9. **Upload artifact** - Upload to GitHub Actions artifacts

**Output:**
- `ImageClassificationTool` - Main executable
- `run_app.sh` - Recommended launcher script
- `launch_clean.sh` - Alternative launcher with maximum isolation
- `README.md` - Documentation

**Artifact Name:** `MLDatasetTool-Linux`

### 3. macOS Build (`build-macos`)

**Runner:** `macos-latest` (macOS 12 Monterey)

**Steps:**
1. **Checkout code** - Clone the repository
2. **Install Qt 5.15.2** - Using `jurplel/install-qt-action@v3`
   - No modules specified (qtbase included by default)
   - Caching enabled for faster subsequent builds
3. **Verify Qt installation** - Check qmake version
4. **Clean build directory** - Remove any existing build artifacts
5. **Configure CMake** - Generate Unix Makefiles with macOS deployment target
6. **Build** - Compile using make with all CPU cores
7. **Verify build** - Check that MLDatasetTool.app exists
8. **Deploy Qt dependencies** - Run macdeployqt to bundle frameworks
9. **Verify Qt frameworks** - Check that frameworks are bundled
10. **Package** - Copy .app bundle to artifact directory
11. **Create DMG** (optional) - Create disk image for distribution
12. **Upload artifacts** - Upload .app and DMG to GitHub Actions

**Output:**
- `MLDatasetTool.app` - Application bundle with Qt frameworks
- `MLDatasetTool.dmg` - Disk image (optional)

**Artifact Names:** 
- `MLDatasetTool-macOS` (app bundle)
- `MLDatasetTool-macOS-DMG` (disk image)

### 4. Release Summary (`create-release`)

**Runner:** `ubuntu-latest`

**Dependencies:** Requires all three build jobs to complete successfully

**Condition:** Only runs on push to `master` or `main` branches

**Steps:**
1. **Download all artifacts** - Download from all build jobs
2. **Display structure** - Show artifact contents and sizes
3. **Create release info** - Generate markdown summary with:
   - Build metadata (commit, branch, date)
   - Artifact descriptions for each platform
   - Usage instructions
   - Feature highlights
   - Documentation links
4. **Upload release info** - Upload summary as artifact

**Output:**
- `release-info.md` - Comprehensive build summary

**Artifact Name:** `release-info`

## Key Fixes Applied

### 1. Qt Installation Fix (Windows & macOS)

**Problem:** 
```
The packages ['qtbase'] were not found while parsing XML of package information!
```

**Root Cause:** 
The `--modules qtbase` flag was incorrect. In Qt5, `qtbase` is not a separate module but the base installation that's included by default.

**Solution:**
Removed the `modules: 'qtbase'` line from the Qt installation step. The base Qt5 installation includes all necessary components (QtCore, QtGui, QtWidgets) by default.

**Before:**
```yaml
- name: Install Qt
  uses: jurplel/install-qt-action@v3
  with:
    version: '5.15.2'
    modules: 'qtbase'  # ❌ Incorrect
```

**After:**
```yaml
- name: Install Qt
  uses: jurplel/install-qt-action@v3
  with:
    version: '5.15.2'
    # ✓ No modules specified - qtbase included by default
```

### 2. Build Directory Conflict Fix (All Platforms)

**Problem:**
```
mkdir: cannot create directory 'build': File exists
```

**Root Cause:**
Build directory persisted from previous runs or caching, causing conflicts.

**Solution:**
Added explicit build directory cleanup before creating new one.

**Windows:**
```powershell
if (Test-Path build) { Remove-Item -Recurse -Force build }
```

**Linux/macOS:**
```bash
rm -rf build
```

### 3. CMake Generator Fix (Windows)

**Problem:**
Using Ninja generator which may not be available or properly configured.

**Solution:**
Changed to Visual Studio 2019 generator which is guaranteed to be available on `windows-latest` runners.

**Before:**
```yaml
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
```

**After:**
```yaml
cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release
```

### 4. Output Verification (All Platforms)

**Problem:**
Builds could fail silently without clear error messages.

**Solution:**
Added explicit verification steps that check for expected output files and provide clear success/failure messages.

**Example (Windows):**
```powershell
if (Test-Path build\Release\MLDatasetTool.exe) {
  Write-Host "✓ MLDatasetTool.exe built successfully"
} else {
  Write-Error "✗ MLDatasetTool.exe not found!"
  exit 1
}
```

### 5. macOS Deployment Target

**Problem:**
No minimum macOS version specified, potentially causing compatibility issues.

**Solution:**
Added `CMAKE_OSX_DEPLOYMENT_TARGET=10.14` to ensure compatibility with macOS 10.14 (Mojave) and later.

```yaml
cmake .. -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14
```

## Parallel Execution

All three build jobs run **in parallel** for maximum efficiency:
- No dependencies between platform builds
- Each job has isolated environment
- Build directory cleanup prevents conflicts
- Caching is platform-specific

**Benefits:**
- Faster total build time (~10-15 minutes vs 30-45 minutes sequential)
- Independent failure isolation
- Efficient resource utilization

## Artifact Retention

All artifacts are retained for **30 days** by default. This can be adjusted in the workflow file:

```yaml
retention-days: 30
```

## Downloading Artifacts

### Via GitHub UI

1. Go to the repository's "Actions" tab
2. Click on a workflow run
3. Scroll to "Artifacts" section
4. Click on artifact name to download

### Via GitHub CLI

```bash
# List artifacts for a run
gh run view <run-id> --log

# Download specific artifact
gh run download <run-id> -n MLDatasetTool-Windows

# Download all artifacts
gh run download <run-id>
```

## Local Testing

To test the workflow locally before pushing:

### Using act (GitHub Actions local runner)

```bash
# Install act
brew install act  # macOS
# or
curl https://raw.githubusercontent.com/nektos/act/master/install.sh | sudo bash  # Linux

# Run workflow locally
act push

# Run specific job
act -j build-windows
act -j build-linux
act -j build-macos
```

**Note:** Local testing has limitations:
- May not perfectly replicate GitHub's environment
- Some actions may not work locally
- Useful for catching obvious errors

## Troubleshooting

### Build Fails on Windows

**Check:**
1. Qt installation succeeded
2. MSVC tools are available
3. CMake can find Qt (`CMAKE_PREFIX_PATH` set correctly)
4. windeployqt is in PATH

**Common Issues:**
- Qt installation timeout (increase timeout or retry)
- MSVC version mismatch (ensure Visual Studio 2019)
- Path issues (check `$env:Qt5_DIR`)

### Build Fails on Linux

**Check:**
1. All dependencies installed (`qtbase5-dev`, etc.)
2. Qt version is 5.x (not Qt6)
3. Build directory cleaned properly
4. Sufficient disk space

**Common Issues:**
- Missing dependencies (add to apt-get install)
- Qt4 vs Qt5 conflicts (ensure Qt5 packages)
- Permission issues (shouldn't occur on GitHub runners)

### Build Fails on macOS

**Check:**
1. Qt installation succeeded
2. Xcode Command Line Tools available
3. macdeployqt runs successfully
4. Frameworks bundled in .app

**Common Issues:**
- Qt installation timeout (increase timeout or retry)
- macdeployqt not found (check Qt installation)
- Framework bundling fails (check disk space)

### Artifacts Not Uploaded

**Check:**
1. Build step completed successfully
2. Artifact path exists
3. Artifact size within limits (GitHub has size limits)
4. Upload action version is current

**Common Issues:**
- Path typos in upload step
- Artifact too large (>2GB limit per artifact)
- Network issues (retry workflow)

## Workflow Optimization

### Current Optimizations

1. **Caching:** Qt installations are cached to speed up subsequent runs
2. **Parallel execution:** All platforms build simultaneously
3. **Minimal dependencies:** Only install what's needed
4. **Efficient packaging:** Only include necessary files

### Future Improvements

1. **Matrix builds:** Build multiple Qt versions or configurations
2. **Conditional builds:** Skip unchanged platforms
3. **Incremental builds:** Cache build artifacts between runs
4. **Automated testing:** Run tests before packaging
5. **Code signing:** Sign executables for distribution
6. **Release automation:** Auto-create GitHub releases

## Security Considerations

1. **No secrets required:** Workflow uses only public actions
2. **Read-only checkout:** Code is checked out read-only
3. **Isolated environments:** Each job runs in fresh VM
4. **Artifact encryption:** GitHub encrypts artifacts at rest
5. **Limited retention:** Artifacts auto-delete after 30 days

## Monitoring

### Workflow Status

Check workflow status:
- Repository "Actions" tab
- Commit status checks
- Email notifications (if enabled)
- GitHub mobile app

### Build Metrics

Monitor:
- Build duration (target: <15 minutes total)
- Success rate (target: >95%)
- Artifact sizes (Windows: ~20MB, macOS: ~25MB, Linux: ~5MB)
- Cache hit rate (target: >80%)

## Support

For workflow issues:
1. Check workflow logs in GitHub Actions
2. Review this documentation
3. Check platform-specific build guides:
   - Windows: `BUILD_WINDOWS.md`
   - macOS: `BUILD_MACOS.md`
   - Linux: `README.md`
4. Open an issue with workflow logs attached

## References

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [install-qt-action](https://github.com/jurplel/install-qt-action)
- [Qt Documentation](https://doc.qt.io/qt-5/)
- [CMake Documentation](https://cmake.org/documentation/)


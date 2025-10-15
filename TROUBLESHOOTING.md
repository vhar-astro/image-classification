# Troubleshooting Guide

This document provides solutions for common issues encountered when running the Image Classification Tool.

## Runtime Errors

### 1. Symbol Lookup Error with libpthread

**Error Message:**
```
./build/ImageClassificationTool: symbol lookup error: /snap/core20/current/lib/x86_64-linux-gnu/libpthread.so.0: undefined symbol: __libc_pthread_init, version GLIBC_PRIVATE
```

**Root Cause:**
This error occurs due to conflicts between system glibc libraries and snap package libraries. The system has a newer glibc version (e.g., 2.39) while snap packages use an older version (e.g., 2.31 from core20). The dynamic linker tries to load the older libpthread from the snap environment, which lacks symbols expected by the newer system.

**Solution:**
Use one of the provided launcher scripts that clean the environment:

```bash
# Recommended approach
./run_app.sh

# Alternative with maximum isolation
./launch_clean.sh
```

### 2. Qt Threading Error

**Error Message:**
```
QSocketNotifier: Can only be used with threads started with QThread
```

**Root Cause:**
This error is related to Qt's threading model being affected by environment variable conflicts from snap packages.

**Solution:**
The launcher scripts also resolve this issue by setting proper Qt environment variables and cleaning snap-related variables.

### 3. Application Won't Start

**Symptoms:**
- Application exits immediately
- No window appears
- Various library loading errors

**Solutions:**

1. **Use the launcher scripts:**
   ```bash
   ./run_app.sh
   ```

2. **Check executable permissions:**
   ```bash
   chmod +x ./build/ImageClassificationTool
   ```

3. **Verify Qt5 runtime libraries:**
   ```bash
   sudo apt install qt5-default qtbase5-dev
   ```

4. **Check display environment:**
   ```bash
   echo $DISPLAY
   # Should show something like :0 or :1
   ```

## Build Issues

### CMake Can't Find Qt5

**Error Message:**
```
Could not find a package configuration file provided by "Qt5"
```

**Solution:**
```bash
# Install Qt5 development packages
sudo apt install qtbase5-dev qtbase5-dev-tools

# If still not found, specify Qt5 path explicitly
cmake -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt5 ..
```

### Compiler Errors

**Error Message:**
```
error: 'class' does not name a type
```

**Solution:**
Ensure you have a C++11 compatible compiler:
```bash
g++ --version  # Should be 4.8.1 or later
sudo apt install build-essential
```

## Environment Issues

### Snap Package Conflicts

**Problem:**
Snap packages (like VS Code) can pollute the environment with variables that interfere with native applications.

**Prevention:**
Always use the provided launcher scripts when running the application, especially if you have snap packages installed.

**Manual Environment Cleaning:**
If you need to run the application directly, clean these variables first:
```bash
unset SNAP SNAP_ARCH SNAP_COMMON SNAP_DATA SNAP_LIBRARY_PATH
unset LOCPATH GTK_PATH FONTCONFIG_PATH
export PATH=$(echo "$PATH" | tr ':' '\n' | grep -v '/snap/' | tr '\n' ':' | sed 's/:$//')
export LD_LIBRARY_PATH="/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu"
export QT_QPA_PLATFORM=xcb
export QT_PLUGIN_PATH="/usr/lib/x86_64-linux-gnu/qt5/plugins"
```

## Launcher Scripts Explained

### run_app.sh
- Cleans snap-related environment variables
- Sets proper library paths
- Configures Qt environment
- Recommended for most users

### launch_clean.sh
- Uses `env -i` for complete environment isolation
- Sets only essential environment variables
- Maximum protection against conflicts
- Use if `run_app.sh` doesn't work

## System Requirements

### Minimum Requirements
- Ubuntu 18.04 or later (or equivalent)
- Qt5 >= 5.9
- glibc >= 2.27
- X11 or Wayland display server

### Recommended
- Ubuntu 20.04 or later
- Qt5 >= 5.12
- 4GB RAM
- Dedicated graphics card for better performance

## Getting Help

If you continue to experience issues:

1. **Check system compatibility:**
   ```bash
   ldd --version  # Check glibc version
   qmake --version  # Check Qt version
   ```

2. **Run with debugging:**
   ```bash
   strace -e trace=openat ./run_app.sh 2>&1 | head -20
   ```

3. **Check library dependencies:**
   ```bash
   ldd ./build/ImageClassificationTool
   ```

4. **Verify no conflicting processes:**
   ```bash
   ps aux | grep ImageClassification
   ```

## Prevention

To avoid these issues in the future:

1. **Always use launcher scripts** instead of running the executable directly
2. **Keep system libraries updated** but avoid mixing package managers
3. **Be cautious with snap packages** that might affect system environment
4. **Use virtual environments** or containers for complex setups

## Quick Reference

| Issue | Solution |
|-------|----------|
| Symbol lookup error | Use `./run_app.sh` |
| Qt threading error | Use `./run_app.sh` |
| App won't start | Check permissions, use launcher |
| Build fails | Install Qt5 dev packages |
| Library conflicts | Use `./launch_clean.sh` |

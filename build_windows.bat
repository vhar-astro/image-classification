@echo off
REM ============================================================================
REM ML Dataset Preparation Tool - Windows Build Script
REM ============================================================================
REM This script automates the build process for Windows 10
REM 
REM Prerequisites:
REM   - Visual Studio 2019 or 2022 installed
REM   - CMake installed and in PATH
REM   - Qt5 installed (adjust QT_PATH below)
REM
REM Usage:
REM   1. Edit QT_PATH to match your Qt installation
REM   2. Double-click this file or run from command prompt
REM ============================================================================

echo.
echo ============================================================================
echo   ML Dataset Preparation Tool - Windows Build Script
echo ============================================================================
echo.

REM ============================================================================
REM CONFIGURATION - ADJUST THESE PATHS TO MATCH YOUR SYSTEM
REM ============================================================================

REM Qt installation path - CHANGE THIS TO YOUR QT INSTALLATION
set QT_PATH=C:\Qt\5.15.2\msvc2019_64

REM Visual Studio version (2019 or 2022)
set VS_VERSION=2019

REM Build configuration (Release or Debug)
set BUILD_CONFIG=Release

REM ============================================================================
REM VALIDATION
REM ============================================================================

echo [1/6] Validating environment...

REM Check if CMake is available
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: CMake not found in PATH!
    echo Please install CMake from https://cmake.org/download/
    echo Make sure to add CMake to system PATH during installation.
    pause
    exit /b 1
)
echo   - CMake found

REM Check if Qt path exists
if not exist "%QT_PATH%" (
    echo ERROR: Qt installation not found at: %QT_PATH%
    echo.
    echo Please edit this script and set QT_PATH to your Qt installation directory.
    echo Example: C:\Qt\5.15.2\msvc2019_64
    echo.
    echo If you haven't installed Qt yet, download it from:
    echo https://www.qt.io/download-qt-installer
    pause
    exit /b 1
)
echo   - Qt found at: %QT_PATH%

REM Check if qmake exists
if not exist "%QT_PATH%\bin\qmake.exe" (
    echo ERROR: qmake.exe not found in Qt installation!
    echo Expected location: %QT_PATH%\bin\qmake.exe
    pause
    exit /b 1
)
echo   - qmake found

REM ============================================================================
REM BUILD DIRECTORY SETUP
REM ============================================================================

echo.
echo [2/6] Setting up build directory...

if not exist build (
    mkdir build
    echo   - Created build directory
) else (
    echo   - Build directory exists
)

cd build

REM ============================================================================
REM CMAKE CONFIGURATION
REM ============================================================================

echo.
echo [3/6] Configuring project with CMake...
echo   - Generator: Visual Studio %VS_VERSION%
echo   - Platform: x64
echo   - Qt Path: %QT_PATH%
echo.

if "%VS_VERSION%"=="2019" (
    set CMAKE_GENERATOR=Visual Studio 16 2019
) else if "%VS_VERSION%"=="2022" (
    set CMAKE_GENERATOR=Visual Studio 17 2022
) else (
    echo ERROR: Unsupported Visual Studio version: %VS_VERSION%
    echo Please set VS_VERSION to either 2019 or 2022
    cd ..
    pause
    exit /b 1
)

cmake -G "%CMAKE_GENERATOR%" -A x64 -DCMAKE_PREFIX_PATH="%QT_PATH%" ..

if %errorlevel% neq 0 (
    echo.
    echo ERROR: CMake configuration failed!
    echo.
    echo Common solutions:
    echo   1. Check that QT_PATH is correct in this script
    echo   2. Ensure Visual Studio %VS_VERSION% is installed
    echo   3. Try running from "x64 Native Tools Command Prompt for VS %VS_VERSION%"
    echo.
    cd ..
    pause
    exit /b %errorlevel%
)

echo   - Configuration successful

REM ============================================================================
REM BUILD PROJECT
REM ============================================================================

echo.
echo [4/6] Building project (%BUILD_CONFIG% configuration)...
echo   This may take a few minutes...
echo.

cmake --build . --config %BUILD_CONFIG% -- /m

if %errorlevel% neq 0 (
    echo.
    echo ERROR: Build failed!
    echo.
    echo Please check the error messages above.
    echo If you see Qt-related errors, verify your Qt installation path.
    echo.
    cd ..
    pause
    exit /b %errorlevel%
)

echo   - Build successful

REM ============================================================================
REM DEPLOY QT DEPENDENCIES
REM ============================================================================

echo.
echo [5/6] Deploying Qt dependencies...

set WINDEPLOYQT=%QT_PATH%\bin\windeployqt.exe
set EXE_PATH=%BUILD_CONFIG%\MLDatasetTool.exe

if not exist "%EXE_PATH%" (
    echo ERROR: Executable not found at: %EXE_PATH%
    cd ..
    pause
    exit /b 1
)

echo   - Running windeployqt...
"%WINDEPLOYQT%" --no-translations --no-system-d3d-compiler --no-opengl-sw "%EXE_PATH%"

if %errorlevel% neq 0 (
    echo WARNING: windeployqt encountered issues
    echo The application may still work, but might be missing some Qt DLLs
) else (
    echo   - Qt dependencies deployed successfully
)

REM ============================================================================
REM VERIFY BUILD
REM ============================================================================

echo.
echo [6/6] Verifying build...

if exist "%EXE_PATH%" (
    echo   - Executable found: %EXE_PATH%
    
    REM Get file size
    for %%A in ("%EXE_PATH%") do set EXE_SIZE=%%~zA
    echo   - File size: %EXE_SIZE% bytes
    
    REM Check for Qt DLLs
    if exist "%BUILD_CONFIG%\Qt5Core.dll" (
        echo   - Qt DLLs deployed: YES
    ) else (
        echo   - Qt DLLs deployed: NO (may need manual deployment)
    )
) else (
    echo ERROR: Executable not found!
    cd ..
    pause
    exit /b 1
)

REM ============================================================================
REM SUCCESS
REM ============================================================================

cd ..

echo.
echo ============================================================================
echo   BUILD COMPLETED SUCCESSFULLY!
echo ============================================================================
echo.
echo Executable location:
echo   %cd%\build\%BUILD_CONFIG%\MLDatasetTool.exe
echo.
echo To run the application:
echo   1. Navigate to: build\%BUILD_CONFIG%\
echo   2. Double-click: MLDatasetTool.exe
echo.
echo To create a portable distribution:
echo   1. Copy the entire build\%BUILD_CONFIG%\ folder
echo   2. Rename it to MLDatasetTool_Portable
echo   3. Distribute the folder (includes all required DLLs)
echo.
echo For more information, see BUILD_WINDOWS.md
echo ============================================================================
echo.

pause


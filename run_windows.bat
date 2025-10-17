@echo off
REM ============================================================================
REM ML Dataset Preparation Tool - Windows Launcher
REM ============================================================================
REM Simple launcher script for the application
REM ============================================================================

REM Check if executable exists in Release folder
if exist "build\Release\MLDatasetTool.exe" (
    echo Starting ML Dataset Preparation Tool...
    start "" "build\Release\MLDatasetTool.exe"
    exit /b 0
)

REM Check if executable exists in Debug folder
if exist "build\Debug\MLDatasetTool.exe" (
    echo Starting ML Dataset Preparation Tool (Debug)...
    start "" "build\Debug\MLDatasetTool.exe"
    exit /b 0
)

REM Executable not found
echo ERROR: Application not found!
echo.
echo Please build the application first using one of these methods:
echo   1. Double-click: build_windows.bat
echo   2. See BUILD_WINDOWS.md for detailed instructions
echo.
pause
exit /b 1


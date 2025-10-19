cmake_minimum_required(VERSION 3.10)
project(ImageClassificationTool VERSION 2.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic MOC, UIC, and RCC for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt5 packages
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# Source files
set(SOURCES
    main.cpp
    MainWindow.cpp
    ModeSelectionDialog.cpp
    ObjectDetectionWindow.cpp
    ImageCanvas.cpp
    BoundingBox.cpp
    AnnotationManager.cpp
)

# Header files
set(HEADERS
    MainWindow.h
    ModeSelectionDialog.h
    ObjectDetectionWindow.h
    ImageCanvas.h
    BoundingBox.h
    AnnotationManager.h
)

# Platform-specific settings
if(WIN32)
    # On Windows, create a GUI application (no console window)
    # WIN32 flag tells CMake to create a Windows GUI application
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

    # Set output name to MLDatasetTool.exe for clarity
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "MLDatasetTool")

    # Windows-specific: Copy Qt DLLs to output directory for standalone execution
    # This is handled by windeployqt tool (see build instructions)
elseif(APPLE)
    # On macOS, create an application bundle
    # MACOSX_BUNDLE flag tells CMake to create a macOS .app bundle
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS})

    # Set output name to MLDatasetTool.app for clarity
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "MLDatasetTool"
        MACOSX_BUNDLE_BUNDLE_NAME "ML Dataset Tool"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.mldataset.tool"
        MACOSX_BUNDLE_BUNDLE_VERSION "2.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "2.0"
        MACOSX_BUNDLE_INFO_STRING "ML Dataset Preparation Tool - Image Classification and Object Detection"
    )
else()
    # On Linux/Unix, create standard executable
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
endif()

# Link Qt libraries
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# Platform-specific: Install Qt runtime dependencies
if(WIN32)
    # Windows: Get Qt5 installation path
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    # Add custom command to run windeployqt after build
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --no-translations --no-system-d3d-compiler --no-opengl-sw "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Running windeployqt to copy Qt runtime dependencies..."
    )
elseif(APPLE)
    # macOS: Get Qt5 installation path
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_MACDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_MACDEPLOYQT_EXECUTABLE "${QT5_MACDEPLOYQT_EXECUTABLE}/macdeployqt")

    # Add custom command to run macdeployqt after build
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${QT5_MACDEPLOYQT_EXECUTABLE} "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>" -always-overwrite
        COMMENT "Running macdeployqt to bundle Qt frameworks..."
    )
endif()


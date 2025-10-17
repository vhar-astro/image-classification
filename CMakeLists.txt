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
)

# Windows-specific: Install Qt runtime dependencies
if(WIN32)
    # Get Qt5 installation path
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    # Add custom command to run windeployqt after build
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --no-translations --no-system-d3d-compiler --no-opengl-sw "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Running windeployqt to copy Qt runtime dependencies..."
    )
endif()


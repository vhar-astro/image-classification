cmake_minimum_required(VERSION 3.10)
project(ImageClassificationTool VERSION 1.0 LANGUAGES CXX)

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

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

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


cmake_minimum_required(VERSION 3.16)
project(ImageBrightnessViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 Widgets (prefer) or fall back to Qt5 if needed
find_package(Qt6 COMPONENTS Widgets QUIET)
if (Qt6_FOUND)
    message(STATUS "Configuring with Qt6")
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    message(STATUS "Configuring with Qt5")
    set(QT_PACKAGE Qt5)
endif()

add_executable(ImageBrightnessViewer
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/ImageProcessor.h
    src/ImageProcessor.cpp
)

target_link_libraries(ImageBrightnessViewer PRIVATE ${QT_PACKAGE}::Widgets)

# Improve Windows console output and app icon handling (no console window)
if (WIN32)
    set_target_properties(ImageBrightnessViewer PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
cmake_minimum_required(VERSION 3.5)
project(IntellifireUI)

set(CMAKE_CXX_STANDARD 17)
set(/usr/include/x86_64-linux-gnu/qt5)


# Enable Qt's Meta-Object Compiler (MOC)
set(CMAKE_AUTOMOC ON)

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Add Qt Resource File
qt5_add_resources(RESOURCES resources.qrc)

# Define Executable BEFORE Linking Libraries
add_executable(IntelliFireUI
    main.cpp
    mainwindow.cpp mainwindow.h
    sensorContainer.cpp sensorContainer.h
    ${RESOURCES}  # Include compiled Qt resources
)

# Link Qt Libraries AFTER Defining Executable
target_link_libraries(IntelliFireUI Qt5::Widgets)
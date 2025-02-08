cmake_minimum_required(VERSION 3.5)
project(IntellifireUI)

set(CMAKE_CXX_STANDARD 17)
set(/usr/include/x86_64-linux-gnu/qt5)


# Enable Qt's Meta-Object Compiler (MOC)
set(CMAKE_AUTOMOC ON)

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Add source files
add_executable(IntellifireUI main.cpp mainwindow.cpp sensorContainer.cpp sensorContainer.h mainwindow.h)

# Link Qt5 libraries
target_link_libraries(IntellifireUI Qt5::Widgets)
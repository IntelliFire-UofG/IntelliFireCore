cmake_minimum_required(VERSION 3.10)
project(IntelliFireCore LANGUAGES CXX)

# Enable automatic Qt meta-object and resource processing.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#------------------------------------------------------------------------------
# Find required packages
#------------------------------------------------------------------------------
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

# Locate libgpiod for UltraSonicSensor library
find_library(GPIOD_LIB gpiod)
if(NOT GPIOD_LIB)
  message(FATAL_ERROR "Could not find libgpiod library")
endif()

#------------------------------------------------------------------------------
# Include Directories
#------------------------------------------------------------------------------
# Common include directory for sensor headers.
include_directories(${CMAKE_SOURCE_DIR}/include)
# Include directory for UI headers.
include_directories(${CMAKE_SOURCE_DIR}/src/IntelliFireUI/include)

#------------------------------------------------------------------------------
# Build UltraSonicSensorLib as a static library
#------------------------------------------------------------------------------
add_library(UltraSonicSensorLib
    src/UltraSonicSensor.cpp
    include/UltraSonicSensor.h
)

target_include_directories(UltraSonicSensorLib PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(UltraSonicSensorLib PRIVATE Qt5::Core ${GPIOD_LIB})

#------------------------------------------------------------------------------
# Build the IntelliFire UI Executable
#------------------------------------------------------------------------------
# The UI uses its own main() located in src/IntelliFireUI/main.cpp, and its
# resources (resources.qrc) are now included so that the Qt resource system is used.
add_executable(IntelliFireUI
    src/IntelliFireUI/main.cpp
    src/IntelliFireUI/mainwindow.cpp
    src/IntelliFireUI/sensorContainer.cpp
    src/IntelliFireUI/include/mainwindow.h
    src/IntelliFireUI/include/sensorContainer.h
    src/IntelliFireUI/resources.qrc
)

target_link_libraries(IntelliFireUI PRIVATE Qt5::Widgets UltraSonicSensorLib)

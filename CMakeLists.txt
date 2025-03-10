cmake_minimum_required(VERSION 3.10)

project(IntelliFireCore
    DESCRIPTION "IntelliFire Core with UI, camera preview, and sensor management"
    LANGUAGES CXX
)

#------------------------------------------------------------------------------
# General Settings
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wno-unused-parameter")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

#------------------------------------------------------------------------------
# Enable Qt's Automatic Processing
#------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#------------------------------------------------------------------------------
# Find Required Packages
#------------------------------------------------------------------------------
find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

# OpenCV info
message(STATUS "Qt WebEngineWidgets included")

# Locate libgpiod for UltraSonicSensor library
find_library(GPIOD_LIB gpiod)
if(NOT GPIOD_LIB)
  message(FATAL_ERROR "Could not find libgpiod library")
endif()

#------------------------------------------------------------------------------
# Include Directories
#------------------------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/IntelliFireUI/include
)

#------------------------------------------------------------------------------
# Build UltraSonicSensorLib as a Static Library
#------------------------------------------------------------------------------
add_library(UltraSonicSensorLib STATIC
    src/UltraSonicSensor.cpp
    include/UltraSonicSensor.h
)

target_include_directories(UltraSonicSensorLib PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(UltraSonicSensorLib PRIVATE Qt5::Core ${GPIOD_LIB})

#------------------------------------------------------------------------------
# IntelliFire UI Executable (with Camera and Sensor Support)
#------------------------------------------------------------------------------
add_executable(IntelliFireUI
    src/IntelliFireUI/main.cpp
    src/IntelliFireUI/mainwindow.cpp
    src/IntelliFireUI/sensorContainer.cpp
    src/IntelliFireUI/src/keyLogger.cpp
    src/IntelliFireUI/resources.qrc
    src/IntelliFireUI/include/mainwindow.h
    src/IntelliFireUI/include/sensorContainer.h
    src/IntelliFireUI/include/keyLogger.h
)

# Linking all required libraries
target_link_libraries(IntelliFireUI
    Qt5::Widgets
    Qt5::WebEngineWidgets
    UltraSonicSensorLib
    qwt-qt5
    ${GPIOD_LIB}
)

#------------------------------------------------------------------------------
# Install Rules (Optional)
#------------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS IntelliFireUI RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS UltraSonicSensorLib ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
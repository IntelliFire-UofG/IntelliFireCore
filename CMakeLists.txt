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
find_package(OpenCV REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

# libcamera library
pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)
message(STATUS "libcamera library found:")
message(STATUS "    version: ${LIBCAMERA_VERSION}")
message(STATUS "    libraries: ${LIBCAMERA_LINK_LIBRARIES}")
message(STATUS "    include path: ${LIBCAMERA_INCLUDE_DIRS}")

# OpenCV info
message(STATUS "OpenCV:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

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
    ${LIBCAMERA_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

#------------------------------------------------------------------------------
# Build UltraSonicSensorLib as a Static Library
#------------------------------------------------------------------------------
add_library(UltraSonicSensorLib STATIC
    src/UltraSonicSensor.cpp
    src/IRSensor.cpp
    include/UltraSonicSensor.h
    include/IRSensor.h
)

target_include_directories(UltraSonicSensorLib PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(UltraSonicSensorLib PRIVATE Qt5::Core ${GPIOD_LIB})

#------------------------------------------------------------------------------
# Build ADS1115Lib as a static library
#------------------------------------------------------------------------------
add_library(ADS1115Lib STATIC
    src/ads1115rpi.cpp
    include/ads1115rpi.h
)

target_include_directories(ADS1115Lib PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ADS1115Lib PRIVATE ${GPIOD_LIB})

#------------------------------------------------------------------------------
# Build ADS1115ManagerLib as a static library
#------------------------------------------------------------------------------
add_library(ADS1115ManagerLib STATIC
    src/IntelliFireUI/src/ads1115manager.cpp
    src/IntelliFireUI/include/ads1115manager.h
)

target_include_directories(ADS1115ManagerLib PRIVATE ${CMAKE_SOURCE_DIR}/src/IntelliFireUI/include)
target_link_libraries(ADS1115ManagerLib PRIVATE Qt5::Core ADS1115Lib)

#------------------------------------------------------------------------------
# Build cam2opencv as a Static Library
#------------------------------------------------------------------------------
add_library(cam2opencv STATIC src/IntelliFireUI/libcam2opencv.cpp)
target_include_directories(cam2opencv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cam2opencv PkgConfig::LIBCAMERA ${OpenCV_LIBS})
target_compile_options(cam2opencv PRIVATE ${LIBCAMERA_CFLAGS_OTHER})

#------------------------------------------------------------------------------
# IntelliFire UI Executable (with Camera and Sensor Support)
#------------------------------------------------------------------------------
add_executable(IntelliFireUI
    src/IntelliFireUI/main.cpp
    src/IntelliFireUI/mainwindow.cpp
    src/IntelliFireUI/src/sensorContainer.cpp
    src/IntelliFireUI/src/keyLogger.cpp
    src/fireDetector.cpp
    src/pumpControl.cpp
    src/IntelliFireUI/resources.qrc
    src/IntelliFireUI/include/mainwindow.h
    src/IntelliFireUI/include/sensorContainer.h
    src/IntelliFireUI/include/keyLogger.h
)

# Linking all required libraries
target_link_libraries(IntelliFireUI
    Qt5::Widgets
    UltraSonicSensorLib
    ADS1115Lib 
    ADS1115ManagerLib
    cam2opencv
    qwt-qt5
    ${OpenCV_LIBS}
    ${GPIOD_LIB}
    PkgConfig::LIBCAMERA
)

#------------------------------------------------------------------------------
# Install Rules (Optional)
#------------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS IntelliFireUI RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS UltraSonicSensorLib cam2opencv ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

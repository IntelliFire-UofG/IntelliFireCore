cmake_minimum_required(VERSION 3.10)
project(IntelliFireCore VERSION 1.0 LANGUAGES CXX)

#------------------------------------------------------------------------------
# General Configuration
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Winvalid-pch -Wno-unused-parameter")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#------------------------------------------------------------------------------
# Required Packages
#------------------------------------------------------------------------------
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(CURL REQUIRED)
find_library(GPIOD_LIB gpiod REQUIRED)

#------------------------------------------------------------------------------
# Include Directories
#------------------------------------------------------------------------------
include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src/IntelliFireUI/include
)

#------------------------------------------------------------------------------
# Core Libraries
#------------------------------------------------------------------------------
add_library(UltraSonicSensorLib STATIC
  src/UltraSonicSensor.cpp
  src/IRSensor.cpp
  include/UltraSonicSensor.h
  include/IRSensor.h
)
target_link_libraries(UltraSonicSensorLib PRIVATE Qt5::Core ${GPIOD_LIB})

add_library(ADS1115Lib STATIC
  src/ads1115rpi.cpp
  include/ads1115rpi.h
)
target_link_libraries(ADS1115Lib PRIVATE ${GPIOD_LIB})

add_library(ADS1115ManagerLib STATIC
  src/IntelliFireUI/src/ads1115manager.cpp
  src/IntelliFireUI/include/ads1115manager.h
)
target_link_libraries(ADS1115ManagerLib PRIVATE Qt5::Core ADS1115Lib)

add_library(PumpControlLib STATIC
  src/pumpControl.cpp
  include/pumpControl.h
)
target_link_libraries(PumpControlLib PRIVATE Qt5::Core ${GPIOD_LIB})

#------------------------------------------------------------------------------
# IntelliFire UI Application
#------------------------------------------------------------------------------
add_executable(IntelliFireUI
  src/IntelliFireUI/main.cpp
  src/IntelliFireUI/mainwindow.cpp
  src/IntelliFireUI/src/sensorContainer.cpp
  src/IntelliFireUI/src/keyLogger.cpp
  src/fireDetector.cpp
  src/IntelliFireUI/resources.qrc
)

target_link_libraries(IntelliFireUI
  Qt5::Widgets
  UltraSonicSensorLib
  ADS1115Lib 
  ADS1115ManagerLib
  PumpControlLib
  ${GPIOD_LIB}
)

#------------------------------------------------------------------------------
# CLI-Based Core Application
#------------------------------------------------------------------------------
add_executable(IntelliFire
  src/main.cpp
  src/LN298MotorControlV3.cpp
  src/basicMotionV3.cpp
)

target_link_libraries(IntelliFire
  ADS1115Lib
  ${GPIOD_LIB}
)

#------------------------------------------------------------------------------
# MediaMTX HTTP Scanner Utility
#------------------------------------------------------------------------------
add_executable(mediamtx_http_scanner
  mediamtx_scanner/mediamtx_http_scanner.cpp
)
target_link_libraries(mediamtx_http_scanner PRIVATE CURL::libcurl)

#------------------------------------------------------------------------------
# Installation Rules
#------------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS IntelliFireUI RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS UltraSonicSensorLib ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

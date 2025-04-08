cmake_minimum_required(VERSION 3.5)
project(IntelliFire VERSION 1.0)

#------------------------------------------------------------------------------
# Compiler Settings
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wno-unused-parameter")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

#------------------------------------------------------------------------------
# Enable Qt Automatic Processing
#------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#------------------------------------------------------------------------------
# Find Required Qt5 Packages
#------------------------------------------------------------------------------
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

#------------------------------------------------------------------------------
# Find libgpiod for GPIO sensor drivers
#------------------------------------------------------------------------------
find_library(GPIOD_LIB gpiod)
if(NOT GPIOD_LIB)
  message(FATAL_ERROR "❌ Could not find libgpiod library")
endif()

#------------------------------------------------------------------------------
# Include Directories
#------------------------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/IntelliFireUI/include
)

#------------------------------------------------------------------------------
# UltraSonic + IRSensor Library (Sensors + GPIO Threading)
#------------------------------------------------------------------------------
add_library(UltraSonicSensorLib STATIC
    src/UltraSonicSensor.cpp
    src/IRSensor.cpp
    include/UltraSonicSensor.h
    include/IRSensor.h
)
target_link_libraries(UltraSonicSensorLib PRIVATE Qt5::Core ${GPIOD_LIB})

#------------------------------------------------------------------------------
# ADS1115 Low-Level Driver
#------------------------------------------------------------------------------
add_library(ADS1115Lib STATIC
    src/ads1115rpi.cpp
    include/ads1115rpi.h
)
target_link_libraries(ADS1115Lib PRIVATE ${GPIOD_LIB})

#------------------------------------------------------------------------------
# ADS1115 Manager UI Signal Dispatcher
#------------------------------------------------------------------------------
add_library(ADS1115ManagerLib STATIC
    src/IntelliFireUI/src/ads1115manager.cpp
    src/IntelliFireUI/include/ads1115manager.h
)
target_link_libraries(ADS1115ManagerLib PRIVATE Qt5::Core ADS1115Lib)

#------------------------------------------------------------------------------
# Pump Control Logic
#------------------------------------------------------------------------------
add_library(PumpControlLib STATIC
    src/pumpControl.cpp
    include/pumpControl.h
)
target_link_libraries(PumpControlLib PRIVATE Qt5::Core ${GPIOD_LIB})

#------------------------------------------------------------------------------
# IntelliFire Main UI
#------------------------------------------------------------------------------
add_executable(IntelliFireUI
    src/IntelliFireUI/main.cpp
    src/IntelliFireUI/mainwindow.cpp
    src/IntelliFireUI/src/sensorContainer.cpp
    src/IntelliFireUI/src/keyLogger.cpp
    src/fireDetector.cpp
    src/IntelliFireUI/resources.qrc
    src/IntelliFireUI/include/mainwindow.h
    src/IntelliFireUI/include/sensorContainer.h
    src/IntelliFireUI/include/keyLogger.h
)

target_link_libraries(IntelliFireUI
    Qt5::Widgets
    UltraSonicSensorLib
    ADS1115Lib 
    ADS1115ManagerLib
    PumpControlLib
    qwt-qt5
    ${GPIOD_LIB}
)

#------------------------------------------------------------------------------
# Install Rules
#------------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS IntelliFireUI RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS UltraSonicSensorLib ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

#------------------------------------------------------------------------------
# MediaMTX HTTP Scanner Utility
#------------------------------------------------------------------------------
find_package(CURL REQUIRED)

add_executable(mediamtx_http_scanner mediamtx_scanner/mediamtx_http_scanner.cpp)
target_link_libraries(mediamtx_http_scanner PRIVATE CURL::libcurl)

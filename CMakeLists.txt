cmake_minimum_required(VERSION 3.5)
project(IntelliFire VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source files
set(SOURCES
    src/main.cpp
    src/LN298MotorControlV3.cpp
    src/basicMotionV3.cpp
    # Add other source files
)

# Define include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create executable
add_executable(IntelliFire ${SOURCES})

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
# Build ADS1115ManagerLib as a static library
#------------------------------------------------------------------------------
add_library(PumpControlLib STATIC
    src/pumpControl.cpp
    include/pumpControl.h
)

target_include_directories(PumpControlLib PRIVATE ${CMAKE_SOURCE_DIR}/src/IntelliFireUI/include)
target_link_libraries(PumpControlLib PRIVATE Qt5::Core ${GPIOD_LIB})

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
    PumpControlLib
    cam2opencv
    qwt-qt5
    ${OpenCV_LIBS}
    ${GPIOD_LIB}
    PkgConfig::LIBCAMERA
)


# Print status
message(STATUS "Configuring Autonomous Fire Truck Control System")
message(STATUS "Using libgpiod: ${LIBGPIOD_LIBRARY}")
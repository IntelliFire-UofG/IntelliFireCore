cmake_minimum_required(VERSION 3.10.0)
project(LIBGPIOD_RaspberryPi_Sensors VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Set default build type if none specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Include GNUInstallDirs for standard install locations
include(GNUInstallDirs)

# Enable compiler warnings
add_compile_options(-Wall -Wconversion -Wextra -pedantic)

# Define the paths to the source and header files
set(LIBSRC_IR ${CMAKE_SOURCE_DIR}/src/IRSensor.cpp)
set(LIBINCLUDE_IR ${CMAKE_SOURCE_DIR}/include/IRSensor.h)

set(LIBSRC_US ${CMAKE_SOURCE_DIR}/src/UltraSonicSensor.cpp)
set(LIBINCLUDE_US ${CMAKE_SOURCE_DIR}/include/UltraSonicSensor.h)

# ----------------------- Build the IR Sensor Library -----------------------

# Shared Library
add_library(IRSensor SHARED ${LIBSRC_IR})
set_target_properties(IRSensor PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER "${LIBINCLUDE_IR}"
)
target_link_libraries(IRSensor gpiod)

install(TARGETS IRSensor EXPORT IRSensor-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Static Library
add_library(IRSensor_static STATIC ${LIBSRC_IR})
set_target_properties(IRSensor_static PROPERTIES
    PUBLIC_HEADER "${LIBINCLUDE_IR}"
)
target_link_libraries(IRSensor_static gpiod)

install(TARGETS IRSensor_static EXPORT IRSensor_static-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ------------------ Build the UltraSonic Sensor Library -------------------

# Shared Library
add_library(UltraSonicSensor SHARED ${LIBSRC_US})
set_target_properties(UltraSonicSensor PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER "${LIBINCLUDE_US}"
)
target_link_libraries(UltraSonicSensor gpiod)

install(TARGETS UltraSonicSensor EXPORT UltraSonicSensor-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Static Library
add_library(UltraSonicSensor_static STATIC ${LIBSRC_US})
set_target_properties(UltraSonicSensor_static PROPERTIES
    PUBLIC_HEADER "${LIBINCLUDE_US}"
)
target_link_libraries(UltraSonicSensor_static gpiod)

install(TARGETS UltraSonicSensor_static EXPORT UltraSonicSensor_static-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ---------------------- Build Test Executables -----------------------

# Test executable for IR Sensor
add_executable(testIRSensor ${CMAKE_SOURCE_DIR}/tests/testIRSensor.cpp)
target_link_libraries(testIRSensor IRSensor_static)
target_include_directories(testIRSensor PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Test executable for UltraSonic Sensor
add_executable(testUltraSonicSensor ${CMAKE_SOURCE_DIR}/tests/testUltraSonicSensor.cpp)
target_link_libraries(testUltraSonicSensor UltraSonicSensor_static)
target_include_directories(testUltraSonicSensor PRIVATE ${CMAKE_SOURCE_DIR}/include)

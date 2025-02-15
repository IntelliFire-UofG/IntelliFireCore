cmake_minimum_required(VERSION 3.10.0)
project(LIBGPIOD_RaspberryPi_IRSensor VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 14)

# Set default build type if none specified.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Include GNUInstallDirs for standard install locations.
include(GNUInstallDirs)

# Enable compiler warnings.
add_compile_options(-Wall -Wconversion -Wextra -pedantic)

# Define the paths to the source and header files.
set(LIBSRC ${CMAKE_SOURCE_DIR}/src/IRSensor.cpp)
set(LIBINCLUDE ${CMAKE_SOURCE_DIR}/include/IRSensor.h)

# Build the shared library.
add_library(IRSensor SHARED ${LIBSRC})
set_target_properties(IRSensor PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER "${LIBINCLUDE}"
)
target_link_libraries(IRSensor gpiod)

install(TARGETS IRSensor EXPORT IRSensor-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Build the static library.
add_library(IRSensor_static STATIC ${LIBSRC})
set_target_properties(IRSensor_static PROPERTIES
    PUBLIC_HEADER "${LIBINCLUDE}"
)
target_link_libraries(IRSensor_static gpiod)

install(TARGETS IRSensor_static EXPORT IRSensor_static-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Build the test executable.
# Make sure the filename and path below exactly match your test file.
add_executable(testIRSensor ${CMAKE_SOURCE_DIR}/tests/testIRSensor.cpp)
target_link_libraries(testIRSensor IRSensor_static)
target_include_directories(testIRSensor PRIVATE ${CMAKE_SOURCE_DIR}/include)

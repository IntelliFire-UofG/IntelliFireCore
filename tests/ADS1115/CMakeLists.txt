cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MultipleChannelsADS1115)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find libgpiod
find_library(GPIOD_LIBRARY gpiod)
if(NOT GPIOD_LIBRARY)
    message(FATAL_ERROR "libgpiod not found")
endif()

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/../../include)

# Add the source files
set(SOURCES
    ${PROJECT_SOURCE_DIR}/../../src/ads1115rpi.cpp
    ${PROJECT_SOURCE_DIR}/multipleChannelsADC.cpp
)


# Add the executable
add_executable(MultipleChannelsADS1115 ${SOURCES})
target_link_libraries(MultipleChannelsADC gpiod)
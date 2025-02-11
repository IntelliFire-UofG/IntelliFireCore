cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MultipleChannelsADC)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/../include)

# Add the source files
set(SOURCES
    ${PROJECT_SOURCE_DIR}/../src/ads1015.cpp
    ${PROJECT_SOURCE_DIR}/multipleChannelsADC.cpp
)

target_link_libraries(ads1115rpi gpiod)
# Add the executable
add_executable(MultipleChannelsADC ${SOURCES})
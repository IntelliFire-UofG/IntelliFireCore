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

# Find and link required packages
find_library(LIBGPIOD_LIBRARY NAMES gpiod)
if(NOT LIBGPIOD_LIBRARY)
    message(FATAL_ERROR "libgpiod not found, please install it")
endif()

# Link libraries
target_link_libraries(IntelliFire
    ${LIBGPIOD_LIBRARY}
    pthread
)

# Add compiler warnings
target_compile_options(IntelliFire PRIVATE -Wall -Wextra)

# Installation (optional)
install(TARGETS IntelliFire DESTINATION bin)

# Print status
message(STATUS "Configuring Autonomous Fire Truck Control System")
message(STATUS "Using libgpiod: ${LIBGPIOD_LIBRARY}")
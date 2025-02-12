cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

project(LIBGPIOD_RaspberryPi_IRSensor VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)

add_subdirectory(example)

add_compile_options(-Wall -Wconversion -Wextra -pedantic)

set(LIBSRC ir_sensor.cpp)
set(LIBINCLUDE ir_sensor.h)

# Build shared library
add_library(ir_sensor
  SHARED
  ${LIBSRC}
)

set_target_properties(ir_sensor PROPERTIES
  SOVERSION 1
  PUBLIC_HEADER "${LIBINCLUDE}"
)

target_link_libraries(ir_sensor gpiod)

install(TARGETS ir_sensor EXPORT ir_sensor-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Build static library
add_library(ir_sensor_static
  STATIC
  ${LIBSRC}
)

set_target_properties(ir_sensor_static PROPERTIES
  PUBLIC_HEADER "${LIBINCLUDE}"
)

target_link_libraries(ir_sensor_static gpiod)

install(TARGETS ir_sensor_static EXPORT ir_sensor_static-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

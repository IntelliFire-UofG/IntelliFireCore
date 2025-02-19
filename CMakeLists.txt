# cmake_minimum_required(VERSION 3.10)
# project(IntelliFire VERSION 1.0.0 LANGUAGES CXX)

# # Set the C++ standard
# set(CMAKE_CXX_STANDARD 17)

# # Set default build type if none specified
# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#     set(CMAKE_BUILD_TYPE "RelWithDebInfo")
# endif()

# # Include GNUInstallDirs for standard install locations
# include(GNUInstallDirs)

# # Enable compiler warnings
# add_compile_options(-Wall -Wconversion -Wextra -pedantic)

# # Find Qt5
# find_package(Qt5 REQUIRED COMPONENTS Widgets)

# # Define paths
# set(CORE_INCLUDE ${CMAKE_SOURCE_DIR}/IntelliFireCore/include)
# set(CORE_SRC ${CMAKE_SOURCE_DIR}/IntelliFireCore/src)

# set(UI_INCLUDE ${CMAKE_SOURCE_DIR}/IntelliFireUI/include)
# set(UI_SRC ${CMAKE_SOURCE_DIR}/IntelliFireUI/src)

# set(SUPPORT_INCLUDE ${CMAKE_SOURCE_DIR}/SupportCodes)
# set(SUPPORT_SRC ${CMAKE_SOURCE_DIR}/SupportCodes)

# # ---- Build IRSensor as a shared library ----
# set(LIBSRC ${CORE_SRC}/IRSensor.cpp)
# set(LIBINCLUDE ${CORE_INCLUDE}/IRSensor.h)

# add_library(IRSensor SHARED ${LIBSRC})
# set_target_properties(IRSensor PROPERTIES
#     SOVERSION 1
#     PUBLIC_HEADER "${LIBINCLUDE}"
# )
# target_link_libraries(IRSensor gpiod)

# install(TARGETS IRSensor EXPORT IRSensor-targets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# # ---- Build the static IRSensor library ----
# add_library(IRSensor_static STATIC ${LIBSRC})
# set_target_properties(IRSensor_static PROPERTIES
#     PUBLIC_HEADER "${LIBINCLUDE}"
# )
# target_link_libraries(IRSensor_static gpiod)

# install(TARGETS IRSensor_static EXPORT IRSensor_static-targets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# # ---- Build the main executable ----
# add_executable(IntelliFireUI
#     ${UI_SRC}/main.cpp
#     ${UI_SRC}/mainwindow.cpp
#     ${UI_SRC}/sensorContainer.cpp
#     ${UI_SRC}/UltraSonicSensor.cpp
#     ${CORE_SRC}/LM393FlameSensor.cpp
#     ${CORE_SRC}/LN298MotorControl.cpp
#     ${CORE_SRC}/basicMotion.cpp
#     ${CORE_SRC}/eventHandler.cpp
#     ${CORE_SRC}/pumpControl.cpp
#     ./IntelliFireUI/resources/resources.qrc
# )

# # Link Qt Libraries and GPIO
# target_link_libraries(IntelliFireUI
#     Qt5::Widgets
#     gpiod
#     IRSensor
# )

# # ---- Build support utilities ----
# # add_executable(differentialRobot
# #     ${SUPPORT_SRC}/differentialRobot.cpp
# # )
# # target_include_directories(differentialRobot PRIVATE ${SUPPORT_INCLUDE})

# # ---- Test IRSensor (optional) ----
# # add_executable(testIRSensor ${CMAKE_SOURCE_DIR}/tests/testIRSensor.cpp)
# # target_link_libraries(testIRSensor IRSensor_static)
# # target_include_directories(testIRSensor PRIVATE ${CORE_INCLUDE})

cmake_minimum_required(VERSION 3.10)
project(IntelliFire VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set default build type if none specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Include GNUInstallDirs for standard install locations
include(GNUInstallDirs)

# Enable compiler warnings
add_compile_options(-Wall -Wconversion -Wextra -pedantic)

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Define paths
set(CORE_INCLUDE ${CMAKE_SOURCE_DIR}/IntelliFireCore/include)
set(CORE_SRC ${CMAKE_SOURCE_DIR}/IntelliFireCore/src)

set(UI_INCLUDE ${CMAKE_SOURCE_DIR}/IntelliFireUI/include)
set(UI_SRC ${CMAKE_SOURCE_DIR}/IntelliFireUI/src)

# ---- Build testUI as a shared library ----
set(LIBSRC ${CORE_SRC}/IRSensor.cpp)
set(LIBINCLUDE ${CORE_INCLUDE}/IRSensor.h)

add_library(testUI SHARED ${LIBSRC})
set_target_properties(testUI PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER "${LIBINCLUDE}"
)
target_link_libraries(testUI gpiod)

install(TARGETS testUI EXPORT testUI-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ---- Build the static testUI library ----
add_library(testUI_static STATIC ${LIBSRC})
set_target_properties(testUI_static PROPERTIES
    PUBLIC_HEADER "${LIBINCLUDE}"
)
target_link_libraries(testUI_static gpiod)

install(TARGETS testUI_static EXPORT testUI_static-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ---- Build the main IntelliFireUI executable ----
add_executable(IntelliFireUI
    ${UI_SRC}/main.cpp
    ${UI_SRC}/mainwindow.cpp
    ${UI_SRC}/sensorContainer.cpp
    ./IntelliFireUI/resources/resources.qrc
    ./IntelliFireUI/src/UltraSonicSensor.cpp
)

# Link Qt Libraries and GPIO
target_link_libraries(IntelliFireUI
    Qt5::Widgets
    gpiod
)

# ---- Test testUI (optional) ----
add_executable(testIRSensor ${CMAKE_SOURCE_DIR}/tests/testIRSensor.cpp)
target_link_libraries(testIRSensor testUI_static)
target_include_directories(testIRSensor PRIVATE ${CORE_INCLUDE})

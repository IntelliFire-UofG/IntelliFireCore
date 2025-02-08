cmake_minimum_required(VERSION 3.5)
project(IntellifireUI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(IntellifireUI
    main.cpp
    mainwindow.cpp
    sensorContainer.cpp
    mainwindow.h
    sensorContainer.h
)

target_link_libraries(IntellifireUI PRIVATE Qt5::Widgets)
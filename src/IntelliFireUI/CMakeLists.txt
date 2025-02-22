# cmake_minimum_required(VERSION 3.5)
# project(IntellifireUI)

# set(CMAKE_CXX_STANDARD 17)

# #  Enable Qt's Meta-Object Compiler (MOC) and Resource Compiler (RCC)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)  

# # Find Qt5
# find_package(Qt5 REQUIRED COMPONENTS Widgets)


# add_executable(IntelliFireUI
#     main.cpp
#     mainwindow.cpp
#     sensorContainer.cpp
#     ./resources/resources.qrc
#     ./src/IntelliFireUI/src/UltraSonicSensor.cpp
# )

# # Link Qt Libraries AFTER Defining Executable
# target_link_libraries(IntelliFireUI Qt5::Widgets)
# target_link_libraries(IntelliFireUI gpiod)

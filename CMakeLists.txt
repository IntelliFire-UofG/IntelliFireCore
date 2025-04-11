# Project Name
project(MotorControl)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find and Link libgpiod for GPIO Control
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

# Find Threads package for std::thread support
find_package(Threads REQUIRED)

# Create Executable
add_executable(MotorControl
    src/LN298MotorControlV3.cpp
    src/basicMotionV3.cpp
    src/main.cpp
    src/EventLoop.cpp
    src/rpi_pwm.cpp    # RPI_PWM implementation
    src/Logger.cpp     # Added Logger implementation
)

# Link Libraries
target_link_libraries(MotorControl
    PkgConfig::GPIOD
    Threads::Threads
)

# Add install target
install(TARGETS MotorControl
    RUNTIME DESTINATION bin
)

# Create log directory during installation
install(CODE "file(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/var/log/firetruck)")

# Set permissions for log directory
install(CODE "execute_process(COMMAND chmod 777 \${CMAKE_INSTALL_PREFIX}/var/log/firetruck)")
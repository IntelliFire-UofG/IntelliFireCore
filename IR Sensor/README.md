# IRSensorRealtime Project: Build and Run Instructions

This document provides step-by-step instructions to build and run the IRSensorRealtime project using CMake and Make.

## Prerequisites

- **CMake:**  
  Ensure CMake is installed on your system. For Ubuntu/Debian-based systems, run:
  ```bash
  sudo apt-get update
  sudo apt-get install cmake
  ```
  
- **Make:**  
  Make sure `make` is installed (it is usually available by default on most Linux distributions).

- **libgpiod:**  
  The project requires `libgpiod` and its development files. On Ubuntu/Debian-based systems, install it with:
  ```bash
  sudo apt-get install libgpiod-dev
  ```

- **Root Permissions:**  
  Accessing GPIO typically requires root permissions. You'll need to run the executable with `sudo`.

## Hardware Setup

### Components Required:
- IR Sensor Module
- Raspberry Pi / Linux-based system with GPIO access
- Connecting Wires

### Pin Connections:
| IR Sensor | Raspberry Pi GPIO |
|-----------|------------------|
| VCC       | 3.3V / 5V        |
| GND       | GND              |
| OUT       | GPIO12           |

Ensure proper wiring before running the project.

## Project File Structure

Your project directory should contain the following files:

```
IRSensorRealtime/
├── CMakeLists.txt
├── ir_sensor.h
├── ir_sensor.cpp
├── example/
│   ├── CMakeLists.txt
│   ├── ir_sensor_printer.cpp
├── README.md
```

## Building the Project

1. **Open a Terminal** and navigate to the root directory of your project (the directory containing `CMakeLists.txt`).

2. **Create and Enter a Build Directory:**
   ```bash
   mkdir build && cd build
   ```

3. **Run CMake to Configure the Build System:**
   ```bash
   cmake ..
   ```

4. **Build the Project Using Make:**
   ```bash
   make
   ```

## Running the Executable

The project builds the `ir_sensor_printer` executable, located in the `example` folder:

### `ir_sensor_printer` (simple sensor event logger)
To run the logging utility:
```bash
sudo ./example/ir_sensor_printer
```

This program prints obstacle detection messages when an IR sensor event occurs.

## Stopping the Program
The program can be stopped gracefully by pressing `CTRL+C` or exiting as prompted. This ensures proper cleanup and termination.

## Summary of Commands

```bash
mkdir build && cd build
cmake ..
make
sudo ./example/ir_sensor_printer
```

## Troubleshooting

- **cmake: command not found:**  
  Install CMake using your system's package manager.

- **Missing Dependencies:**  
  Ensure that `libgpiod` and its development files are installed.

- **Permission Issues:**  
  If you encounter permission errors when accessing GPIO, make sure to run the executable with `sudo`.

Follow these instructions to build and run your IRSensorRealtime project successfully!


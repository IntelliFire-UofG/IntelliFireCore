# **IntelliFireCore Project: Build and Run Instructions**

This document provides step-by-step instructions to build and run the **IR Sensor** and **UltraSonic Sensor** IntelliFire project using **CMake** and **Make**.

---

## **Prerequisites**

### **1. Install Required Software**
- **CMake:** Install CMake on your system. For Ubuntu/Debian-based systems, run:
  ```bash
  sudo apt-get update
  sudo apt-get install cmake
  ```
- **Make:** Ensure `make` is installed (it is usually available by default on most Linux distributions).
- **libgpiod:** The project requires `libgpiod` and its development files. On Ubuntu/Debian-based systems, install it with:
  ```bash
  sudo apt-get install libgpiod-dev
  ```
- **Root Permissions:** Accessing GPIO typically requires root permissions. You'll need to run the executable with `sudo`.

---

## **Hardware Setup**

### **Components Required:**
- **IR Sensor Module**
- **UltraSonic Sensor Module**
- **Raspberry Pi / Linux-based system with GPIO access**
- **Connecting Wires**

### **Pin Connections**

#### **IR Sensor Wiring**
| IR Sensor | Raspberry Pi GPIO |
| --------- | ----------------- |
| VCC       | 3.3V / 5V         |
| GND       | GND               |
| OUT       | GPIO12            |

#### **UltraSonic Sensor Wiring**
| UltraSonic Sensor | Raspberry Pi GPIO |
| ----------------- | ----------------- |
| VCC              | 5V                 |
| GND              | GND                |
| TRIG             | GPIO23              |
| ECHO             | GPIO24              |

Ensure proper wiring before running the project.

---

## **Project File Structure**

Your project directory should contain the following files:

```
IntelliFireCore/
├── CMakeLists.txt
├── include/
│   ├── IRSensor.h
│   ├── UltraSonicSensor.h
├── src/
│   ├── IRSensor.cpp
│   ├── UltraSonicSensor.cpp
├── tests/
│   ├── testIRSensor.cpp
│   ├── testUltraSonicSensor.cpp
├── README_SENSORS.md
```

---

## **Building the Project**

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

---

## **Running the Executables**

The project builds two test executables:

### **1. `testIRSensor` (IR Sensor Obstacle Logger)**

This program detects obstacles using the **IR sensor** and prints messages when an obstacle is detected or removed.

#### **Run the IR Sensor Test:**
```bash
sudo ./tests/testIRSensor
```

### **2. `testUltraSonicSensor` (Distance Measurement Logger)**

This program measures the **distance to an object** using the **UltraSonic sensor** and prints the measured distance in **centimeters**.

#### **Run the UltraSonic Sensor Test:**
```bash
sudo ./tests/testUltraSonicSensor
```

---

## **Stopping the Program**

Both programs can be stopped **gracefully** by pressing `CTRL+C` or exiting as prompted. This ensures proper cleanup and termination.

---

## **Summary of Commands**

```bash
mkdir build && cd build
cmake ..
make
sudo ./tests/testIRSensor
sudo ./tests/testUltraSonicSensor
```

---

## **Troubleshooting**

- **cmake: command not found:** Install CMake using your system's package manager.
- **Missing Dependencies:** Ensure that `libgpiod` and its development files are installed.
- **Permission Issues:** If you encounter permission errors when accessing GPIO, make sure to run the executable with `sudo`.
- **Incorrect Wiring:** Double-check the GPIO pin connections and ensure they match the wiring tables above.

---

## **Conclusion**
This guide provides the complete setup to build and run both **IR Sensor** and **UltraSonic Sensor** in the **IntelliFireCore** project. Follow the steps carefully, and your system should work flawlessly.

Let us know if you need any modifications! 🚀


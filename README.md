# _**Welcome to IntelliFire UofG Core Repository**_
# 🚒 Autonomous Fire Truck Control System 🔥  

### **Real-Time Fire Detection & Response System for Raspberry Pi**  

![Fire Truck](https://img.shields.io/badge/Project-Embedded%20System-blue.svg)  
![C++](https://img.shields.io/badge/Language-C%2B%2B-red.svg)  
![Raspberry Pi](https://img.shields.io/badge/Platform-Raspberry%20Pi-green.svg)  
![License](https://img.shields.io/badge/License-Apache%202.0-lightgrey.svg)  

## 📌 **Project Description**  
The **Autonomous Fire Truck Control System** is an **embedded real-time application** designed for **fire detection and autonomous movement**. Running on a **Raspberry Pi** with **Linux**, this system utilizes an **event-driven architecture** to detect flames via **LM393 flame sensors** (connected through an **ADS1015/ADS1115 ADC**) and control **four motors** via an **L298N motor driver**.  

## 🔥 **Main Features:**  
✅ **Event-driven architecture** (using threads, callbacks, and interrupts).  
✅ **Motor control** for movement based on flame detection.  
✅ **Real-time sensor data processing** using an ADC.  
✅ **Graphical display** for real-time flame visualization.  
✅ **Interrupt-based button control** for manual intervention.  
✅ **Unit testing** for maintainability.  

---

## 🚀 **How It Works**  
1️⃣ **System Initialization** → Motors, sensors, buttons, and display are initialized.  
2️⃣ **Register Callbacks** → Sensor readings & button presses trigger motor actions.  
3️⃣ **Start Event Loop** → The system continuously monitors inputs & executes in real time.  
4️⃣ **Fire Detection** → The truck stops moving and logs fire locations.  
5️⃣ **User Interaction** → Buttons allow manual movement control when necessary.  

---

## 🛠 **Tech Stack**  
- **Programming Language**: C++  
- **Platform**: Raspberry Pi (Linux)  
- **Architecture**: Object-Oriented Programming (OOP)  
- **Event Handling**: Threads, Callbacks, Interrupts  
- **Visualization**: Qt5  
- **Testing**: Unit testing framework  

---
```bash
📂 IntelliFireCore
 ├── 📂 include                # Header files (Interface definitions)
 │   ├── ads1115rpi.h          # ADS1115 class definition
 │   ├── basicMotion.h         # Motor control class definition
 │   ├── eventLoop.h           # Event loop handling
 │   ├── fireDetector.h        # Event handler for detecting fire
 │   ├── IRSensor.h            # IR sensor class definition
 │   ├── LN298MotorControl.h   # Motor control class definition
 │   ├── pumpControl.h         # Fire extinguishing pump control
 │   ├── UltraSonicSensor.h    # Ultrasonic sensor class definition
 │
 ├── 📂 src                    # Implementation files
 │   ├── 📂 IntelliFireUI      # Graphical UI visualization for sensor data
 │   ├── ads1115manager.cpp    # ADS1115 manager implementation
 │   ├── ads1115rpi.cpp        # ADS1115 implementation
 │   ├── basicMotion.cpp       # Basic motion control implementation
 │   ├── eventLoop.cpp         # Event loop implementation
 │   ├── fireDetector.cpp      # Fire detector implementation
 │   ├── IRSensor.cpp          # IR sensor implementation
 │   ├── keyLogger.cpp         # Key logger implementation
 │   ├── libcam2opencv.cpp     # Camera to OpenCV implementation
 │   ├── LN298MotorControl.cpp # Motor control implementation
 │   ├── main.cpp              # Main entry point, event-driven execution
 │   ├── mainwindow.cpp        # Main window implementation
 │   ├── pumpControl.cpp       # Pump activation logic
 │   ├── sensorContainer.cpp   # Sensor container implementation
 │   ├── UltraSonicSensor.cpp  # Ultrasonic sensor implementation
 │
 ├── CMakeLists.txt            # CMake build configuration
 ├── CODE_REVISION_GUIDELINES.md  # Internal development guidelines
 ├── IntelliFireLogo.png       # Project logo
 ├── LICENSE                   # Apache v2.0 License
 ├── README.md                 # Project documentation
```
---

## 🚦 **Installation & Setup**
### 1️⃣ Prerequisites
Raspberry Pi with Linux
C++ compiler (CMake)
lgpiod (for GPIO control)
ADS1015/ADS1115 ADC module
L298N Motor Driver
### 2️⃣ Build & Run
```bash

# Clone the repository
git clone [https://github.com/IntelliFire-UofG/IntelliFireCore.git]

# Navigate to project directory
cd IntelliFireCore

# Create a build folder
mkdir build && cd build

# Compile the project with CMake and Make
cmake ..
make

# Run the executable
./IntelliFireCore

```

## 📸 **Demo & Screenshots**
(Add images or GIFs showing the system in action!)

---

## 📟 **RPi GPIOs Table**

> [!IMPORTANT]
> Make sure of connecting the following properly, if not ask the team through our different channels.
> 
| IntelliFire Feature | # GPIO |
| :---------------- | :---:|
| Left Motor PWM    | 12 |
| Left Motor FWD    | 17 |
| Left Motor BWD    | 27 |
| Right Motor PWM   | 13 |
| Right Motor FWD   | 23 |
| Right Motor BWD   | 22 |
| ADS1115 Data R.   | 5 |
| ADS1115 I2C SDA   | 2 |
| ADS1115 I2C SCL   | 3 |
| L393M Flame 1     | 9 |
| L393M Flame 2     | 10 |
| Pump              | 11 |
| Ultrasonic Echo   | 24 |
| Ultrasonic Trig   | 25 |
| IR Presence       | 16 |

---

## 🤝 **Contributing**
Contributions are welcome! Please open an issue or submit a pull request.

## 📜 **License**
This project is licensed under the Apache License 2.0.
You can freely use, modify, and distribute it, but you must provide attribution.
See the LICENSE file for details.

## 📞 **Contact & Social Media**
📧 Email: intellifire2025@outlook.com
💼 Instagram: @intellifire25
🌎 Github: [https://github.com/IntelliFire-UofG/]

## 🏷️ **Tags**

C++ Raspberry Pi Event-Driven Programming Real-Time Systems Flame Sensor Motor Control Embedded Systems

# 🔥🚒 Built with passion & innovation! 🚒🔥

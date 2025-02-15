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
 ├── 📂 SupportCodes           # Extra support codes for reference/testing
 │   ├── LM393_SupportCode_README.md
 │   ├── LM393_SupportCode_Read.cpp
 │   ├── LN298N_SupportCode_README.md
 │   ├── LN298_SupportCode_Motor.cpp
 │
 ├── 📂 include                # Header files (Interface definitions)
 │   ├── LM393FlameSensor.h    # Flame sensor class definition
 │   ├── LN298MotorControl.h   # Motor control class definition
 │   ├── eventHandler.h        # Event handler for buttons, callbacks
 │   ├── pumpControl.h         # Fire extinguishing pump control
 │
 ├── 📂 src                    # Implementation files
 │   ├── 📂 IntellifireUI                # Graphical UI visualization for sensor data
 │   ├── LM393FlameSensor.cpp  # Implementation of flame sensor handling
 │   ├── LN298MotorControl.cpp # Implementation of motor driver logic
 │   ├── eventHandler.cpp      # Implementation of event-driven interactions
 │   ├── main.cpp              # Main entry point, event-driven execution
 │   ├── pumpControl.cpp       # Pump activation logic
 │
 ├── 📂 tests                  # Unit tests
 │   ├── testMotor.cpp         # Test suite for motor control
 │   ├── testPump.cpp          # Test suite for pump activation
 │   ├── testSensor.cpp        # Test suite for flame sensor readings
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
cd src

# Compile the project
make

# Run the executable
./intellifire

```

## 📸 **Demo & Screenshots**
(Add images or GIFs showing the system in action!)

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

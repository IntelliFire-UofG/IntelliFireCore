# IntelliFire: Flame-Responsive Car Control System

## 1. Overview
**IntelliFire** is a Raspberry Pi 5-based, threaded and event-driven robotic platform programmed in C++. It can be controlled via keyboard input and detects fires using a flame sensor array. The system includes live video streaming, real-time UI over SSH, and automatic flame suppression. It integrates multiple sensors to detect both frontal and rear obstacles and logs all events, movements, and sensor data to a remote console.

Additionally, two ARTs (Agile Release Train) were implemented along the project. The main project components can be defined in the following document [Agile Rituals](https://github.com/IntelliFire-UofG/IntelliFireCore/blob/main/Documentation/ARTRituals.md)

---

## 2. Core Components

### 2.1 Flame Detection System
A 4-element LM393 IR flame sensor array is interfaced through an ADS1015 analog-to-digital converter. Each sensor provides continuous analog signals indicating flame presence and proximity. These are read at fixed intervals, translated into directional inference, and displayed on the UI.

### 2.2 Video Streaming
A Raspberry Pi Camera module streams live video through an **MTX Media Server**. The stream is accessible locally on the same network, allowing visual confirmation of navigation and flame conditions.

### 2.3 Obstacle Detection
- **Ultrasonic Sensor (HC-SR04)** mounted at the front calculates distances based on pulse reflection time. The system alerts or stops the car when obstacles are too close.
- **IR Presence Sensor** at the rear detects objects within a short range, useful for safe reversing. Both sensors are reflected in the UI when active.

### 2.4 Motion Control
The car uses four DC motors driven by an L298N H-Bridge. Each motor pair (left/right) is controlled by:
- PWM signals for velocity (fixed profiles).
- Forward and backward direction pins.

### 2.5 Flame Suppression
A directional water pump is triggered via GPIO when a flame is detected based on the sensor array. The decision is based on analog thresholds and relative intensity across the four sensors. The action and flame location are logged in the UI.

---

## 3. User Interface (UI)
Accessible via SSH, the UI displays:
- Real-time analog values from the flame sensors.
- Distance from ultrasonic sensor.
- Rear object detection status from the IR sensor.
- Pressed/released keys with current movement status.
- Whether the flame suppression pump is activated.

The UI remains responsive and continuously updates via a dedicated thread loop.

---

## 4. Software Architecture

### 4.1 Multithreading & Event Handling
The system operates through separate threads, each responsible for:
- Sensor polling (ADS1015, ultrasonic, IR).
- Keyboard input detection.
- Motor actuation control.
- UI rendering.
- Video streaming (handled externally by MTX Media).

All modules use mutex-protected shared queues and callback-based event triggers to ensure thread safety and low latency.

### 4.2 Error Handling
Each subsystem implements robust error-checking:
- I2C/ADS1015 read failures are captured with retry and fallback mechanisms.
- Ultrasonic timeouts and misreads are filtered.
- Motor driver logic includes fail-safe motor stops if input is invalid or lost.

### 4.3 Codebase Separation
The code is split into:
- **Hardware Layer**: Interfaces with all sensors, actuators, and communication buses.
- **Application/UI Layer**: Manages the user interface, sensor readings, and command execution logic.

This separation ensures maintainability and scalability of the system.

---

## 5. Pin Allocation Table

| IntelliFire Feature | GPIO Pin |
|---------------------|-----------|
| Left Motor PWM      | 12        |
| Left Motor FWD      | 17        |
| Left Motor BWD      | 27        |
| Right Motor PWM     | 13        |
| Right Motor FWD     | 23        |
| Right Motor BWD     | 22        |
| ADS1015 Data Ready  | 5         |
| I2C SDA (ADS1015)   | 2         |
| I2C SCL (ADS1015)   | 3         |
| Flame Sensor 1      | 9         |
| Flame Sensor 2      | 10        |
| Water Pump          | 11        |
| Ultrasonic Echo     | 24        |
| Ultrasonic Trig     | 25        |
| IR Rear Presence    | 16        |

---

## 6. Work Principle Summary
1. Upon startup, all sensor modules initialize and start continuous polling threads.
2. UI begins listening to keyboard commands over SSH.
3. Key events are mapped to directional motor commands using PWM with fixed speed settings.
4. Flame sensors constantly feed analog data into the UI and logic controller.
5. If flame presence surpasses a threshold, the water pump is activated.
6. Obstacle detection from front and rear updates the UI and may halt or reverse the car.
7. Every sensor state, action taken, and input received is logged in real-time on the remote UI.

---


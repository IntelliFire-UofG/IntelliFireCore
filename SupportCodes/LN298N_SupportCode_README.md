# Event-Driven Motor Control System for Raspberry Pi

## Description
This project implements an event-driven motor control system using a Raspberry Pi and an L298N motor driver. The system uses **GPIO interrupts** to react in real time to button presses, controlling the movement and speed of four DC motors. The buttons allow the user to move the motors **forward**, **backward**, or **stop** them.

The system is written in C++ using the **WiringPi library** for GPIO access and PWM control.

---

## Key Features
- **Event-Driven Design:** Uses interrupts to handle button presses for real-time motor control.
- **PWM Speed Control:** Adjust motor speeds with software PWM on enable pins.
- **Real-Time Responsiveness:** Instant reaction to user inputs via GPIO events.
- **Modular Code:** Easily extendable for additional motors or controls.

---

## Hardware Requirements
- Raspberry Pi (any model with GPIO support).
- L298N Motor Driver.
- Four DC Motors.
- Three push buttons.
- Power supply for motors (external power recommended).
- Wiring setup:
  - **Motor Driver Pins:**
    - IN1 -> GPIO18 (WiringPi pin 1)
    - IN2 -> GPIO27 (WiringPi pin 2)
    - IN3 -> GPIO23 (WiringPi pin 4)
    - IN4 -> GPIO24 (WiringPi pin 5)
    - ENA -> GPIO17 (WiringPi pin 0)
    - ENB -> GPIO22 (WiringPi pin 3)
  - **Buttons:**
    - Forward -> GPIO25 (WiringPi pin 6)
    - Backward -> GPIO4 (WiringPi pin 7)
    - Stop -> GPIO14 (WiringPi pin 8)

---

## Software Requirements
- Linux-based OS on Raspberry Pi (e.g., Raspberry Pi OS).
- WiringPi library.

### Install WiringPi:
```bash
sudo apt update
sudo apt install wiringpi
```

### Compile the Code:

```bash
g++ -o LN298N_SupportCode_Motor LN298N_SupportCode_Motor.cpp -lwiringPi
```

### Run the Application: Execute the program with root privileges:

``` bash
sudo ./LN298N_SupportCode_Motor.cpp
```
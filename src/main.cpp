
/**
 * @file main.cpp
 * @brief Autonomous Fire Truck Control System - Entry Point
 *
 * This embedded real-time system runs on a Raspberry Pi and controls an autonomous 
 * fire truck equipped with four motors (L298N driver) and a flame detection system 
 * (LM393 sensors via ADS1015/ADS1115 ADC). The system responds to fire events and 
 * user inputs in real time, using an event-driven architecture.
 *
 * 🔥 Features:
 * - Real-time event-driven execution using threads & callbacks.
 * - Motor control for autonomous movement (forward, backward, stop).
 * - Flame detection using LM393 sensors & ADC.
 * - Graphical display for real-time sensor readings.
 * - Interrupt-based button control for manual intervention.
 *
 * 🚀 Execution Flow:
 * 1️⃣ System Initialization: Setup motors, sensors, buttons, display.
 * 2️⃣ Register Callbacks: Bind sensor & button events to motor control.
 * 3️⃣ Start Event Loop: Monitors inputs & executes actions in real-time.
 * 4️⃣ Fire Detection: Stops movement when fire is detected & logs data.
 * 5️⃣ User Interaction: Allows manual movement control via buttons. // For further development
 *
 * 📂 File Structure:
 * - MotorController.hpp: Manages motor movements.
 * - EventHandler.hpp: Handles button interrupts.
 * - ADCReader.hpp: Reads sensor data via ADC.
 * - DisplayManager.hpp: Displays real-time sensor values.
 * - EventLoop.hpp: Manages event-driven execution.
 *
 * 🛠️ Development:
 * - Coding Language: C++
 * - OS: Linux (Raspberry Pi)
 * - Paradigm: Object-Oriented Programming (OOP)
 * - Event Handling: Threads, Callbacks, Interrupts
 * - Testing: Unit tests included in 'tests/' directory.
 *
 * @author Misael Rivera
 * @date 2025-02-01
 */

#include "../include/LN298MotorControlV3.h"
#include '../include/basicMotionV2.h'
#include "../include/eventHandler.h"
#include "../include/ADCReader.h" // Consider for further dev
// #include "DisplayManager.hpp" // Consider for further dev
#include "../include/eventLoop.h" // Consider for further dev

#include <iostream>
#include <thread>

#include <libgpio.h>
#include <atomic>

// #define LEFT_PWM 12
// #define LEFT_IN1 17
// #define LEFT_IN2 27
// #define RIGHT_PWM 13
// #define RIGHT_IN1 22
// #define RIGHT_IN2 23


// #define BUTTON_FORWARD 6
// #define BUTTON_BACKWARD 7
// #define BUTTON_STOP 8

int main() {
    std::cout << "🔥 Autonomous Fire Truck System Initializing... 🔥" << std::endl;
    
    // call basicMotion function to allow button movement
    /*CODE REVIEW: March 12th -> basicMotion won't be returned and just instantiated. */
    basicMotion();

    // Initialize event handler
    EventHandler eventHandler(BUTTON_FORWARD, BUTTON_BACKWARD, BUTTON_STOP);  // Button GPIO pins

    // Initialize ADC Reader for flame sensors
    ADCReader adcReader(0x48);  // I2C address for ADS1015/ADS1115 

    /*
    This is considered for further use
    // Initialize Display Manager for real-time visualization
    // DisplayManager displayManager;
    */

    // Register ADC callback to react to fire detection
    adcReader.registerFlameCallback([&](int channel, int value) {
        std::cout << "🔥 Flame detected on sensor " << channel << " with value: " << value << std::endl;
        motorController.stop();  // Example response: Stop if flame is detected
    });

    // Start Event Loop (Runs in Separate Thread)
    EventLoop eventLoop;
    std::thread eventThread(&EventLoop::run, &eventLoop);

    std::cout << "✅ System Running..." << std::endl;
    eventThread.join();  // Keep main thread alive
}

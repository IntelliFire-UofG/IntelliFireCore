/**
 * @file main.cpp
 * @brief Autonomous Fire Truck Control System - Entry Point
 *
 * This embedded real-time system runs on a Raspberry Pi and controls an autonomous 
 * fire truck equipped with four motors (L298N driver) and a flame detection system 
 * (LM393 sensors via ADS1015/ADS1115 ADC). The system responds to fire events and 
 * user inputs in real time, using an event-driven architecture.
 *
 * @author Misael Rivera
 * @date 2025-02-01
 */

#include "../include/LN298MotorControlV3.h"
#include "../include/basicMotion.h"
#include "../include/ADCReader.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <memory>

int main() {
    std::cout << "🔥 Autonomous Fire Truck System Initializing... 🔥" << std::endl;
    
    // Initialize ADC Reader for flame sensors
    //ADCReader adcReader(0x48);  // I2C address for ADS1015/ADS1115 
    
    // Register ADC callback to react to fire detection
    //adcReader.registerFlameCallback([](int channel, int value) {
        //std::cout << "🔥 Flame detected on sensor " << channel << " with value: " << value << std::endl;
        // Note: This callback can't directly control motors created in basicMotion()
    //});
    
    // Start the basic motion system with keyboard control
    std::cout << "Starting keyboard control system..." << std::endl;
    
    // Call the basicMotion function defined in basicMotionV3.cpp
    int result = basicMotion();
    
    std::cout << "Basic motion system exited with code: " << result << std::endl;
    std::cout << "Shutting down..." << std::endl;
    
    return result;
}

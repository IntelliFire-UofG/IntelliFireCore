#include "../include/LN298MotorControlV3.h"
#include <atomic>
#include <thread>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "Logger.h"

/**
 * Main function to control the robot using keyboard input.
 * 
 * Creates two Motor objects representing the left and right motors
 * and allows controlling them using keyboard keys.
 * The motors will only move while a key is held down and will
 * stop automatically when the key is released.
 * 
 * This version includes memory management and key logging.
 */
int basicMotion() {
    // Initialize the logger
    auto logger = Logger::getInstance("firetruck_control.log");
    logger->log("FireTruck Control System starting");
    
    try {
        // Initialize left and right motors with PWM channels, GPIO pins, duty cycle, and names
        logger->log("Initializing motors");
        Motor leftMotor(0, 17, 27, 75, "left");    // Left motor: PWM0, IN1=GPIO17, IN2=GPIO27
        Motor rightMotor(1, 22, 23, 75, "right");  // Right motor: PWM1, IN1=GPIO22, IN2=GPIO23
        
        // Create KeyboardState for tracking keyboard input
        KeyboardState keyState;
        
        // Start a separate thread to listen for keyboard inputs continuously
        logger->log("Starting keyboard listener thread");
        std::thread listenerThread(keyboardListener, std::ref(keyState));
        
        // Run the keyboard control loop
        keyboardControl(leftMotor, rightMotor, keyState, logger);
        
        // Signal the listener thread to exit and wait for it to finish
        logger->log("Shutting down listener thread");
        keyState.shouldExit.store(true);
        
        if (listenerThread.joinable()) {
            listenerThread.join();
        }
        
        logger->log("FireTruck Control System shutdown complete");
    }
    catch (const std::exception& e) {
        logger->log("CRITICAL ERROR: " + std::string(e.what()));
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

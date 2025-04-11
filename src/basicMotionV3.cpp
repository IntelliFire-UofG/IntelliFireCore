#include "../include/LN298MotorControlV3.h"
#include <atomic>
#include <thread>
#include <iostream>

/**
 * Main function to control the robot using keyboard input.
 * 
 * Creates two Motor objects representing the left and right motors
 * and allows controlling them using keyboard keys.
 */
int basicMotion() {
    // Initialize left and right motors with PWM channels, GPIO pins, and duty cycle.
    // GPIO12 (PWM0) -> Left Motor, GPIO13 (PWM1) -> Right Motor
    // PWM Frequency is set to 100Hz and Duty Cycle is initially set to 75%.
    Motor leftMotor(0, 17, 27, 75);    // Left motor: PWM0, IN1=GPIO17, IN2=GPIO27
    Motor rightMotor(1, 22, 23, 75);   // Right motor: PWM1, IN1=GPIO22, IN2=GPIO23

    // Create atomic variables to store the last key pressed and whether a key is pressed.
    std::atomic<char> lastKey('\0');      // Stores the last pressed key, initialized to null character.
    std::atomic<bool> keyPressed(false);  // Indicates if a key is pressed, initialized to false.
    std::atomic<bool> running(true);      // Flag to control thread execution

    // Start a separate thread to listen for keyboard inputs continuously.
    // It will update 'lastKey' and 'keyPressed' accordingly, and detect key releases.
    std::thread listenerThread(keyboardListener, std::ref(lastKey), std::ref(keyPressed), std::ref(running));

    // Run the keyboard control loop, which reads the 'lastKey' variable and controls the motors accordingly.
    keyboardControl(leftMotor, rightMotor, lastKey, keyPressed);

    // Signal the listener thread to exit
    running.store(false);

    // Wait for the listener thread to finish before exiting the program.
    if (listenerThread.joinable()) {
        listenerThread.join();
    }

    // Ensure motors are stopped before exit
    leftMotor.stop();
    rightMotor.stop();

    return 0;
}
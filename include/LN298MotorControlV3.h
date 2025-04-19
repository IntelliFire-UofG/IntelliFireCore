#ifndef LN298MOTORCONTROLV3_H
#define LN298MOTORCONTROLV3_H

#include <iostream>
#include <gpiod.h>       // GPIO library for controlling pins on Raspberry Pi
#include <thread>        // For handling threading operations
#include <atomic>        // For thread-safe variables (used for keyboard control)
#include <chrono>        // For time-based operations (e.g., delays)
#include <termios.h>     // For handling terminal I/O settings for keyboard input
#include <unistd.h>      // For low-level I/O operations
#include <memory>        // For smart pointers
#include "rpi_pwm.h"     // Include your PWM handling class for motor control
#include "Logger.h"      // For logging functionality
#include "keyLogger.h"   // For keyboard event handling

#define CHIP_NAME "gpiochip0" // Raspberry Pi 5 GPIO chip name used by libgpiod

/**
 * Motor Class
 * This class provides a simple interface for controlling a motor using the L298N motor driver.
 * It supports basic operations like moving forward, backward, stopping, and adjusting speed via PWM.
 */
class Motor {
private:
    gpiod_chip* chip;                   // GPIO chip for accessing GPIO lines (raw pointer to avoid incomplete type issues)
    RPI_PWM pwm;                        // PWM object used for generating PWM signals for motor speed control
    int in1_pin;                        // GPIO pin for motor IN1 (controls motor direction)
    int in2_pin;                        // GPIO pin for motor IN2 (controls motor direction)
    int pwm_channel;                    // PWM channel used for controlling motor speed
    int dutyCycle;                      // Initial duty cycle value for PWM (default: 75%)
    std::string motorName;              // Name identifier for this motor (for logging)
    std::shared_ptr<Logger> logger;     // Logger for recording motor actions

public:
    /**
     * Constructor: Initializes the motor with specified PWM channel, GPIO pins, and duty cycle.
     * 
     * @param pwm_channel The PWM channel used for speed control (e.g., 0, 1)
     * @param in1_pin GPIO pin number for IN1
     * @param in2_pin GPIO pin number for IN2
     * @param dutyCycle Initial duty cycle for PWM signal (default: 75%)
     * @param name Name identifier for this motor (default: "unnamed")
     */
    Motor(int pwm_channel, int in1_pin, int in2_pin, int dutyCycle = 75, const std::string& name = "unnamed");

    /**
     * moveForward: Sets the motor to move forward.
     * Activates the IN1 pin, deactivates the IN2 pin, and starts PWM signal.
     */
    void moveForward();

    /**
     * moveBackward: Sets the motor to move backward.
     * Activates the IN2 pin, deactivates the IN1 pin, and starts PWM signal.
     */
    void moveBackward();

    /**
     * stop: Stops the motor by deactivating both IN1 and IN2 pins.
     * Optionally stops the PWM signal if desired.
     */
    void stop();

    /**
     * setSpeed: Adjusts the motor speed by modifying the PWM duty cycle.
     * 
     * @param speed New speed value (0 to 100, representing percentage of max speed)
     */
    void setSpeed(int speed);

    /**
     * Destructor: Cleans up resources, ensuring the motor is properly stopped and GPIO pins are released.
     */
    ~Motor();

    // Prevent copying and moving
    Motor(const Motor&) = delete;
    Motor& operator=(const Motor&) = delete;
    Motor(Motor&&) = delete;
    Motor& operator=(Motor&&) = delete;
};

/**
 * KeyboardState struct to hold all state related to keyboard input
 * Using a struct helps group related variables together
 */
struct KeyboardState {
    std::atomic<char> lastKey{'\0'};       // Last key pressed
    std::atomic<bool> keyPressed{false};   // Flag for key press event
    std::atomic<bool> isKeyHeld{false};    // Flag for key held state
    std::atomic<bool> shouldExit{false};   // Flag to signal thread termination
    
    struct termios originalTermios;         // Store original terminal settings
};

/**
 * keyboardListener: Continuously monitors keyboard input and updates the last key pressed.
 * Uses atomic variables for thread safety when accessed by other threads.
 * Also detects when keys are released by using a timeout mechanism.
 * 
 * @param state Reference to the KeyboardState struct containing all keyboard-related state
 */
void keyboardListener(KeyboardState& state);

/**
 * keyboardControl: Handles motor control based on keyboard input.
 * Uses 'w', 's', 'a', 'd' keys for movement control and 'x' for stopping the motors.
 * Motors will only move while a key is held down and will stop when the key is released.
 * 
 * @param leftMotor Reference to the left motor object.
 * @param rightMotor Reference to the right motor object.
 * @param state Reference to the KeyboardState struct
 * @param logger Shared pointer to the logger for recording events
 */
void keyboardControl(Motor &leftMotor, Motor &rightMotor, KeyboardState& state, std::shared_ptr<Logger> logger);

/**
 * keyboardControl: Handles motor control based on keyboard input.
 * Uses 'w', 's', 'a', 'd' keys for movement control and 'x' for stopping the motors.
 * Motors will only move while a key is held down and will stop when the key is released.
 * 
 * @param leftMotor Reference to the left motor object.
 * @param rightMotor Reference to the right motor object.
 * @param key Reference to the KeyboardState struct
 * @param logger Shared pointer to the logger for recording events
 */
void keyboardEventControl(std::shared_ptr<Motor> leftMotor, std::shared_ptr<Motor> rightMotor, KeyEventInfo& keyInfo);

/**
 * setupTerminal: Configures terminal for non-canonical, non-blocking input
 * 
 * @param state Reference to KeyboardState struct to store original settings
 * @return 0 on success, -1 on failure
 */
int setupTerminal(KeyboardState& state);

/**
 * restoreTerminal: Restores original terminal settings
 * 
 * @param state Reference to KeyboardState struct containing original settings
 */
void restoreTerminal(KeyboardState& state);

#endif // LN298MOTORCONTROLV3_H
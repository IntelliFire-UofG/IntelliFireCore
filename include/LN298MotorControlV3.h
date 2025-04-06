#ifndef LN298MOTORCONTROLV3_H
#define LN298MOTORCONTROLV3_H

#include <iostream>
#include <gpiod.h>       // GPIO library for controlling pins on Raspberry Pi
#include <thread>        // For handling threading operations
#include <atomic>        // For thread-safe variables (used for keyboard control)
#include <chrono>        // For time-based operations (e.g., delays)
#include <termios.h>     // For handling terminal I/O settings for keyboard input
#include <unistd.h>      // For low-level I/O operations
#include "rpi_pwm.h"     // Include your PWM handling class for motor control

#define CHIP_NAME "gpiochip0" // Raspberry Pi 5 GPIO chip name used by libgpiod

/**
 * Motor Class
 * This class provides a simple interface for controlling a motor using the L298N motor driver.
 * It supports basic operations like moving forward, backward, stopping, and adjusting speed via PWM.
 */
class Motor {
private:
    gpiod_chip* chip;     // Pointer to GPIO chip for accessing GPIO lines
    RPI_PWM pwm;          // PWM object used for generating PWM signals for motor speed control
    int in1_pin;          // GPIO pin for motor IN1 (controls motor direction)
    int in2_pin;          // GPIO pin for motor IN2 (controls motor direction)
    int pwm_channel;      // PWM channel used for controlling motor speed
    int dutyCycle;        // Initial duty cycle value for PWM (default: 75%)

public:
    /**
     * Constructor: Initializes the motor with specified PWM channel, GPIO pins, and duty cycle.
     * 
     * @param pwm_channel The PWM channel used for speed control (e.g., 0, 1)
     * @param in1_pin GPIO pin number for IN1
     * @param in2_pin GPIO pin number for IN2
     * @param dutyCycle Initial duty cycle for PWM signal (default: 75%)
     */
    Motor(int pwm_channel, int in1_pin, int in2_pin, int dutyCycle = 75);

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
};

/**
 * keyboardListener: Continuously monitors keyboard input and updates the last key pressed.
 * Uses atomic variables for thread safety when accessed by other threads.
 * 
 * @param lastKey Atomic character variable to store the last pressed key.
 * @param keyPressed Atomic boolean variable to indicate if a key was pressed.
 */
void keyboardListener(std::atomic<char>& lastKey, std::atomic<bool>& keyPressed);

/**
 * keyboardControl: Handles motor control based on keyboard input.
 * Uses 'w', 's', 'a', 'd' keys for movement control and 'x' for stopping the motors.
 * 
 * @param leftMotor Reference to the left motor object.
 * @param rightMotor Reference to the right motor object.
 * @param lastKey Atomic character representing the last pressed key.
 * @param keyPressed Atomic boolean flag indicating if a key was pressed.
 */
void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey, std::atomic<bool>& keyPressed);

#endif // LN298MOTORCONTROLV3_H
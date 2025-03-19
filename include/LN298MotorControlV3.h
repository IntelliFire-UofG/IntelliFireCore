#ifndef LN298MOTORCONTROLV2_H
#define LN298MOTORCONTROLV2_H

#include <iostream>
#include <gpiod.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <termios.h>
#include <unistd.h>

#define CHIP_NAME "gpiochip0" // Raspberry Pi 5 GPIO chip

/**
 * @class Motor
 * @brief Controls DC motors using the L298N motor driver via GPIO pins.
 *
 * This class provides methods to control motor movement through the L298N motor driver,
 * allowing forward, backward, stop, and turning operations.
 *
 * @author Misael Rivera
 * @date 2025-02-01
 */
class Motor {
private:
    gpiod_chip *chip;     ///< GPIO chip interface
    gpiod_line *pwm;      ///< PWM control GPIO line
    gpiod_line *in1;      ///< Input 1 GPIO line
    gpiod_line *in2;      ///< Input 2 GPIO line

public:
    /**
     * @brief Constructs a Motor object connected to specific GPIO pins.
     * 
     * @param pwm_pin GPIO pin number for PWM control
     * @param in1_pin GPIO pin number for Input 1
     * @param in2_pin GPIO pin number for Input 2
     */
    Motor(int pwm_pin, int in1_pin, int in2_pin);

    /**
     * @brief Moves the motor in forward direction.
     */
    void moveForward();

    /**
     * @brief Moves the motor in backward direction.
     */
    void moveBackward();

    /**
     * @brief Stops the motor by cutting power.
     */
    void stop();
    /**
     * @brief Executes a left turn operation.
     */
    void turnLeft();

    /**
     * @brief Executes a right turn operation.
     */
    void turnRight();

    /**
     * @brief Destructor that releases GPIO resources.
     */
    ~Motor();
};

/**
 * @brief Listens for keyboard input in a separate thread.
 * 
 * This function runs in a separate thread and monitors keyboard input,
 * storing the last key pressed in an atomic variable.
 * 
 * @param lastKey Atomic reference to store the most recent key pressed
 */
void keyboardListener(std::atomic<char>& lastKey);

/**
 * @brief Controls motors based on keyboard input.
 * 
 * This function interprets keyboard commands and translates them
 * into motor movements (W: forward, S: backward, A: left, D: right).
 * 
 * @param leftMotor Reference to the left motor object
 * @param rightMotor Reference to the right motor object
 * @param lastKey Atomic reference containing the last key pressed
 */
void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey);

#endif // LN298MOTORCONTROLV2_H
#ifndef LN298MOTORCONTROLV2_H
#define LN298MOTORCONTROLV2_H

#include <iostream>
#include <gpiod.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include "rpi_pwm.h"

#define CHIP_NAME "gpiochip0" // Raspberry Pi 5 GPIO chip

/**
 * @class Motor
 * @brief Controls DC motors using the L298N motor driver via GPIO pins.
 *
 * This class provides methods to control motor movement through the L298N motor driver,
 * allowing forward, backward, stop, and turning operations.
 *
 * @author Abdul-Fattah Abdulateef 
 * @date 2025-02-01
 */
class Motor {
private:
    gpiod_chip *chip;     ///< GPIO chip interface
    gpiod_line *in1;      ///< Input 1 GPIO line
    gpiod_line *in2;      ///< Input 2 GPIO line
    int pwm_channel;      ///< PWM channel for speed control
    RPI_PWM pwm;          ///< PWM controller instance
    int current_speed;    ///< Current speed percentage (0-100)
    bool is_running;      ///< Flag indicating if motor is active

public:
    /**
     * @brief Constructs a Motor object connected to specific GPIO pins.
     * 
     * @param pwm_channel PWM channel for speed control (2 or 3 on Raspberry Pi 5)
     * @param in1_pin GPIO pin number for Input 1
     * @param in2_pin GPIO pin number for Input 2
     * @param frequency PWM frequency in Hz (default 100)
     */
    Motor(int pwm_channel, int in1_pin, int in2_pin, int frequency = 100);

        /**
     * @brief Moves the motor in forward direction.
     * 
     * @param speed Speed percentage (0-100, default 100)
     */
    void moveForward(int speed = 100);

    /**
     * @brief Moves the motor in backward direction.
     * 
     * @param speed Speed percentage (0-100, default 100)
     */
    void moveBackward(int speed = 100);

    /**
     * @brief Stops the motor by cutting power.
     */
    void stop();

    /**
     * @brief Executes a left turn operation.
     * 
     * @param speed Speed percentage (0-100, default 100)
     */
    void turnLeft(int speed = 100);

    /**
     * @brief Executes a right turn operation.
     * 
     * @param speed Speed percentage (0-100, default 100)
     */
    void turnRight(int speed = 100);

    /**
     * @brief Sets the motor speed.
     * 
     * @param speed Speed percentage (0-100)
     */
    void setSpeed(int speed);

    /**
     * @brief Gets the current motor speed.
     * 
     * @return Current speed percentage (0-100)
     */
    int getSpeed() const;

    /**
     * @brief Gradually increases motor speed.
     * 
     * @param target_speed Target speed percentage (0-100)
     * @param step_size Speed increment per step (default 5)
     * @param delay_ms Delay between steps in milliseconds (default 50)
     */
    void accelerate(int target_speed, int step_size = 5, int delay_ms = 50);

    /**
     * @brief Gradually decreases motor speed.
     * 
     * @param target_speed Target speed percentage (0-100)
     * @param step_size Speed decrement per step (default 5)
     * @param delay_ms Delay between steps in milliseconds (default 50)
     */
    void decelerate(int target_speed, int step_size = 5, int delay_ms = 50);

    /**
     * @brief Destructor that releases GPIO and PWM resources.
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
 * Also supports speed control with number keys.
 * 
 * @param leftMotor Reference to the left motor object
 * @param rightMotor Reference to the right motor object
 * @param lastKey Atomic reference containing the last key pressed
 */
void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey);

#endif // LN298MOTORCONTROLV3_H

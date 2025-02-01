#ifndef LN298MOTORCONTROL_H
#define LN298MOTORCONTROL_H

#include <functional>

/**
 * @class MotorController
 * @brief Controls 4 motors using L298N motor driver via Raspberry Pi GPIO.
 *
 * This class provides methods to move the motors forward, backward, and stop. It also
 * supports event-driven callbacks for dynamic motor control.
 *
 * @author Misael Rivera
 * @date 2025-02-01
 */
class MotorController {
public:
    /**
     * @brief Constructs a MotorController object and initializes motor control pins.
     * 
     * @param enA GPIO pin for left motor speed control (PWM).
     * @param in1 GPIO pin for left motor direction.
     * @param in2 GPIO pin for left motor direction.
     * @param enB GPIO pin for right motor speed control (PWM).
     * @param in3 GPIO pin for right motor direction.
     * @param in4 GPIO pin for right motor direction.
     */
    MotorController(int enA, int in1, int in2, int enB, int in3, int in4);

    /**
     * @brief Moves the fire truck forward at a specified speed.
     * 
     * @param speed Speed of the motors (0-100%).
     */
    void moveForward(int speed);

    /**
     * @brief Moves the fire truck backward at a specified speed.
     * 
     * @param speed Speed of the motors (0-100%).
     */
    void moveBackward(int speed);

    /**
     * @brief Moves the fire truck left at a specified speed.
     * 
     * @param speed Speed of the motors (0-100%).
     */
    void moveLeft(int speed);

    /**
     * @brief Moves the fire truck right at a specified speed.
     * 
     * @param speed Speed of the motors (0-100%).
     */
    void moveRight(int speed);

    /**
     * @brief Stops all motors.
     */
    void stop();

    /**
     * @brief Registers a callback for event-driven motor actions.
     * 
     * @param callback Function to execute when an event occurs.
     */
    void registerCallback(std::function<void()> callback);

private:
    int ENA, IN1, IN2, ENB, IN3, IN4;
    std::function<void()> motorEventCallback;

    /**
     * @brief Sets the motor direction.
     * 
     * @param forward True for forward, false for backward.
     */
    void setMotorState(bool forward);
};

#endif // LN298MOTORCONTROL_H

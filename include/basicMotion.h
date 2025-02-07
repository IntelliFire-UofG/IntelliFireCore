#ifndef BASIC_MOTION_H
#define BASIC_MOTION_H

#include "LN298MotorControl.h"
/**
 * @class BasicMotion
 * @brief Defines the basic motions the robot is capable to achieve with two motors
 *
 * @author Brenda Camacho
 * @date 2025-02-07
 */
class BasicMotion
{
public:
    /**
     * @brief Constructor for BasicMotion.
     * @param motor_right Pointer to the right motor controller.
     * @param motor_left Pointer to the left motor controller.
     */
    BasicMotion(MotorController *motor_right, MotorController *motor_left);

    /**
     * @brief Moves the robot forward.
     */
    void forward();

    /**
     * @brief Moves the robot backward.
     */
    void backward();

    /**
     * @brief Turns the robot to the right.
     */
    void turnRight();

    /**
     * @brief Turns the robot to the left.
     */
    void turnLeft();

    /**
     * @brief Sets the speed of the robot.
     */
    void setSpeed();

private:
    MotorController *motor_right; ///< Pointer to the right motor controller.
    MotorController *motor_left;  ///< Pointer to the left motor controller.
    int speed = 50; ///< Default speed value.
};

#endif // BASIC_MOTION_H
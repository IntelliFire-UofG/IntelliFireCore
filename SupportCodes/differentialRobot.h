#ifndef DIFFERENTIAL_ROBOT_H
#define DIFFERENTIAL_ROBOT_H

#include "LN298MotorControl.h"
/**
 * @class DifferentialRobot
 * @brief Defines the kinematic equations for a differential robot
 * 
 * This class implements kinematics for a differential drive robot.
 *
 * @author Brenda Camacho
 * @date 2025-02-07
 */
class DifferentialRobot
{
public:
    DifferentialRobot(double wheels_distance, double wheels_radius, MotorController *motorA, MotorController *motorB)
    void move(double linear_velocity, double angular_velocity)
private:
    double wheels_distance;
    double wheels_radius; 
    MotorController *motorA; 
    MotorController *motorB;
}

#endif // DIFFERENTIAL_ROBOT_H
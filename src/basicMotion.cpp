#include "basicMotion.h"

BasicMotion::BasicMotion(MotorController *motor_right, MotorController *motor_left): 
motor_right(motor_right), motor_left (motor_left) 
{}
void BasicMotion::forward()
{
    motor_right.setMotorSpeed(speed);
    motor_left.setMotorSpeed(speed);
}
void BasicMotion::backward()
{
    motor_right.setMotorSpeed(-1 * speed);
    motor_left.setMotorSpeed(-1 * speed);
}

void BasicMotion::turnRight()
{
    motor_right.setMotorSpeed(-1 * speed);
    motor_left.setMotorSpeed(speed);
}
void BasicMotion::turnLeft()
{
    motor_right.setMotorSpeed(speed);
    motor_left.setMotorSpeed(-1 * speed);
}
void BasicMotion::setSpeed(int new_speed)
{
    speed = new_speed;
}
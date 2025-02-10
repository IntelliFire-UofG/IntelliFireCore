#include "LN298MotorControl.h"
#include <cmath>


MotorController::MotorController(int enA, int in1, int in2)
{

}

void MotorController::setMotorSpeed(int8 speed)
{
    if (speed < 0)
    {
        setMotorDirection(Direction::NEGATIVE);
    } else 
    {
        setMotorDirection(Direction::POSITIVE);
    }
    setPWMDuty(std::abs(speed));
}


void MotorController::registerCallback(std::function<void()> callback);


void MotorController::setPWMDuty(int8 duty)
{
    // TODO: Write the code for the PWM
}


void MotorController::setMotorDirection(Direction dir)
{
    //TODO: Write code to set GPIOs high and low according to direction
    if (dir == Direction::POSITIVE)
    {}
    else
    {}
}

#endif // LN298MOTORCONTROL_H

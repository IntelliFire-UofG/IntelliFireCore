#include "LN298MotorControl.h"
#include <pigpio.h>
#include <cmath>


MotorController::MotorController(int enA, int in1, int in2): enablePin(enA), in1(in1), in2(in2)
{
    gpioSetMode(enablePin, PI_OUTPUT);
    gpioSetMode(in1, PI_OUTPUT);
    gpioSetMode(in2, PI_OUTPUT);
    gpioSetPWMfrequency(enablePin, 1000);

}

void MotorController::setMotorSpeed(int8_t speed)
{
    if (speed == 0)
    {
        gpioWrite(in1, 0);
        gpioWrite(in2, 0);
        setPWMDuty(0);
    }
    else {
        if (speed < 0)
        {
            setMotorDirection(Direction::NEGATIVE);
        } else 
        {
            setMotorDirection(Direction::POSITIVE);
        }
        setPWMDuty(std::abs(speed));
    }
}


// void MotorController::registerCallback(std::function<void()> callback);


void MotorController::setPWMDuty(int8_t duty)
{
    // TODO: Write the code for the PWM
    int pwmValue = (duty * 255) / 100;
    gpioPWM(enablePin, pwmValue);
}


void MotorController::setMotorDirection(Direction dir)
{
    //TODO: Write code to set GPIOs high and low according to direction
    if (dir == Direction::POSITIVE)
    {
        gpioWrite(in1, 1);
        gpioWrite(in2, 0);
    }
    else
    {
        gpioWrite(in1, 0);
        gpioWrite(in2, 1);
    }
}
void MotorController::registerCallback(std::function<void()> callback)
{
    motorEventCallback = callback;
}

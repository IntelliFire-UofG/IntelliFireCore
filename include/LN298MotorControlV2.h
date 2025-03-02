#ifndef LN298MOTORCONTROLV2_H
#define LN298MOTORCONTROLV2_H

#include <iostream>
#include <gpiod.h>
#include <thread>
#include <chrono>

#define CHIP_NAME "gpiochip0" // Raspberry Pi 5 GPIO chip

class Motor {
private:
    gpiod_chip *chip;
    gpiod_line *pwm;
    gpiod_line *in1;
    gpiod_line *in2;

public:
    // Constructor
    Motor(int pwm_pin, int in1_pin, int in2_pin);

    // Motor control functions
    void moveForward();
    void moveBackward();
    void stop();

    // Destructor
    ~Motor();
};

// Turning functions
void turnLeft(Motor &leftMotor, Motor &rightMotor);
void turnRight(Motor &leftMotor, Motor &rightMotor);

#endif // LN298MOTORCONTROLV2_H

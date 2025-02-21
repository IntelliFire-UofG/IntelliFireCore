#ifndef MOTOR_H
#define MOTOR_H

#include <gpiod.h>
#include <iostream>

#define CHIP_NAME "gpiochip0"  // Raspberry Pi 5 GPIO chip

class Motor {
private:
    gpiod_chip *chip;
    gpiod_line *pwm;
    gpiod_line *in1;
    gpiod_line *in2;

public:
    // Constructor to initialize motor pins
    Motor(int pwm_pin, int in1_pin, int in2_pin);

    // Function to move motor forward
    void moveForward();

    // Function to move motor backward
    void moveBackward();

    // Function to stop the motor
    void stop();

    // Destructor to release GPIO pins
    ~Motor();
};

#endif

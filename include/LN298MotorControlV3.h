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
    void turnLeft();
    void turnRight();

    // Destructor
    ~Motor();
};

// Ensure keyboard listener function is declared
void keyboardListener(std::atomic<char>& lastKey);
void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey);

#endif // LN298MOTORCONTROLV2_H

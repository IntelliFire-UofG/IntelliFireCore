#include "LN298MotorControl.h"
#include <iostream>
#include <cmath>
#define CHIP_NAME "gpiochip0"

/*
CODE REVIEW -> 19/February/2025
Please check naming convenction and functions organization
Remember to keep up to date branches
*/

MotorController::MotorController(int enA, int in1Pin, int in2Pin)
{
    chip = gpiod_chip_open_by_name(CHIP_NAME);
    if (!chip){
        std::cerr << "ERROR: Unable to open GPIO Chip!" << std::endl;
        exit(1);
    }

    // Get GPIO lines
    enablePin = gpiod_chip_get_line(chip, enA);
    in1 = gpiod_chip_get_line(chip, in1Pin);
    in2 = gpiod_chip_get_line(chip, in2Pin);

    if (!enablePin || !in1 || !in2) {
        std::cerr << "Error: Unable to access GPIO pins!" << std::endl;
        gpiod_chip_close(chip);
        exit(1);
    }

    // Request GPIO lines as outputs
    gpiod_line_request_output(enablePin, "motor_enable", 0);
    gpiod_line_request_output(in1, "motor_in1", 0);
    gpiod_line_request_output(in2, "motor_in2", 0);
}

MotorController::~MotorController() {
    // Release all GPIO resources
    gpiod_line_release(enablePin);
    gpiod_line_release(in1);
    gpiod_line_release(in2);
    gpiod_chip_close(chip);
}

void MotorController::setMotorSpeed(int8_t speed) {
    if (speed == 0) {
        // Stop the motor
        gpiod_line_set_value(in1, 0);
        gpiod_line_set_value(in2, 0);
        gpiod_line_set_value(enablePin, 0);
    } else {
        // Set direction and speed
        if (speed < 0) {
            setMotorDirection(Direction::NEGATIVE);
        } else {
            setMotorDirection(Direction::POSITIVE);
        }
        setPWMDuty(std::abs(speed));
    }

    // Call callback if registered
    // if (motorEventCallback) {
    //     motorEventCallback();
    // }
}


// void MotorController::registerCallback(std::function<void()> callback);


void MotorController::setPWMDuty(int8_t duty) {
    // For now, we're using binary control (ON/OFF)
    // Future enhancement could implement proper PWM
    gpiod_line_set_value(enablePin, duty > 0 ? 1 : 0);
}


void MotorController::setMotorDirection(Direction dir) {
    if (dir == Direction::POSITIVE) {
        gpiod_line_set_value(in1, 1);
        gpiod_line_set_value(in2, 0);
    } else {
        gpiod_line_set_value(in1, 0);
        gpiod_line_set_value(in2, 1);
    }
}

void MotorController::registerCallback(std::function<void()> callback)
{
    motorEventCallback = callback;

}

#endif // LN298MOTORCONTROL_H

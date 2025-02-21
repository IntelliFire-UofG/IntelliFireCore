#include "Motor.h"

Motor::Motor(int pwm_pin, int in1_pin, int in2_pin) {
    chip = gpiod_chip_open_by_name(CHIP_NAME);
    if (!chip) {
        std::cerr << "❌ Error: Unable to open GPIO chip!" << std::endl;
        exit(1);
    }

    pwm = gpiod_chip_get_line(chip, pwm_pin);
    in1 = gpiod_chip_get_line(chip, in1_pin);
    in2 = gpiod_chip_get_line(chip, in2_pin);

    if (!pwm || !in1 || !in2) {
        std::cerr << "❌ Error: Unable to access GPIO pins!" << std::endl;
        gpiod_chip_close(chip);
        exit(1);
    }

    gpiod_line_request_output(pwm, "motor_pwm", 0);
    gpiod_line_request_output(in1, "motor_in1", 0);
    gpiod_line_request_output(in2, "motor_in2", 0);
}

void Motor::moveForward() {
    gpiod_line_set_value(in1, 1);
    gpiod_line_set_value(in2, 0);
    gpiod_line_set_value(pwm, 1); // Enable PWM
    std::cout << "🚗 Moving Forward" << std::endl;
}

void Motor::moveBackward() {
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 1);
    gpiod_line_set_value(pwm, 1);
    std::cout << "🔄 Moving Backward" << std::endl;
}

void Motor::stop() {
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 0);
    gpiod_line_set_value(pwm, 0);
    std::cout << "⏹️ Motor Stopped" << std::endl;
}

Motor::~Motor() {
    gpiod_line_release(pwm);
    gpiod_line_release(in1);
    gpiod_line_release(in2);
    gpiod_chip_close(chip);
}

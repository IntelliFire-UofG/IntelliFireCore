#include "LN298MotorControlV2.h"
#include <atomic>  // Fix atomic variable errors
#include <thread>  // Fix thread-related errors
#include <unistd.h> // Required for STDIN_FILENO
#include <atomic>   // Required for std::atomic
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool keyChanged = false;


Motor::Motor(int pwm_pin, int in1_pin, int in2_pin) {
    chip = gpiod_chip_open_by_name(CHIP_NAME);
    if (!chip) {
        std::cerr << "? Error: Unable to open GPIO chip!" << std::endl;
        exit(1);
    }

    pwm = gpiod_chip_get_line(chip, pwm_pin);
    in1 = gpiod_chip_get_line(chip, in1_pin);
    in2 = gpiod_chip_get_line(chip, in2_pin);

    if (!pwm || !in1 || !in2) {
        std::cerr << "? Error: Unable to access GPIO pins!" << std::endl;
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
    gpiod_line_set_value(pwm, 1);
    std::cout << "?? Moving Forward" << std::endl;
}

void Motor::moveBackward() {
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 1);
    gpiod_line_set_value(pwm, 1);
    std::cout << "?? Moving Backward" << std::endl;
}

void Motor::stop() {
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 0);
    gpiod_line_set_value(pwm, 0);
    std::cout << "?? Motor Stopped" << std::endl;
}

void Motor::turnLeft() {
    stop();
    gpiod_line_set_value(in1, 1);
    gpiod_line_set_value(in2, 0);
    gpiod_line_set_value(pwm, 1);
    std::cout << "?? Turning Left" << std::endl;
}

void Motor::turnRight() {
    stop();
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 1);
    gpiod_line_set_value(pwm, 1);
    std::cout << "?? Turning Right" << std::endl;
}

Motor::~Motor() {
    gpiod_line_release(pwm);
    gpiod_line_release(in1);
    gpiod_line_release(in2);
    gpiod_chip_close(chip);
}

void keyboardListener(std::atomic<char>& lastKey) {
    struct termios oldt, newt;
    char ch;
    fd_set readfds;
    struct timeval timeout;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 50000;  // Check every 50ms

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
            if (read(STDIN_FILENO, &ch, 1) > 0) {
                lastKey.store(ch);
            }
        } else {
            lastKey.store('\0');  // If no key is pressed, stop the motor
        }

        // Notify motor control that the key state changed
        {
            std::lock_guard<std::mutex> lock(mtx);
            keyChanged = true;
        }
        cv.notify_one();
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey) {
    std::cout << "?? Hold W/A/S/D to move, release to stop. Press Q to quit." << std::endl;

    char prevKey = '\0';  // Track previous key state
    bool motorRunning = false;  // Track if motor is running

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return keyChanged; });  // Wait for key change event
        keyChanged = false;

        char key = lastKey.load();  // Get latest key press

        if (key != prevKey) {  // Only act if key state changes
            prevKey = key;  // Update previous key state

            if (key == 'w') {
                std::cout << "?? Moving Forward" << std::endl;
                leftMotor.moveForward();
                rightMotor.moveForward();
                motorRunning = true;
            } else if (key == 's') {
                std::cout << "?? Moving Backward" << std::endl;
                leftMotor.moveBackward();
                rightMotor.moveBackward();
                motorRunning = true;
            } else if (key == 'a') {
                std::cout << "?? Turning Left" << std::endl;
                leftMotor.turnLeft();
                motorRunning = true;
            } else if (key == 'd') {
                std::cout << "?? Turning Right" << std::endl;
                rightMotor.turnRight();
                motorRunning = true;
            } else if (key == '\0' && motorRunning) {  // Stop motor only if it was running
                std::cout << "?? Motor Stopped" << std::endl;
                leftMotor.stop();
                rightMotor.stop();
                motorRunning = false;
            } else if (key == 'q') {  // Quit program
                std::cout << "?? Exiting..." << std::endl;
                return;
            }
        }
    }
}
















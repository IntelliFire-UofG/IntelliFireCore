#include "../include/LN298MotorControlV3.h"
#include <atomic>  // Fix atomic variable errors
#include <thread>  // Fix thread-related errors
#include <unistd.h> // Required for STDIN_FILENO
#include <atomic>   // Required for std::atomic
#include <condition_variable>
#include <chrono>
#include <fcntl.h>
#include <sys/select.h>

std::mutex mtx;
std::condition_variable cv;
bool keyChanged = false;


Motor::Motor(int pwm_channel, int in1_pin, int in2_pin, int frequency) {
    chip = gpiod_chip_open_by_name(CHIP_NAME);
    if (!chip) {
        std::cerr << "? Error: Unable to open GPIO chip!" << std::endl;
        exit(1);
    }
    in1 = gpiod_chip_get_line(chip, in1_pin);
    in2 = gpiod_chip_get_line(chip, in2_pin);

    if (!in1 || !in2) {
        std::cerr << "? Error: Unable to access GPIO pins!" << std::endl;
        gpiod_chip_close(chip);
        exit(1);
    }
    gpiod_line_request_output(in1, "motor_in1", 0);
    gpiod_line_request_output(in2, "motor_in2", 0);

    this->pwm_channel = pwm_channel;

    if (pwm.start(pwm_channel, frequency) < 0) {
        std::cerr << "? Error: Failed to initialize PWM on channel" << pwm_channel << std::endl;
        std::cerr << " Make sure 'dtoverlay=pwm-2chan' is added to /boot/firmware/config.txt" << std::endl;
        exit(1);
    }

    current_speed = 0;
    pwm.setDutyCycle(0);
    is_running = false;

    std::cout<< "?? Motor initialized with PWM on channel " << pwm_channel << " at " << frequency << "Hz" << std::endl;
}


void Motor::moveForward(int speed) {
    // stop();
    gpiod_line_set_value(in1, 1);
    gpiod_line_set_value(in2, 0);
    // gpiod_line_set_value(pwm, 1);

    setSpeed(speed);
    is_running = true;
    std::cout << "?? Moving Forward at " << current_speed << "% speed" << std::endl;
}

void Motor::moveBackward(int speed) {
    // stop();
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 1);
    // gpiod_line_set_value(pwm, 1);

    setSpeed(speed);
    is_running = true;
    std::cout << "?? Moving Backward at " << current_speed << "% speed" << std::endl;
}

void Motor::stop() {
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 0);
    // gpiod_line_set_value(pwm, 0);

    setSpeed(0);
    is_running = false;
    std::cout << "?? Motor Stopped" << std::endl;
}

void Motor::turnLeft(int speed) {
    // stop();
    gpiod_line_set_value(in1, 1);
    gpiod_line_set_value(in2, 0);
    // gpiod_line_set_value(pwm, 1);

    setSpeed(0);
    is_running = false;

    std::cout << "?? Turning Left at " << current_speed << "% speed" << std::endl;
}

void Motor::turnRight(int speed) {
    // stop();
    gpiod_line_set_value(in1, 1);
    gpiod_line_set_value(in2, 0);
    // gpiod_line_set_value(pwm, 1);

    setSpeed(speed);
    is_running = true;
    std::cout << "?? Turning Right at " << current_speed << "% speed" << std::endl;
}

void Motor::setSpeed(int speed) {
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;

    current_speed = speed;
    pwm.setDutyCycle(speed);
}
int Motor::getSpeed() const {
    return current_speed;
}

void Motor::accelerate(int target_speed, int step_size, int delay_ms) {
    // Clamp target speed
    if (target_speed > 100) target_speed = 100;
    if (target_speed < 0) target_speed = 0;
    
    // Only accelerate if target is higher than current
    if (current_speed < target_speed) {
        for (int speed = current_speed; speed <= target_speed; speed += step_size) {
            setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }
        // Ensure we hit the exact target speed
        setSpeed(target_speed);
    } else {
        // If already at or above target, just set the speed directly
        setSpeed(target_speed);
    }
}

void Motor::decelerate(int target_speed, int step_size, int delay_ms) {
    // Clamp target speed
    if (target_speed > 100) target_speed = 100;
    if (target_speed < 0) target_speed = 0;
    
    // Only decelerate if target is lower than current
    if (current_speed > target_speed) {
        for (int speed = current_speed; speed >= target_speed; speed -= step_size) {
            setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }
        // Ensure we hit the exact target speed
        setSpeed(target_speed);
    } else {
        // If already at or below target, just set the speed directly
        setSpeed(target_speed);
    }
}

Motor::~Motor() {

    pwm.stop();
    // gpiod_line_release(pwm);
    gpiod_line_release(in1);
    gpiod_line_release(in2);
    gpiod_chip_close(chip);

    std::cout << "?? Motor resources released" << std::endl;
}

void keyboardListener(std::atomic<char>& lastKey) {
    struct termios oldt, newt;
    char ch;
    fd_set readfds;
    struct timeval timeout;

    // Save old terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Set stdin to non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 50000;  // Check every 50ms

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
            if (read(STDIN_FILENO, &ch, 1) > 0) {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    lastKey.store(ch);
                    keyChanged = true;
                }
                cv.notify_one();
            }
        } else {
            // If we had a key pressed previously and no key now, store null character
            char current = lastKey.load();
            if (current != '\0') {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    lastKey.store('\0');
                    keyChanged = true;
                }
                cv.notify_one();
            }
        }
    }

    // Restore old terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey) {
    std::cout << "?? Control instructions:" << std::endl;
    std::cout << "  W/A/S/D: Move forward/left/backward/right" << std::endl;
    std::cout << "  1-9: Set speed (10-90%)" << std::endl;
    std::cout << "  +/-: Increase/decrease speed by 10%" << std::endl;
    std::cout << "  Q: Quit" << std::endl;

    char prevKey = '\0';  // Track previous key state
    bool motorRunning = false;
    int currentSpeed = 70; // Default speed 70%

    while (true) {
        // Wait for key change with timeout
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, std::chrono::milliseconds(100), []{ return keyChanged; });
        
        // Reset key changed flag
        keyChanged = false;
        
        // Get current key
        char key = lastKey.load();

        // Handle speed control with number keys
        if (key >= '1' && key <= '9') {
            currentSpeed = (key - '0') * 10; // Convert key '1'-'9' to speed 10%-90%
            std::cout << "?? Speed set to " << currentSpeed << "%" << std::endl;
            
            // Update speed if motors are running
            if (leftMotor.getSpeed() > 0) {
                leftMotor.setSpeed(currentSpeed);
            }
            if (rightMotor.getSpeed() > 0) {
                rightMotor.setSpeed(currentSpeed);
            }
            continue;
        }
        
        // Handle speed increase/decrease with +/- keys
        if (key == '+' || key == '=') { // '=' is the unshifted '+' on many keyboards
            currentSpeed = std::min(100, currentSpeed + 10);
            std::cout << "?? Speed increased to " << currentSpeed << "%" << std::endl;
            
            // Update speed if motors are running
            if (leftMotor.getSpeed() > 0) {
                leftMotor.setSpeed(currentSpeed);
            }
            if (rightMotor.getSpeed() > 0) {
                rightMotor.setSpeed(currentSpeed);
            }
            continue;
        }
        
        if (key == '-' || key == '_') { // '_' is the shifted '-' on many keyboards
            currentSpeed = std::max(10, currentSpeed - 10);
            std::cout << "?? Speed decreased to " << currentSpeed << "%" << std::endl;
            
            // Update speed if motors are running
            if (leftMotor.getSpeed() > 0) {
                leftMotor.setSpeed(currentSpeed);
            }
            if (rightMotor.getSpeed() > 0) {
                rightMotor.setSpeed(currentSpeed);
            }
            continue;
        }

        // Handle movement keys only if they changed
        if (key != prevKey) {
            prevKey = key;

            if (key == 'w') {
                leftMotor.moveForward(currentSpeed);
                rightMotor.moveForward(currentSpeed);
                motorRunning = true;
            } else if (key == 's') {
                leftMotor.moveBackward(currentSpeed);
                rightMotor.moveBackward(currentSpeed);
                motorRunning = true;
            } else if (key == 'a') {
                leftMotor.moveForward(currentSpeed);
                rightMotor.moveBackward(currentSpeed);
                motorRunning = true;
            } else if (key == 'd') {
                leftMotor.moveBackward(currentSpeed);
                rightMotor.moveForward(currentSpeed);
                motorRunning = true;
            } else if (key == '\0' && motorRunning) {
                leftMotor.stop();
                rightMotor.stop();
                motorRunning = false;
            } else if (key == 'q') {
                std::cout << "?? Exiting..." << std::endl;
                leftMotor.stop();
                rightMotor.stop();
                return;
            }
        }
    }
}
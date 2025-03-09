#include "LN298MotorControlV2.h"
#include <atomic>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Atomic variable for thread-safe key state sharing
std::atomic<char> currentKey('\0');

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
}

void Motor::moveBackward() {
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 1);
    gpiod_line_set_value(pwm, 1);
}

void Motor::stop() {
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 0);
    gpiod_line_set_value(pwm, 0);
}

void Motor::turnLeft() {
    gpiod_line_set_value(in1, 1);
    gpiod_line_set_value(in2, 0);
    gpiod_line_set_value(pwm, 1);
}

void Motor::turnRight() {
    gpiod_line_set_value(in1, 0);
    gpiod_line_set_value(in2, 1);
    gpiod_line_set_value(pwm, 1);
}

Motor::~Motor() {
    gpiod_line_release(pwm);
    gpiod_line_release(in1);
    gpiod_line_release(in2);
    gpiod_chip_close(chip);
}

// Improved keyboard listener with non-blocking input
void keyboardListener() {
    struct termios oldt, newt;
    char ch;
    
    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    
    // Set terminal to raw mode
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;  // Return immediately, even when no input is available
    newt.c_cc[VTIME] = 0; // No timeout
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Set stdin to non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    
    while (true) {
        int result = read(STDIN_FILENO, &ch, 1);
        if (result > 0) {
            // If a key is pressed, store it
            currentKey.store(ch);
            if (ch == 'q') break; // Exit listener if 'q' is pressed
        } else {
            // If no key is pressed, reset to null character
            currentKey.store('\0');
        }
        
        // Much shorter polling interval
        usleep(5000); // 5ms polling interval (reduced from 50ms)
    }
    
    // Restore terminal settings before exiting
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, flags);
}

void keyboardControl(Motor &leftMotor, Motor &rightMotor) {
    std::cout << "?? Hold W/A/S/D to move, release to stop. Press Q to quit." << std::endl;

    // Start the keyboard listener thread
    std::thread listener(keyboardListener);
    
    // Detach to allow it to run independently
    listener.detach();
    
    char lastProcessedKey = '\0';
    bool isRunning = true;
    
    while (isRunning) {
        char key = currentKey.load();
        
        // Process the current key state
        if (key != lastProcessedKey) {
            lastProcessedKey = key;
            
            switch (key) {
                case 'w':
                    std::cout << "?? Moving Forward" << std::endl;
                    leftMotor.moveForward();
                    rightMotor.moveForward();
                    break;
                case 's':
                    std::cout << "?? Moving Backward" << std::endl;
                    leftMotor.moveBackward();
                    rightMotor.moveBackward();
                    break;
                case 'a':
                    std::cout << "?? Turning Left" << std::endl;
                    leftMotor.stop();
                    rightMotor.moveForward();
                    break;
                case 'd':
                    std::cout << "?? Turning Right" << std::endl;
                    leftMotor.moveForward();
                    rightMotor.stop();
                    break;
                case '\0':
                    std::cout << "?? Motor Stopped" << std::endl;
                    leftMotor.stop();
                    rightMotor.stop();
                    break;
                case 'q':
                    std::cout << "?? Exiting..." << std::endl;
                    isRunning = false;
                    break;
            }
        }
        
        // Minimal sleep to prevent CPU hogging
        usleep(2000); // 2ms processing loop
    }
    
    // Make sure motors are stopped when exiting
    leftMotor.stop();
    rightMotor.stop();
}

// int main() {
//     // Example usage with pin numbers
//     Motor leftMotor(18, 23, 24);  // Replace with your actual GPIO pins
//     Motor rightMotor(19, 25, 26); // Replace with your actual GPIO pins
    
//     keyboardControl(leftMotor, rightMotor);
    
//     return 0;
// }
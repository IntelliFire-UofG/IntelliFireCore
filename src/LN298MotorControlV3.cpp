#include "../include/LN298MotorControlV3.h"
#include "rpi_pwm.h"
#include <iostream>
#include <cstdlib>
#include <atomic>
#include <thread>
#include <termios.h>
#include <unistd.h>

// Motor Constructor: Initializes the motor with given parameters.
Motor::Motor(int pwm_channel, int in1_pin, int in2_pin, int dutyCycle) {

    // Initialize the PWM signal on the given PWM channel with 100 Hz frequency and the specified duty cycle
    if (pwm.start(pwm_channel, 100, dutyCycle) < 0) {  // Frequency = 100Hz (Good for DC motors)
        std::cerr << "Failed to initialize PWM on channel " << pwm_channel << std::endl;
        exit(1);  // Exit if initialization fails
    }
    
    // Store motor control pin numbers and duty cycle
    this->in1_pin = in1_pin;
    this->in2_pin = in2_pin;
    this->dutyCycle = dutyCycle;
}

// Function to move the motor forward
void Motor::moveForward() {
    pwm.setDutyCycle(75);  // Set PWM duty cycle to 75% for forward movement (High speed)

    // Set motor direction using GPIO pins: IN1=1, IN2=0
    std::system(("gpioset gpiochip0 " + std::to_string(in1_pin) + "=1").c_str());
    std::system(("gpioset gpiochip0 " + std::to_string(in2_pin) + "=0").c_str());
}

// Function to move the motor backward
void Motor::moveBackward() {
    pwm.setDutyCycle(75);  // Set PWM duty cycle to 75% for backward movement (High speed)

    // Set motor direction using GPIO pins: IN1=0, IN2=1
    std::system(("gpioset gpiochip0 " + std::to_string(in1_pin) + "=0").c_str());
    std::system(("gpioset gpiochip0 " + std::to_string(in2_pin) + "=1").c_str());
}

// Function to stop the motor
void Motor::stop() {
    pwm.setDutyCycle(0);  // Set PWM duty cycle to 0% to stop the motor (No power)
    
    // Set motor control pins to 0 (both low) to stop the motor
    std::system(("gpioset gpiochip0 " + std::to_string(in1_pin) + "=0").c_str());
    std::system(("gpioset gpiochip0 " + std::to_string(in2_pin) + "=0").c_str());
}

// Function to adjust motor speed by changing the duty cycle
void Motor::setSpeed(int speed) {
    dutyCycle = speed;  // Update duty cycle
    pwm.setDutyCycle(dutyCycle);  // Apply the new duty cycle
}

// Motor Destructor: Stops the motor and cleans up resources
Motor::~Motor() {
    pwm.stop();  // Stop the PWM signal
}

// Function to listen for keyboard inputs
void keyboardListener(std::atomic<char>& lastKey, std::atomic<bool>& keyPressed) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);  // Save current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    while (true) {
        ch = getchar();  // Read a character from standard input
        lastKey.store(ch);  // Store the character
        keyPressed.store(true);  // Signal that a key was pressed
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restore original terminal settings
}

// Function to control the motors using keyboard inputs
void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey, std::atomic<bool>& keyPressed) {
    std::cout << "Use W/A/S/D to control the motors. Press Q to quit." << std::endl;

    while (true) {
        char key = lastKey.load();  // Read the last pressed key

        if (keyPressed.load()) {  // If a key was pressed
            switch (key) {
                case 'w':  // Move both motors forward
                    leftMotor.moveForward();
                    rightMotor.moveForward();
                    break;
                
                case 's':  // Move both motors backward
                    leftMotor.moveBackward();
                    rightMotor.moveBackward();
                    break;
                
                case 'a':  // Turn left by stopping the left motor and moving the right motor forward
                    leftMotor.stop();
                    rightMotor.moveForward();
                    break;
                
                case 'd':  // Turn right by stopping the right motor and moving the left motor forward
                    leftMotor.moveForward();
                    rightMotor.stop();
                    break;
                
                case 'x':  // Stop both motors (Emergency Stop)
                    leftMotor.stop();
                    rightMotor.stop();
                    break;

                case 'q':  // Quit the program
                    std::cout << "Exiting..." << std::endl;
                    return;
            }
            keyPressed.store(false);  // Reset key press status
        }
    }
}
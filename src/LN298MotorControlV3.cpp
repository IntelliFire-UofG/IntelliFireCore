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
    if (pwm.setDutyCycle(dutyCycle) < 0) {
        std::cerr << "Error setting duty cycle" << std::endl;
        exit(1);
    }
    pwm.setDutyCycle(dutyCycle);  // Apply the new duty cycle
}

// Motor Destructor: Stops the motor and cleans up resources
Motor::~Motor() {
    pwm.stop();  // Stop the PWM signal
}

// Function to listen for keyboard inputs
void keyboardListener(std::atomic<char>& lastKey, std::atomic<bool>& keyPressed, std::atomic<bool>& running) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);  // Save current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Set stdin to non-blocking mode
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    char ch = '\0';
    char lastKeyPressed = '\0';  // Store the last key that was pressed
    bool keyIsHeld = false;      // Track if a key is currently being held down
    
    // How long (in ms) to wait before considering a key released
    const int KEY_RELEASE_TIMEOUT = 50;
    
    auto lastKeyTime = std::chrono::steady_clock::now();
    
    while (running.load()) {  // Check if we should continue running
        auto currentTime = std::chrono::steady_clock::now();
        bool keyReadThisLoop = false;
        
        // Read from stdin (non-blocking)
        if (read(STDIN_FILENO, &ch, 1) > 0) {  
            keyReadThisLoop = true;
            lastKeyPressed = ch;  // Store the character
            lastKeyTime = currentTime;
            
            if (!keyIsHeld || ch != lastKeyPressed) {
                // Signal a new key press if we weren't already holding a key
                // or if the key is different from the last one
                lastKey.store(ch);  
                keyPressed.store(true);  // Signal that a key was pressed
                keyIsHeld = true;
                lastKeyPressed = ch;
            }
            
            // Check for exit condition
            if (ch == 'q') {
                running.store(false);  // Signal to exit
            }
        }
        
        // If no key was read this loop and a key was being held...
        if (!keyReadThisLoop && keyIsHeld) {
            // Calculate time since last key press
            auto timeSinceLastKey = std::chrono::duration_cast<std::chrono::milliseconds>(
                currentTime - lastKeyTime).count();
                
            // If enough time has passed since the last key press, consider it released
            if (timeSinceLastKey > KEY_RELEASE_TIMEOUT) {
                // Signal key release by storing a special character
                lastKey.store('\0');  // Null character to indicate no key pressed
                keyPressed.store(true);  // Signal that key state has changed
                keyIsHeld = false;
            }
        }
        
        // Small sleep to prevent CPU hogging
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Restore terminal settings before exiting
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

// Function to control the motors using keyboard inputs
void keyboardControl(Motor &leftMotor, Motor &rightMotor, std::atomic<char>& lastKey, std::atomic<bool>& keyPressed) {
    std::cout << "Use W/A/S/D to control the motors. Press Q to quit." << std::endl;
    std::cout << "Motors will stop automatically when keys are released." << std::endl;

    while (true) {
        char key = lastKey.load();  // Read the last pressed key

        if (keyPressed.load()) {  // If a key state has changed
            switch (key) {
                case 'w':  // Move both motors forward
                    std::cout << "Moving forward..." << std::endl;
                    leftMotor.moveForward();
                    rightMotor.moveForward();
                    break;
                
                case 's':  // Move both motors backward
                    std::cout << "Moving backward..." << std::endl;
                    leftMotor.moveBackward();
                    rightMotor.moveBackward();
                    break;
                
                case 'a':  // Turn left by stopping the left motor and moving the right motor forward
                    std::cout << "Turning left..." << std::endl;
                    leftMotor.stop();
                    rightMotor.moveForward();
                    break;
                
                case 'd':  // Turn right by stopping the right motor and moving the left motor forward
                    std::cout << "Turning right..." << std::endl;
                    leftMotor.moveForward();
                    rightMotor.stop();
                    break;
                
                case 'x':  // Stop both motors (Emergency Stop)
                    std::cout << "Emergency stop!" << std::endl;
                    leftMotor.stop();
                    rightMotor.stop();
                    break;

                case 'q':  // Quit the program
                    std::cout << "Exiting..." << std::endl;
                    return;
                
                case '\0':  // No key pressed (key release detected)
                    std::cout << "Key released, stopping motors..." << std::endl;
                    leftMotor.stop();
                    rightMotor.stop();
                    break;

                default:
                    // Ignore other keys
                    break;
            }
            keyPressed.store(false);  // Reset key press status
        }
        
        // Small sleep to prevent CPU hogging
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
#include "LN298MotorControlV3.h"
#include "rpi_pwm.h"
#include <iostream>
#include <cstdlib>
#include <atomic>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <system_error>
#include "keyLogger.h"

// Motor Constructor: Initializes the motor with given parameters.
Motor::Motor(int pwm_channel, int in1_pin, int in2_pin, int dutyCycle, const std::string& name) 
    : chip(nullptr), in1_pin(in1_pin), in2_pin(in2_pin), pwm_channel(pwm_channel), 
      dutyCycle(dutyCycle), motorName(name) {
    
    // Get the logger instance
    logger = Logger::getInstance();
    logger->log("Initializing motor: " + motorName);
    
    try {
        // Initialize the PWM signal on the given PWM channel with 100 Hz frequency and the specified duty cycle
        if (pwm.start(pwm_channel, 100, dutyCycle) < 0) {
            throw std::runtime_error("Failed to initialize PWM on channel " + std::to_string(pwm_channel));
        }
        
        // Initialize GPIO pins and set them to low initially
        std::system(("gpioset gpiochip0 " + std::to_string(in1_pin) + "=0").c_str());
        std::system(("gpioset gpiochip0 " + std::to_string(in2_pin) + "=0").c_str());
        
        logger->log("Motor " + motorName + " initialized successfully");
    }
    catch (const std::exception& e) {
        logger->log("ERROR initializing motor " + motorName + ": " + e.what());
        throw; // Re-throw the exception for higher level handling
    }
}

// Function to move the motor forward
void Motor::moveForward() {
    pwm.setDutyCycle(75);  // Set PWM duty cycle to 75% for forward movement (High speed)

    // Set motor direction using GPIO pins: IN1=1, IN2=0
    std::system(("gpioset gpiochip0 " + std::to_string(in1_pin) + "=1").c_str());
    std::system(("gpioset gpiochip0 " + std::to_string(in2_pin) + "=0").c_str());
    
    // Log the action
    logger->logMotorAction(motorName, "forward");
}

// Function to move the motor backward
void Motor::moveBackward() {
    pwm.setDutyCycle(75);  // Set PWM duty cycle to 75% for backward movement (High speed)

    // Set motor direction using GPIO pins: IN1=0, IN2=1
    std::system(("gpioset gpiochip0 " + std::to_string(in1_pin) + "=0").c_str());
    std::system(("gpioset gpiochip0 " + std::to_string(in2_pin) + "=1").c_str());
    
    // Log the action
    logger->logMotorAction(motorName, "backward");
}

// Function to stop the motor
void Motor::stop() {
    pwm.setDutyCycle(0);  // Set PWM duty cycle to 0% to stop the motor (No power)
    
    // Set motor control pins to 0 (both low) to stop the motor
    std::system(("gpioset gpiochip0 " + std::to_string(in1_pin) + "=0").c_str());
    std::system(("gpioset gpiochip0 " + std::to_string(in2_pin) + "=0").c_str());
    
    // Log the action
    logger->logMotorAction(motorName, "stop");
}

// Function to adjust motor speed by changing the duty cycle
void Motor::setSpeed(int speed) {
    if (speed < 0 || speed > 100) {
        logger->log("WARNING: Invalid speed value: " + std::to_string(speed) + ". Speed must be between 0-100");
        return;
    }
    
    dutyCycle = speed;  // Update duty cycle
    pwm.setDutyCycle(dutyCycle);  // Apply the new duty cycle
    
    // Log the action
    logger->logMotorAction(motorName, "speed set to " + std::to_string(speed));
}

// Motor Destructor: Stops the motor and cleans up resources
Motor::~Motor() {
    logger->log("Shutting down motor: " + motorName);
    stop();  // Make sure the motor is stopped
    pwm.stop();  // Stop the PWM signal
    
    // Clean up GPIO chip if it was allocated
    if (chip != nullptr) {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
}

// Setup terminal for non-canonical mode (character-by-character input)
int setupTerminal(KeyboardState& state) {
    struct termios newt;
    
    // Get the current terminal attributes and save them
    if (tcgetattr(STDIN_FILENO, &state.originalTermios) < 0) {
        std::cerr << "Error: Unable to get terminal attributes" << std::endl;
        return -1;
    }
    
    // Copy the original settings and modify them
    newt = state.originalTermios;
    
    // Disable canonical mode and echo, enable non-blocking reads
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;  // Non-blocking read
    newt.c_cc[VTIME] = 0; // No timeout
    
    // Apply the new settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) < 0) {
        std::cerr << "Error: Unable to set terminal attributes" << std::endl;
        return -1;
    }
    
    // Set stdin to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    
    return 0;
}

// Restore original terminal settings
void restoreTerminal(KeyboardState& state) {
    // Set stdin back to blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    
    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &state.originalTermios);
}

// Function to listen for keyboard inputs with key release detection
void keyboardListener(KeyboardState& state) {
    auto logger = Logger::getInstance();
    logger->log("Keyboard listener started");
    
    // Setup terminal for proper input handling
    if (setupTerminal(state) < 0) {
        logger->log("ERROR: Failed to setup terminal for input");
        return;
    }
    
    char ch;
    auto lastKeyPressTime = std::chrono::steady_clock::now();
    
    try {
        while (!state.shouldExit.load()) {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                currentTime - lastKeyPressTime).count();
            
            int bytesRead = read(STDIN_FILENO, &ch, 1);  // Non-blocking read
            if (bytesRead > 0) {  // Key was pressed
                state.lastKey.store(ch);  // Store the character
                state.keyPressed.store(true);  // Signal that a key was pressed
                state.isKeyHeld.store(true);   // Mark key as held
                lastKeyPressTime = currentTime;  // Update last key press time
                
                // Log the key press
                logger->logKeyEvent(ch, "pressed");
                
                // Check for quit command
                if (ch == 'q') {
                    state.shouldExit.store(true);
                    logger->log("Quit command received");
                    break;
                }
            } else if (elapsed > 50 && state.isKeyHeld.load()) {  // No keypress for 50ms, consider key released
                state.isKeyHeld.store(false);  // Mark key as released
                
                // Log the key release
                logger->logKeyEvent(state.lastKey.load(), "released");
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Sleep to prevent CPU spinning
        }
    }
    catch (const std::exception& e) {
        logger->log("ERROR in keyboard listener: " + std::string(e.what()));
    }
    
    // Restore terminal settings before exiting
    restoreTerminal(state);
    logger->log("Keyboard listener stopped");
}

// Function to control the motors using keyboard inputs
void keyboardControl(Motor &leftMotor, Motor &rightMotor, KeyboardState& state, std::shared_ptr<Logger> logger) {
    logger->log("Motor control started");
    std::cout << "Use W/A/S/D to control the motors. Press Q to quit." << std::endl;
    
    char currentAction = '\0';  // Current active action
    
    try {
        while (!state.shouldExit.load()) {
            // Check if a key was pressed
            if (state.keyPressed.load()) {
                char key = state.lastKey.load();  // Read the last pressed key
                state.keyPressed.store(false);    // Reset key press status
                currentAction = key;              // Update current action
                
                // Quit check handled in keyboardListener
            }
            
            // If key is held, perform the action
            if (state.isKeyHeld.load() && currentAction != '\0') {
                switch (currentAction) {
                    case 'w':  // Move both motors forward
                        leftMotor.moveBackward();
                        rightMotor.moveBackward();
                        break;
                    
                    case 's':  // Move both motors backward
                        leftMotor.moveForward();
                        rightMotor.moveForward();
                        break;
                    
                    case 'a':  // Turn left
                        leftMotor.stop();
                        rightMotor.moveForward();
                        break;
                    
                    case 'd':  // Turn right
                        leftMotor.moveForward();
                        rightMotor.stop();
                        break;
                    
                    case 'x':  // Stop both motors (Emergency Stop)
                        leftMotor.stop();
                        rightMotor.stop();
                        break;
                }
            } else if (!state.isKeyHeld.load() && currentAction != '\0') {
                // No key held, stop the motors (only if we had an action before)
                leftMotor.stop();
                rightMotor.stop();
                currentAction = '\0'; // Reset current action
            }
            
            // Small sleep to prevent CPU spinning
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (const std::exception& e) {
        logger->log("ERROR in motor control: " + std::string(e.what()));
    }
    
    // Make sure motors are stopped before exiting
    leftMotor.stop();
    rightMotor.stop();
    logger->log("Motor control stopped");
}

// Function to control the motors using keyboard inputs
void keyboardEventControl(std::shared_ptr<Motor> leftMotor, std::shared_ptr<Motor> rightMotor, KeyEventInfo& keyEvent)
{  
     try {        
        // If key is held, perform the action
        if (keyEvent.eventType == KeyEventType::KEY_PRESSED) {
            if (keyEvent.keyCode == Qt::Key_W || keyEvent.keyCode == Qt::Key_Up) {  // Move both motors forward
                leftMotor->moveBackward();
                rightMotor->moveBackward();

            } else if (keyEvent.keyCode == Qt::Key_S || keyEvent.keyCode == Qt::Key_Down) {  // Move both motors backward
                leftMotor->moveForward();
                rightMotor->moveForward();

            } else if (keyEvent.keyCode == Qt::Key_A || keyEvent.keyCode == Qt::Key_Left) {  // Turn left
                leftMotor->stop();
                rightMotor->moveForward();

            } else if (keyEvent.keyCode == Qt::Key_D || keyEvent.keyCode == Qt::Key_Right) {  // Turn right
                leftMotor->moveForward();
                rightMotor->stop();

            } else if (keyEvent.keyCode == Qt::Key_X) {  // Stop both motors (Emergency Stop)
                leftMotor->stop();
                rightMotor->stop();
            }
            
        } else if (keyEvent.eventType == KeyEventType::KEY_RELEASED) {
            // No key held, stop the motors (only if we had an action before)
            leftMotor->stop();
            rightMotor->stop();
        }
    }
    catch (const std::exception& e) {
        throw("ERROR in motor control: " + std::string(e.what()));
    }
    
    // Make sure motors are stopped before exiting
    leftMotor->stop();
    rightMotor->stop();
}
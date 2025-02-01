#include <iostream>
#include <pigpio.h>
#include <unistd.h> // For sleep functions

// Define motor control pins
#define ENA 12 // PWM pin for Motor A speed
#define IN1 5  // Motor A direction pin 1
#define IN2 6  // Motor A direction pin 2

#define ENB 18 // PWM pin for Motor B speed
#define IN3 13 // Motor B direction pin 1
#define IN4 19 // Motor B direction pin 2

// Function to initialize motor control pins
void setupMotors()
{
    if (gpioInitialise() < 0)
    {
        std::cerr << "Failed to initialize GPIO!" << std::endl;
        exit(1);
    }

    // Set direction pins as OUTPUT
    gpioSetMode(IN1, PI_OUTPUT);
    gpioSetMode(IN2, PI_OUTPUT);
    gpioSetMode(IN3, PI_OUTPUT);
    gpioSetMode(IN4, PI_OUTPUT);

    // Set PWM pins as OUTPUT with frequency
    gpioSetMode(ENA, PI_OUTPUT);
    gpioSetMode(ENB, PI_OUTPUT);
    gpioSetPWMfrequency(ENA, 1000); // 1kHz PWM frequency
    gpioSetPWMfrequency(ENB, 1000);
}

// Function to move Motor A forward
void motorA_Forward(int speed)
{
    gpioWrite(IN1, 1);
    gpioWrite(IN2, 0);
    gpioPWM(ENA, speed);
}

// Function to move Motor A backward
void motorA_Backward(int speed)
{
    gpioWrite(IN1, 0);
    gpioWrite(IN2, 1);
    gpioPWM(ENA, speed);
}

// Function to stop Motor A
void motorA_Stop()
{
    gpioWrite(IN1, 0);
    gpioWrite(IN2, 0);
    gpioPWM(ENA, 0);
}

// Function to move Motor B forward
void motorB_Forward(int speed)
{
    gpioWrite(IN3, 1);
    gpioWrite(IN4, 0);
    gpioPWM(ENB, speed);
}

// Function to move Motor B backward
void motorB_Backward(int speed)
{
    gpioWrite(IN3, 0);
    gpioWrite(IN4, 1);
    gpioPWM(ENB, speed);
}

// Function to stop Motor B
void motorB_Stop()
{
    gpioWrite(IN3, 0);
    gpioWrite(IN4, 0);
    gpioPWM(ENB, 0);
}

// Main function
int main()
{
    setupMotors();

    std::cout << "Moving forward..." << std::endl;
    motorA_Forward(150); // Move Motor A at 60% speed
    motorB_Forward(150); // Move Motor B at 60% speed
    sleep(2);

    std::cout << "Stopping..." << std::endl;
    motorA_Stop();
    motorB_Stop();
    sleep(1);

    std::cout << "Moving backward..." << std::endl;
    motorA_Backward(100);
    motorB_Backward(100);
    sleep(2);

    std::cout << "Stopping motors." << std::endl;
    motorA_Stop();
    motorB_Stop();

    gpioTerminate(); // Clean up GPIO
    return 0;
}
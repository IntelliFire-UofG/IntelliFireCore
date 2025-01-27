#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <thread>
#include <chrono>

// Define GPIO pins for L298N Motor Driver
#define ENA 0  // Enable pin for Motor A (GPIO17)
#define IN1 1  // IN1 pin for Motor A (GPIO18)
#define IN2 2  // IN2 pin for Motor A (GPIO27)
#define ENB 3  // Enable pin for Motor B (GPIO22)
#define IN3 4  // IN3 pin for Motor B (GPIO23)
#define IN4 5  // IN4 pin for Motor B (GPIO24)

// Define GPIO pins for buttons
#define BUTTON_FORWARD 6  // Button for forward movement (GPIO25)
#define BUTTON_BACKWARD 7 // Button for backward movement (GPIO4)
#define BUTTON_STOP 8     // Button for stopping motors (GPIO14)

/**
 * @brief Initialize GPIO pins for L298N motor driver and buttons.
 */
void setupGPIO() {
    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        std::cerr << "Failed to initialize WiringPi" << std::endl;
        exit(1);
    }

    // Set motor control pins
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Enable software PWM on ENA and ENB
    softPwmCreate(ENA, 0, 100); // Range 0-100 for speed control
    softPwmCreate(ENB, 0, 100); // Range 0-100 for speed control

    // Set button pins
    pinMode(BUTTON_FORWARD, INPUT);
    pinMode(BUTTON_BACKWARD, INPUT);
    pinMode(BUTTON_STOP, INPUT);

    // Enable pull-up resistors for buttons
    pullUpDnControl(BUTTON_FORWARD, PUD_UP);
    pullUpDnControl(BUTTON_BACKWARD, PUD_UP);
    pullUpDnControl(BUTTON_STOP, PUD_UP);
}

/**
 * @brief Control both motors' direction and speed.
 *
 * @param direction "forward", "backward", or "stop"
 * @param speed Value between 0 and 100
 */
void controlMotors(const std::string& direction, int speed) {
    if (direction == "forward") {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    } else if (direction == "backward") {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    }

    softPwmWrite(ENA, speed);
    softPwmWrite(ENB, speed);
}

/**
 * @brief ISR for the forward button.
 */
void forwardISR() {
    std::cout << "Forward button pressed. Moving forward..." << std::endl;
    controlMotors("forward", 75); // Move forward at 75% speed
}

/**
 * @brief ISR for the backward button.
 */
void backwardISR() {
    std::cout << "Backward button pressed. Moving backward..." << std::endl;
    controlMotors("backward", 75); // Move backward at 75% speed
}

/**
 * @brief ISR for the stop button.
 */
void stopISR() {
    std::cout << "Stop button pressed. Stopping motors..." << std::endl;
    controlMotors("stop", 0); // Stop motors
}

int main() {
    setupGPIO();

    // Attach interrupt handlers to buttons
    if (wiringPiISR(BUTTON_FORWARD, INT_EDGE_FALLING, &forwardISR) < 0) {
        std::cerr << "Failed to set up interrupt for forward button" << std::endl;
        return 1;
    }

    if (wiringPiISR(BUTTON_BACKWARD, INT_EDGE_FALLING, &backwardISR) < 0) {
        std::cerr << "Failed to set up interrupt for backward button" << std::endl;
        return 1;
    }

    if (wiringPiISR(BUTTON_STOP, INT_EDGE_FALLING, &stopISR) < 0) {
        std::cerr << "Failed to set up interrupt for stop button" << std::endl;
        return 1;
    }

    std::cout << "Event-driven motor control system is running. Press buttons to control motors." << std::endl;

    // Keep the program running to listen for events
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}


/**
 * @file main.cpp
 * @brief Autonomous Fire Truck Control System - Entry Point
 *
 * This embedded real-time system runs on a Raspberry Pi and controls an autonomous 
 * fire truck equipped with four motors (L298N driver) and a flame detection system 
 * (LM393 sensors via ADS1015/ADS1115 ADC). The system responds to fire events and 
 * user inputs in real time, using an event-driven architecture.
 *
 * 🔥 Features:
 * - Real-time event-driven execution using threads & callbacks.
 * - Motor control for autonomous movement (forward, backward, stop).
 * - Flame detection using LM393 sensors & ADC.
 * - Graphical display for real-time sensor readings.
 * - Interrupt-based button control for manual intervention.
 *
 * 🚀 Execution Flow:
 * 1️⃣ System Initialization: Setup motors, sensors, buttons, display.
 * 2️⃣ Register Callbacks: Bind sensor & button events to motor control.
 * 3️⃣ Start Event Loop: Monitors inputs & executes actions in real-time.
 * 4️⃣ Fire Detection: Stops movement when fire is detected & logs data.
 * 5️⃣ User Interaction: Allows manual movement control via buttons. // For further development
 *
 * 📂 File Structure:
 * - MotorController.hpp: Manages motor movements.
 * - EventHandler.hpp: Handles button interrupts.
 * - ADCReader.hpp: Reads sensor data via ADC.
 * - DisplayManager.hpp: Displays real-time sensor values.
 * - EventLoop.hpp: Manages event-driven execution.
 *
 * 🛠️ Development:
 * - Coding Language: C++
 * - OS: Linux (Raspberry Pi)
 * - Paradigm: Object-Oriented Programming (OOP)
 * - Event Handling: Threads, Callbacks, Interrupts
 * - Testing: Unit tests included in 'tests/' directory.
 *
 * @author Misael Rivera
 * @date 2025-02-01
 */

#include "../include/LN298MotorControl.h"
#include "../include/eventHandler.h"
#include "../include/ADCReader.h" // Consider for further dev
// #include "DisplayManager.hpp" // Consider for further dev
#include "../include/eventLoop.h" // Consider for further dev

#include <iostream>
#include <thread>
#include <chrono>
#include <gpiod.h>
#include <termios.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <atomic>
#include <functional>
#include <signal.h>

# define CHIP_NAME "gpiochip0"

/*

CODE REVIEW -> 19/February/2025

Set a better naming convention
Update branches

*/

// TODO: Define the pins
#define ENA 13
#define IN1 22
#define IN2 23
#define ENB 12
#define IN3 17
#define IN4 27

#define INACTIVITY_TIMEOUT_MS

// #define BUTTON_FORWARD 6
// #define BUTTON_BACKWARD 7
// #define BUTTON_LEFT 8
// #define BUTTON_RIGHT 9
// #define BUTTON_STOP 10
int main() {
    std::cout << "🔥 Autonomous Fire Truck System Initializing... 🔥" << std::endl;
    struct sched_param SchedParam;
    SchedParam.sched_priority = 80;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &schedParam) != 0) {
        perror("Failed to set real-time scheduling");
    }

    // Save current terminal settings and switch to non-canonical mode with no echo
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);


    // Set STDIN to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Create an epoll instance to watch for keyboard input events
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return 1;
    }
    
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        perror("epoll_ctl: STDIN_FILENO");
        return 1;
    }

    // Variables for main control loop
    bool running = true;
    struct epoll_event events[10];

    // setup inactivity tracking
    auto lastKeyTime = std::chrono::steady_clock::now()
    const int inactivityThresholdMs = INACTIVITY_TIMEOUT_MS;

    std::cout << "Real-Time Interrupt-Based Keyboard Control Mode:" << std::endl;
    std::cout << "Press WSAD to move, Q to quit." << std::endl;

    // Initialize MotorRight and MotorLeft controller
    // MotorController motorController(ENA, IN1, IN2, ENB, IN3, IN4);  // Example GPIO pins
    MotorController motorRight(ENA, IN1, IN2);
    MotorController motorLeft(ENB, IN3, IN4);
    
    // Create basicMotion objects with pointers and set speed
    BasicMotion motion(&motorRight, &motorLeft);
    motion.setSpeed(100);


    motorRight.registerCallback([](){
        std::cout << "Right motor state changed" << std::endl;
    }
    );

    motorLeft.registerCallback([]() {
        std::cout << "Left motor state changed" << std::endl;
    });

    std::cout << "System initialized. Ready for keyboard input!" << std::endl;
    while (running) {
        // Wait up to 100 ms for a keyboard event
        int nfds = epoll_wait(epoll_fd, events, 10, 100);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }
        
        if (nfds == 0) {
            // No key event within the timeout; if inactivity exceeds threshold, stop the truck
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastKeyTime).count() >= inactivityThresholdMs) {
                motion.stop();
            }
            continue;
        }
    
    // Process keyboard events
    for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == STDIN_FILENO) {
                char key;
                ssize_t count = read(STDIN_FILENO, &key, 1);
                if (count > 0) {
                    // Update last key press time
                    lastKeyTime = std::chrono::steady_clock::now();

                    // Process Key commands
                    switch (key) {
                        case 'w':
                        case 'W':
                            std::cout << "Moving forward" << std::endl;
                            motion.forward();
                            break;
                        case 's':
                        case 'S':
                            std::cout << "Moving backward" << std::endl;
                            motion.backward();
                            break;
                        case 'a':
                        case 'A':
                            std::cout << "Turning left" << std::endl;
                            motion.turnLeft();
                            break;
                        case 'd':
                        case 'D':
                            std::cout << "Turning right" << std::endl;
                            motion.turnRight();
                            break;
                        case 'q':
                        case 'Q':
                            std::cout << "Exiting control mode" << std::endl;
                            running = false;
                            break;
                        case ' ':
                            std::cout << "Emergency stop" << std::endl;
                            motion.stop();
                            break;
                        default:
                            // Ignore any other keys
                            break;
                    }
                }
            }
        }
    }
    
    motion.stop()
    // Clean up: close epoll and restore original terminal settings
    close(epoll_fd);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    return 0;
}
//     // Initialize event handler
//     EventHandler eventHandler(BUTTON_FORWARD, BUTTON_BACKWARD, BUTTON_STOP);  // Button GPIO pins

//     // Initialize ADC Reader for flame sensors
//     ADCReader adcReader(0x48);  // I2C address for ADS1015/ADS1115 

//     /*

//     This is considered for further use
//     // Initialize Display Manager for real-time visualization
//     // DisplayManager displayManager;
//     */

//     // Register event-driven callbacks
//     eventHandler.registerForwardCallback([&]() { motorController.moveForward(75); });
//     eventHandler.registerBackwardCallback([&]() { motorController.moveBackward(75); });
//     eventHandler.registerStopCallback([&]() { motorController.stop(); });

//     // Register ADC callback to react to fire detection
//     adcReader.registerFlameCallback([&](int channel, int value) {
//         std::cout << "🔥 Flame detected on sensor " << channel << " with value: " << value << std::endl;
//         motorController.stop();  // Example response: Stop if flame is detected
//     });

//     // Start Event Loop (Runs in Separate Thread)
//     EventLoop eventLoop;
//     std::thread eventThread(&EventLoop::run, &eventLoop);

//     std::cout << "✅ System Running..." << std::endl;
//     eventThread.join();  // Keep main thread alive

//     return 0;
// }

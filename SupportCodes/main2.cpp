/**
 * @file main.cpp
 * @brief Fire Truck Control System with Keyboard Input
 *
 * This system handles keyboard input from a Raspberry Pi to control
 * the fire truck movement using WASD keys. It uses callbacks for event-driven
 * motor control with a focus on real-time response.
 *
 * @author Misael Rivera, Modified by [Your Name]
 * @date 2025-02-26
 */

#include "../include/LN298MotorControl.h"
#include "../include/basicMotion.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <atomic>
#include <functional>
#include <signal.h>

// Motor controller pins
#define ENA 13  // Right motor enable
#define IN1 22  // Right motor direction 1
#define IN2 23  // Right motor direction 2
#define ENB 12  // Left motor enable
#define IN3 17  // Left motor direction 1
#define IN4 27  // Left motor direction 2

// Control parameters
#define DEFAULT_SPEED 75  // Default motor speed (0-100)
#define INACTIVITY_TIMEOUT_MS 2000  // Stop motors after 2 seconds of inactivity

// Global variable for program control
std::atomic<bool> running(true);

// Callback function type
using MotionCallback = std::function<void()>;

// For handling Ctrl+C to safely exit the program
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nReceived interrupt signal. Shutting down safely..." << std::endl;
        running = false;
    }
}

int main() {
    std::cout << "🔥 Fire Truck Control System Initializing... 🔥" << std::endl;
    
    // Register signal handler for Ctrl+C
    signal(SIGINT, signalHandler);
    
    // Set up real-time scheduling priority for responsive control
    struct sched_param schedParam;
    schedParam.sched_priority = 80;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &schedParam) != 0) {
        perror("Failed to set real-time scheduling");
        // Continue anyway, just with reduced responsiveness
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
    struct epoll_event events[10];
    
    // Setup inactivity tracking
    auto lastKeyTime = std::chrono::steady_clock::now();
    const int inactivityThresholdMs = INACTIVITY_TIMEOUT_MS;

    std::cout << "Keyboard Control Mode:" << std::endl;
    std::cout << "- W: Move forward" << std::endl;
    std::cout << "- S: Move backward" << std::endl;
    std::cout << "- A: Turn left" << std::endl;
    std::cout << "- D: Turn right" << std::endl;
    std::cout << "- Space: Emergency stop" << std::endl;
    std::cout << "- Q: Quit program" << std::endl;

    // Initialize motor controllers
    MotorController motorRight(ENA, IN1, IN2);
    MotorController motorLeft(ENB, IN3, IN4);
    
    // Create BasicMotion object with pointers and set speed
    BasicMotion motion(&motorRight, &motorLeft);
    motion.setSpeed(DEFAULT_SPEED);

    // Define motion callbacks for keyboard events
    MotionCallback forwardCallback = [&motion]() {
        std::cout << "Moving forward" << std::endl;
        motion.forward();
    };
    
    MotionCallback backwardCallback = [&motion]() {
        std::cout << "Moving backward" << std::endl;
        motion.backward();
    };
    
    MotionCallback leftCallback = [&motion]() {
        std::cout << "Turning left" << std::endl;
        motion.turnLeft();
    };
    
    MotionCallback rightCallback = [&motion]() {
        std::cout << "Turning right" << std::endl;
        motion.turnRight();
    };
    
    MotionCallback stopCallback = [&motion]() {
        std::cout << "Stopping" << std::endl;
        motion.stop();
    };

    // Set up motor event callbacks to monitor motor state changes
    motorRight.registerCallback([]() {
        std::cout << "Right motor state changed" << std::endl;
    });
    
    motorLeft.registerCallback([]() {
        std::cout << "Left motor state changed" << std::endl;
    });

    std::cout << "System initialized. Ready for keyboard input!" << std::endl;

    // Main control loop
    while (running) {
        // Wait up to 100 ms for a keyboard event
        int nfds = epoll_wait(epoll_fd, events, 10, 100);
        if (nfds == -1) {
            if (errno == EINTR) {
                // Interrupted by signal, likely our SIGINT handler
                continue;
            }
            perror("epoll_wait");
            break;
        }
        
        // Check for inactivity timeout
        if (nfds == 0) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastKeyTime).count() >= inactivityThresholdMs) {
                // Safety feature: Stop motors after inactivity period
                stopCallback();
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
                    
                    // Process key commands using callbacks
                    switch (key) {
                        case 'w':
                        case 'W':
                            forwardCallback();
                            break;
                        case 's':
                        case 'S':
                            backwardCallback();
                            break;
                        case 'a':
                        case 'A':
                            leftCallback();
                            break;
                        case 'd':
                        case 'D':
                            rightCallback();
                            break;
                        case ' ':
                            stopCallback();
                            break;
                        case 'q':
                        case 'Q':
                            std::cout << "Exiting control mode" << std::endl;
                            running = false;
                            break;
                        default:
                            // Ignore any other keys
                            break;
                    }
                }
            }
        }
    }
    
    // Clean up
    std::cout << "Shutting down..." << std::endl;
    
    // Stop motors before exiting
    stopCallback();
    
    // Close epoll and restore original terminal settings
    close(epoll_fd);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    std::cout << "System shutdown complete." << std::endl;
    return 0;
}
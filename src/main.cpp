#include "../include/LN298MotorControlV3.h"
#include "../include/basicMotionV2.h"
#include "../include/eventHandler.h"
#include "../include/ADCReader.h"
#include "../include/eventLoop.h"

#include <iostream>
#include <thread>
#include <libgpio.h>
#include <atomic>
#include <memory>
#include <csignal>

std::atomic<bool> keepRunning(true);

// Signal handler for graceful exit
void signalHandler(int) {
    keepRunning.store(false);
}

int main() {
    std::cout << "🔥 Autonomous Fire Truck System Initializing..." << std::endl;

    // Catch termination signals
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    // Run basicMotion in its own thread
    std::atomic<bool> motionRunning(true);
    std::thread motionThread([&motionRunning]() {
        try {
            basicMotion();
        } catch (const std::exception& e) {
            std::cerr << "❌ basicMotion error: " << e.what() << std::endl;
        }
        motionRunning.store(false);
    });

    // ADC Reader (handles flame detection)
    std::unique_ptr<ADCReader> adcReader;
    try {
        adcReader = std::make_unique<ADCReader>(0x48); // I2C address
        adcReader->registerFlameCallback([](int channel, int value) {
            std::cout << "🔥 Flame detected on sensor " << channel << " with value: " << value << std::endl;
            stopMotors();
        });
    } catch (...) {
        std::cerr << "⚠️ Failed to initialize ADCReader." << std::endl;
        motionRunning.store(false);
    }

    // Event Loop runs in background
    EventLoop eventLoop;
    std::thread eventThread([&]() {
        try {
            eventLoop.run();
        } catch (const std::exception& e) {
            std::cerr << "❌ EventLoop error: " << e.what() << std::endl;
            keepRunning.store(false);
        }
    });

    std::cout << "✅ System Running..." << std::endl;

    // Main thread waits until either signal received or motion completes
    while (keepRunning.load() && motionRunning.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Clean shutdown
    std::cout << "🛑 Shutting down system..." << std::endl;

    if (motionThread.joinable()) {
        motionThread.join();
    }

    eventLoop.stop();

    if (eventThread.joinable()) {
        eventThread.join();
    }

    std::cout << "✅ System stopped gracefully.\n";
    return 0;
}

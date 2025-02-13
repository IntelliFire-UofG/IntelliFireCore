#include <iostream>
#include <csignal>
#include "ir_sensor.h"

void sensorEventHandler(const gpiod_line_event* event) {
    if (event->event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
        std::cout << "Obstacle detected!" << std::endl;
    } else if (event->event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
        std::cout << "Obstacle removed!" << std::endl;
    }
}

int main() {
    const char* gpioChip = "/dev/gpiochip0";
    int sensorPin = 12;

    IRSensor sensor(gpioChip, sensorPin);
    
    // Register our event callback.
    sensor.registerCallback(sensorEventHandler);

    if (!sensor.start()) {
        std::cerr << "Failed to start IR sensor event loop." << std::endl;
        return -1;
    }

    std::cout << "IR Sensor initialized. Monitoring for obstacles..." << std::endl;
    std::cout << "Press Enter to stop." << std::endl;
    std::cin.get();

    sensor.stop();
    std::cout << "IR Sensor stopped. Exiting..." << std::endl;
    return 0;
}

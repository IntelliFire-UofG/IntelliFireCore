#include <iostream>
#include "IRSensor.h"

// This class implements the IRSensor callback interface.
class SensorEventPrinter : public IRSensor::IRSensorCallbackInterface {
public:
    virtual void hasEvent(gpiod_line_event& e) override {
        if (e.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
            std::cout << "Obstacle detected!" << std::endl;
        } else if (e.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
            std::cout << "Obstacle removed!" << std::endl;
        } else {
            std::cout << "Unknown event!" << std::endl;
        }
    }
};

int main() {
    const char* gpioChip = "/dev/gpiochip0";
    int sensorPin = 12;
    
    std::cout << "IR Sensor initialized. Monitoring for obstacles..." << std::endl;
    std::cout << "Press Enter to stop." << std::endl;
    
    SensorEventPrinter callbackInterface;
    IRSensor sensor;
    sensor.registerCallback(&callbackInterface);
    
    try {
        sensor.start(gpioChip, sensorPin);
    } catch (const char* error) {
        std::cerr << "Error starting IRSensor: " << error << std::endl;
        return -1;
    }
    
    std::cin.get();
    sensor.stop();
    
    std::cout << "IR Sensor stopped. Exiting..." << std::endl;
    return 0;
}

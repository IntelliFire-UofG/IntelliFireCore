#include <iostream>
#include "UltraSonicSensor.h"

class SensorEventPrinter : public UltraSonicSensor::UltraSonicSensorCallbackInterface {
public:
    virtual void onDistanceMeasured(float distance) override {
        std::cout << "Measured Distance: " << distance << " cm" << std::endl;
    }
};

int main() {
    const char* gpioChip = "/dev/gpiochip0";
    int triggerPin = 23;
    int echoPin = 24;

    std::cout << "UltraSonic Sensor initialized. Measuring distance..." << std::endl;
    std::cout << "Press Enter to stop." << std::endl;

    SensorEventPrinter callbackInterface;
    UltraSonicSensor sensor;
    sensor.registerCallback(&callbackInterface);

    try {
        sensor.start(gpioChip, triggerPin, echoPin);
    } catch (const char* error) {
        std::cerr << "Error starting UltraSonicSensor: " << error << std::endl;
        return -1;
    }

    std::cin.get();
    sensor.stop();

    std::cout << "UltraSonic Sensor stopped. Exiting..." << std::endl;
    return 0;
}

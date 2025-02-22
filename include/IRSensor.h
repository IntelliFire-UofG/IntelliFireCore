#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

#include <gpiod.h>
#include <thread>
#include <vector>
#include <stdio.h>

// Enable debug messages if not defined otherwise.
#ifndef NDEBUG
#define DEBUG
#endif

#define ISR_TIMEOUT 1 // sec

class IRSensor {
public:
    /**
     * Destructor that ensures the sensor stops on exit.
     **/
    ~IRSensor() {
        stop();
    }

    // Callback interface similar to GPIOPin.
    struct IRSensorCallbackInterface {
        /**
         * Called when a new event is available.
         * Must be implemented by the client.
         * \param e The event (rising or falling edge).
         **/
        virtual void hasEvent(gpiod_line_event& e) = 0;
    };

    /**
     * Registers a callback interface.
     **/
    void registerCallback(IRSensorCallbackInterface* ci) {
        callbacks.push_back(ci);
    }

    /**
     * Starts listening on the IR sensor.
     * \param chipPath Path to the GPIO chip.
     * \param pin Pin number for the sensor.
     **/
    void start(const char* chipPath, int pin);

    /**
     * Stops listening to the sensor.
     **/
    void stop();

private:
    // Notifies all registered callbacks of a new event.
    void irEvent(gpiod_line_event& event);

    // Worker thread function.
    void worker();

    // gpiod objects.
    gpiod_chip* chip = nullptr;
    gpiod_line* sensor_line = nullptr;

    // Thread.
    std::thread thr;

    // Running flag.
    bool running = false;

    // Vector of callback interfaces.
    std::vector<IRSensorCallbackInterface*> callbacks;
};

#endif // __IR_SENSOR_H__

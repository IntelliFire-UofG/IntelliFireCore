#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

#include <gpiod.h>
#include <thread>
#include <vector>
#include <functional>

// Define a callback type that accepts a pointer to a gpiod_line_event.
using IRSensorCallback = std::function<void(const gpiod_line_event* event)>;

class IRSensor {
public:
    // Constructor now only needs the chip path and pin number.
    IRSensor(const char* chipPath, int pin);
    ~IRSensor();

    // Start and stop the event loop.
    bool start();
    void stop();

    // Register a callback. Multiple callbacks can be added.
    void registerCallback(IRSensorCallback cb);

private:
    // Main event loop function.
    void eventLoop();

    const char* chipPath_;
    int pin_;
    gpiod_chip* chip_;
    gpiod_line* sensor_line_;
    bool running_;
    std::thread eventThread_;

    // A vector to hold all registered callbacks.
    std::vector<IRSensorCallback> callbacks_;
};

#endif // __IR_SENSOR_H__

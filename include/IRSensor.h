#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

#include <gpiod.h>
#include <thread>
#include <vector>
#include <stdio.h>
#include <atomic>
#include <mutex>

// Enable debug messages if not defined otherwise.
#ifndef NDEBUG
#define DEBUG
#endif

#ifndef ISR_TIMEOUT
#define ISR_TIMEOUT 1 
#endif

class IRSensor {
public:
    IRSensor() = default;

    ~IRSensor() {
        stop();
    }

    IRSensor(const IRSensor&) = delete;
    IRSensor& operator=(const IRSensor&) = delete;
    IRSensor(IRSensor&&) = delete;
    IRSensor& operator=(IRSensor&&) = delete;

    // Callback interface similar to GPIOPin.
    struct IRSensorCallbackInterface {
        virtual void hasEvent(gpiod_line_event& e) = 0;
	virtual ~IRSensorCallbackInterface() = default;
    };

    void registerCallback(IRSensorCallbackInterface* ci) {
        std::lock_guard<std::mutex> lock(cb_mutex);
        callbacks.push_back(ci);
    }

    void start(const char* chipPath, int pin);
    void stop();

private:
    void irEvent(gpiod_line_event& event);
    void worker();

    gpiod_chip* chip = nullptr;
    gpiod_line* sensor_line = nullptr;

    std::thread thr;
    std::atomic<bool> running{false};

    std::vector<IRSensorCallbackInterface*> callbacks;
    std::mutex cb_mutex;
};

#endif // __IR_SENSOR_H__

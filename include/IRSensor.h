#ifndef __IR_SENSOR_H__
#define __IR_SENSOR_H__

#include <gpiod.h>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <stdio.h>

#ifndef NDEBUG
#define DEBUG
#endif

#define ISR_TIMEOUT 1 // sec

class IRSensor {
public:
    IRSensor() = default;
    ~IRSensor();  // destructor will handle cleanup

    struct IRSensorCallbackInterface {
        virtual void hasEvent(gpiod_line_event& e) = 0;
        virtual ~IRSensorCallbackInterface() = default;
    };

    void registerCallback(IRSensorCallbackInterface* ci);

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
    std::mutex callback_mutex;
};

#endif // __IR_SENSOR_H__

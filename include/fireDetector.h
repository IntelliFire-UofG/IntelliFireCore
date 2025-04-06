#ifndef __FIREDETECTOR_H
#define __FIREDETECTOR_H

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <gpiod.h>

#ifndef NDEBUG
#define DEBUG
#endif

class FireDetector {
public:
    FireDetector();
    ~FireDetector();

    struct FireDetectorCallbackInterface {
        virtual void fireDetected(unsigned int sensor_id, int event_type) = 0;
        virtual ~FireDetectorCallbackInterface() = default;
    };

    void registerCallback(FireDetectorCallbackInterface* ci);
    void start(unsigned int id, unsigned int chip, unsigned int line);
    void stop();

    enum EventType {
        RISING_EDGE = 0,
        FALLING_EDGE
    };

private:
    void worker();

    std::vector<FireDetectorCallbackInterface*> fire_callback_interfaces;
    std::mutex callback_mutex;

    gpiod_chip* gpio_chip = nullptr;
    gpiod_line* gpio_line = nullptr;

    std::thread thr;
    std::atomic<bool> running{false};

    unsigned int id = 0;
};

#endif // __FIREDETECTOR_H

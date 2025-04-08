#ifndef __FIREDETECTOR_H
#define __FIREDETECTOR_H

#include <iostream>
#include <thread>
#include <gpiod.h>
#include <vector>
#include <atomic>
#include <mutex>

// enable debug messages and error messages to stderr
#ifndef NDEBUG
#define DEBUG
#endif

class FireDetector {
public:
    /**
     * @brief Constructor for FireDetector
     */
    FireDetector();

    // Rule of Five
    ~FireDetector() {
        stop();
    }

    FireDetector(const FireDetector&) = delete;
    FireDetector& operator=(const FireDetector&) = delete;
    FireDetector(FireDetector&&) = delete;
    FireDetector& operator=(FireDetector&&) = delete;

    struct FireDetectorCallbackInterface {
        /**
         * Called when a new sample is available.
         * This needs to be implemented in a derived
         * class by the client. Defined as abstract.
         * \param sample Voltage from the selected channel.
         **/
        virtual void fireDetected(unsigned int sensor_id, int event_type) = 0;
    };

    void registerCallback(FireDetectorCallbackInterface* ci) {
        std::lock_guard<std::mutex> lock(callback_mutex);
        fire_callback_interfaces.push_back(ci);
    }

    void start(unsigned int id, unsigned int chip, unsigned int line);
    void stop();
    void worker();

    enum EventType {
        RISING_EDGE = 0,
        FALLING_EDGE
    };

private:
    std::vector<FireDetectorCallbackInterface*> fire_callback_interfaces;
    std::mutex callback_mutex;

    struct gpiod_chip* gpio_chip = nullptr;
    struct gpiod_line* gpio_line = nullptr;

    std::thread thr;
    std::atomic<bool> running{false};
    unsigned int id = 0;
};

#endif // __FIREDETECTOR_H

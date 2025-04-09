#ifndef __FIREDETECTOR_H
#define __FIREDETECTOR_H

#include <iostream>
#include <thread>
#include <gpiod.h>
#include <vector>

#define GPIO_CHIP    4
#define SENSOR_PIN_0 9
#define SENSOR_PIN_1 10
#define PUMP_PIN     11

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
        fire_callback_interfaces.push_back(ci);
    }

    void start(unsigned int id, unsigned int chip, unsigned int line);

    void stop();

    void worker();

    enum EventType{
        RISING_EDGE = 0,
        FALLING_EDGE
    };

private:
    std::vector<FireDetectorCallbackInterface*> fire_callback_interfaces;
    struct gpiod_chip *gpio_chip = nullptr;
    struct gpiod_line *gpio_line =  nullptr;

    std::thread thr;
    bool running = false;
    unsigned int id;
};

#endif // __FIREDETECTOR_H
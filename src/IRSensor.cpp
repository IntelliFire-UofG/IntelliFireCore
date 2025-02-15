#include "../include/IRSensor.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

// This start() method mirrors the GPIOPin::start() method.
// It opens the chip and line using the given chip path and pin number.
void IRSensor::start(const char* chipPath, int pin) {
#ifdef DEBUG
    fprintf(stderr, "Init.\n");
#endif

    chip = gpiod_chip_open(chipPath);
    if (NULL == chip) {
#ifdef DEBUG
        fprintf(stderr, "GPIO chip could not be accessed.\n");
#endif
        throw "GPIO chip error.";
    }
    
    sensor_line = gpiod_chip_get_line(chip, pin);
    if (NULL == sensor_line) {
#ifdef DEBUG
        fprintf(stderr, "GPIO line could not be accessed.\n");
#endif
        gpiod_chip_close(chip);
        chip = NULL;
        throw "GPIO line error.";
    }
    
    int ret = gpiod_line_request_both_edges_events(sensor_line, "Consumer");
    if (ret < 0) {
#ifdef DEBUG
        fprintf(stderr, "Request event notification failed on pin %d.\n", pin);
#endif
        gpiod_chip_close(chip);
        chip = NULL;
        sensor_line = NULL;
        throw "Could not request event for IRQ.";
    }
    
    running = true;
    thr = std::thread(&IRSensor::worker, this);
}

// This method is analogous to GPIOPin::gpioEvent().
// It notifies all registered callbacks of the new event.
void IRSensor::irEvent(gpiod_line_event& event) {
    for (auto &cb : callbacks) {
        cb->hasEvent(event);
    }
}

// The worker() method follows the same pattern as in GPIOPin.
// It waits (with a timeout) for an event on the line, reads it,
// and then passes it on via irEvent().
void IRSensor::worker() {
    while (running) {
        const timespec ts = { ISR_TIMEOUT, 0 };
        int r = gpiod_line_event_wait(sensor_line, &ts);
        if (1 == r) {
            gpiod_line_event event;
            gpiod_line_event_read(sensor_line, &event);
            irEvent(event);
        } else if (r < 0) {
#ifdef DEBUG
            fprintf(stderr, "GPIO error while waiting for event.\n");
#endif
        }
    }
}

// Finally, stop() stops the worker thread and releases the resources.
void IRSensor::stop() {
    if (!running) return;
    running = false;
    thr.join();
    gpiod_line_release(sensor_line);
    gpiod_chip_close(chip);
}

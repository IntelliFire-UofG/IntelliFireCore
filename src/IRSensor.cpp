#include "../include/IRSensor.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

IRSensor::~IRSensor() {
    stop();
}

void IRSensor::registerCallback(IRSensorCallbackInterface* ci) {
    std::lock_guard<std::mutex> lock(callback_mutex);
    callbacks.push_back(ci);
}

void IRSensor::start(const char* chipPath, int pin) {
#ifdef DEBUG
    fprintf(stderr, "IRSensor: Initializing on pin %d\n", pin);
#endif

    stop(); // Ensure any previous thread is shut down

    chip = gpiod_chip_open(chipPath);
    if (!chip) {
#ifdef DEBUG
        fprintf(stderr, "IRSensor: Failed to open GPIO chip %s\n", chipPath);
#endif
        throw "IRSensor: Could not open GPIO chip.";
    }

    sensor_line = gpiod_chip_get_line(chip, pin);
    if (!sensor_line) {
        gpiod_chip_close(chip);
        chip = nullptr;
#ifdef DEBUG
        fprintf(stderr, "IRSensor: Failed to get GPIO line %d\n", pin);
#endif
        throw "IRSensor: Could not get GPIO line.";
    }

    if (gpiod_line_request_both_edges_events(sensor_line, "IRSensor") < 0) {
        gpiod_chip_close(chip);
        chip = nullptr;
        sensor_line = nullptr;
#ifdef DEBUG
        fprintf(stderr, "IRSensor: Failed to request both edge events on pin %d\n", pin);
#endif
        throw "IRSensor: Could not request events.";
    }

    running = true;
    thr = std::thread(&IRSensor::worker, this);
}

void IRSensor::stop() {
    if (!running.exchange(false)) return;

    if (thr.joinable()) {
        thr.join();
    }

    if (sensor_line) {
        gpiod_line_release(sensor_line);
        sensor_line = nullptr;
    }

    if (chip) {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
}

void IRSensor::irEvent(gpiod_line_event& event) {
    std::lock_guard<std::mutex> lock(callback_mutex);
    for (auto& cb : callbacks) {
        if (cb) {
            cb->hasEvent(event);
        }
    }
}

void IRSensor::worker() {
    while (running) {
        const struct timespec ts = { ISR_TIMEOUT, 0 };
        int wait_result = gpiod_line_event_wait(sensor_line, &ts);
        if (wait_result == 1) {
            gpiod_line_event event;
            if (gpiod_line_event_read(sensor_line, &event) == 0) {
                irEvent(event);
            }
        } else if (wait_result < 0) {
#ifdef DEBUG
            fprintf(stderr, "IRSensor: Error waiting for GPIO event.\n");
#endif
        }
    }
}

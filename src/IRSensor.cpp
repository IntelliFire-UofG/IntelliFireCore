#include "../include/IRSensor.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdexcept>
#include <QDebug>

void IRSensor::start(const char* chipPath, int pin) {
try{
#ifdef DEBUG
    fprintf(stderr, "Init.\n");
#endif

    chip = gpiod_chip_open(chipPath);
    if (chip == nullptr) {
#ifdef DEBUG
        fprintf(stderr, "GPIO chip could not be accessed.\n");
#endif
        throw std::runtime_error("GPIO chip error.");
    }

    sensor_line = gpiod_chip_get_line(chip, pin);
    if (sensor_line == nullptr) {
#ifdef DEBUG
        fprintf(stderr, "GPIO line could not be accessed.\n");
#endif
        gpiod_chip_close(chip);
        chip = nullptr;
        throw std::runtime_error("GPIO line error.");
    }

    int ret = gpiod_line_request_both_edges_events(sensor_line, "Consumer");
    if (ret < 0) {
#ifdef DEBUG
        fprintf(stderr, "Request event notification failed on pin %d.\n", pin);
#endif
        gpiod_line_release(sensor_line);
        gpiod_chip_close(chip);
        chip = nullptr;
        sensor_line = nullptr;
        throw std::runtime_error("Could not request event for IRQ.");
    }

    running = true;
    thr = std::thread(&IRSensor::worker, this);


//////////////////////////////////////////////
} catch (const std::exception& ex) {
    qCritical() << "Exception in IRSensor::start." << ex.what();
    throw;
} catch (...) {
    qCritical() << "Unknown exception in IRSensor::start.";
    throw;
}
}


void IRSensor::irEvent(gpiod_line_event& event) {
try{

    std::lock_guard<std::mutex> lock(cb_mutex);
    for (auto& cb : callbacks) {
        cb->hasEvent(event);
    }



    ///////////////////////
} catch (const std::exception& ex) {
    qWarning() << "Exception in IRSensor::irEvent." << ex.what();
} catch (...) {
    qWarning() << "Unknown exception in IRSensor::irEvent.";
}
}
// The worker() method follows the same pattern as in GPIOPin.
// It waits (with a timeout) for an event on the line, reads it,
// and then passes it on via irEvent().

void IRSensor::worker() {
try{
    while (running) {
        const timespec ts = { ISR_TIMEOUT, 0 };
        int r = gpiod_line_event_wait(sensor_line, &ts);
        if (r == 1) {
            gpiod_line_event event;
            if (gpiod_line_event_read(sensor_line, &event) == 0) {
                irEvent(event);
            }
        } else if (r < 0) {
#ifdef DEBUG
            fprintf(stderr, "GPIO error while waiting for event.\n");
#endif
        }
    }

    //////////////////////
} catch (const std::exception& ex) {
    qCritical() << "Exception in IRSensor::worker." << ex.what();
} catch (...) {
    qCritical() << "Unknown exception in IRSensor::worker.";
}
}

void IRSensor::stop() {
try{
    if (!running) return;
    running = false;

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
} catch (const std::exception& ex) {
    qWarning() << "Exception in IRSensor::stop." << ex.what();
} catch (...) {
    qWarning() << "Unknown exception in IRSensor::stop.";
}
}

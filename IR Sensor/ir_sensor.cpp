#include "ir_sensor.h"
#include <iostream>
#include <poll.h>
#include <csignal>
#include <errno.h>
#include <string.h>

IRSensor::IRSensor(const char* chipPath, int pin)
    : chipPath_(chipPath),
      pin_(pin),
      chip_(nullptr),
      sensor_line_(nullptr),
      running_(false)
{
#ifdef DEBUG
    std::cerr << "Initializing IR Sensor on chip " << chipPath_ 
              << ", pin " << pin_ << std::endl;
#endif

    chip_ = gpiod_chip_open(chipPath_);
    if (!chip_) {
#ifdef DEBUG
        std::cerr << "Error: Unable to open GPIO chip " << chipPath_ << std::endl;
#endif
        throw "GPIO chip error.";
    }

    sensor_line_ = gpiod_chip_get_line(chip_, pin_);
    if (!sensor_line_) {
#ifdef DEBUG
        std::cerr << "Error: Unable to get GPIO line for pin " << pin_ << std::endl;
#endif
        gpiod_chip_close(chip_);
        chip_ = nullptr;
        throw "GPIO line error.";
    }

    int ret = gpiod_line_request_both_edges_events(sensor_line_, "ir_sensor");
    if (ret < 0) {
#ifdef DEBUG
        std::cerr << "Error: Failed to request events on GPIO pin " << pin_ << std::endl;
#endif
        gpiod_chip_close(chip_);
        chip_ = nullptr;
        sensor_line_ = nullptr;
        throw "Could not request event for IRQ.";
    }
}

IRSensor::~IRSensor() {
    stop();
    if (sensor_line_) {
        gpiod_line_release(sensor_line_);
        sensor_line_ = nullptr;
    }
    if (chip_) {
        gpiod_chip_close(chip_);
        chip_ = nullptr;
    }
}

bool IRSensor::start() {
    if (!chip_ || !sensor_line_) {
        return false;
    }
    running_ = true;
    eventThread_ = std::thread(&IRSensor::eventLoop, this);
    return true;
}

void IRSensor::stop() {
    running_ = false;
    if (eventThread_.joinable()) {
        eventThread_.join();
    }
}

void IRSensor::registerCallback(IRSensorCallback cb) {
    callbacks_.push_back(cb);
}

void IRSensor::eventLoop() {
    int fd = gpiod_line_event_get_fd(sensor_line_);
    if (fd < 0) {
        std::cerr << "Error: Unable to get event file descriptor for IR sensor." << std::endl;
        return;
    }

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN | POLLPRI;

    while (running_) {
        int ret = poll(&pfd, 1, -1);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            }
            std::cerr << "Error: poll() failed in IR sensor event loop: " 
                      << strerror(errno) << std::endl;
            break;
        }

        if (pfd.revents & (POLLIN | POLLPRI)) {
            gpiod_line_event event;
            if (gpiod_line_event_read(sensor_line_, &event) < 0) {
                std::cerr << "Error: Failed to read GPIO event in IR sensor." << std::endl;
                continue;
            }
            // Notify all registered callbacks.
            for (auto &cb : callbacks_) {
                cb(&event);
            }
        }
    }
}

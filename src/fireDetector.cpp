#include "fireDetector.h"
#include <gpiod.h>
#include <stdio.h>
#include <time.h>

FireDetector::FireDetector() {}

FireDetector::~FireDetector() {
    stop();  // Ensure proper cleanup
}

void FireDetector::registerCallback(FireDetectorCallbackInterface* ci) {
    std::lock_guard<std::mutex> lock(callback_mutex);
    fire_callback_interfaces.push_back(ci);
}

void FireDetector::start(unsigned int id_, unsigned int chip, unsigned int line) {
    stop();  // Prevent duplicate starts

    id = id_;

    gpio_chip = gpiod_chip_open_by_number(chip);
    if (!gpio_chip) {
        throw "FireDetector: Failed to open GPIO chip.";
    }

    gpio_line = gpiod_chip_get_line(gpio_chip, line);
    if (!gpio_line) {
        gpiod_chip_close(gpio_chip);
        gpio_chip = nullptr;
        throw "FireDetector: Failed to get GPIO line.";
    }

    if (gpiod_line_request_both_edges_events(gpio_line, "FireSensor") < 0) {
        gpiod_chip_close(gpio_chip);
        gpio_chip = nullptr;
        gpio_line = nullptr;
        throw "FireDetector: Could not request GPIO edge events.";
    }

    running = true;
    thr = std::thread(&FireDetector::worker, this);
}

void FireDetector::stop() {
    if (!running.exchange(false)) return;

    if (thr.joinable()) {
        thr.join();
    }

    if (gpio_line) {
        gpiod_line_release(gpio_line);
        gpio_line = nullptr;
    }

    if (gpio_chip) {
        gpiod_chip_close(gpio_chip);
        gpio_chip = nullptr;
    }
}

void FireDetector::worker() {
#ifdef DEBUG
    printf("FireDetector Worker Started (Sensor ID: %u)\n", id);
#endif

    while (running) {
        const struct timespec timeout = {1, 0};  // 1s
        int result = gpiod_line_event_wait(gpio_line, &timeout);
        if (result == 1) {
            struct gpiod_line_event event;
            if (gpiod_line_event_read(gpio_line, &event) == 0) {
                std::lock_guard<std::mutex> lock(callback_mutex);
                for (auto& cb : fire_callback_interfaces) {
                    if (cb) {
                        cb->fireDetected(id, event.event_type);
                    }
                }
            }
        } else if (result < 0) {
#ifdef DEBUG
            fprintf(stderr, "FireDetector: Error waiting for GPIO event.\n");
#endif
        }
    }
}

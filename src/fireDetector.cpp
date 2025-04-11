#include "fireDetector.h"
#include <stdexcept>

FireDetector::FireDetector()
{}

void FireDetector::start(unsigned int id_, unsigned int chip, unsigned int line)
{
    id = id_;
    gpio_chip = gpiod_chip_open_by_number(chip);
    if (!gpio_chip) {
#ifdef DEBUG
        fprintf(stderr, "Failed to open GPIO chip %u.\n", chip);
#endif
        throw std::runtime_error("FireDetector: Could not open GPIO chip.");
    }

    gpio_line = gpiod_chip_get_line(gpio_chip, line);
    if (!gpio_line) {
#ifdef DEBUG
        fprintf(stderr, "Failed to get GPIO line %u.\n", line);
#endif
        gpiod_chip_close(gpio_chip);
        throw std::runtime_error("FireDetector: Could not get GPIO line.");
    }

    int ret = gpiod_line_request_both_edges_events(gpio_line, "FireSensor");
    if (ret < 0)
    {
#ifdef DEBUG
        fprintf(stderr, "Request event notification failed on pin %d and chip %d.\n", line, chip);
#endif
        gpiod_chip_close(gpio_chip);
        gpio_chip = nullptr;
        gpio_line = nullptr;
        throw std::runtime_error("FireDetector: Could not request event for IRQ.");
    }

    running = true;
    thr = std::thread(&FireDetector::worker, this);
}

void FireDetector::stop()
{
    if (!running) return;
    running = false;
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

void FireDetector::worker()
{
    printf("FireDetector ID: %i \n", id);
    while (running) {
        const struct timespec ts = { 1, 0 };
        int wait_result = gpiod_line_event_wait(gpio_line, &ts);
        if (wait_result == 1) {
            struct gpiod_line_event event;
            if (gpiod_line_event_read(gpio_line, &event) == 0) {
                for (auto& cb : fire_callback_interfaces) {
                    cb->fireDetected(this->id, event.event_type);
                }
            }
        } else if (wait_result < 0) {
#ifdef DEBUG
            fprintf(stderr, "GPIO event wait failed for FireDetector ID %u.\n", id);
#endif
        }
    }
}

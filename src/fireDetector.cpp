#include "fireDetector.h"

FireDetector::FireDetector()
{}

void FireDetector::start(unsigned int id_, unsigned int chip, unsigned int line)
{
    id = id_;
    gpio_chip = gpiod_chip_open_by_number(chip);
    gpio_line = gpiod_chip_get_line(gpio_chip, line);

    int ret = gpiod_line_request_both_edges_events( gpio_line, "FireSensor");
    if (ret < 0)
    {
#ifdef DEBUG
            fprintf(stderr,"Request event notification failed on pin %d and chip %d.\n",GPIO_CHIP,SENSOR_PIN_0);
#endif
            throw "FireDetector: Could not request event for IRQ.";
    }

    running = true;
    thr = std::thread(&FireDetector::worker, this);

}

void FireDetector::stop()
{
    if (!running) return;
    running = false;
    thr.join();
    gpiod_line_release(gpio_line);
    gpiod_chip_close(gpio_chip);
}

void FireDetector::worker()
{
    printf("FireDetector ID: %i \n", id);
    while (running) {
        const struct timespec ts = { 1, 0 };
        gpiod_line_event_wait(gpio_line, &ts);
        struct gpiod_line_event event;
        gpiod_line_event_read(gpio_line, &event);
        // printf("---------------------------------------- \n");
        // printf("FireDetector ID: %i -- Event: %i \n", id, event.event_type);
        for(auto &cb: fire_callback_interfaces) {
            cb->fireDetected(this->id, event.event_type);
        }
    }
}
#include "ultrasonicDistance.h"
#include <iostream>
#include <gpiod.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int ultrasonicDistance() {
    double duration;
    int distance;
    const char *chipname = "gpiochip0";  // Ensure correct GPIO chip
    int triggerPin = 23;
    int echoPin = 24;
    struct timespec start, end;
    struct gpiod_line_event event;
    struct timespec timeout = { 1, 0 }; // 1-second timeout

    std::cout << "Opening GPIO chip...\n";

    // Open GPIO chip
    gpiod_chip *chipGPIO = gpiod_chip_open_by_name(chipname);
    if (!chipGPIO) {
        perror("Failed to open GPIO chip");
        return -1;
    }
    std::cout << "GPIO chip opened successfully.\n";

    // Get GPIO lines
    gpiod_line *triggerGPIO = gpiod_chip_get_line(chipGPIO, triggerPin);
    gpiod_line *echoGPIO = gpiod_chip_get_line(chipGPIO, echoPin);

    if (!triggerGPIO || !echoGPIO) {
        perror("Failed to get GPIO line");
        gpiod_chip_close(chipGPIO);
        return -1;
    }
    std::cout << "GPIO lines retrieved successfully.\n";

    // Configure trigger pin as output
    if (gpiod_line_request_output(triggerGPIO, "triggerPin", 0) < 0) {
        perror("Failed to configure trigger pin as output");
        gpiod_chip_close(chipGPIO);
        return -1;
    }
    std::cout << "Trigger pin set as output.\n";

    // Configure echo pin as input with BOTH edge detections (rising + falling)
    if (gpiod_line_request_both_edges_events(echoGPIO, "echoPin") < 0) {
        perror("Failed to configure echo pin with both edge detection");
        gpiod_chip_close(chipGPIO);
        return -1;
    }
    std::cout << "Echo pin set as input with both edge detection.\n";

    std::cout << "Setup complete. Starting measurement...\n";

    while (true) {
        std::cout << "Sending trigger pulse...\n";
        gpiod_line_set_value(triggerGPIO, 0);
        usleep(5);
        gpiod_line_set_value(triggerGPIO, 1);
        usleep(10);
        gpiod_line_set_value(triggerGPIO, 0);

        std::cout << "Waiting for Echo HIGH event...\n";

        int ret = gpiod_line_event_wait(echoGPIO, &timeout);
        if (ret <= 0) {
            std::cout << "Timeout waiting for echo signal to start!\n";
            continue;
        }

        gpiod_line_event_read(echoGPIO, &event);
        if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            std::cout << "Echo HIGH detected, measuring time...\n";
        } else {
            std::cout << "Unexpected event detected. Ignoring...\n";
            continue;
        }

        // Wait for the Falling Edge event (echo goes LOW)
        std::cout << "Waiting for Echo LOW event...\n";
        ret = gpiod_line_event_wait(echoGPIO, &timeout);
        if (ret <= 0) {
            std::cout << "Timeout waiting for echo signal to stop!\n";
            continue;
        }

        gpiod_line_event_read(echoGPIO, &event);
        if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
            clock_gettime(CLOCK_MONOTONIC, &end);
            std::cout << "Echo LOW detected, calculating distance...\n";
        } else {
            std::cout << "Unexpected event detected. Ignoring...\n";
            continue;
        }

        // Calculate duration in microseconds
        duration = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1000.0;

        // Calculate distance in cm (Speed of sound = 34300 cm/s)
        distance = duration * 0.034 / 2;

        std::cout << "Distance: " << distance << " cm\n";
        gpiod_line_release(triggerGPIO);
        gpiod_line_release(echoGPIO);
        gpiod_chip_close(chipGPIO);


        return distance;
    }

    gpiod_chip_close(chipGPIO);
    return 0;
}

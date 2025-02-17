#include "../include/UltraSonicSensor.h"

#include <iostream>
#include <unistd.h>
#include <sys/time.h>

void UltraSonicSensor::start(const char* chipPath, int triggerPin, int echoPin) {
#ifdef DEBUG
    std::cerr << "Initializing UltraSonic Sensor..." << std::endl;
#endif

    chip = gpiod_chip_open(chipPath);
    if (!chip) {
#ifdef DEBUG
        std::cerr << "Failed to access GPIO chip." << std::endl;
#endif
        throw "GPIO chip error.";
    }
    
    trigger_line = gpiod_chip_get_line(chip, triggerPin);
    echo_line = gpiod_chip_get_line(chip, echoPin);

    if (!trigger_line || !echo_line) {
#ifdef DEBUG
        std::cerr << "Failed to access GPIO lines." << std::endl;
#endif
        gpiod_chip_close(chip);
        throw "GPIO line error.";
    }

    if (gpiod_line_request_output(trigger_line, "Consumer", 0) < 0 ||
        gpiod_line_request_input(echo_line, "Consumer") < 0) {
#ifdef DEBUG
        std::cerr << "Failed to request GPIO lines." << std::endl;
#endif
        gpiod_chip_close(chip);
        throw "GPIO request error.";
    }

    running = true;
    thr = std::thread(&UltraSonicSensor::worker, this);
}

void UltraSonicSensor::measureDistance() {
    // Send trigger pulse
    gpiod_line_set_value(trigger_line, 1);
    usleep(10);
    gpiod_line_set_value(trigger_line, 0);

    // Wait for echo high
    struct timeval start, end;
    while (gpiod_line_get_value(echo_line) == 0) {
        gettimeofday(&start, NULL);
    }

    // Wait for echo low
    while (gpiod_line_get_value(echo_line) == 1) {
        gettimeofday(&end, NULL);
    }

    // Calculate time difference
    double timeElapsed = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    float distance = (timeElapsed * SOUND_SPEED) / (2 * 1e6);

    for (auto &cb : callbacks) {
        cb->onDistanceMeasured(distance);
    }
}

void UltraSonicSensor::worker() {
    while (running) {
        measureDistance();
        usleep(100000); // 100ms delay
    }
}

void UltraSonicSensor::stop() {
    if (!running) return;
    running = false;
    thr.join();
    gpiod_line_release(trigger_line);
    gpiod_line_release(echo_line);
    gpiod_chip_close(chip);
}

#include "UltraSonicSensor.h"

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <QMetaObject>
#include <stdexcept>
#include <QDebug>

void UltraSonicSensor::start(const char* chipPath, int triggerPin, int echoPin) {
try{
#ifdef DEBUG
    std::cerr << "Initializing UltraSonic Sensor..." << std::endl;
#endif

    chip = gpiod_chip_open(chipPath);
    if (!chip) {
#ifdef DEBUG
        std::cerr << "Failed to access GPIO chip." << std::endl;
#endif
        throw std::runtime_error("GPIO chip error.");
    }

    trigger_line = gpiod_chip_get_line(chip, triggerPin);
    echo_line = gpiod_chip_get_line(chip, echoPin);

    if (!trigger_line || !echo_line) {
#ifdef DEBUG
        std::cerr << "Failed to access GPIO lines." << std::endl;
#endif
        gpiod_chip_close(chip);
        chip = nullptr;
        trigger_line = nullptr;
        echo_line = nullptr;
        throw std::runtime_error("GPIO line error.");
    }

    if (gpiod_line_request_output(trigger_line, "Consumer", 0) < 0 ||
        gpiod_line_request_input(echo_line, "Consumer") < 0) {
#ifdef DEBUG
        std::cerr << "Failed to request GPIO lines." << std::endl;
#endif
        gpiod_line_release(trigger_line);
        gpiod_line_release(echo_line);
        gpiod_chip_close(chip);
        chip = nullptr;
        trigger_line = nullptr;
        echo_line = nullptr;
        throw std::runtime_error("GPIO request error.");
    }

    running = true;
    thr = std::thread(&UltraSonicSensor::worker, this);



} catch (const std::exception& ex) {
    qCritical() << "Exception in UltraSonicSensor::start" << ex.what();
    throw; 
} catch (...) {
    qCritical() << "Unknown exception in UltraSonicSensor::start.";
    throw;
}
}

void UltraSonicSensor::measureDistance() {
    try{
    // Send trigger pulse
    gpiod_line_set_value(trigger_line, 1);
    usleep(10);
    gpiod_line_set_value(trigger_line, 0);

    struct timeval start = {}, end = {};

    // Wait for echo high
    while (gpiod_line_get_value(echo_line) == 0) {
        gettimeofday(&start, NULL);
    }

    // Wait for echo low
    while (gpiod_line_get_value(echo_line) == 1) {
        gettimeofday(&end, NULL);
    }

    double timeElapsed = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    float distance = (timeElapsed * SOUND_SPEED) / (2 * 1e6);

    {
        std::lock_guard<std::mutex> lock(cb_mutex);
        for (auto& cb : callbacks) {
            cb->onDistanceMeasured(distance);
        }
    }

    QMetaObject::invokeMethod(this, "measuredDistance", Qt::QueuedConnection, Q_ARG(float, distance));
} catch (const std::exception& ex) {
    qWarning() << "Exception in UltraSonicSensor::measureDistance." << ex.what();
} catch (...) {
    qWarning() << "Unknown exception in UltraSonicSensor::measureDistance.";
}
}

void UltraSonicSensor::worker() {
try{
    while (running) {
        measureDistance();
        usleep(100000); // 100ms delay
    }


} catch (const std::exception& ex) {
    qCritical() << "Exception in UltraSonicSensor::worker." << ex.what();
} catch (...) {
    qCritical() << "Unknown exception in UltraSonicSensor::worker.";
}
}

void UltraSonicSensor::stop() {
try{
    if (!running) return;
    running = false;

    if (thr.joinable()) {
        thr.join();
    }

    if (trigger_line) {
        gpiod_line_release(trigger_line);
        trigger_line = nullptr;
    }

    if (echo_line) {
        gpiod_line_release(echo_line);
        echo_line = nullptr;
    }

    if (chip) {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
} catch (const std::exception& ex) {
    qWarning() << "Exception in UltraSonicSensor::stop." << ex.what();
} catch (...) {
    qWarning() << "Unknown exception in UltraSonicSensor::stop.";
}
}

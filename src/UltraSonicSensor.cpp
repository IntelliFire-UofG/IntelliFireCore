#include "UltraSonicSensor.h"
#include <unistd.h>
#include <sys/time.h>
#include <QMetaObject>

UltraSonicSensor::UltraSonicSensor(QObject *parent)
    : QObject(parent) {
}

UltraSonicSensor::~UltraSonicSensor() {
    stop();  // Ensure safe cleanup on destruction
}

void UltraSonicSensor::registerCallback(UltraSonicSensorCallbackInterface* ci) {
    std::lock_guard<std::mutex> lock(callback_mutex);
    callbacks.push_back(ci);
}

void UltraSonicSensor::start(const char* chipPath, int triggerPin, int echoPin) {
    stop(); // Stop any previous thread and cleanup

#ifdef DEBUG
    std::cerr << "UltraSonicSensor: Initializing on pins TRIG=" << triggerPin << ", ECHO=" << echoPin << std::endl;
#endif

    chip = gpiod_chip_open(chipPath);
    if (!chip) {
        throw "UltraSonicSensor: Failed to open GPIO chip.";
    }

    trigger_line = gpiod_chip_get_line(chip, triggerPin);
    echo_line = gpiod_chip_get_line(chip, echoPin);

    if (!trigger_line || !echo_line) {
        gpiod_chip_close(chip);
        chip = nullptr;
        throw "UltraSonicSensor: Failed to get GPIO lines.";
    }

    if (gpiod_line_request_output(trigger_line, "UltraSonicTrigger", 0) < 0 ||
        gpiod_line_request_input(echo_line, "UltraSonicEcho") < 0) {
        gpiod_chip_close(chip);
        chip = nullptr;
        trigger_line = nullptr;
        echo_line = nullptr;
        throw "UltraSonicSensor: Failed to request GPIO lines.";
    }

    running = true;
    thr = std::thread(&UltraSonicSensor::worker, this);
}

void UltraSonicSensor::stop() {
    if (!running.exchange(false)) return;

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
}

void UltraSonicSensor::measureDistance() {
    // Trigger pulse
    gpiod_line_set_value(trigger_line, 1);
    usleep(10);
    gpiod_line_set_value(trigger_line, 0);

    struct timeval start = {0}, end = {0};

    // Wait for echo start
    while (gpiod_line_get_value(echo_line) == 0 && running) {
        gettimeofday(&start, nullptr);
    }

    // Wait for echo end
    while (gpiod_line_get_value(echo_line) == 1 && running) {
        gettimeofday(&end, nullptr);
    }

    // Time difference in microseconds
    double timeElapsed = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
    float distance = (timeElapsed * SOUND_SPEED) / (2 * 1e6);  // in cm

    {
        std::lock_guard<std::mutex> lock(callback_mutex);
        for (auto& cb : callbacks) {
            if (cb) cb->onDistanceMeasured(distance);
        }
    }

    QMetaObject::invokeMethod(this, "measuredDistance", Qt::QueuedConnection, Q_ARG(float, distance));
}

void UltraSonicSensor::worker() {
    while (running) {
        measureDistance();
        usleep(100000); // 100ms sampling interval
    }
}

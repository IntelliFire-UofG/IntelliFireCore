#ifndef __ULTRA_SONIC_SENSOR_H__
#define __ULTRA_SONIC_SENSOR_H__

#include <QObject> 
#include <gpiod.h>
#include <thread>
#include <vector>
#include <stdio.h>
#include <chrono>
#include <atomic>
#include <mutex>

#ifndef NDEBUG
#define DEBUG
#endif

#define SOUND_SPEED 34300 // cm/s
#define USR_TIMEOUT 1 // sec

class UltraSonicSensor : public QObject {

    Q_OBJECT

public:
    explicit UltraSonicSensor(QObject *parent = nullptr) : QObject(parent), running(false) {}

    ~UltraSonicSensor() {
        stop();
    }

    UltraSonicSensor(const UltraSonicSensor&) = delete;
    UltraSonicSensor& operator=(const UltraSonicSensor&) = delete;
    UltraSonicSensor(UltraSonicSensor&&) = delete;
    UltraSonicSensor& operator=(UltraSonicSensor&&) = delete;

    struct UltraSonicSensorCallbackInterface {
        /**
         * Called when a new distance measurement is available.
         * Must be implemented by the client.
         * \param distance The measured distance in centimeters.
         **/
        virtual void onDistanceMeasured(float distance) = 0;
    };

    void registerCallback(UltraSonicSensorCallbackInterface* ci) {
        std::lock_guard<std::mutex> lock(cb_mutex);
        callbacks.push_back(ci);
    }

    void start(const char* chipPath, int triggerPin, int echoPin);
    void stop();

signals:
    void measuredDistance(float distance);

private:
    void measureDistance();
    void worker();

    gpiod_chip* chip = nullptr;
    gpiod_line* trigger_line = nullptr;
    gpiod_line* echo_line = nullptr;

    std::thread thr;
    std::atomic<bool> running{false};

    std::vector<UltraSonicSensorCallbackInterface*> callbacks;
    std::mutex cb_mutex;
};

#endif // __ULTRA_SONIC_SENSOR_H__

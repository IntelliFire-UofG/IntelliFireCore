#ifndef __ULTRA_SONIC_SENSOR_H__
#define __ULTRA_SONIC_SENSOR_H__

#include <QObject>
#include <gpiod.h>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>

#ifndef NDEBUG
#define DEBUG
#endif

#define SOUND_SPEED 34300  // cm/s
#define USR_TIMEOUT 1      // sec

class UltraSonicSensor : public QObject {
    Q_OBJECT

public:
    explicit UltraSonicSensor(QObject *parent = nullptr);
    ~UltraSonicSensor();

    struct UltraSonicSensorCallbackInterface {
        virtual void onDistanceMeasured(float distance) = 0;
        virtual ~UltraSonicSensorCallbackInterface() = default;
    };

    void registerCallback(UltraSonicSensorCallbackInterface* ci);
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
    std::mutex callback_mutex;
};

#endif // __ULTRA_SONIC_SENSOR_H__

#ifndef __ULTRA_SONIC_SENSOR_H__
#define __ULTRA_SONIC_SENSOR_H__


#include <QObject> 
#include <gpiod.h>
#include <thread>
#include <vector>
#include <stdio.h>
#include <chrono>

#ifndef NDEBUG
#define DEBUG
#endif

#define SOUND_SPEED 34300 // cm/s
#define USR_TIMEOUT 1 // sec

class UltraSonicSensor : public QObject{

    Q_OBJECT

public:
    explicit UltraSonicSensor(QObject *parent = nullptr) : QObject(parent), running(false) {}  // ✅ Fix constructor
    ~UltraSonicSensor() {
        stop();
    }

    struct UltraSonicSensorCallbackInterface {
        /**
         * Called when a new distance measurement is available.
         * Must be implemented by the client.
         * \param distance The measured distance in centimeters.
         **/
        virtual void onDistanceMeasured(float distance) = 0;
    };

    void registerCallback(UltraSonicSensorCallbackInterface* ci) {
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
    bool running = false;

    std::vector<UltraSonicSensorCallbackInterface*> callbacks;
};

#endif // __ULTRA_SONIC_SENSOR_H__

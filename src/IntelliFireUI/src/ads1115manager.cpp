// ads1115manager.cpp
#include "ads1115manager.h"

ADS1115Manager::ADS1115Manager(QObject *parent) : QObject(parent) {
    ads1115.registerCallback(this);
}

void ADS1115Manager::start() {
    ADS1115settings settings;
    settings.samplingRate = ADS1115settings::FS64HZ;
    settings.drdy_chip = 4; // For RPI5, use chip 4
    ads1115.start(settings);
}

void ADS1115Manager::stop() {
    ads1115.stop();
}

void ADS1115Manager::hasADS1115Sample(float v) {
    static int sensorIndex = 0;
    emit newSensorValue(sensorIndex, v);
    sensorIndex = (sensorIndex + 1) % 4; // Cycle through 4 sensors
}

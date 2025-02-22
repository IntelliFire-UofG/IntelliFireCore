// ads1115manager.cpp
#include "ads1115manager.h"

ADS1115Manager::ADS1115Manager(QObject *parent) : QObject(parent) {
    ads1115.registerCallback(this);
}

void ADS1115Manager::start() {
    ADS1115settings s;
    s.samplingRate = ADS1115settings::FS128HZ;
    s.drdy_chip = 4; // for RPI1-4 chip = 0. For RPI5 it's chip = 4.
    ads1115rpi.start(s);
}

void ADS1115Manager::stop() {
    ads1115rpi.stop();
}

void ADS1115Manager::hasADS1115Sample(float v) {
    if (discard == true)
        {
            discard = false;
            return;
        }
        sensorValues[current_channel] = v;
        nextChannel();
        discard = true; // Cycle through 4 sensors
        emit newSensorValue(current_channel, v);
}

void ADS1115Manager::nextChannel()
{
    switch (current_channel)
    {
    case ADS1115settings::AIN0:
        current_channel = ADS1115settings::AIN1;
        break;
    case ADS1115settings::AIN1:
        current_channel = ADS1115settings::AIN2;
        break;
    case ADS1115settings::AIN2:
        current_channel = ADS1115settings::AIN3;
        break;
    case ADS1115settings::AIN3:
        current_channel = ADS1115settings::AIN0;
        break;
    }
    ads1115rpi.setChannel(current_channel);
}
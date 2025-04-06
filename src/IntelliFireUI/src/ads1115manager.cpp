#include "ads1115manager.h"
#include <QDebug>

ADS1115Manager::ADS1115Manager(QObject *parent)
    : QObject(parent)
{
    ads1115rpi.registerCallback(this);
}

ADS1115Manager::~ADS1115Manager()
{
    stop();  // Ensure safe cleanup
}

void ADS1115Manager::start()
{
    ADS1115settings s;
    s.samplingRate = ADS1115settings::FS128HZ;
    s.pgaGain = ADS1115settings::FSR4_096;
    s.drdy_chip = 4;  // For RPi 5. Use 0 for RPi 1–4.

    try {
        ads1115rpi.start(s);
    } catch (const char* msg) {
        qWarning() << "ADS1115Manager: I2C error -" << msg;
    } catch (...) {
        qWarning() << "ADS1115Manager: Unknown error during ADS1115 startup.";
    }
}

void ADS1115Manager::stop()
{
    ads1115rpi.stop();
}

void ADS1115Manager::hasADS1115Sample(float v)
{
    if (discard) {
        discard = false;
        return;
    }

    sensorValues[static_cast<int>(current_channel)] = v;
    emit newSensorValue(static_cast<int>(current_channel), v);

    nextChannel();
    discard = true;  // skip first sample after switching channels
}

void ADS1115Manager::nextChannel()
{
    switch (current_channel) {
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

#ifndef ADS1115MANAGER_H
#define ADS1115MANAGER_H

#include <QObject>
#include <array>
#include "ads1115rpi.h"

/**
 * @class ADS1115Manager
 * @brief Manages ADS1115 sensor data and emits real-time values.
 */
class ADS1115Manager : public QObject, public ADS1115rpi::ADSCallbackInterface {
    Q_OBJECT

public:
    explicit ADS1115Manager(QObject *parent = nullptr);
    ~ADS1115Manager() override;

    void start();
    void stop();

Q_SIGNALS:
    void newSensorValue(int sensorIndex, float value);

private:
    void hasADS1115Sample(float v) override;
    void nextChannel();

    ADS1115rpi ads1115rpi;
    std::array<float, 4> sensorValues{};  // ✅ std::array is bounds-safe
    bool discard = false;
    ADS1115settings::Input current_channel = ADS1115settings::AIN0;
    float sensor_threshold = 2.0f;
};

#endif // ADS1115MANAGER_H

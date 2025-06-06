#ifndef ADS1115MANAGER_H
#define ADS1115MANAGER_H

#include <QObject>
#include <mutex>
#include "ads1115rpi.h"

/**
 * @class ADS1115Manager
 * @brief Manages ADS1115 sensor data and emits real-time values.
 *
 * This class interfaces with the ADS1115 ADC to read sensor values
 * and emits signals to update the UI.
 */
class ADS1115Manager : public QObject, public ADS1115rpi::ADSCallbackInterface {
    Q_OBJECT

public:
    /**
     * @brief Constructor for ADS1115Manager
     * @param parent Optional QObject parent
     */
    explicit ADS1115Manager(QObject *parent = nullptr);
    ~ADS1115Manager();

    // Rule of Five: prevent copying
    ADS1115Manager(const ADS1115Manager&) = delete;
    ADS1115Manager& operator=(const ADS1115Manager&) = delete;
    ADS1115Manager(ADS1115Manager&&) = delete;
    ADS1115Manager& operator=(ADS1115Manager&&) = delete;

    /**
     * @brief Starts the ADS1115 data acquisition.
     */
    void start();

    /**
     * @brief Stops the ADS1115 data acquisition.
     */
    void stop();

Q_SIGNALS:
    /**
     * @brief Signal emitted when a new sensor value is read.
     * @param sensorIndex The index of the sensor (0-3)
     * @param value The corresponding sensor reading
     */
    void newSensorValue(int sensorIndex, float value);

private:
    /**
     * @brief Callback function for new ADS1115 data.
     * @param v The sensor reading
     */
    void hasADS1115Sample(float v) override;

    void nextChannel();

    ADS1115rpi ads1115rpi;          ///< Instance of ADS1115 driver
    float sensorValues[4];          ///< Array to store sensor values
    bool discard = false;           ///< Flag to discard first sample
    ADS1115settings::Input current_channel = ADS1115settings::AIN0;
    float sensor_threshold = 2.0f;

    std::mutex sample_mutex;        ///< For safe access to sensor values and state
};

#endif // ADS1115MANAGER_H

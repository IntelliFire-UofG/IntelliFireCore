#ifndef ADS1115MANAGER_H
#define ADS1115MANAGER_H

#include <QObject>
#include "ads1115.h" // Assuming this is your driver interface

/**
 * @class ADS1115Manager
 * @brief Manages ADS1115 sensor data and emits real-time values.
 *
 * This class interfaces with the ADS1115 ADC to read sensor values
 * and emits signals to update the UI.
 */
class ADS1115Manager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor for ADS1115Manager
     * @param parent Optional QObject parent
     */
    explicit ADS1115Manager(QObject *parent = nullptr);

    /**
     * @brief Starts the ADS1115 data acquisition.
     */
    void start();

    /**
     * @brief Stops the ADS1115 data acquisition.
     */
    void stop();

signals:
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
    void hasADS1115Sample(float v);

    ADS1115 ads1115; ///< Instance of ADS1115 driver
};

#endif // ADS1115MANAGER_H

#ifndef SENSORCONTAINER_H
#define SENSORCONTAINER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "../include/UltraSonicSensor.h"
#include "../include/IRSensor.h"

/**
 * @class SensorContainer
 * @brief UI container for displaying sensor values.
 *
 * This class provides a visual representation of a sensor, including an icon,
 * a title, and a label to show real-time sensor values.
 */
class SensorContainer : public QWidget, public IRSensor::IRSensorCallbackInterface {
    Q_OBJECT

public:
    /**
     * @brief Constructor for SensorContainer
     * @param containerNumber The sensor number (1-4) for UI representation
     * @param parent Optional parent QWidget
     */
    explicit SensorContainer(int containerNumber, QWidget *parent = nullptr);
    ~SensorContainer();

    // Callback for IR sensor events
    void hasEvent(gpiod_line_event& e) override;

public Q_SLOTS:
    /**
     * @brief Updates the sensor value displayed in the UI.
     * @param value The new sensor reading
     */
    //void updateSensorValue(float value);
    //void updateUltrasonicSensorValue();
    void updateUltrasonicUI(int value);
    void updateIRUI(const QString& message);
    void sensorValueUpdated(int id, float value);


//Q_SIGNALS:
    //void sensorValueUpdated(int newValue);


private:
    QTimer *timer;
    QLabel *title;       ///< Title label displaying sensor information
    QLabel *value_label;  ///< Label showing real-time sensor value
    int sensorNumber;    ///< Identifier for the sensor
    UltraSonicSensor *ultrasonicSensor = nullptr;
    IRSensor *irSensor = nullptr;
};

#endif // SENSORCONTAINER_H
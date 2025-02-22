#ifndef SENSORCONTAINER_H
#define SENSORCONTAINER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "UltraSonicSensor.h"

/**
 * @class SensorContainer
 * @brief UI container for displaying sensor values.
 *
 * This class provides a visual representation of a sensor, including an icon,
 * a title, and a label to show real-time sensor values.
 */
class SensorContainer : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for SensorContainer
     * @param containerNumber The sensor number (1-4) for UI representation
     * @param parent Optional parent QWidget
     */
    explicit SensorContainer(int containerNumber, QWidget *parent = nullptr);

public slots:
    /**
     * @brief Updates the sensor value displayed in the UI.
     * @param value The new sensor reading
     */
    //void updateSensorValue(float value);
    //void updateUltrasonicSensorValue();
    void updateUI(int value);


signals:
    void sensorValueUpdated(int newValue);


private:
    QTimer *timer;
    QLabel *title;       ///< Title label displaying sensor information
    QLabel *value_label;  ///< Label showing real-time sensor value
    int sensorNumber;    ///< Identifier for the sensor
    UltraSonicSensor *sensor;
};

#endif // SENSORCONTAINER_H
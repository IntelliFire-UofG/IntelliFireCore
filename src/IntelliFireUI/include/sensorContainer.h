#ifndef SENSORCONTAINER_H
#define SENSORCONTAINER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <memory>
#include "../include/UltraSonicSensor.h"
#include "../include/IRSensor.h"

/**
 * @class SensorContainer
 * @brief UI container for displaying sensor values.
 *
 * Provides real-time updates and visuals for sensors like Flame, IR, and Ultrasonic.
 */
class SensorContainer : public QWidget, public IRSensor::IRSensorCallbackInterface {
    Q_OBJECT

public:
    explicit SensorContainer(int containerNumber, QWidget *parent = nullptr);
    ~SensorContainer();

    // Callback from IR sensor
    void hasEvent(gpiod_line_event& e) override;

public Q_SLOTS:
    void updateUltrasonicUI(int value);
    void updateIRUI(const QString& message);
    void sensorValueUpdated(int id, float value);

private:
    QTimer* timer = nullptr;
    QLabel* title = nullptr;
    QLabel* value_label = nullptr;
    int sensorNumber = -1;

    std::unique_ptr<UltraSonicSensor> ultrasonicSensor;
    std::unique_ptr<IRSensor> irSensor;
};

#endif // SENSORCONTAINER_H

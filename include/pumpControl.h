#ifndef __PUMPCONTROL_H__
#define __PUMPCONTROL_H__

#include <QObject>
#include <gpiod.h>
#include "../include/fireDetector.h"

/**
 * @class PumpControl
 * @brief Controls the water pump based on fire detection signals from flame sensors.
 *
 * Uses two FireDetector instances to determine whether the pump should be activated.
 * Emits a signal when the pump status changes.
 */
class PumpControl : public QObject, public FireDetector::FireDetectorCallbackInterface {
    Q_OBJECT

public:
    explicit PumpControl(QObject *parent = nullptr);
    ~PumpControl();

    /**
     * @brief Starts monitoring flame sensors and initializes GPIO for the pump.
     */
    void start();

    /**
     * @brief Stops monitoring and releases GPIO resources.
     */
    void stop();

    /**
     * @brief Callback when fire is detected by a sensor.
     * @param id Sensor ID (0 or 1)
     * @param event_type Rising or falling edge event
     */
    void fireDetected(unsigned int id, int event_type) override;

signals:
    /**
     * @brief Emitted when the pump status changes (on/off).
     * @param active true if pump is on, false if off
     */
    void pumpStatusChanged(bool active);

private:
    FireDetector fire_range_detector_0;
    FireDetector fire_range_detector_1;

    gpiod_chip* pump_chip = nullptr;
    gpiod_line* pump_out = nullptr;

    bool fire_detected[2] = {false, false};
    bool prev_pump_status = false;
    bool activate_pump = false;
};

#endif // __PUMPCONTROL_H__

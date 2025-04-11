#ifndef __PUMP_CONTROL_H
#define __PUMP_CONTROL_H

#include <QObject>
#include <gpiod.h>
#include <atomic>
#include <mutex>
#include "fireDetector.h"

#define GPIO_CHIP    4
#define SENSOR_PIN_0 9
#define SENSOR_PIN_1 10
#define PUMP_PIN     11

class PumpControl : public QObject, public FireDetector::FireDetectorCallbackInterface
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for PumpControl
     * @param parent Optional QObject parent
     */
    explicit PumpControl(QObject *parent = nullptr);

    /**
     * @brief Destructor for PumpControl
     */
    ~PumpControl();

    PumpControl(const PumpControl&) = delete;
    PumpControl& operator=(const PumpControl&) = delete;
    PumpControl(PumpControl&&) = delete;
    PumpControl& operator=(PumpControl&&) = delete;

    void start();
    void stop();

Q_SIGNALS:
    /**
     * @brief Signal emitted when there's a change in the pump status
     * @param pumpStatus Boolean status of the pump
     */
    void pumpStatusChanged(bool pumpStatus);

private:
    void fireDetected(unsigned int id, int event_type) override;

    bool fire_detected[2] = {false, false};
    bool activate_pump = false;
    bool prev_pump_status = false;

    FireDetector fire_range_detector_0;
    FireDetector fire_range_detector_1;

    struct gpiod_chip* pump_chip = nullptr;
    struct gpiod_line* pump_out = nullptr;

    std::mutex status_mutex;
    std::atomic<bool> running{false};
};

#endif /* __PUMP_CONTROL_H */

#ifndef __PUMP_CONTROL_H
#define __PUMP_CONTROL_H

#include <QObject>
#include "fireDetector.h"

#define GPIO_CHIP    4
#define SENSOR_PIN_0 22
#define SENSOR_PIN_1 26
#define PUMP_PIN     27

class PumpControl : public FireDetector::FireDetectorCallbackInterface
{
public:
    PumpControl();
    ~PumpControl();
    void start();
    void stop();
private: 

    void fireDetected(unsigned int id, int event_type) override;

    bool fire_detected [2] = {false, false};
    bool activate_pump = false ;
    FireDetector fire_range_detector_0;
    FireDetector fire_range_detector_1;
    struct gpiod_chip * pump_chip = nullptr;
    struct gpiod_line * pump_out = nullptr;
};

#endif /* __PUMP_CONTROL_H */

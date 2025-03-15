#include "pumpControl.h"

PumpControl::PumpControl(QObject *parent): QObject(parent) 
{
    fire_range_detector_0.registerCallback(this);
    fire_range_detector_1.registerCallback(this);
}

PumpControl::~PumpControl()
{
    stop();
}

void PumpControl::start()
{
    pump_chip = gpiod_chip_open_by_number(GPIO_CHIP);
    pump_out = gpiod_chip_get_line(pump_chip, PUMP_PIN);

    gpiod_line_request_output(pump_out, "pump", 0);

    fire_range_detector_0.start(0, GPIO_CHIP, SENSOR_PIN_0);
    fire_range_detector_1.start(1, GPIO_CHIP, SENSOR_PIN_1);
}

void PumpControl::stop()
{
    fire_range_detector_0.stop();
    fire_range_detector_1.stop();
}

void PumpControl::fireDetected(unsigned int id, int event_type)
{
    if (id < 2)
    {
        if (event_type == GPIOD_LINE_EVENT_RISING_EDGE)
            fire_detected[id] = false;
        else if (event_type == GPIOD_LINE_EVENT_FALLING_EDGE)
            fire_detected[id] = true;
    }
    activate_pump = fire_detected[0] && fire_detected [1];
    if (activate_pump) {
        gpiod_line_set_value(pump_out, 1);
        emit pumpStatusChanged(activate_pump);
    }
    else 
    {
        gpiod_line_set_value(pump_out, 0);
        emit pumpStatusChanged(activate_pump);
    }
}
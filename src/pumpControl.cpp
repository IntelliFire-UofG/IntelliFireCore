#include "pumpControl.h"
#include <stdexcept>

PumpControl::PumpControl(QObject *parent) : QObject(parent)
{
    fire_range_detector_0.registerCallback(this);
    fire_range_detector_1.registerCallback(this);
}

PumpControl::~PumpControl()
{
    stop();

    if (pump_out) {
        gpiod_line_set_value(pump_out, 0);
        gpiod_line_release(pump_out);
        pump_out = nullptr;
    }

    if (pump_chip) {
        gpiod_chip_close(pump_chip);
        pump_chip = nullptr;
    }
}

void PumpControl::start()
{
    pump_chip = gpiod_chip_open_by_number(GPIO_CHIP);
    if (!pump_chip) {
#ifdef DEBUG
        fprintf(stderr, "Failed to open GPIO chip %d for pump control.\n", GPIO_CHIP);
#endif
        throw std::runtime_error("PumpControl: Could not open GPIO chip.");
    }

    pump_out = gpiod_chip_get_line(pump_chip, PUMP_PIN);
    if (!pump_out) {
#ifdef DEBUG
        fprintf(stderr, "Failed to get pump GPIO line %d.\n", PUMP_PIN);
#endif
        gpiod_chip_close(pump_chip);
        pump_chip = nullptr;
        throw std::runtime_error("PumpControl: Could not get GPIO line.");
    }

    int ret = gpiod_line_request_output(pump_out, "pump", 0);
    if (ret < 0) {
#ifdef DEBUG
        fprintf(stderr, "Failed to request output line for pump.\n");
#endif
        gpiod_line_release(pump_out);
        gpiod_chip_close(pump_chip);
        pump_out = nullptr;
        pump_chip = nullptr;
        throw std::runtime_error("PumpControl: Could not request output line.");
    }

    fire_range_detector_0.start(0, GPIO_CHIP, SENSOR_PIN_0);
    fire_range_detector_1.start(1, GPIO_CHIP, SENSOR_PIN_1);
}

void PumpControl::stop()
{
    fire_range_detector_0.stop();
    fire_range_detector_1.stop();

    if (pump_out) {
        gpiod_line_set_value(pump_out, 0);
        gpiod_line_release(pump_out);
        pump_out = nullptr;
    }

    if (pump_chip) {
        gpiod_chip_close(pump_chip);
        pump_chip = nullptr;
    }
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

    activate_pump = fire_detected[0] && fire_detected[1];

    if (prev_pump_status != activate_pump)
    {
        if (activate_pump)
        {
            gpiod_line_set_value(pump_out, 1);
        }
        else
        {
            gpiod_line_set_value(pump_out, 0);
        }

        emit pumpStatusChanged(activate_pump);
        prev_pump_status = activate_pump;
    }
}

#ifndef LN298MOTORCONTROL_H
#define LN298MOTORCONTROL_H

#include <functional>
#include <cstdint>
#include <gpiod.h>

/**
 * @class MotorController
 * @brief Controls a motor using L298N motor driver via libgpiod.
 *
 * This class provides methods to move the motors forward, backward, and stop. It also
 * supports event-driven callbacks for dynamic motor control.
 *
 * @author Misael Rivera, Brenda Camacho
 * @date 2025-02-01
 */
class MotorController {
public:
    /**
     * @brief Constructs a MotorController object and initializes motor control pins.
     * 
     * @param enA GPIO pin for motor speed control (PWM).
     * @param in1 GPIO pin for motor direction.
     * @param in2 GPIO pin for motor direction.
     */
    MotorController(int enA, int in1, int in2);
    
    /**
     * @brief Destructor for MotorController.
     */
    ~MotorController();

    /**
     * @brief Activates motor to move at certain speed (-100 to 100%).
     * Negative values indicate reverse direction.
     * 
     * @param speed Speed of the motors (-100 to 100%).
     */
    void setMotorSpeed(int8_t speed);

    /**
     * @brief Registers a callback for event-driven motor actions.
     * 
     * @param callback Function to execute when an event occurs.
     */
    void registerCallback(std::function<void()> callback);

private:
    enum class Direction
    {
        POSITIVE,
        NEGATIVE,
    };

    struct gpiod_chip *chip;
    struct gpiod_line *enablePin;
    struct gpiod_line *in1;
    struct gpiod_line *in2;

    std::function<void()> motorEventCallback;

    /**
     * @brief General function to configure PWM duty cycle in the enable pin
     * to achieve the desired speed percentage.
     * 
     * @param duty Speed of the motors (0-100%).
     */
    void setPWMDuty(int8_t duty);

    /**
     * @brief General function to configure inX GPIOs to set rotation direction of the motors
     * 
     * @param dir Direction of the motor (POSITIVE for forward, NEGATIVE for reverse).
     */
    void setMotorDirection(Direction dir);
};

#endif // LN298MOTORCONTROL_H
#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <functional>

/**
 * @class EventHandler
 * @brief Handles button interrupts for controlling the fire truck.
 *
 * This class registers and handles GPIO interrupts for forward, backward, and stop buttons.
 * It allows real-time event-driven movement control.
 *
 * @author Your Name
 * @date 2025-01-30
 */
class EventHandler {
public:
    /**
     * @brief Constructs an EventHandler and initializes button pins.
     * 
     * @param forwardBtn GPIO pin for forward button.
     * @param backwardBtn GPIO pin for backward button.
     * @param stopBtn GPIO pin for stop button.
     */
    EventHandler(int forwardBtn, int backwardBtn, int stopBtn);

    /**
     * @brief Sets up GPIO interrupts for button events.
     */
    void setupInterrupts();

    /**
     * @brief Registers a callback for the forward button.
     * 
     * @param callback Function to execute when the forward button is pressed.
     */
    void registerForwardCallback(std::function<void()> callback);

    /**
     * @brief Registers a callback for the backward button.
     * 
     * @param callback Function to execute when the backward button is pressed.
     */
    void registerBackwardCallback(std::function<void()> callback);

    /**
     * @brief Registers a callback for the stop button.
     * 
     * @param callback Function to execute when the stop button is pressed.
     */
    void registerStopCallback(std::function<void()> callback);

private:
    int forwardPin, backwardPin, stopPin;
    static std::function<void()> forwardCallback;
    static std::function<void()> backwardCallback;
    static std::function<void()> stopCallback;

    /**
     * @brief Interrupt Service Routine (ISR) for the forward button.
     */
    static void forwardISR();

    /**
     * @brief Interrupt Service Routine (ISR) for the backward button.
     */
    static void backwardISR();

    /**
     * @brief Interrupt Service Routine (ISR) for the stop button.
     */
    static void stopISR();
};

#endif // EVENTHANDLER_HPP

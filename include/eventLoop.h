#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <atomic>
#include <thread>

/**
 * @class EventLoop
 * @brief Manages event-driven execution in a separate thread.
 *
 * The event loop listens for sensor inputs, button presses, and updates the motor state accordingly.
 *
 * @author Your Name
 * @date 2025-01-30
 */
class EventLoop {
public:
    /**
     * @brief Runs the main event loop.
     * This function should be executed in a separate thread.
     */
    void run();
};

#endif // EVENTLOOP_H

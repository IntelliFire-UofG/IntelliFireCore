/**
 * @file EventLoop.cpp
 * @brief Implementation of the EventLoop class
 *
 * @author Misael Rivera
 * @date 2025-03-26
 */

#include "../include/EventLoop.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

// Define event types
enum EventType {
    EVENT_FLAME_DETECTED = 1,
    EVENT_BUTTON_PRESSED = 2,
    EVENT_TIMER = 3,
    // Add more event types as needed
};

EventLoop::EventLoop() : running(false), nextCallbackId(1) {
    std::cout << "EventLoop initialized" << std::endl;
}

EventLoop::~EventLoop() {
    // Ensure the loop is stopped when the object is destroyed
    if (running) {
        stop();
    }
    std::cout << "EventLoop destroyed" << std::endl;
}

void EventLoop::run() {
    if (running) {
        std::cout << "EventLoop is already running" << std::endl;
        return;
    }

    running = true;
    std::cout << "EventLoop started" << std::endl;

    while (running) {
        // Process any pending events
        processEvents();

        // Sleep to avoid high CPU usage
        // Adjust the sleep time based on your system's requirements
        // Shorter sleep times = faster response, but higher CPU usage
        // Longer sleep times = lower CPU usage, but slower response
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cout << "EventLoop stopped" << std::endl;
}

void EventLoop::stop() {
    std::cout << "Stopping EventLoop..." << std::endl;
    running = false;
}

int EventLoop::registerCallback(int eventType, std::function<void()> callback) {
    std::lock_guard<std::mutex> lock(callbacksMutex);

    int id = nextCallbackId++;
    callbacks.push_back({id, eventType, callback});

    std::cout << "Registered callback " << id << " for event type " << eventType << std::endl;
    return id;
}

bool EventLoop::removeCallback(int callbackId) {
    std::lock_guard<std::mutex> lock(callbacksMutex);

    auto it = std::find_if(callbacks.begin(), callbacks.end(),
                          [callbackId](const CallbackInfo& info) {
                              return info.id == callbackId;
                          });

    if (it != callbacks.end()) {
        callbacks.erase(it);
        std::cout << "Removed callback " << callbackId << std::endl;
        return true;
    }

    std::cout << "Callback " << callbackId << " not found" << std::endl;
    return false;
}

void EventLoop::processEvents() {
    // This is a simplified implementation
    // In a real system, you would check for actual events from sensors, buttons, etc.

    // For demonstration, we'll just process any callbacks that are registered
    std::lock_guard<std::mutex> lock(callbacksMutex);

    // In a real implementation, you would only execute callbacks for events that have occurred
    // For example, if a flame was detected, you would execute all EVENT_FLAME_DETECTED callbacks

    // For now, we'll just leave this as a placeholder
    // The actual event processing will depend on your specific system architecture
}
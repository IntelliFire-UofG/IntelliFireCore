#include <gpiod.h>
#include <iostream>
#include <chrono>
#include <thread>

#define CHIP_NAME "/dev/gpiochip0" // GPIO chip device
#define GPIO_PIN 17               // GPIO pin number (BCM numbering)

/**
 * @brief Monitors a flame sensor connected to a GPIO pin in real time.
 *
 * This function uses the libgpiod library to access the GPIO character device interface.
 * It continuously polls the specified GPIO pin for sensor data and prints the status
 * of the flame sensor (flame detected or not) to the console.
 *
 * @note Requires the libgpiod library to be installed on the system.
 * 
 * @throws std::runtime_error if the GPIO chip or line cannot be accessed.
 */
void monitorFlameSensor() {
    // Open the GPIO chip
    gpiod_chip* chip = gpiod_chip_open(CHIP_NAME);
    if (!chip) {
        throw std::runtime_error("Failed to open GPIO chip: " + std::string(CHIP_NAME));
    }

    // Get the GPIO line
    gpiod_line* line = gpiod_chip_get_line(chip, GPIO_PIN);
    if (!line) {
        gpiod_chip_close(chip);
        throw std::runtime_error("Failed to get GPIO line: " + std::to_string(GPIO_PIN));
    }

    // Request the GPIO line as input
    if (gpiod_line_request_input(line, "flame-sensor") < 0) {
        gpiod_chip_close(chip);
        throw std::runtime_error("Failed to request GPIO line as input");
    }

    std::cout << "Monitoring flame sensor on GPIO " << GPIO_PIN << "..." << std::endl;

    // Monitor the sensor in real-time
    while (true) {
        int value = gpiod_line_get_value(line);
        if (value < 0) {
            std::cerr << "Failed to read GPIO value" << std::endl;
        } else if (value == 1) {
            std::cout << "No flame detected." << std::endl;
        } else if (value == 0) {
            std::cout << "Flame detected!" << std::endl;
        }

        // Add a short delay for polling (500ms)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Release the GPIO line and close the chip
    gpiod_line_release(line);
    gpiod_chip_close(chip);
}

/**
 * @brief Main function to initiate flame sensor monitoring.
 *
 * Calls the monitorFlameSensor() function to start real-time monitoring.
 *
 * @return 0 on successful execution, non-zero on failure.
 */
int main() {
    try {
        monitorFlameSensor();
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

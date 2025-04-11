#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <memory>
#include <sstream>

/**
 * Logger class for recording system events and key presses.
 * Thread-safe implementation with timestamp support.
 */
class Logger {
private:
    std::ofstream logFile;
    std::mutex logMutex;
    static std::shared_ptr<Logger> instance;

    /**
     * Private constructor to enforce singleton pattern.
     * @param filename The file to write logs to
     */
    Logger(const std::string& filename);

public:
    /**
     * Destructor ensures log file is properly closed.
     */
    ~Logger();

    /**
     * Get or create the singleton logger instance.
     * @param filename Optional filename for the log (only used on first call)
     * @return Shared pointer to the Logger instance
     */
    static std::shared_ptr<Logger> getInstance(const std::string& filename = "firetruck_control.log");

    /**
     * Log a message with a timestamp.
     * @param message The message to log
     */
    void log(const std::string& message);

    /**
     * Log a key press event.
     * @param key The key that was pressed
     * @param action Description of the action taken (e.g., "pressed", "released")
     */
    void logKeyEvent(char key, const std::string& action);

    /**
     * Log a motor action.
     * @param motorName Name of the motor ("left" or "right")
     * @param action Description of the action (e.g., "forward", "backward", "stop")
     */
    void logMotorAction(const std::string& motorName, const std::string& action);

    /**
     * Get current timestamp as string.
     * @return Formatted timestamp string
     */
    static std::string getTimestamp();
};

#endif // LOGGER_H
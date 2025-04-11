#include "Logger.h"
#include <iostream>

// Initialize static instance pointer
std::shared_ptr<Logger> Logger::instance = nullptr;

Logger::Logger(const std::string& filename) {
    // Open log file in append mode
    logFile.open(filename, std::ios::app);
    
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not open log file " << filename << std::endl;
    } else {
        // Log start of session
        logFile << "\n" << getTimestamp() << " --- LOGGING SESSION STARTED ---" << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        // Log end of session
        logFile << getTimestamp() << " --- LOGGING SESSION ENDED ---\n" << std::endl;
        logFile.close();
    }
}

std::shared_ptr<Logger> Logger::getInstance(const std::string& filename) {
    // Double-checked locking pattern
    if (instance == nullptr) {
        static std::mutex initMutex;
        std::lock_guard<std::mutex> lock(initMutex);
        
        if (instance == nullptr) {
            // Using make_shared for exception safety
            instance = std::shared_ptr<Logger>(new Logger(filename));
        }
    }
    return instance;
}

void Logger::log(const std::string& message) {
    if (!logFile.is_open()) return;
    
    std::lock_guard<std::mutex> lock(logMutex);
    logFile << getTimestamp() << " " << message << std::endl;
    logFile.flush(); // Ensure data is written immediately
}

void Logger::logKeyEvent(char key, const std::string& action) {
    std::stringstream ss;
    ss << "KEY[" << key << "] " << action;
    log(ss.str());
}

void Logger::logMotorAction(const std::string& motorName, const std::string& action) {
    std::stringstream ss;
    ss << "MOTOR[" << motorName << "] " << action;
    log(ss.str());
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto nowTime = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&nowTime), "[%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count() << "]";
    
    return ss.str();
}

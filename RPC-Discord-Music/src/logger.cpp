#include "logger.h"
#include <ctime>
#include <iomanip>
#include <iostream>

Logger::Logger() {
    logFile.open("debug.log", std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::Log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (!logFile.is_open()) return;

    auto now = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &now);
    
    std::string levelStr;
    switch (level) {
        case LogLevel::DEBUG: levelStr = "[DEBUG]"; break;
        case LogLevel::INFO:  levelStr = "[INFO] "; break;
        case LogLevel::ERR: levelStr = "[ERROR]"; break;
    }

    logFile << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " " << levelStr << " " << message << std::endl;
    
#ifdef _DEBUG
    std::cout << levelStr << " " << message << std::endl;
#endif
}

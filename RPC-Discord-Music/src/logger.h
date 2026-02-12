#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <filesystem>

enum class LogLevel {
    DEBUG,
    INFO,
    ERR
};

class Logger {
public:
    static Logger& Get() {
        static Logger instance;
        return instance;
    }

    void Log(LogLevel level, const std::string& message);
    
private:
    Logger();
    ~Logger();
    
    std::ofstream logFile;
    std::mutex logMutex;
};

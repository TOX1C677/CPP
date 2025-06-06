#pragma once
#include <string>
#include <mutex>
#include <fstream>

class Logger {
public:
    static Logger& GetInstance();

    void LogInfo(const std::string& message);
    void LogError(const std::string& message);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger();
    ~Logger();

    void Log(const std::string& message, const std::string& level);

    std::mutex logMutex_;
    std::ofstream fileStream_;
};
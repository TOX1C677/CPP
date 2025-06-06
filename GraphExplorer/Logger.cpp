#include "pch.h"
#include "Logger.h"
#include <iomanip>
#include <Windows.h>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    fileStream_.open("log.txt", std::ios::app);
}

Logger::~Logger() {
    if (fileStream_.is_open()) {
        fileStream_.close();
    }
}

void Logger::LogInfo(const std::string& message) {
    Log(message, "INFO");
}

void Logger::LogError(const std::string& message) {
    Log(message, "ERROR");
}

void Logger::Log(const std::string& message, const std::string& level) {
    std::lock_guard<std::mutex> lock(logMutex_);

    if (!fileStream_.is_open()) return;

    SYSTEMTIME time;
    GetLocalTime(&time);

    fileStream_ << "[" << time.wYear << "-"
        << std::setw(2) << std::setfill('0') << time.wMonth << "-"
        << std::setw(2) << std::setfill('0') << time.wDay << " "
        << std::setw(2) << std::setfill('0') << time.wHour << ":"
        << std::setw(2) << std::setfill('0') << time.wMinute << ":"
        << std::setw(2) << std::setfill('0') << time.wSecond << "] "
        << "[" << level << "] " << message << "\n";

    fileStream_.flush();
}
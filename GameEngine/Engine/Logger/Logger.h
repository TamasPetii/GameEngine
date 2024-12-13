#pragma once
#include "EngineApi.h"
#include <string>
#include <deque>
#include <mutex>

enum class LogType {
    LOG_INFO,
    LOG_ERROR,
    LOG_DEBUG
};

struct LogEntry {
    LogType type;
    std::string origin;
    std::string message;
    std::string timestamp;
};

class ENGINE_API Logger {
public:
    static Logger* Instance();
    static void Destroy();
    std::string GetCurrentTimestamp();

    void Log(LogType type, const std::string& origin, const std::string& message);
    const std::deque<LogEntry>& GetEntries() const { return m_LogEntires; }
private:
    Logger() = default;
    static Logger* m_Instance;
    std::mutex m_Mutex;
    std::deque<LogEntry> m_LogEntires;
};

#define LOG_INFO(origin, message) Logger::Instance()->Log(LogType::LOG_INFO, origin, message)
#define LOG_ERROR(origin, message) Logger::Instance()->Log(LogType::LOG_ERROR, origin, message)
#define LOG_DEBUG(origin, message) Logger::Instance()->Log(LogType::LOG_DEBUG, origin, message)
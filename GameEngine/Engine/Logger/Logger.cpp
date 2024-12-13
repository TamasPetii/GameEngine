#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

Logger* Logger::m_Instance = nullptr;

Logger* Logger::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new Logger();
	return m_Instance;
}

void Logger::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
	m_Instance = nullptr;
}

std::string Logger::GetCurrentTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "[%Y.%m.%d | %H:%M:%S]");
	return ss.str();
}

void Logger::Log(LogType type, const std::string& origin, const std::string& message)
{
	LogEntry log;
	log.type = type;
	log.origin = origin;
	log.message = message;
	log.timestamp = GetCurrentTimestamp();

	std::lock_guard<std::mutex> lock(m_Mutex);
	m_LogEntires.push_front(log);

	#ifdef _DEBUG
	std::cout << log.timestamp << " <<" << log.origin << ">> " << log.message << std::endl;
	#endif
}
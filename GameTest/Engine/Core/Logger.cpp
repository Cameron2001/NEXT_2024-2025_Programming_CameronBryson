// Logger.cpp
#include "stdafx.h"
#include "Logger.h"
#include <windows.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <mutex>

Logger &Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{
    const std::string logDir = "logs";

    // Check if the 'logs' directory exists
    DWORD ftyp = GetFileAttributesA(logDir.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
    {
        CreateDirectoryA(logDir.c_str(), NULL);
    }
    else if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY))
    {
        // Handle the error
    }

    // Generate a unique log file name with timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << logDir << "/Log_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".log";
    std::string logFileName = oss.str();

    // Open the log file in append mode
    m_outputFile.open(logFileName, std::ios::out | std::ios::app);
    if (m_outputFile.is_open())
    {
        WriteLog("Logging Started.");
    }
    else
    {
        // Handle the error
    }
}

Logger::~Logger()
{
    if (m_outputFile.is_open())
    {
        WriteLog("Logging Ended.");
        m_outputFile.close();
    }
}

void Logger::WriteLog(const std::string &message)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_outputFile.is_open())
    {
        // Get the current time
        std::time_t t = std::time(nullptr);
        std::tm tm;
        localtime_s(&tm, &t); // Thread-safe function on Windows

        char timeBuffer[20];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &tm);

        // Write the timestamp and message
        m_outputFile << "[" << timeBuffer << "] " << message << std::endl;
    }
}

void Logger::LogInfo(const std::string &message)
{
    WriteLog("[INFO]: " + message);
}

void Logger::LogWarning(const std::string &message)
{
    WriteLog("[WARNING]: " + message);
}

void Logger::LogError(const std::string &message)
{
    WriteLog("[ERROR]: " + message);
}

void Logger::LogTestResult(const TestResult &result)
{
    std::ostringstream oss;
    oss << "[TEST " << (result.passed ? "PASSED" : "FAILED") << "]: " << result.name;
    if (!result.message.empty())
    {
        oss << " - " << result.message;
    }
    WriteLog(oss.str());
}

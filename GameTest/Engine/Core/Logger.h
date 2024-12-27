#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include "Engine/Core/TestResult.h"

class Logger
{
  public:
    static Logger &GetInstance();

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void LogInfo(const std::string &message);
    void LogWarning(const std::string &message);
    void LogError(const std::string &message);
    void LogTestResult(const TestResult& result);

  private:
    Logger();
    ~Logger();

    void WriteLog(const std::string &message);

    std::ofstream m_outputFile;
    std::mutex m_mutex;
};

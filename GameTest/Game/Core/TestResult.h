#pragma once
#include <string>
struct TestResult
{
    TestResult(const std::string &name, const bool passed, const std::string &message)
        : name(name), message(message), passed(passed)
    {
    }
    std::string name;
    std::string message;
    bool passed;
    
};

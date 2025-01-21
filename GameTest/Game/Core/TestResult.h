#pragma once
#include <string>
struct TestResult
{
    TestResult(const std::string &name, const bool passed, const std::string &message)
        : name(name), passed(passed), message(message)
    {
    }
    std::string name;
    std::string message;
    bool passed;
    
};

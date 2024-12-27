#pragma once
#include <string>
struct TestResult
{
    std::string name;
    bool passed;
    std::string message;
    TestResult(const std::string &name, bool passed, const std::string &message)
        : name(name), passed(passed), message(message)
    {
    }
};

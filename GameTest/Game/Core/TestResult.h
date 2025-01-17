#pragma once
#include <string>
struct TestResult
{
    bool passed;
    std::string name;
    std::string message;
    TestResult(const std::string &name, bool passed, const std::string &message)
        : name(name), passed(passed), message(message)
    {
    }
};

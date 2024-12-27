#pragma once
#include "Engine/Core/TestResult.h"
#include "Engine/Math/Vector4.h"
#include <vector>
#include <string>

namespace Vector4Tests
{
inline TestResult FVectorAddition()
{
    FVector4 v1(1.0f, 2.0f, 3.0f, 4.0f);
    FVector4 v2(5.0f, 6.0f, 7.0f, 8.0f);
    FVector4 expected(6.0f, 8.0f, 10.0f, 12.0f);
    FVector4 result = v1 + v2;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (6.0, 8.0, 10.0, 12.0), Actual: (" + std::to_string(result.X) + ", " +
                                        std::to_string(result.Y) + ", " + std::to_string(result.Z) + ", " +
                                        std::to_string(result.W) + ")";
    return TestResult("Vector4Tests::FVectorAddition", isEqual, message);
}

inline TestResult FVectorSubtraction()
{
    FVector4 v1(10.0f, 8.0f, 6.0f, 4.0f);
    FVector4 v2(1.0f, 2.0f, 3.0f, 4.0f);
    FVector4 expected(9.0f, 6.0f, 3.0f, 0.0f);
    FVector4 result = v1 - v2;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (9.0, 6.0, 3.0, 0.0), Actual: (" + std::to_string(result.X) + ", " +
                                        std::to_string(result.Y) + ", " + std::to_string(result.Z) + ", " +
                                        std::to_string(result.W) + ")";
    return TestResult("Vector4Tests::FVectorSubtraction", isEqual, message);
}

inline TestResult FVectorScalarMultiplication()
{
    FVector4 v(2.0f, 3.0f, 4.0f, 5.0f);
    float scalar = 3.0f;
    FVector4 expected(6.0f, 9.0f, 12.0f, 15.0f);
    FVector4 result = v * scalar;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (6.0, 9.0, 12.0, 15.0), Actual: (" + std::to_string(result.X) + ", " +
                                        std::to_string(result.Y) + ", " + std::to_string(result.Z) + ", " +
                                        std::to_string(result.W) + ")";
    return TestResult("Vector4Tests::FVectorScalarMultiplication", isEqual, message);
}

inline TestResult FVectorScalarDivision()
{
    FVector4 v(6.0f, 9.0f, 12.0f, 15.0f);
    float scalar = 3.0f;
    FVector4 expected(2.0f, 3.0f, 4.0f, 5.0f);
    FVector4 result = v / scalar;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (2.0, 3.0, 4.0, 5.0), Actual: (" + std::to_string(result.X) + ", " +
                                        std::to_string(result.Y) + ", " + std::to_string(result.Z) + ", " +
                                        std::to_string(result.W) + ")";
    return TestResult("Vector4Tests::FVectorScalarDivision", isEqual, message);
}

inline TestResult FVectorDotProduct()
{
    FVector4 v1(1.0f, 3.0f, -5.0f, 2.0f);
    FVector4 v2(4.0f, -2.0f, -1.0f, 3.0f);
    float expected = 9.0f;
    float result = v1.Dot(v2);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 9.0, Actual: " + std::to_string(result);
    return TestResult("Vector4Tests::FVectorDotProduct", isEqual, message);
}

inline TestResult FVectorLength()
{
    FVector4 v(2.0f, 3.0f, 6.0f, 1.0f);
    float expected = 7.07107f;
    float result = v.Length();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 7.07107, Actual: " + std::to_string(result);
    return TestResult("Vector4Tests::FVectorLength", isEqual, message);
}

inline TestResult FVectorLengthSquared()
{
    FVector4 v(2.0f, 3.0f, 6.0f, 1.0f);
    float expected = 50.0f;
    float result = v.LengthSquared();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 50.0, Actual: " + std::to_string(result);
    return TestResult("Vector4Tests::FVectorLengthSquared", isEqual, message);
}

inline TestResult FVectorNormalize()
{
    FVector4 v(0.0f, 3.0f, 4.0f, 0.0f);
    FVector4 expected(0.0f, 0.6f, 0.8f, 0.0f);
    FVector4 result = v.Normalize();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (0.0, 0.6, 0.8, 0.0), Actual: (" + std::to_string(result.X) + ", " +
                                        std::to_string(result.Y) + ", " + std::to_string(result.Z) + ", " +
                                        std::to_string(result.W) + ")";
    return TestResult("Vector4Tests::FVectorNormalize", isEqual, message);
}

inline TestResult FVectorClamp()
{
    FVector4 v(5.0f, -3.0f, 10.0f, 7.0f);
    float min = 0.0f;
    float max = 6.0f;
    FVector4 expected(5.0f, 0.0f, 6.0f, 6.0f);
    FVector4 result = v.Clamp(min, max);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (5.0, 0.0, 6.0, 6.0), Actual: (" + std::to_string(result.X) + ", " +
                                        std::to_string(result.Y) + ", " + std::to_string(result.Z) + ", " +
                                        std::to_string(result.W) + ")";
    return TestResult("Vector4Tests::FVectorClamp", isEqual, message);
}

inline TestResult FVectorProject()
{
    FVector4 v(3.0f, 4.0f, 0.0f, 0.0f);
    FVector4 normal(1.0f, 0.0f, 0.0f, 0.0f);
    FVector4 expected(3.0f, 0.0f, 0.0f, 0.0f);
    FVector4 result = v.Project(normal);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (3.0, 0.0, 0.0, 0.0), Actual: (" + std::to_string(result.X) + ", " +
                                        std::to_string(result.Y) + ", " + std::to_string(result.Z) + ", " +
                                        std::to_string(result.W) + ")";
    return TestResult("Vector4Tests::FVectorProject", isEqual, message);
}

inline std::vector<TestResult> RunAllTests()
{
    return {FVectorAddition(),   FVectorSubtraction(), FVectorScalarMultiplication(), FVectorScalarDivision(),
            FVectorDotProduct(), FVectorLength(),      FVectorLengthSquared(),        FVectorNormalize(),
            FVectorClamp(),      FVectorProject()};
}
} // namespace Vector4Tests

#pragma once

#include "Engine/Core/TestResult.h"
#include "Engine/Math/Vector2.h"
#include <vector>
#include <string>

namespace Vector2Tests
{
inline TestResult FVectorAddition()
{
    FVector2 v1(1.0f, 2.0f);
    FVector2 v2(3.0f, 4.0f);
    FVector2 expected(4.0f, 6.0f);
    FVector2 result = v1 + v2;
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (4.0, 6.0), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::FVectorAddition", isEqual, message);
}

inline TestResult FVectorSubtraction()
{
    FVector2 v1(5.0f, 7.0f);
    FVector2 v2(2.0f, 3.0f);
    FVector2 expected(3.0f, 4.0f);
    FVector2 result = v1 - v2;
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (3.0, 4.0), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::FVectorSubtraction", isEqual, message);
}

inline TestResult FVectorScalarMultiplication()
{
    FVector2 v(2.0f, 3.0f);
    float scalar = 4.0f;
    FVector2 expected(8.0f, 12.0f);
    FVector2 result = v * scalar;
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (8.0, 12.0), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::FVectorScalarMultiplication", isEqual, message);
}

inline TestResult FVectorScalarDivision()
{
    FVector2 v(8.0f, 12.0f);
    float scalar = 4.0f;
    FVector2 expected(2.0f, 3.0f);
    FVector2 result = v / scalar;
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (2.0, 3.0), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::FVectorScalarDivision", isEqual, message);
}

inline TestResult FVectorDotProduct()
{
    FVector2 v1(1.0f, 3.0f);
    FVector2 v2(4.0f, -2.0f);
    float expected = 1.0f * 4.0f + 3.0f * -2.0f;
    float result = v1.Dot(v2);
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed" : "Expected: " + std::to_string(expected) + ", Actual: " + std::to_string(result);
    return TestResult("Vector2Tests::FVectorDotProduct", isEqual, message);
}

inline TestResult FVectorCrossProduct()
{
    FVector2 v1(1.0f, 2.0f);
    FVector2 v2(3.0f, 4.0f);
    float expected = 1.0f * 4.0f - 2.0f * 3.0f;
    float result = v1.Cross(v2);
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed" : "Expected: " + std::to_string(expected) + ", Actual: " + std::to_string(result);
    return TestResult("Vector2Tests::FVectorCrossProduct", isEqual, message);
}

inline TestResult FVectorLength()
{
    FVector2 v(3.0f, 4.0f);
    float expected = 5.0f;
    float result = v.Length();
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed" : "Expected: " + std::to_string(expected) + ", Actual: " + std::to_string(result);
    return TestResult("Vector2Tests::FVectorLength", isEqual, message);
}

inline TestResult FVectorLengthSquared()
{
    FVector2 v(3.0f, 4.0f);
    float expected = 25.0f;
    float result = v.LengthSquared();
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed" : "Expected: " + std::to_string(expected) + ", Actual: " + std::to_string(result);
    return TestResult("Vector2Tests::FVectorLengthSquared", isEqual, message);
}

inline TestResult FVectorNormalize()
{
    FVector2 v(3.0f, 4.0f);
    FVector2 expected(0.6f, 0.8f);
    FVector2 result = v.Normalize();
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (0.6, 0.8), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::FVectorNormalize", isEqual, message);
}

inline TestResult FVectorClamp()
{
    FVector2 v(5.0f, -3.0f);
    float min = 0.0f;
    float max = 4.0f;
    FVector2 expected(4.0f, 0.0f);
    FVector2 result = v.Clamp(min, max);
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (4.0, 0.0), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::FVectorClamp", isEqual, message);
}

inline TestResult FVectorProject()
{
    FVector2 v(3.0f, 4.0f);
    FVector2 normal(1.0f, 0.0f);
    FVector2 expected(3.0f, 0.0f);
    FVector2 result = v.Project(normal);
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (3.0, 0.0), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::FVectorProject", isEqual, message);
}

inline TestResult IVectorAddition()
{
    IVector2 v1(1, 2);
    IVector2 v2(3, 4);
    IVector2 expected(4, 6);
    IVector2 result = v1 + v2;
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (4, 6), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::IVectorAddition", isEqual, message);
}

inline TestResult IVectorSubtraction()
{
    IVector2 v1(5, 7);
    IVector2 v2(2, 3);
    IVector2 expected(3, 4);
    IVector2 result = v1 - v2;
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (3, 4), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::IVectorSubtraction", isEqual, message);
}

inline TestResult IVectorScalarMultiplication()
{
    IVector2 v(2, 3);
    int scalar = 4;
    IVector2 expected(8, 12);
    IVector2 result = v * scalar;
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (8, 12), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::IVectorScalarMultiplication", isEqual, message);
}

inline TestResult IVectorScalarDivision()
{
    IVector2 v(8, 12);
    int scalar = 4;
    IVector2 expected(2, 3);
    IVector2 result = v / scalar;
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (2, 3), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::IVectorScalarDivision", isEqual, message);
}

inline TestResult IVectorDotProduct()
{
    IVector2 v1(1, 3);
    IVector2 v2(4, -2);
    float expected = static_cast<float>((1 * 4) + (3 * -2));
    float result = v1.Dot(v2);
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed" : "Expected: " + std::to_string(expected) + ", Actual: " + std::to_string(result);
    return TestResult("Vector2Tests::IVectorDotProduct", isEqual, message);
}

inline TestResult IVectorCrossProduct()
{
    IVector2 v1(1, 2);
    IVector2 v2(3, 4);
    float expected = static_cast<float>((1 * 4) - (2 * 3));
    float result = v1.Cross(v2);
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed" : "Expected: " + std::to_string(expected) + ", Actual: " + std::to_string(result);
    return TestResult("Vector2Tests::IVectorCrossProduct", isEqual, message);
}

inline TestResult IVectorLength()
{
    IVector2 v(3, 4);
    float expected = 5.0f;
    float result = v.Length();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 5.0, Actual: " + std::to_string(result);
    return TestResult("Vector2Tests::IVectorLength", isEqual, message);
}

inline TestResult IVectorLengthSquared()
{
    IVector2 v(3, 4);
    float expected = 25.0f;
    float result = v.LengthSquared();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 25.0, Actual: " + std::to_string(result);
    return TestResult("Vector2Tests::IVectorLengthSquared", isEqual, message);
}


inline TestResult IVectorClamp()
{
    IVector2 v(5, -3);
    int min = 0;
    int max = 4;
    IVector2 expected(4, 0);
    IVector2 result = v.Clamp(min, max);
    bool isEqual = (result == expected);
    std::string message =
        isEqual ? "Passed"
                : "Expected: (4, 0), Actual: (" + std::to_string(result.x) + ", " + std::to_string(result.y) + ")";
    return TestResult("Vector2Tests::IVectorClamp", isEqual, message);
}



inline std::vector<TestResult> RunAllTests()
{
    return {FVectorAddition(),
            FVectorSubtraction(),
            FVectorScalarMultiplication(),
            FVectorScalarDivision(),
            FVectorDotProduct(),
            FVectorCrossProduct(),
            FVectorLength(),
            FVectorLengthSquared(),
            FVectorNormalize(),
            FVectorClamp(),
            FVectorProject(),
            IVectorAddition(),
            IVectorSubtraction(),
            IVectorScalarMultiplication(),
            IVectorScalarDivision(),
            IVectorDotProduct(),
            IVectorCrossProduct(),
            IVectorLength(),
            IVectorLengthSquared(),
            IVectorClamp()};
}
} // namespace Vector2Tests

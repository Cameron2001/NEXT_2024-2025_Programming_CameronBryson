#pragma once

#include "Engine/Core/TestResult.h"
#include "Engine/Math/Vector3.h"
#include <vector>
#include <string>

namespace Vector3Tests
{
inline TestResult FVectorAddition()
{
    FVector3 v1(1.0f, 2.0f, 3.0f);
    FVector3 v2(4.0f, 5.0f, 6.0f);
    FVector3 expected(5.0f, 7.0f, 9.0f);
    FVector3 result = v1 + v2;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (5.0, 7.0, 9.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::FVectorAddition", isEqual, message);
}

inline TestResult FVectorSubtraction()
{
    FVector3 v1(5.0f, 7.0f, 9.0f);
    FVector3 v2(2.0f, 3.0f, 4.0f);
    FVector3 expected(3.0f, 4.0f, 5.0f);
    FVector3 result = v1 - v2;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (3.0, 4.0, 5.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::FVectorSubtraction", isEqual, message);
}

inline TestResult FVectorScalarMultiplication()
{
    FVector3 v(2.0f, 3.0f, 4.0f);
    float scalar = 3.0f;
    FVector3 expected(6.0f, 9.0f, 12.0f);
    FVector3 result = v * scalar;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (6.0, 9.0, 12.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::FVectorScalarMultiplication", isEqual, message);
}

inline TestResult FVectorScalarDivision()
{
    FVector3 v(6.0f, 9.0f, 12.0f);
    float scalar = 3.0f;
    FVector3 expected(2.0f, 3.0f, 4.0f);
    FVector3 result = v / scalar;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (2.0, 3.0, 4.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::FVectorScalarDivision", isEqual, message);
}

inline TestResult FVectorDotProduct()
{
    FVector3 v1(1.0f, 3.0f, -5.0f);
    FVector3 v2(4.0f, -2.0f, -1.0f);
    float expected = 3.0f;
    float result = v1.Dot(v2);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 3.0, Actual: " + std::to_string(result);
    return TestResult("Vector3Tests::FVectorDotProduct", isEqual, message);
}

inline TestResult FVectorCrossProduct()
{
    FVector3 v1(1.0f, 2.0f, 3.0f);
    FVector3 v2(4.0f, 5.0f, 6.0f);
    FVector3 expected(-3.0f, 6.0f, -3.0f);
    FVector3 result = v1.Cross(v2);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (-3.0, 6.0, -3.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::FVectorCrossProduct", isEqual, message);
}

inline TestResult FVectorLength()
{
    FVector3 v(3.0f, 4.0f, 12.0f);
    float expected = 13.0f;
    float result = v.Length();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 13.0, Actual: " + std::to_string(result);
    return TestResult("Vector3Tests::FVectorLength", isEqual, message);
}

inline TestResult FVectorLengthSquared()
{
    FVector3 v(3.0f, 4.0f, 12.0f);
    float expected = 169.0f;
    float result = v.LengthSquared();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 169.0, Actual: " + std::to_string(result);
    return TestResult("Vector3Tests::FVectorLengthSquared", isEqual, message);
}

inline TestResult FVectorNormalize()
{
    FVector3 v(0.0f, 3.0f, 4.0f);
    FVector3 expected(0.0f, 0.6f, 0.8f);
    FVector3 result = v.Normalize();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (0.0, 0.6, 0.8), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::FVectorNormalize", isEqual, message);
}

inline TestResult FVectorClamp()
{
    FVector3 v(5.0f, -3.0f, 10.0f);
    float min = 0.0f;
    float max = 4.0f;
    FVector3 expected(4.0f, 0.0f, 4.0f);
    FVector3 result = v.Clamp(min, max);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (4.0, 0.0, 4.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::FVectorClamp", isEqual, message);
}

inline TestResult FVectorProject()
{
    FVector3 v(3.0f, 4.0f, 0.0f);
    FVector3 normal(1.0f, 0.0f, 0.0f);
    FVector3 expected(3.0f, 0.0f, 0.0f);
    FVector3 result = v.Project(normal);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (3.0, 0.0, 0.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::FVectorProject", isEqual, message);
}

inline TestResult IVectorAddition()
{
    IVector3 v1(1, 2, 3);
    IVector3 v2(4, 5, 6);
    IVector3 expected(5, 7, 9);
    IVector3 result = v1 + v2;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (5, 7, 9), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::IVectorAddition", isEqual, message);
}

inline TestResult IVectorSubtraction()
{
    IVector3 v1(5, 7, 9);
    IVector3 v2(2, 3, 4);
    IVector3 expected(3, 4, 5);
    IVector3 result = v1 - v2;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (3, 4, 5), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::IVectorSubtraction", isEqual, message);
}

inline TestResult IVectorScalarMultiplication()
{
    IVector3 v(2, 3, 4);
    int scalar = 3;
    IVector3 expected(6, 9, 12);
    IVector3 result = v * scalar;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (6, 9, 12), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::IVectorScalarMultiplication", isEqual, message);
}

inline TestResult IVectorScalarDivision()
{
    IVector3 v(6, 9, 12);
    int scalar = 3;
    IVector3 expected(2, 3, 4);
    IVector3 result = v / scalar;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (2, 3, 4), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::IVectorScalarDivision", isEqual, message);
}

inline TestResult IVectorDotProduct()
{
    IVector3 v1(1, 3, -5);
    IVector3 v2(4, -2, -1);
    int expected = 3;
    int result = v1.Dot(v2);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 3, Actual: " + std::to_string(result);
    return TestResult("Vector3Tests::IVectorDotProduct", isEqual, message);
}

inline TestResult IVectorCrossProduct()
{
    IVector3 v1(1, 2, 3);
    IVector3 v2(4, 5, 6);
    IVector3 expected(-3, 6, -3);
    IVector3 result = v1.Cross(v2);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (-3, 6, -3), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::IVectorCrossProduct", isEqual, message);
}

inline TestResult IVectorLength()
{
    IVector3 v(3, 4, 12);
    float expected = 13.0f;
    float result = v.Length();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 13.0, Actual: " + std::to_string(result);
    return TestResult("Vector3Tests::IVectorLength", isEqual, message);
}

inline TestResult IVectorLengthSquared()
{
    IVector3 v(3, 4, 12);
    float expected = 169.0f;
    float result = v.LengthSquared();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 169.0, Actual: " + std::to_string(result);
    return TestResult("Vector3Tests::IVectorLengthSquared", isEqual, message);
}



inline TestResult IVectorClamp()
{
    IVector3 v(5, -3, 10);
    int min = 0;
    int max = 4;
    IVector3 expected(4, 0, 4);
    IVector3 result = v.Clamp(min, max);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (4, 0, 4), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ")";
    return TestResult("Vector3Tests::IVectorClamp", isEqual, message);
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
} // namespace Vector3Tests

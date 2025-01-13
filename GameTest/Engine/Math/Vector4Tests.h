#pragma once
#include "Engine/Core/TestResult.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Vector3Tests.h" // Including Vector3Tests for consistency
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
                                  : "Expected: (6.0, 8.0, 10.0, 12.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
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
                                  : "Expected: (9.0, 6.0, 3.0, 0.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
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
                                  : "Expected: (6.0, 9.0, 12.0, 15.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
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
                                  : "Expected: (2.0, 3.0, 4.0, 5.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
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
    bool isEqual = (std::abs(result - expected) < 1e-4f);
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
    bool isEqual = (std::abs(result.x - expected.x) < 1e-5f) && (std::abs(result.y - expected.y) < 1e-5f) &&
                   (std::abs(result.z - expected.z) < 1e-5f) && (std::abs(result.w - expected.w) < 1e-5f);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (0.0, 0.6, 0.8, 0.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
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
                                  : "Expected: (5.0, 0.0, 6.0, 6.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
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
                                  : "Expected: (3.0, 0.0, 0.0, 0.0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
    return TestResult("Vector4Tests::FVectorProject", isEqual, message);
}

inline TestResult IVectorAddition()
{
    IVector4 v1(1, 2, 3, 4);
    IVector4 v2(5, 6, 7, 8);
    IVector4 expected(6, 8, 10, 12);
    IVector4 result = v1 + v2;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (6, 8, 10, 12), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
    return TestResult("Vector4Tests::IVectorAddition", isEqual, message);
}

inline TestResult IVectorSubtraction()
{
    IVector4 v1(10, 8, 6, 4);
    IVector4 v2(1, 2, 3, 4);
    IVector4 expected(9, 6, 3, 0);
    IVector4 result = v1 - v2;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (9, 6, 3, 0), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
    return TestResult("Vector4Tests::IVectorSubtraction", isEqual, message);
}

inline TestResult IVectorScalarMultiplication()
{
    IVector4 v(2, 3, 4, 5);
    int scalar = 3;
    IVector4 expected(6, 9, 12, 15);
    IVector4 result = v * scalar;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (6, 9, 12, 15), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
    return TestResult("Vector4Tests::IVectorScalarMultiplication", isEqual, message);
}

inline TestResult IVectorScalarDivision()
{
    IVector4 v(6, 9, 12, 15);
    int scalar = 3;
    IVector4 expected(2, 3, 4, 5);
    IVector4 result = v / scalar;
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (2, 3, 4, 5), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
    return TestResult("Vector4Tests::IVectorScalarDivision", isEqual, message);
}

inline TestResult IVectorDotProduct()
{
    IVector4 v1(1, 3, -5, 2);
    IVector4 v2(4, -2, -1, 3);
    int expected = 9;
    int result = v1.Dot(v2);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 9, Actual: " + std::to_string(result);
    return TestResult("Vector4Tests::IVectorDotProduct", isEqual, message);
}

inline TestResult IVectorLength()
{
    IVector4 v(3, 4, 12, 0);
    float expected = 13.0f;
    float result = v.Length();
    bool isEqual = (std::abs(result - expected) < 1e-5f);
    std::string message = isEqual ? "Passed" : "Expected: 13.0, Actual: " + std::to_string(result);
    return TestResult("Vector4Tests::IVectorLength", isEqual, message);
}

inline TestResult IVectorLengthSquared()
{
    IVector4 v(3, 4, 12, 0);
    float expected = 169.0f;
    float result = v.LengthSquared();
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed" : "Expected: 169.0, Actual: " + std::to_string(result);
    return TestResult("Vector4Tests::IVectorLengthSquared", isEqual, message);
}

inline TestResult IVectorClamp()
{
    IVector4 v(5, -3, 10, 7);
    int min = 0;
    int max = 6;
    IVector4 expected(5, 0, 6, 6);
    IVector4 result = v.Clamp(min, max);
    bool isEqual = (result == expected);
    std::string message = isEqual ? "Passed"
                                  : "Expected: (5, 0, 6, 6), Actual: (" + std::to_string(result.x) + ", " +
                                        std::to_string(result.y) + ", " + std::to_string(result.z) + ", " +
                                        std::to_string(result.w) + ")";
    return TestResult("Vector4Tests::IVectorClamp", isEqual, message);
}

inline std::vector<TestResult> RunAllTests()
{
    return {FVectorAddition(),
            FVectorSubtraction(),
            FVectorScalarMultiplication(),
            FVectorScalarDivision(),
            FVectorDotProduct(),
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
            IVectorLength(),
            IVectorLengthSquared(),
            IVectorClamp()};
}
} // namespace Vector4Tests

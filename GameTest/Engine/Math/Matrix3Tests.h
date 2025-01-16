#pragma once

#include "Engine/Core/TestResult.h"
#include "Engine/Math/Matrix3.h"
#include "Engine/Math/Vector3.h"
#include <vector>
#include <sstream>

namespace Matrix3Tests
{
inline TestResult IdentityMatrix()
{
    Matrix3 identity;
    Matrix3 expected;
    expected.SetIdentity();
    bool isEqual = (identity == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: Expected identity matrix, but got:";
        for (int i = 0; i < 9; ++i)
        {
            oss << " [" << i / 3 << "][" << i % 3 << "]=" << identity.m[i];
        }
    }
    return TestResult("Matrix3Tests::IdentityMatrix", isEqual, oss.str());
}

inline TestResult MatrixMultiplication()
{
    Matrix3 a;
    a.Set(0, 0, 1.0f);
    a.Set(0, 1, 2.0f);
    a.Set(1, 0, 3.0f);
    a.Set(1, 1, 4.0f);
    a.Set(2, 2, 1.0f);

    Matrix3 b;
    b.Set(0, 0, 5.0f);
    b.Set(0, 1, 6.0f);
    b.Set(1, 0, 7.0f);
    b.Set(1, 1, 8.0f);
    b.Set(2, 2, 1.0f);

    Matrix3 result = a * b;

    Matrix3 expected;
    expected.Set(0, 0, 19.0f);
    expected.Set(0, 1, 22.0f);
    expected.Set(1, 0, 43.0f);
    expected.Set(1, 1, 50.0f);
    expected.Set(2, 2, 1.0f);

    bool isEqual = (result == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                float expectedValue = expected.m[col * 3 + row];
                float actualValue = result.m[col * 3 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix3Tests::MatrixMultiplication", isEqual, oss.str());
}

inline TestResult InverseMatrix()
{
    Matrix3 matrix;
    matrix.Set(0, 0, 4.0f);
    matrix.Set(1, 1, 7.0f);
    matrix.Set(2, 2, 2.0f);

    Matrix3 inverse = matrix.Inverse();

    Matrix3 expected;
    expected.Set(0, 0, 0.25f);
    expected.Set(1, 1, 0.142857f);
    expected.Set(2, 2, 0.5f);

    bool isEqual = (inverse == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                float expectedValue = expected.m[col * 3 + row];
                float actualValue = inverse.m[col * 3 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix3Tests::InverseMatrix", isEqual, oss.str());
}

inline TestResult TransposeMatrix()
{
    Matrix3 matrix;
    matrix.Set(0, 1, 2.0f);
    matrix.Set(1, 0, 3.0f);
    matrix.Set(2, 2, 4.0f);
    Matrix3 transposed = matrix.Transpose();
    Matrix3 expected;
    expected.Set(0, 1, 3.0f);
    expected.Set(1, 0, 2.0f);
    expected.Set(2, 2, 4.0f);
    bool isEqual = (transposed == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                float expectedValue = expected.m[col * 3 + row];
                float actualValue = transposed.m[col * 3 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix3Tests::TransposeMatrix", isEqual, oss.str());
}

inline TestResult GetRight()
{
    Matrix3 matrix;
    FVector3 right = matrix.GetRight();
    FVector3 expected(1.0f, 0.0f, 0.0f);
    bool isEqual = (right == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (1.0, 0.0, 0.0), Actual: (" << right.x << ", " << right.y << ", " << right.z << ").";
    return TestResult("Matrix3Tests::GetRight", isEqual, oss.str());
}

inline TestResult GetUp()
{
    Matrix3 matrix;
    FVector3 up = matrix.GetUp();
    FVector3 expected(0.0f, 1.0f, 0.0f);
    bool isEqual = (up == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (0.0, 1.0, 0.0), Actual: (" << up.x << ", " << up.y << ", " << up.z << ").";
    return TestResult("Matrix3Tests::GetUp", isEqual, oss.str());
}

inline TestResult GetForward()
{
    Matrix3 matrix;
    FVector3 forward = matrix.GetForward();
    FVector3 expected(0.0f, 0.0f, 1.0f);
    bool isEqual = (forward == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (0.0, 0.0, 1.0), Actual: (" << forward.x << ", " << forward.y << ", " << forward.z
            << ").";
    return TestResult("Matrix3Tests::GetForward", isEqual, oss.str());
}

inline TestResult OperatorMultiplyFVector3()
{
    Matrix3 matrix;
    FVector3 obj(1.0f, 2.0f, 3.0f);
    FVector3 result = matrix * obj;
    FVector3 expected(1.0f, 2.0f, 3.0f);
    bool isEqual = (result == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (1.0, 2.0, 3.0), Actual: (" << result.x << ", " << result.y << ", " << result.z
            << ").";
    return TestResult("Matrix3Tests::OperatorMultiplyFVector3", isEqual, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {IdentityMatrix(), MatrixMultiplication(), InverseMatrix(),           TransposeMatrix(), GetRight(),
            GetUp(),          GetForward(),           OperatorMultiplyFVector3()};
}
} // namespace Matrix3Tests

#pragma once

#include "Game/Core/TestResult.h"
#include "Game/Math/Matrix4.h"
#include "Game/Math/Vector3.h"
#include "Game/Math/Vector4.h"
#include <vector>
#include <sstream>

namespace Matrix4Tests
{
inline TestResult IdentityMatrix()
{
    Matrix4 identity;
    Matrix4 expected;
    bool isEqual = (identity == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: Expected identity matrix, but got:";
        for (int i = 0; i < 16; ++i)
        {
            oss << " [" << i / 4 << "][" << i % 4 << "]=" << identity.m[i];
        }
    }
    return TestResult("Matrix4Tests::IdentityMatrix", isEqual, oss.str());
}

inline TestResult MatrixMultiplication()
{
    Matrix4 a;
    a.Set(0, 0, 1.0f);
    a.Set(0, 1, 2.0f);
    a.Set(1, 0, 3.0f);
    a.Set(1, 1, 4.0f);
    a.Set(2, 2, 1.0f);
    a.Set(3, 3, 1.0f);

    Matrix4 b;
    b.Set(0, 0, 5.0f);
    b.Set(0, 1, 6.0f);
    b.Set(1, 0, 7.0f);
    b.Set(1, 1, 8.0f);
    b.Set(2, 2, 1.0f);
    b.Set(3, 3, 1.0f);

    Matrix4 result = a * b;

    Matrix4 expected;
    expected.Set(0, 0, 19.0f);
    expected.Set(0, 1, 22.0f);
    expected.Set(1, 0, 43.0f);
    expected.Set(1, 1, 50.0f);
    expected.Set(2, 2, 1.0f);
    expected.Set(3, 3, 1.0f);

    bool isEqual = (result == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                float expectedValue = expected.m[col * 4 + row];
                float actualValue = result.m[col * 4 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix4Tests::MatrixMultiplication", isEqual, oss.str());
}

inline TestResult InverseMatrix()
{
    Matrix4 matrix;
    matrix.Set(0, 0, 4.0f);
    matrix.Set(1, 1, 7.0f);
    matrix.Set(2, 2, 2.0f);
    matrix.Set(3, 3, 1.0f);

    Matrix4 inverse = matrix.Inverse();

    Matrix4 expected;
    expected.Set(0, 0, 0.25f);
    expected.Set(1, 1, 0.142857f);
    expected.Set(2, 2, 0.5f);
    expected.Set(3, 3, 1.0f);

    bool isEqual = (inverse == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                float expectedValue = expected.m[col * 4 + row];
                float actualValue = inverse.m[col * 4 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix4Tests::InverseMatrix", isEqual, oss.str());
}

inline TestResult TransposeMatrix()
{
    Matrix4 matrix;
    matrix.Set(0, 1, 2.0f);
    matrix.Set(1, 0, 3.0f);
    matrix.Set(2, 3, 4.0f);
    matrix.Set(3, 2, 5.0f);
    Matrix4 transposed = matrix.Transpose();
    Matrix4 expected;
    expected.Set(0, 1, 3.0f);
    expected.Set(1, 0, 2.0f);
    expected.Set(2, 3, 5.0f);
    expected.Set(3, 2, 4.0f);
    bool isEqual = (transposed == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                float expectedValue = expected.m[col * 4 + row];
                float actualValue = transposed.m[col * 4 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix4Tests::TransposeMatrix", isEqual, oss.str());
}

inline TestResult CreateTranslationMatrix()
{
    FVector3 translation(5.0f, -3.0f, 2.0f);
    Matrix4 translationMatrix = Matrix4::CreateTranslationMatrix(translation);
    Matrix4 expected;
    expected.Set(0, 3, 5.0f);
    expected.Set(1, 3, -3.0f);
    expected.Set(2, 3, 2.0f);
    bool isEqual = (translationMatrix == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                float expectedValue = expected.m[col * 4 + row];
                float actualValue = translationMatrix.m[col * 4 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix4Tests::CreateTranslationMatrix", isEqual, oss.str());
}

inline TestResult CreateScaleMatrix()
{
    FVector3 scale(2.0f, 3.0f, 4.0f);
    Matrix4 scaleMatrix = Matrix4::CreateScaleMatrix(scale);
    Matrix4 expected;
    expected.Set(0, 0, 2.0f);
    expected.Set(1, 1, 3.0f);
    expected.Set(2, 2, 4.0f);
    bool isEqual = (scaleMatrix == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                float expectedValue = expected.m[col * 4 + row];
                float actualValue = scaleMatrix.m[col * 4 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix4Tests::CreateScaleMatrix", isEqual, oss.str());
}

inline TestResult CreatePerspectiveMatrix()
{
    float fov = 90.0f;
    float aspectRatio = 16.0f / 9.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;
    Matrix4 perspectiveMatrix = Matrix4::CreatePerspectiveMatrix(fov, aspectRatio, zNear, zFar);
    Matrix4 expected;
    float radFov = 3.14159265358979323846f / 2.0f;
    float tanHalfFOV = tanf(radFov / 2.0f);
    expected.Set(0, 0, 1.0f / (aspectRatio * tanHalfFOV));
    expected.Set(1, 1, 1.0f / tanHalfFOV);
    expected.Set(2, 2, -(zFar + zNear) / (zFar - zNear));
    expected.Set(2, 3, -1.0f);
    expected.Set(3, 2, -(2.0f * zFar * zNear) / (zFar - zNear));
    expected.Set(3, 3, 0.0f);
    bool isEqual = (perspectiveMatrix == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                float expectedValue = expected.m[col * 4 + row];
                float actualValue = perspectiveMatrix.m[col * 4 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix4Tests::CreatePerspectiveMatrix", isEqual, oss.str());
}

inline TestResult CreateViewMatrix()
{
    FVector3 origin(0.0f, 0.0f, 0.0f);
    FVector3 target(0.0f, 0.0f, -1.0f);
    FVector3 up(0.0f, 1.0f, 0.0f);
    Matrix4 viewMatrix = Matrix4::CreateViewMatrix(origin, target, up);
    Matrix4 expected;
    expected.Set(0, 0, 1.0f);
    expected.Set(1, 1, 1.0f);
    expected.Set(2, 2, 1.0f);
    expected.Set(3, 3, 1.0f);
    expected.Set(2, 3, 1.0f);
    bool isEqual = (viewMatrix == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
    {
        oss << "Failed: ";
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                float expectedValue = expected.m[col * 4 + row];
                float actualValue = viewMatrix.m[col * 4 + row];
                if (expectedValue != actualValue)
                {
                    oss << "Expected [" << row << "][" << col << "]=" << expectedValue << ", Actual=[" << row << "]["
                        << col << "]=" << actualValue << "; ";
                }
            }
        }
    }
    return TestResult("Matrix4Tests::CreateViewMatrix", isEqual, oss.str());
}

inline TestResult GetRight()
{
    Matrix4 matrix;
    FVector3 right = matrix.GetRight();
    FVector3 expected(1.0f, 0.0f, 0.0f);
    bool isEqual = (right == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (1.0, 0.0, 0.0), Actual: (" << right.x << ", " << right.y << ", " << right.z << ").";
    return TestResult("Matrix4Tests::GetRight", isEqual, oss.str());
}

inline TestResult GetUp()
{
    Matrix4 matrix;
    FVector3 up = matrix.GetUp();
    FVector3 expected(0.0f, 1.0f, 0.0f);
    bool isEqual = (up == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (0.0, 1.0, 0.0), Actual: (" << up.x << ", " << up.y << ", " << up.z << ").";
    return TestResult("Matrix4Tests::GetUp", isEqual, oss.str());
}

inline TestResult GetForward()
{
    Matrix4 matrix;
    FVector3 forward = matrix.GetForward();
    FVector3 expected(0.0f, 0.0f, 1.0f);
    bool isEqual = (forward == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (0.0, 0.0, 1.0), Actual: (" << forward.x << ", " << forward.y << ", " << forward.z
            << ").";
    return TestResult("Matrix4Tests::GetForward", isEqual, oss.str());
}

inline TestResult OperatorMultiplyFVector3()
{
    Matrix4 matrix;
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
    return TestResult("Matrix4Tests::OperatorMultiplyFVector3", isEqual, oss.str());
}

inline TestResult OperatorMultiplyFVector4()
{
    Matrix4 matrix;
    FVector4 obj(1.0f, 2.0f, 3.0f, 1.0f);
    FVector4 result = matrix * obj;
    FVector4 expected(1.0f, 2.0f, 3.0f, 1.0f);
    bool isEqual = (result == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (1.0, 2.0, 3.0, 1.0), Actual: (" << result.x << ", " << result.y << ", " << result.z
            << ", " << result.w << ").";
    return TestResult("Matrix4Tests::OperatorMultiplyFVector4", isEqual, oss.str());
}

inline TestResult TransformWithPerspectiveDivide()
{
    Matrix4 matrix;
    FVector3 obj(1.0f, 2.0f, 3.0f);
    FVector3 result = matrix.TransformWithPerspectiveDivide(obj);
    FVector3 expected(1.0f, 2.0f, 3.0f);
    bool isEqual = (result == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (1.0, 2.0, 3.0), Actual: (" << result.x << ", " << result.y << ", " << result.z
            << ").";
    return TestResult("Matrix4Tests::TransformWithPerspectiveDivide", isEqual, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {IdentityMatrix(),
            MatrixMultiplication(),
            InverseMatrix(),
            TransposeMatrix(),
            CreateTranslationMatrix(),
            CreateScaleMatrix(),
            CreatePerspectiveMatrix(),
            CreateViewMatrix(),
            GetRight(),
            GetUp(),
            GetForward(),
            OperatorMultiplyFVector3(),
            OperatorMultiplyFVector4(),
            TransformWithPerspectiveDivide()};
}
} // namespace Matrix4Tests

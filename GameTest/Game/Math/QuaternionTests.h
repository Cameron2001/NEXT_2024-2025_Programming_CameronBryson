#pragma once
#include "Game/Core/TestResult.h"
#include "Game/Math/Quaternion.h"
#include "Game/Math/Vector3.h"
#include <vector>
#include <sstream>
#include <cmath>
#include "MathUtil.h"

namespace QuaternionTests
{
inline TestResult DefaultConstructor()
{
    Quaternion q;
    Quaternion expected(1.0f, 0.0f, 0.0f, 0.0f);
    bool isEqual = (q == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected default quaternion to be (1, 0, 0, 0), but got (" << q.w << ", " << q.x << ", " << q.y
            << ", " << q.z << ").";
    return TestResult("QuaternionTests::DefaultConstructor", isEqual, oss.str());
}

inline TestResult ParameterizedConstructor()
{
    Quaternion q(0.7071f, 0.7071f, 0.0f, 0.0f);
    bool isEqual = (q.w == 0.7071f) && (q.x == 0.7071f) && (q.y == 0.0f) && (q.z == 0.0f);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Quaternion parameters did not initialize correctly.";
    return TestResult("QuaternionTests::ParameterizedConstructor", isEqual, oss.str());
}

inline TestResult Conjugate()
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion conj = q.Conjugate();
    Quaternion expected(1.0f, -2.0f, -3.0f, -4.0f);
    bool isEqual = (conj == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected conjugate to be (1, -2, -3, -4), but got (" << conj.w << ", " << conj.x << ", "
            << conj.y << ", " << conj.z << ").";
    return TestResult("QuaternionTests::Conjugate", isEqual, oss.str());
}

inline TestResult Normalize()
{
    Quaternion q(0.0f, 3.0f, 4.0f, 0.0f);
    Quaternion normalized = q.Normalize();
    float length = normalized.Length();
    bool isUnit = std::abs(length - 1.0f) < MathUtil::EPSILON;
    std::ostringstream oss;
    if (isUnit)
        oss << "Passed.";
    else
        oss << "Failed: Expected normalized quaternion length to be 1, but got " << length << ".";
    return TestResult("QuaternionTests::Normalize", isUnit, oss.str());
}

inline TestResult Inverse()
{
    Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
    Quaternion inv = q.Inverse();
    Quaternion expected(0.0f, 0.0f, 0.0f, 1.0f);
    bool isEqual = (inv == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected inverse to be (0, 0, 0, 1), but got (" << inv.w << ", " << inv.x << ", " << inv.y
            << ", " << inv.z << ").";
    return TestResult("QuaternionTests::Inverse", isEqual, oss.str());
}

inline TestResult QuaternionMultiplication()
{
    Quaternion q1(1.0f, 0.0f, 1.0f, 0.0f);
    Quaternion q2(1.0f, 0.5f, 0.5f, 0.75f);
    Quaternion result = q1 * q2;
    Quaternion expected(0.25f, 1.25f, 1.0f, 1.75f);
    bool isEqual = (result == expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected multiplication result to be (0.25, 1.25, 1.0, 1.75), but got (" << result.w << ", "
            << result.x << ", " << result.y << ", " << result.z << ").";
    return TestResult("QuaternionTests::QuaternionMultiplication", isEqual, oss.str());
}

inline TestResult RotateVector()
{
    // Rotate vector (1, 0, 0) by 90 degrees around Z-axis
    Quaternion q = Quaternion::FromAxisAngle(FVector3(0.0f, 0.0f, 1.0f), static_cast<float>(PI));
    FVector3 vector(1.0f, 0.0f, 0.0f);
    FVector3 rotated = q.RotateVector3(vector);
    FVector3 expected(0.0f, 1.0f, 0.0f);
    bool isEqual = (rotated.x == expected.x) && (rotated.y == expected.y) && (rotated.z == expected.z);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected rotated vector to be (0, 1, 0), but got (" << rotated.x << ", " << rotated.y << ", "
            << rotated.z << ").";
    return TestResult("QuaternionTests::RotateVector", isEqual, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {DefaultConstructor(),
            ParameterizedConstructor(),
            Conjugate(),
            Normalize(),
            Inverse(),
            QuaternionMultiplication(),
            RotateVector()};
}

} // namespace QuaternionTests

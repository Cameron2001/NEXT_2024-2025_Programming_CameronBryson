#pragma once
#include "Engine/Core/TestResult.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Matrix4.h"
#include <vector>
#include <sstream>
#include <cmath>
#include <corecrt_math_defines.h>

namespace QuaternionTests
{
const float EPSILON = 1e-5f;

inline bool IsNearlyEqual(float a, float b, float epsilon = EPSILON)
{
    return std::fabs(a - b) < epsilon;
}

inline bool AreQuaternionsNearlyEqual(const Quaternion &q1, const Quaternion &q2, float epsilon = EPSILON)
{
    return IsNearlyEqual(q1.w, q2.w, epsilon) && IsNearlyEqual(q1.x, q2.x, epsilon) &&
           IsNearlyEqual(q1.y, q2.y, epsilon) && IsNearlyEqual(q1.z, q2.z, epsilon);
}

inline TestResult DefaultConstructor()
{
    Quaternion q;
    Quaternion expected(1.0f, 0.0f, 0.0f, 0.0f);
    bool isEqual = AreQuaternionsNearlyEqual(q, expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (1.0, 0.0, 0.0, 0.0), got (" << q.w << ", " << q.x << ", " << q.y << ", " << q.z
            << ").";
    return TestResult("QuaternionTests::DefaultConstructor", isEqual, oss.str());
}

inline TestResult ConstructorWithValues()
{
    Quaternion q(0.7071f, 0.0f, 0.7071f, 0.0f);
    Quaternion expected(0.7071f, 0.0f, 0.7071f, 0.0f);
    bool isEqual = AreQuaternionsNearlyEqual(q, expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (0.7071, 0.0, 0.7071, 0.0), got (" << q.w << ", " << q.x << ", " << q.y << ", " << q.z
            << ").";
    return TestResult("QuaternionTests::ConstructorWithValues", isEqual, oss.str());
}

inline TestResult ConstructorFromEulerAngles()
{
    // 90 degrees rotation around Y-axis
    FVector3 eulerAnglesXYZ(0.0f, static_cast<float>(M_PI) / 2.0f, 0.0f);
    Quaternion q(eulerAnglesXYZ);
    Quaternion expected(std::cos(static_cast<float>(M_PI) / 4.0f), 0.0f, std::sin(static_cast<float>(M_PI) / 4.0f),
                        0.0f);
    bool isEqual = AreQuaternionsNearlyEqual(q, expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (" << expected.w << ", " << expected.x << ", " << expected.y << ", " << expected.z
            << "), got (" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ").";
    return TestResult("QuaternionTests::ConstructorFromEulerAngles", isEqual, oss.str());
}

inline TestResult DotProduct()
{
    Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q2(0.0f, 1.0f, 0.0f, 0.0f);
    float expectedDot = 0.0f;
    float actualDot = q1.Dot(q2);
    bool isEqual = IsNearlyEqual(actualDot, expectedDot);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected Dot Product = " << expectedDot << ", got " << actualDot << ".";
    return TestResult("QuaternionTests::DotProduct", isEqual, oss.str());
}

inline TestResult Length()
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    float expectedLength = std::sqrt(1.0f + 4.0f + 9.0f + 16.0f); // sqrt(30)
    float actualLength = q.Length();
    bool isEqual = IsNearlyEqual(actualLength, expectedLength);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected Length = " << expectedLength << ", got " << actualLength << ".";
    return TestResult("QuaternionTests::Length", isEqual, oss.str());
}

inline TestResult Normalize()
{
    Quaternion q(0.0f, 3.0f, 0.0f, 4.0f);
    Quaternion normalized = q.Normalize();
    float length = q.Length();
    Quaternion expected(q.w / length, q.x / length, q.y / length, q.z / length);
    bool isEqual = AreQuaternionsNearlyEqual(normalized, expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (" << expected.w << ", " << expected.x << ", " << expected.y << ", " << expected.z
            << "), got (" << normalized.w << ", " << normalized.x << ", " << normalized.y << ", " << normalized.z
            << ").";
    return TestResult("QuaternionTests::Normalize", isEqual, oss.str());
}

inline TestResult Conjugate()
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion conjugate = q.Conjugate();
    Quaternion expected(1.0f, -2.0f, -3.0f, -4.0f);
    bool isEqual = AreQuaternionsNearlyEqual(conjugate, expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (" << expected.w << ", " << expected.x << ", " << expected.y << ", " << expected.z
            << "), got (" << conjugate.w << ", " << conjugate.x << ", " << conjugate.y << ", " << conjugate.z << ").";
    return TestResult("QuaternionTests::Conjugate", isEqual, oss.str());
}

inline TestResult QuaternionMultiplication()
{
    Quaternion q1(1.0f, 0.0f, 1.0f, 0.0f);
    Quaternion q2(1.0f, 0.5f, 0.5f, 0.75f);
    Quaternion product = q1 * q2;

    Quaternion expected(0.5f, 1.25f, 1.5f, 0.25f);
    bool isEqual = AreQuaternionsNearlyEqual(product, expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (" << expected.w << ", " << expected.x << ", " << expected.y << ", " << expected.z
            << "), got (" << product.w << ", " << product.x << ", " << product.y << ", " << product.z << ").";
    return TestResult("QuaternionTests::QuaternionMultiplication", isEqual, oss.str());
}

inline TestResult Slerp()
{
    Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q2(0.0f, 1.0f, 0.0f, 0.0f);
    float t = 0.5f;
    Quaternion result = Quaternion::Slerp(q1, q2, t);

    float sqrt2_over_2 = std::sqrt(2.0f) / 2.0f;
    Quaternion expected(sqrt2_over_2, sqrt2_over_2, 0.0f, 0.0f);
    bool isEqual = AreQuaternionsNearlyEqual(result.Normalize(), expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (" << expected.w << ", " << expected.x << ", " << expected.y << ", " << expected.z
            << "), got (" << result.w << ", " << result.x << ", " << result.y << ", " << result.z << ").";
    return TestResult("QuaternionTests::Slerp", isEqual, oss.str());
}

inline TestResult RotateFVector3()
{

    FVector3 v(1.0f, 0.0f, 0.0f);
    FVector3 expected(0.0f, 1.0f, 0.0f);

    float angle = static_cast<float>(M_PI) / 2.0f;
    Quaternion q(std::cos(angle / 2.0f), 0.0f, 0.0f, std::sin(angle / 2.0f));
    FVector3 rotated = q * v;
    bool isEqual = IsNearlyEqual(rotated.x, expected.x) && IsNearlyEqual(rotated.y, expected.y) &&
                   IsNearlyEqual(rotated.z, expected.z);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (" << expected.x << ", " << expected.y << ", " << expected.z << "), got (" << rotated.x
            << ", " << rotated.y << ", " << rotated.z << ").";
    return TestResult("QuaternionTests::RotateFVector3", isEqual, oss.str());
}

inline TestResult GetEulerAnglesXYZ()
{

    FVector3 originalEuler(0.5f, 1.0f, 1.5f); // radians
    Quaternion q(originalEuler);
    FVector3 retrievedEuler = q.GetEulerAnglesXYZ();

    bool isEqual = IsNearlyEqual(originalEuler.x, retrievedEuler.x) &&
                   IsNearlyEqual(originalEuler.y, retrievedEuler.y) && IsNearlyEqual(originalEuler.z, retrievedEuler.z);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Original Euler angles (" << originalEuler.x << ", " << originalEuler.y << ", "
            << originalEuler.z << "), Retrieved Euler angles (" << retrievedEuler.x << ", " << retrievedEuler.y << ", "
            << retrievedEuler.z << ").";
    return TestResult("QuaternionTests::GetEulerAnglesXYZ", isEqual, oss.str());
}

inline TestResult NormalizeZeroQuaternion()
{
    Quaternion q(0.0f, 0.0f, 0.0f, 0.0f);
    Quaternion normalized = q.Normalize();
    Quaternion expected(1.0f, 0.0f, 0.0f, 0.0f);
    bool isEqual = AreQuaternionsNearlyEqual(normalized, expected);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Expected (" << expected.w << ", " << expected.x << ", " << expected.y << ", " << expected.z
            << "), got (" << normalized.w << ", " << normalized.x << ", " << normalized.y << ", " << normalized.z
            << ").";
    return TestResult("QuaternionTests::NormalizeZeroQuaternion", isEqual, oss.str());
}

inline TestResult NegativeQuaternionEquivalence()
{
    Quaternion q(0.7071f, 0.0f, 0.7071f, 0.0f);
    Quaternion negQ(-q.w, -q.x, -q.y, -q.z);
    FVector3 v(1.0f, 0.0f, 0.0f);
    FVector3 rotatedQ = q * v;
    FVector3 rotatedNegQ = negQ * v;
    bool isEqual = IsNearlyEqual(rotatedQ.x, rotatedNegQ.x) && IsNearlyEqual(rotatedQ.y, rotatedNegQ.y) &&
                   IsNearlyEqual(rotatedQ.z, rotatedNegQ.z);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Rotating with q and -q did not yield same results. Rotated q: (" << rotatedQ.x << ", "
            << rotatedQ.y << ", " << rotatedQ.z << "), Rotated -q: (" << rotatedNegQ.x << ", " << rotatedNegQ.y << ", "
            << rotatedNegQ.z << ").";
    return TestResult("QuaternionTests::NegativeQuaternionEquivalence", isEqual, oss.str());
}

inline TestResult RepeatedRotations()
{
    float angle = static_cast<float>(M_PI) / 2.0f; // 90 degrees
    Quaternion q(std::cos(angle / 2.0f), 0.0f, 0.0f, std::sin(angle / 2.0f));

    FVector3 v(1.0f, 0.0f, 0.0f);
    FVector3 rotatedOnce = q * v;
    FVector3 rotatedTwice = q * rotatedOnce;

    FVector3 expected(-1.0f, 0.0f, 0.0f);

    bool isEqual = IsNearlyEqual(rotatedTwice.x, expected.x) && IsNearlyEqual(rotatedTwice.y, expected.y) &&
                   IsNearlyEqual(rotatedTwice.z, expected.z);

    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Failed: Rotating (1,0,0) twice by 90 degrees around Z-axis should result in (-1,0,0), got ("
            << rotatedTwice.x << ", " << rotatedTwice.y << ", " << rotatedTwice.z << ").";
    return TestResult("QuaternionTests::RepeatedRotations", isEqual, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {DefaultConstructor(),
            ConstructorWithValues(),
            ConstructorFromEulerAngles(),
            DotProduct(),
            Length(),
            Normalize(),
            Conjugate(),
            QuaternionMultiplication(),
            Slerp(),
            RotateFVector3(),
            GetEulerAnglesXYZ(),
            NormalizeZeroQuaternion(),
            NegativeQuaternionEquivalence(),
            RepeatedRotations()};
}
} // namespace QuaternionTests

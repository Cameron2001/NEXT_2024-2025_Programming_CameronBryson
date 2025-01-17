#pragma once
#include <cmath>
#include "App/AppSettings.h"
#include "Vector2.h"
#include "Vector3.h"

namespace MathUtil
{
constexpr float EPSILON = 1e-5f;
inline float DegreesToRadians(const float degrees)
{
    return degrees * (PI / 180.0f);
}

// Converts radians to degrees
inline float RadiansToDegrees(const float radians)
{
    return radians * (180.0f / PI);
}
// Clamps a value between a minimum and maximum range
template <typename T> T Clamp(const T &value, const T &minVal, const T &maxVal)
{
    return (value < minVal) ? minVal : ((value > maxVal) ? maxVal : value);
}
inline float Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

// Linear interpolation for FVector2
inline FVector2 Lerp(const FVector2 &a, const FVector2 &b, float t)
{
    return a + (b - a) * t;
}

// Linear interpolation for FVector3
inline FVector3 Lerp(const FVector3 &a, const FVector3 &b, float t)
{
    return a + (b - a) * t;
}
} // namespace MathUtil

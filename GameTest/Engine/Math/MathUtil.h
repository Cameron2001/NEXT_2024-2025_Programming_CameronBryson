#pragma once
#include <cmath>

namespace MathUtil
{
constexpr float PI = 3.14159265358979323846f;

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
} // namespace MathUtil
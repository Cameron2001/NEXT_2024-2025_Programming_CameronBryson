#pragma once
#include <cmath>
namespace MathUtil
{
	constexpr float PI = 3.14159265358979323846f;

	inline float DegreesToRadians(float degrees)
	{
		return degrees * (PI / 180.0f);
	}

	// Converts radians to degrees
	inline float RadiansToDegrees(float radians)
	{
		return radians * (180.0f / PI);
	}
}
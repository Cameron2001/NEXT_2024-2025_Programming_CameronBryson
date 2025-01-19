#pragma once
#include "Game/Core/TestResult.h"
#include "Game/Math/Quaternion.h"
#include "Game/Math/Vector3.h"
#include "Game/Math/Matrix4.h"
#include <vector>
#include <sstream>
#include <cmath>
#include <corecrt_math_defines.h>

namespace QuaternionTests
{
const float EPSILON = 1e-5f;


inline std::vector<TestResult> RunAllTests()
{
    return {};
}
} // namespace QuaternionTests

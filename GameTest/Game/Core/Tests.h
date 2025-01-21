#pragma once
#include "Game/Math/OctreeTests.h"
#include "Game/Math/BoundingBoxTests.h"
#include "Game/Math/Matrix4Tests.h"
#include "Game/Math/QuadtreeTests.h"
#include <vector>
#include "TestResult.h"
#include <Game/Math/Vector2Tests.h>
#include <Game/Math/Vector3Tests.h>
#include <Game/Math/Vector4Tests.h>
#include <Game/Math/BoundingSphereTests.h>
#include <Game/Math/QuaternionTests.h>
#include <Game/Math/Matrix3Tests.h>

inline std::vector<TestResult> RunTests()
{
    std::vector<TestResult> results;

    std::vector<TestResult> vector2Results = Vector2Tests::RunAllTests();
    results.insert(results.end(), vector2Results.begin(), vector2Results.end());

    std::vector<TestResult> vector3Results = Vector3Tests::RunAllTests();
    results.insert(results.end(), vector3Results.begin(), vector3Results.end());

    std::vector<TestResult> vector4Results = Vector4Tests::RunAllTests();
    results.insert(results.end(), vector4Results.begin(), vector4Results.end());


    std::vector<TestResult> matrix4Results = Matrix4Tests::RunAllTests();
    results.insert(results.end(), matrix4Results.begin(), matrix4Results.end());

    std::vector<TestResult> boundingBoxResults = BoundingBoxTests::RunAllTests();
    results.insert(results.end(), boundingBoxResults.begin(), boundingBoxResults.end());

    std::vector<TestResult> boundingSphereResults = BoundingSphereTests::RunAllTests();
    results.insert(results.end(), boundingSphereResults.begin(), boundingSphereResults.end());

    std::vector<TestResult> quadTreeResults = QuadtreeTests::RunAllTests();
    results.insert(results.end(), quadTreeResults.begin(), quadTreeResults.end());


    return results;
}

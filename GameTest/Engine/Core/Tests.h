#pragma once
#include "Engine/Math/OctreeTests.h"
#include "Engine/Math/BoundingBoxTests.h"
#include "Engine/Math/Matrix4Tests.h"
#include "Engine/Math/QuadtreeTests.h"
#include <vector>
#include "TestResult.h"
#include <Engine/Math/Vector2Tests.h>
#include <Engine/Math/Vector3Tests.h>
#include <Engine/Math/Vector4Tests.h>
#include <Engine/Math/BoundingSphereTests.h>
#include <Engine/Math/QuaternionTests.h>
#include <Engine/Math/Matrix3Tests.h>

inline std::vector<TestResult> RunTests()
{
    std::vector<TestResult> results;

    std::vector<TestResult> vector2Results = Vector2Tests::RunAllTests();
    results.insert(results.end(), vector2Results.begin(), vector2Results.end());

    std::vector<TestResult> vector3Results = Vector3Tests::RunAllTests();
    results.insert(results.end(), vector3Results.begin(), vector3Results.end());

    std::vector<TestResult> vector4Results = Vector4Tests::RunAllTests();
    results.insert(results.end(), vector4Results.begin(), vector4Results.end());

    std::vector<TestResult> quaternionResults = QuaternionTests::RunAllTests();
    results.insert(results.end(), quaternionResults.begin(), quaternionResults.end());

    std::vector<TestResult> matrix3Results = Matrix3Tests::RunAllTests();
    results.insert(results.end(), matrix3Results.begin(), matrix3Results.end());

    std::vector<TestResult> matrix4Results = Matrix4Tests::RunAllTests();
    results.insert(results.end(), matrix4Results.begin(), matrix4Results.end());

    std::vector<TestResult> boundingBoxResults = BoundingBoxTests::RunAllTests();
    results.insert(results.end(), boundingBoxResults.begin(), boundingBoxResults.end());

    std::vector<TestResult> boundingSphereResults = BoundingSphereTests::RunAllTests();
    results.insert(results.end(), boundingSphereResults.begin(), boundingSphereResults.end());

    std::vector<TestResult> quadTreeResults = QuadtreeTests::RunAllTests();
    results.insert(results.end(), quadTreeResults.begin(), quadTreeResults.end());

    std::vector<TestResult> octreeResults = OctreeTests::RunAllTests();
    results.insert(results.end(), octreeResults.begin(), octreeResults.end());

    return results;
}

#pragma once
#include "Engine/Core/TestResult.h"
#include "Engine/Math/BoundingSphere.h"
#include "Engine/Math/Vector3.h"
#include <vector>
#include <sstream>

namespace BoundingSphereTests
{
inline TestResult IntersectOverlap()
{
    BoundingSphere3D sphere1(FVector3(0.0f, 0.0f, 0.0f), 10.0f);
    BoundingSphere3D sphere2(FVector3(5.0f, 5.0f, 5.0f), 10.0f);
    bool intersects = sphere1.Intersects(sphere2);
    std::ostringstream oss;
    if (intersects)
        oss << "Passed.";
    else
        oss << "Expected: intersects=true. Actual: intersects=false.";
    return TestResult("BoundingSphereTests::IntersectOverlap", intersects, oss.str());
}

inline TestResult IntersectNoOverlap()
{
    BoundingSphere3D sphere1(FVector3(0.0f, 0.0f, 0.0f), 10.0f);
    BoundingSphere3D sphere2(FVector3(25.0f, 25.0f, 25.0f), 5.0f);
    bool intersects = sphere1.Intersects(sphere2);
    std::ostringstream oss;
    if (!intersects)
        oss << "Passed.";
    else
        oss << "Expected: intersects=false. Actual: intersects=true.";
    return TestResult("BoundingSphereTests::IntersectNoOverlap", !intersects, oss.str());
}

inline TestResult Contains()
{
    BoundingSphere3D outerSphere(FVector3(0.0f, 0.0f, 0.0f), 20.0f);
    BoundingSphere3D innerSphere(FVector3(5.0f, 5.0f, 5.0f), 10.0f);
    bool contains = outerSphere.Contains(innerSphere);
    std::ostringstream oss;
    if (contains)
        oss << "Passed.";
    else
        oss << "Expected: contains=true. Actual: contains=false.";
    return TestResult("BoundingSphereTests::Contains", contains, oss.str());
}

inline TestResult ContainsPointInside()
{
    BoundingSphere3D sphere(FVector3(0.0f, 0.0f, 0.0f), 10.0f);
    FVector3 point(5.0f, 5.0f, 5.0f);
    bool contains = sphere.ContainsPoint(point);
    std::ostringstream oss;
    if (contains)
        oss << "Passed.";
    else
        oss << "Expected: ContainsPoint=true. Actual: ContainsPoint=false.";
    return TestResult("BoundingSphereTests::ContainsPointInside", contains, oss.str());
}

inline TestResult ContainsPointOutside()
{
    BoundingSphere3D sphere(FVector3(0.0f, 0.0f, 0.0f), 10.0f);
    FVector3 point(15.0f, 15.0f, 15.0f);
    bool contains = sphere.ContainsPoint(point);
    std::ostringstream oss;
    if (!contains)
        oss << "Passed.";
    else
        oss << "Expected: ContainsPoint=false. Actual: ContainsPoint=true.";
    return TestResult("BoundingSphereTests::ContainsPointOutside", !contains, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {IntersectOverlap(), IntersectNoOverlap(), Contains(), ContainsPointInside(), ContainsPointOutside()};
}
} // namespace BoundingSphereTests

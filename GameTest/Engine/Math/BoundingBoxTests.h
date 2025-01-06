#pragma once

#include "Engine/Core/TestResult.h"
#include "Engine/Math/BoundingBox.h"
#include "Engine/Math/Vector3.h"
#include <vector>
#include <string>
#include <sstream>

namespace BoundingBoxTests
{
inline TestResult IntersectOverlap()
{
    BoundingBox3D box1(FVector3(0.0f, 0.0f, 0.0f), FVector3(10.0f, 10.0f, 10.0f));
    BoundingBox3D box2(FVector3(5.0f, 5.0f, 5.0f), FVector3(15.0f, 15.0f, 15.0f));
    bool intersects = box1.Intersects(box2);
    std::ostringstream oss;
    if (intersects)
        oss << "Passed.";
    else
        oss << "Expected: intersects=true. Actual: intersects=false.";
    return TestResult("BoundingBoxTests::IntersectOverlap", intersects, oss.str());
}

inline TestResult IntersectNoOverlap()
{
    BoundingBox3D box1(FVector3(0.0f, 0.0f, 0.0f), FVector3(10.0f, 10.0f, 10.0f));
    BoundingBox3D box2(FVector3(20.0f, 20.0f, 20.0f), FVector3(30.0f, 30.0f, 30.0f));
    bool intersects = box1.Intersects(box2);
    std::ostringstream oss;
    if (!intersects)
        oss << "Passed.";
    else
        oss << "Expected: intersects=false. Actual: intersects=true.";
    return TestResult("BoundingBoxTests::IntersectNoOverlap", !intersects, oss.str());
}

inline TestResult Contains()
{
    BoundingBox3D outerBox(FVector3(0.0f, 0.0f, 0.0f), FVector3(20.0f, 20.0f, 20.0f));
    BoundingBox3D innerBox(FVector3(5.0f, 5.0f, 5.0f), FVector3(15.0f, 15.0f, 15.0f));
    bool contains = outerBox.Contains(innerBox);
    std::ostringstream oss;
    if (contains)
        oss << "Passed.";
    else
        oss << "Expected: contains=true. Actual: contains=false.";
    return TestResult("BoundingBoxTests::Contains", contains, oss.str());
}

inline TestResult ContainsPointInside()
{
    BoundingBox3D box(FVector3(0.0f, 0.0f, 0.0f), FVector3(10.0f, 10.0f, 10.0f));
    FVector3 point(5.0f, 5.0f, 5.0f);
    bool contains = box.ContainsPoint(point);
    std::ostringstream oss;
    if (contains)
        oss << "Passed.";
    else
        oss << "Expected: containsPoint=true. Actual: containsPoint=false.";
    return TestResult("BoundingBoxTests::ContainsPointInside", contains, oss.str());
}

inline TestResult ContainsPointOutside()
{
    BoundingBox3D box(FVector3(0.0f, 0.0f, 0.0f), FVector3(10.0f, 10.0f, 10.0f));
    FVector3 point(15.0f, 15.0f, 15.0f);
    bool contains = box.ContainsPoint(point);
    std::ostringstream oss;
    if (!contains)
        oss << "Passed.";
    else
        oss << "Expected: containsPoint=false. Actual: containsPoint=true.";
    return TestResult("BoundingBoxTests::ContainsPointOutside", !contains, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {IntersectOverlap(), IntersectNoOverlap(), Contains(), ContainsPointInside(), ContainsPointOutside()};
}
} // namespace BoundingBoxTests

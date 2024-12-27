#pragma once

#include "Engine/Core/TestResult.h"
#include "Engine/Math/Octree.h"
#include "Engine/Math/BoundingBox.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Core/Components.h"
#include <vector>
#include <string>
#include <sstream>

namespace OctreeTests
{
inline TestResult InsertSphereCollider()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    Octree octree(rootBounds);
    SphereBoundsComponent sphere(10.0f);
    TransformComponent transform(FVector3(0.0f, 0.0f, 0.0f), FVector3(), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID = 1;
    bool inserted = octree.insert(sphere, transform, entityID);
    std::vector<unsigned int> results =
        octree.queryArea(BoundingBox3D(FVector3(-15.0f, -15.0f, -15.0f), FVector3(15.0f, 15.0f, 15.0f)));
    bool found = inserted && (std::find(results.begin(), results.end(), entityID) != results.end());
    std::ostringstream oss;
    if (found)
        oss << "Passed.";
    else
        oss << "Expected: Collider found. Actual: Not found.";
    return TestResult("OctreeTests::InsertSphereCollider", found, oss.str());
}

inline TestResult InsertBoxCollider()
{
    BoundingBox3D rootBounds(FVector3(-50.0f, -50.0f, -50.0f), FVector3(50.0f, 50.0f, 50.0f));
    Octree octree(rootBounds);
    BoxBoundsComponent box(FVector3(5.0f, 5.0f, 5.0f));
    TransformComponent transform(FVector3(10.0f, 10.0f, 10.0f), FVector3(), FVector3(2.0f, 2.0f, 2.0f));
    unsigned int entityID = 2;
    bool inserted = octree.insert(box, transform, entityID);
    std::vector<unsigned int> results =
        octree.queryArea(BoundingBox3D(FVector3(5.0f, 5.0f, 5.0f), FVector3(15.0f, 15.0f, 15.0f)));
    bool found = inserted && (std::find(results.begin(), results.end(), entityID) != results.end());
    std::ostringstream oss;
    if (found)
        oss << "Passed.";
    else
        oss << "Expected: Collider found. Actual: Not found.";
    return TestResult("OctreeTests::InsertBoxCollider", found, oss.str());
}

inline TestResult SubdivideAfterCapacity()
{
    BoundingBox3D rootBounds(FVector3(-50.0f, -50.0f, -50.0f), FVector3(50.0f, 50.0f, 50.0f));
    Octree octree(rootBounds, 2, 3);
    for (unsigned int i = 1; i <= 3; ++i)
    {
        SphereBoundsComponent sphere(5.0f);
        TransformComponent transform(FVector3(i * 10.0f, i * 10.0f, i * 10.0f), FVector3(), FVector3(1.0f, 1.0f, 1.0f));
        octree.insert(sphere, transform, i);
    }
    bool subdivided = octree.queryArea(rootBounds).size() >= 3;
    std::ostringstream oss;
    if (subdivided)
        oss << "Passed.";
    else
        oss << "Expected: Subdivided. Actual: Not subdivided.";
    return TestResult("OctreeTests::SubdivideAfterCapacity", subdivided, oss.str());
}

inline TestResult QuerySphere()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    Octree octree(rootBounds);
    SphereBoundsComponent sphere(10.0f);
    TransformComponent transform1(FVector3(0.0f, 0.0f, 0.0f), FVector3(), FVector3(1.0f, 1.0f, 1.0f));
    TransformComponent transform2(FVector3(20.0f, 20.0f, 20.0f), FVector3(), FVector3(1.0f, 1.0f, 1.0f));
    octree.insert(sphere, transform1, 1);
    octree.insert(sphere, transform2, 2);
    std::vector<unsigned int> results = octree.querySphere(sphere, transform1);
    bool found = (results.size() == 1) && (results[0] == 1);
    std::ostringstream oss;
    if (found)
        oss << "Passed.";
    else
        oss << "Expected: One collider found. Actual: Different number found.";
    return TestResult("OctreeTests::QuerySphere", found, oss.str());
}

inline TestResult QueryBox()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    Octree octree(rootBounds);
    BoxBoundsComponent box1(FVector3(5.0f, 5.0f, 5.0f));
    BoxBoundsComponent box2(FVector3(10.0f, 10.0f, 10.0f));
    TransformComponent transform1(FVector3(-10.0f, -10.0f, -10.0f), FVector3(), FVector3(1.0f, 1.0f, 1.0f));
    TransformComponent transform2(FVector3(10.0f, 10.0f, 10.0f), FVector3(), FVector3(1.0f, 1.0f, 1.0f));
    octree.insert(box1, transform1, 1);
    octree.insert(box2, transform2, 2);
    std::vector<unsigned int> results = octree.queryBox(box1, transform1);
    bool found = (results.size() == 1) && (results[0] == 1);
    std::ostringstream oss;
    if (found)
        oss << "Passed.";
    else
        oss << "Expected: One collider found. Actual: Different number found.";
    return TestResult("OctreeTests::QueryBox", found, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {InsertSphereCollider(), InsertBoxCollider(), SubdivideAfterCapacity(), QuerySphere(), QueryBox()};
}
} // namespace OctreeTests

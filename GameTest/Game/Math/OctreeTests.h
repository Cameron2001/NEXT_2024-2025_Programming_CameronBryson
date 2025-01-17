#pragma once
#include "Game/Core/TestResult.h"
#include "Game/Math/Octree.h"
#include "Game/Math/BoundingBox.h"
#include "Game/Math/Vector3.h"
#include "Game/Core/Components.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm> // For std::find
#include <set>       // For std::set

namespace OctreeTests
{

inline TestResult InsertSphereCollider()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    Octree octree(rootBounds);

    SphereBoundsComponent sphere(10.0f);
    TransformComponent transform(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID = 1;

    octree.Insert(sphere, transform, entityID);

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Since there's only one collider, there should be no potential collisions
    bool passed = potentialCollisions.empty();

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: No potential collisions. Actual: " << potentialCollisions.size() << " found.";
    return TestResult("OctreeTests::InsertSphereCollider", passed, oss.str());
}

inline TestResult InsertBoxCollider()
{
    BoundingBox3D rootBounds(FVector3(-50.0f, -50.0f, -50.0f), FVector3(50.0f, 50.0f, 50.0f));
    Octree octree(rootBounds);

    BoxBoundsComponent box(FVector3(5.0f, 5.0f, 5.0f));
    TransformComponent transform(FVector3(10.0f, 10.0f, 10.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(2.0f, 2.0f, 2.0f));
    unsigned int entityID = 2;

    octree.Insert(box, transform, entityID);

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Since there's only one collider, there should be no potential collisions
    bool passed = potentialCollisions.empty();

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: No potential collisions. Actual: " << potentialCollisions.size() << " found.";
    return TestResult("OctreeTests::InsertBoxCollider", passed, oss.str());
}

inline TestResult DetectPotentialCollisions()
{
    // Test with multiple colliders to check potential collisions
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    int capacity = 2; // Set capacity to 2 to force subdivision
    int maxDepth = 10;
    Octree octree(rootBounds, capacity, maxDepth);

    // Collider 1
    SphereBoundsComponent sphere1(10.0f);
    TransformComponent transform1(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID1 = 1;

    // Collider 2 (overlaps with collider 1)
    SphereBoundsComponent sphere2(10.0f);
    TransformComponent transform2(FVector3(5.0f, 5.0f, 5.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID2 = 2;

    // Collider 3 (does not overlap)
    SphereBoundsComponent sphere3(10.0f);
    TransformComponent transform3(FVector3(50.0f, 50.0f, 50.0f), FVector3(0.0f, 0.0f, 0.0f),
                                  FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID3 = 3;

    // Insert colliders
    octree.Insert(sphere1, transform1, entityID1);
    octree.Insert(sphere2, transform2, entityID2);
    octree.Insert(sphere3, transform3, entityID3);

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Expecting one potential collision between entityID1 and entityID2
    bool passed = (potentialCollisions.size() == 1) &&
                  ((potentialCollisions[0].first == entityID1 && potentialCollisions[0].second == entityID2) ||
                   (potentialCollisions[0].first == entityID2 && potentialCollisions[0].second == entityID1));

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: One potential collision between entities 1 and 2. Actual: " << potentialCollisions.size()
            << " collisions found.";
    return TestResult("OctreeTests::DetectPotentialCollisions", passed, oss.str());
}

inline TestResult SubdivideAfterCapacity()
{
    // Set capacity to 2 to force subdivision after inserting 3 colliders
    BoundingBox3D rootBounds(FVector3(-50.0f, -50.0f, -50.0f), FVector3(50.0f, 50.0f, 50.0f));
    int capacity = 2;
    int maxDepth = 3;
    Octree octree(rootBounds, capacity, maxDepth);

    // Insert 3 colliders to exceed capacity
    for (unsigned int i = 1; i <= 3; ++i)
    {
        SphereBoundsComponent sphere(5.0f);
        TransformComponent transform(FVector3(i * 10.0f, i * 10.0f, i * 10.0f), FVector3(0.0f, 0.0f, 0.0f),
                                     FVector3(1.0f, 1.0f, 1.0f));
        octree.Insert(sphere, transform, i);
    }

    // Since we don't have direct access to check subdivision, we'll check potential collisions
    // There should be potential collisions between colliders if subdivision and insertion are correct
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Verify that the correct number of potential collisions are detected
    // With 3 colliders, only 2 pairs overlap (assuming positions)
    bool passed = (potentialCollisions.size() == 2);

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: 2 potential collisions. Actual: " << potentialCollisions.size() << " collisions found.";

    return TestResult("OctreeTests::SubdivideAfterCapacity", passed, oss.str());
}

inline TestResult NoFalsePositives()
{
    // Ensure that non-overlapping colliders do not generate potential collisions
    BoundingBox3D rootBounds(FVector3(-200.0f, -200.0f, -200.0f), FVector3(200.0f, 200.0f, 200.0f));
    int capacity = 2; // Set capacity to 2 to force subdivision
    int maxDepth = 10;
    Octree octree(rootBounds, capacity, maxDepth);

    // Insert colliders far apart
    for (unsigned int i = 1; i <= 5; ++i)
    {
        SphereBoundsComponent sphere(5.0f);
        TransformComponent transform(FVector3(i * 50.0f, i * 50.0f, i * 50.0f), FVector3(0.0f, 0.0f, 0.0f),
                                     FVector3(1.0f, 1.0f, 1.0f));
        octree.Insert(sphere, transform, i);
    }

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Expecting zero potential collisions since colliders are far apart
    bool passed = potentialCollisions.empty();

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: No potential collisions. Actual: " << potentialCollisions.size() << " collisions found.";

    return TestResult("OctreeTests::NoFalsePositives", passed, oss.str());
}

inline TestResult OverlappingCollidersAcrossNodes()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    int capacity = 1; // Force subdivision quickly
    int maxDepth = 5;
    Octree octree(rootBounds, capacity, maxDepth);

    // Collider 1: Positioned at the center
    SphereBoundsComponent sphere1(30.0f);
    TransformComponent transform1(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID1 = 1;
    octree.Insert(sphere1, transform1, entityID1);

    // Collider 2: Overlaps multiple child nodes
    SphereBoundsComponent sphere2(30.0f);
    TransformComponent transform2(FVector3(25.0f, 25.0f, 25.0f), FVector3(0.0f, 0.0f, 0.0f),
                                  FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID2 = 2;
    octree.Insert(sphere2, transform2, entityID2);

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Expecting one potential collision between entityID1 and entityID2
    bool passed = (potentialCollisions.size() == 1) &&
                  ((potentialCollisions[0].first == entityID1 && potentialCollisions[0].second == entityID2) ||
                   (potentialCollisions[0].first == entityID2 && potentialCollisions[0].second == entityID1));

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: One potential collision between entities 1 and 2. Actual: " << potentialCollisions.size()
            << " collisions found.";

    return TestResult("OctreeTests::OverlappingCollidersAcrossNodes", passed, oss.str());
}

inline TestResult CollidersOnNodeBoundaries()
{
    BoundingBox3D rootBounds(FVector3(-50.0f, -50.0f, -50.0f), FVector3(50.0f, 50.0f, 50.0f));
    int capacity = 1; // Force subdivision
    int maxDepth = 5;
    Octree octree(rootBounds, capacity, maxDepth);

    // Define the midpoint
    float mid = 0.0f;

    // Collider 1: Exactly on the positive boundary of the root
    SphereBoundsComponent sphere1(10.0f);
    TransformComponent transform1(FVector3(50.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID1 = 1;
    octree.Insert(sphere1, transform1, entityID1);

    // Collider 2: Exactly on the negative boundary of the root
    SphereBoundsComponent sphere2(10.0f);
    TransformComponent transform2(FVector3(-50.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID2 = 2;
    octree.Insert(sphere2, transform2, entityID2);

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Expecting no potential collisions
    bool passed = potentialCollisions.empty();

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: No potential collisions. Actual: " << potentialCollisions.size() << " collisions found.";

    return TestResult("OctreeTests::CollidersOnNodeBoundaries", passed, oss.str());
}

inline TestResult MultipleCollidersInSingleNode()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    int capacity = 5; // High capacity to prevent subdivision
    int maxDepth = 5;
    Octree octree(rootBounds, capacity, maxDepth);

    // Insert multiple colliders within the same region
    unsigned int numColliders = 5;
    for (unsigned int i = 1; i <= numColliders; ++i)
    {
        SphereBoundsComponent sphere(10.0f);
        TransformComponent transform(FVector3(i * 10.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f),
                                     FVector3(1.0f, 1.0f, 1.0f));
        octree.Insert(sphere, transform, i);
    }

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Adjust expected collisions to include touching pairs
    // For simplicity, assuming that each collider overlaps with its immediate neighbor
    // Expected unique pairs: (1,2), (2,3), (3,4), (4,5)
    std::set<std::pair<unsigned int, unsigned int>> expectedPairs = {{1, 2}, {2, 3}, {3, 4}, {4, 5}};

    // Convert potential collisions to a set for comparison
    std::set<std::pair<unsigned int, unsigned int>> collisionSet(potentialCollisions.begin(),
                                                                 potentialCollisions.end());

    bool passed = (collisionSet == expectedPairs);

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: " << expectedPairs.size() << " unique collision pairs. Actual: " << potentialCollisions.size()
            << " collisions found.";
    return TestResult("OctreeTests::MultipleCollidersInSingleNode", passed, oss.str());
}

inline TestResult NonOverlappingAdjacentColliders()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    int capacity = 1; // Force subdivision
    int maxDepth = 5;
    Octree octree(rootBounds, capacity, maxDepth);

    // Collider 1: In one child node
    SphereBoundsComponent sphere1(10.0f);
    TransformComponent transform1(FVector3(-50.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID1 = 1;
    octree.Insert(sphere1, transform1, entityID1);

    // Collider 2: In an adjacent child node, no overlap
    SphereBoundsComponent sphere2(10.0f);
    TransformComponent transform2(FVector3(50.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID2 = 2;
    octree.Insert(sphere2, transform2, entityID2);

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Expecting no potential collisions
    bool passed = potentialCollisions.empty();

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected: No potential collisions. Actual: " << potentialCollisions.size() << " collisions found.";

    return TestResult("OctreeTests::NonOverlappingAdjacentColliders", passed, oss.str());
}

//////////////////////////////
// New Tests for Duplicate Pairs
//////////////////////////////

inline TestResult NoDuplicatePairsDifferentOrder()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    Octree octree(rootBounds);

    // Collider 1
    SphereBoundsComponent sphere1(10.0f);
    TransformComponent transform1(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID1 = 1;
    octree.Insert(sphere1, transform1, entityID1);

    // Collider 2
    SphereBoundsComponent sphere2(10.0f);
    TransformComponent transform2(FVector3(5.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID2 = 2;
    octree.Insert(sphere2, transform2, entityID2);

    // Collider 3 (overlapping with both Collider 1 and Collider 2)
    SphereBoundsComponent sphere3(10.0f);
    TransformComponent transform3(FVector3(2.5f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID3 = 3;
    octree.Insert(sphere3, transform3, entityID3);

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Expected unique pairs: (1,2), (1,3), (2,3)
    std::set<std::pair<unsigned int, unsigned int>> expectedPairs = {{1, 2}, {1, 3}, {2, 3}};

    // Convert potential collisions to a set for comparison
    std::set<std::pair<unsigned int, unsigned int>> collisionSet;
    for (const auto &pair : potentialCollisions)
    {
        // Ensure the smaller ID comes first to avoid order issues
        if (pair.first < pair.second)
            collisionSet.emplace(pair.first, pair.second);
        else
            collisionSet.emplace(pair.second, pair.first);
    }

    bool passed = (collisionSet == expectedPairs);

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected " << expectedPairs.size() << " unique collision pairs. Actual: " << potentialCollisions.size()
            << " collisions found.";
    return TestResult("OctreeTests::NoDuplicatePairsDifferentOrder", passed, oss.str());
}

inline TestResult NoDuplicatePairsSameOrder()
{
    BoundingBox3D rootBounds(FVector3(-100.0f, -100.0f, -100.0f), FVector3(100.0f, 100.0f, 100.0f));
    Octree octree(rootBounds);

    // Collider 1
    SphereBoundsComponent sphere1(15.0f);
    TransformComponent transform1(FVector3(10.0f, 10.0f, 10.0f), FVector3(0.0f, 0.0f, 0.0f),
                                  FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID1 = 1;
    octree.Insert(sphere1, transform1, entityID1);

    // Collider 2 (overlaps with collider 1)
    SphereBoundsComponent sphere2(15.0f);
    TransformComponent transform2(FVector3(20.0f, 10.0f, 10.0f), FVector3(0.0f, 0.0f, 0.0f),
                                  FVector3(1.0f, 1.0f, 1.0f));
    unsigned int entityID2 = 2;
    octree.Insert(sphere2, transform2, entityID2);

    // Insert the same pair again
    octree.Insert(sphere1, transform1, entityID1);
    octree.Insert(sphere2, transform2, entityID2);

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Expected unique pair: (1,2)
    std::set<std::pair<unsigned int, unsigned int>> expectedPairs = {{1, 2}};

    // Convert potential collisions to a set for comparison
    std::set<std::pair<unsigned int, unsigned int>> collisionSet;
    for (const auto &pair : potentialCollisions)
    {
        // Ensure the smaller ID comes first to avoid order issues
        if (pair.first < pair.second)
            collisionSet.emplace(pair.first, pair.second);
        else
            collisionSet.emplace(pair.second, pair.first);
    }

    bool passed = (collisionSet == expectedPairs);

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected " << expectedPairs.size() << " unique collision pair. Actual: " << potentialCollisions.size()
            << " collisions found.";
    return TestResult("OctreeTests::NoDuplicatePairsSameOrder", passed, oss.str());
}

inline TestResult NoMultipleDuplicatePairs()
{
    BoundingBox3D rootBounds(FVector3(-200.0f, -200.0f, -200.0f), FVector3(200.0f, 200.0f, 200.0f));
    Octree octree(rootBounds);

    // Collider Pairs
    struct ColliderInfo
    {
        unsigned int entityID;
        SphereBoundsComponent sphere;
        TransformComponent transform;
    };

    std::vector<ColliderInfo> colliders = {
        {1, SphereBoundsComponent(10.0f),
         TransformComponent(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f))},
        {2, SphereBoundsComponent(10.0f),
         TransformComponent(FVector3(5.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f))},
        {3, SphereBoundsComponent(10.0f),
         TransformComponent(FVector3(10.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f))}};

    // Insert each collider multiple times
    for (const auto &collider : colliders)
    {
        octree.Insert(collider.sphere, collider.transform, collider.entityID);
        octree.Insert(collider.sphere, collider.transform, collider.entityID);
    }

    // Retrieve potential collisions
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    octree.GetPotentialCollisions(potentialCollisions);

    // Expected unique pairs: (1,2), (1,3), (2,3)
    std::set<std::pair<unsigned int, unsigned int>> expectedPairs = {{1, 2}, {1, 3}, {2, 3}};

    // Convert potential collisions to a set for comparison
    std::set<std::pair<unsigned int, unsigned int>> collisionSet;
    for (const auto &pair : potentialCollisions)
    {
        // Ensure the smaller ID comes first to avoid order issues
        if (pair.first < pair.second)
            collisionSet.emplace(pair.first, pair.second);
        else
            collisionSet.emplace(pair.second, pair.first);
    }

    bool passed = (collisionSet == expectedPairs);

    std::ostringstream oss;
    if (passed)
        oss << "Passed.";
    else
        oss << "Expected " << expectedPairs.size() << " unique collision pairs. Actual: " << potentialCollisions.size()
            << " collisions found.";
    return TestResult("OctreeTests::NoMultipleDuplicatePairs", passed, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {InsertSphereCollider(),
            InsertBoxCollider(),
            DetectPotentialCollisions(),
            SubdivideAfterCapacity(),
            NoFalsePositives(),
            OverlappingCollidersAcrossNodes(),
            CollidersOnNodeBoundaries(),
            MultipleCollidersInSingleNode(),
            NonOverlappingAdjacentColliders(),
            NoDuplicatePairsDifferentOrder(),
            NoDuplicatePairsSameOrder(),
            NoMultipleDuplicatePairs()};
}

} // namespace OctreeTests

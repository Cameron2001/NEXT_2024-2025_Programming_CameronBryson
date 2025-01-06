#pragma once

#include "Engine/Core/TestResult.h"
#include "Engine/Math/Quadtree.h"
#include "Engine/Math/BoundingBox.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Graphics/Triangle.h"
#include <vector>
#include <sstream>

namespace QuadtreeTests
{
inline TestResult InsertSingleTriangle()
{
    BoundingBox2D bounds(0.0f, 0.0f, 100.0f, 100.0f);
    Quadtree quadtree(bounds, 4, 5);
    Triangle2D triangle(FVector2(10.0f, 10.0f), FVector2(20.0f, 10.0f), FVector2(15.0f, 20.0f), 15.0f);
    bool inserted = quadtree.Insert(triangle);
    std::vector<Triangle2D> queried;
    quadtree.QueryArea(bounds, queried);
    bool isEqual = (queried.size() == 1 && queried[0] == triangle);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Expected: Triangle found. Actual: Not found.";
    return TestResult("QuadtreeTests::InsertSingleTriangle", isEqual, oss.str());
}

inline TestResult InsertMultipleTriangles()
{
    BoundingBox2D bounds(0.0f, 0.0f, 100.0f, 100.0f);
    Quadtree quadtree(bounds, 4, 5);
    std::vector<Triangle2D> triangles = {
        Triangle2D(FVector2(10.0f, 10.0f), FVector2(20.0f, 10.0f), FVector2(15.0f, 20.0f), 15.0f),
        Triangle2D(FVector2(30.0f, 30.0f), FVector2(40.0f, 30.0f), FVector2(35.0f, 40.0f), 35.0f),
        Triangle2D(FVector2(50.0f, 50.0f), FVector2(60.0f, 50.0f), FVector2(55.0f, 60.0f), 55.0f)};
    for (const auto &triangle : triangles)
        quadtree.Insert(triangle);
    std::vector<Triangle2D> queried;
    quadtree.QueryArea(bounds, queried);
    bool allFound = (queried.size() == triangles.size());
    for (const auto &triangle : triangles)
        if (std::find(queried.begin(), queried.end(), triangle) == queried.end())
        {
            allFound = false;
            break;
        }
    std::ostringstream oss;
    if (allFound)
        oss << "Passed.";
    else
        oss << "Expected: All triangles found. Actual: Some triangles missing.";
    return TestResult("QuadtreeTests::InsertMultipleTriangles", allFound, oss.str());
}

inline TestResult QuadtreeSubdivision()
{
    BoundingBox2D bounds(0.0f, 0.0f, 100.0f, 100.0f);
    Quadtree quadtree(bounds, 2, 3);
    std::vector<Triangle2D> triangles = {
        Triangle2D(FVector2(10.0f, 10.0f), FVector2(20.0f, 10.0f), FVector2(15.0f, 20.0f), 15.0f),
        Triangle2D(FVector2(70.0f, 70.0f), FVector2(80.0f, 70.0f), FVector2(75.0f, 80.0f), 75.0f),
        Triangle2D(FVector2(30.0f, 30.0f), FVector2(40.0f, 30.0f), FVector2(35.0f, 40.0f), 35.0f)};
    for (const auto &triangle : triangles)
        quadtree.Insert(triangle);
    std::vector<Triangle2D> queried;
    quadtree.QueryArea(bounds, queried);
    bool correct = (queried.size() == 3);
    std::ostringstream oss;
    if (correct)
        oss << "Passed.";
    else
        oss << "Expected: Subdivided correctly. Actual: Subdivision failed.";
    return TestResult("QuadtreeTests::QuadtreeSubdivision", correct, oss.str());
}

inline TestResult QueryArea()
{
    BoundingBox2D bounds(0.0f, 0.0f, 100.0f, 100.0f);
    Quadtree quadtree(bounds, 4, 5);
    std::vector<Triangle2D> triangles = {
        Triangle2D(FVector2(10.0f, 10.0f), FVector2(20.0f, 10.0f), FVector2(15.0f, 20.0f), 15.0f),
        Triangle2D(FVector2(30.0f, 30.0f), FVector2(40.0f, 30.0f), FVector2(35.0f, 40.0f), 35.0f),
        Triangle2D(FVector2(50.0f, 50.0f), FVector2(60.0f, 50.0f), FVector2(55.0f, 60.0f), 55.0f)};
    for (const auto &triangle : triangles)
        quadtree.Insert(triangle);
    BoundingBox2D queryRange(5.0f, 5.0f, 45.0f, 45.0f);
    std::vector<Triangle2D> queried;
    quadtree.QueryArea(queryRange, queried);
    bool correct = (queried.size() == 2) &&
                   (std::find(queried.begin(), queried.end(), triangles[0]) != queried.end()) &&
                   (std::find(queried.begin(), queried.end(), triangles[1]) != queried.end());
    std::ostringstream oss;
    if (correct)
        oss << "Passed.";
    else
        oss << "Expected: Two triangles found. Actual: Incorrect number found.";
    return TestResult("QuadtreeTests::QueryArea", correct, oss.str());
}

inline TestResult InsertSpanningTriangles()
{
    BoundingBox2D bounds(0.0f, 0.0f, 100.0f, 100.0f);
    Quadtree quadtree(bounds, 2, 3);
    Triangle2D spanningTriangle(FVector2(45.0f, 45.0f), FVector2(55.0f, 45.0f), FVector2(50.0f, 55.0f), 50.0f);
    bool inserted = quadtree.Insert(spanningTriangle);
    std::vector<Triangle2D> queried;
    quadtree.QueryArea(bounds, queried);
    bool isEqual = (queried.size() == 1 && queried[0] == spanningTriangle);
    std::ostringstream oss;
    if (isEqual)
        oss << "Passed.";
    else
        oss << "Expected: Spanning triangle found. Actual: Not found.";
    return TestResult("QuadtreeTests::InsertSpanningTriangles", isEqual, oss.str());
}

inline std::vector<TestResult> RunAllTests()
{
    return {InsertSingleTriangle(), InsertMultipleTriangles(), QuadtreeSubdivision(), QueryArea(),
            InsertSpanningTriangles()};
}
} // namespace QuadtreeTests

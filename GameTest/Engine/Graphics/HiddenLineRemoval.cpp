#include "stdafx.h"
#include "Engine/Graphics/Edge.h"
#include "Engine/Math/BoundingBox.h"
#include "Engine/Math/Vector2.h"
#include "HiddenLineRemoval.h"

#include "Triangle.h"
#include <algorithm>
#include <cstdlib>
#include <Engine/Math/Quadtree.h>
#include <limits>
#include <memory>
#include <utility>
#include <vector>
#include <ppl.h>

HiddenLineRemoval::HiddenLineRemoval()
{
}

std::vector<Edge2D> HiddenLineRemoval::removeHiddenLines(std::vector<Triangle2D> &triangles)
{
    initializeQuadtree(triangles);
    for (const auto &triangle : triangles)
    {
        m_quadtree->insert(triangle);
    }

    concurrency::parallel_for_each(triangles.begin(), triangles.end(), [&](const Triangle2D &triangle) {
        // Thread-local reusable vectors
        thread_local std::vector<Triangle2D> potentialOccluders;
        thread_local std::vector<Edge2D> clippedEdges;
        thread_local std::vector<Edge2D> segments;
        thread_local std::vector<Edge2D> newClippedEdges;

        // Clear and reserve capacity for reuse
        potentialOccluders.clear();
        potentialOccluders.reserve(100);

        clippedEdges.clear();
        clippedEdges.reserve(10);

        segments.clear();
        segments.reserve(20);

        newClippedEdges.clear();
        newClippedEdges.reserve(10);

        processTriangle(triangle, potentialOccluders, clippedEdges, segments, newClippedEdges);
    });

    // Combine all local vectors into a single result
    std::vector<Edge2D> result;
    m_visibleEdges.combine_each([&result](const std::vector<Edge2D> &localEdges) {
        result.insert(result.end(), localEdges.begin(), localEdges.end());
    });
    m_visibleEdges.clear();
    return result;
}

void HiddenLineRemoval::initializeQuadtree(const std::vector<Triangle2D> &triangles)
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto &triangle : triangles)
    {
        minX = std::min(minX, std::min({triangle.v0.X, triangle.v1.X, triangle.v2.X}));
        minY = std::min(minY, std::min({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y}));
        maxX = std::max(maxX, std::max({triangle.v0.X, triangle.v1.X, triangle.v2.X}));
        maxY = std::max(maxY, std::max({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y}));
    }

    BoundingBox2D rootBounds(minX, minY, maxX, maxY);
    m_quadtree = std::make_unique<Quadtree>(rootBounds, 4, 4);
}

void HiddenLineRemoval::processTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &potentialOccluders,
                                        std::vector<Edge2D> &clippedEdges, std::vector<Edge2D> &segments,
                                        std::vector<Edge2D> &newClippedEdges)
{
    m_quadtree->queryTriangle(triangle, potentialOccluders);

    potentialOccluders.erase(
        std::remove_if(potentialOccluders.begin(), potentialOccluders.end(),
                       [&](const Triangle2D &occluder) { return sharesVertex(occluder, triangle); }),
        potentialOccluders.end());

    Edge2D edges[3];
    createTriangleEdges(triangle, edges);

    for (int i = 0; i < 3; ++i)
    {
        const Edge2D &edge = edges[i];
        segments.clear();
        segments.emplace_back(edge);

        processEdge(edge, potentialOccluders, clippedEdges, segments, newClippedEdges);
    }
}

void HiddenLineRemoval::processEdge(const Edge2D &edge, const std::vector<Triangle2D> &occluders,
                                    std::vector<Edge2D> &clippedEdges, std::vector<Edge2D> &segments,
                                    std::vector<Edge2D> &newClippedEdges)
{
    for (const auto &occluder : occluders)
    {
        newClippedEdges.clear();

        for (const auto &segment : segments)
        {
            clipEdgeAgainstTriangle(segment, occluder, newClippedEdges);
        }

        if (newClippedEdges.empty())
        {
            segments.clear();
            break; // Edge is fully occluded
        }

        segments.swap(newClippedEdges);
    }

    // Append remaining visible segments to clippedEdges
    for (const auto &segment : segments)
    {
        clippedEdges.emplace_back(segment);
    }

    // Add the clipped edges to the combinable visible edges
    for (const auto &segment : segments)
    {
        m_visibleEdges.local().emplace_back(segment);
    }
}

void HiddenLineRemoval::clipEdgeAgainstTriangle(const Edge2D &edge, const Triangle2D &triangle,
                                                std::vector<Edge2D> &clippedEdges)
{
    FVector2 intersectionPoints[2];
    size_t intersectionCount = 0;
    FVector2 sortedPoints[4];
    size_t sortedCount = 0;

    Edge2D triEdges[3];
    createTriangleEdges(triangle, triEdges);

    // Find intersection points (maximum of 2)
    for (int i = 0; i < 3 && intersectionCount < 2; ++i)
    {
        FVector2 intersectionPoint;
        if (getEdgeIntersection(edge, triEdges[i], intersectionPoint))
        {
            intersectionPoints[intersectionCount++] = intersectionPoint;
        }
    }

    if (intersectionCount == 0)
    {
        if (isPointInsideTriangle(edge.start, triangle) && isPointInsideTriangle(edge.end, triangle))
        {
            // Edge is fully occluded
            return;
        }
        else
        {
            // Edge is fully visible
            clippedEdges.emplace_back(edge);
            return;
        }
    }

    // Populate sortedPoints with start, intersections, and end
    sortedPoints[sortedCount++] = edge.start;
    for (size_t i = 0; i < intersectionCount; ++i)
    {
        sortedPoints[sortedCount++] = intersectionPoints[i];
    }
    sortedPoints[sortedCount++] = edge.end;

    // Sort sortedPoints based on distance from edge.start
    std::sort(sortedPoints, sortedPoints + sortedCount, [&](const FVector2 &a, const FVector2 &b) {
        return (a - edge.start).LengthSquared() < (b - edge.start).LengthSquared();
    });

    // Create clipped edges based on sorted points
    for (size_t i = 0; i < sortedCount - 1; ++i)
    {
        FVector2 midPoint = (sortedPoints[i] + sortedPoints[i + 1]) * 0.5f;
        if (!isPointInsideTriangle(midPoint, triangle))
        {
            FVector2 delta = sortedPoints[i + 1] - sortedPoints[i];
            if (delta.LengthSquared() >= 1e-5f)
            {
                clippedEdges.emplace_back(sortedPoints[i], sortedPoints[i + 1]);
            }
        }
    }
}

bool HiddenLineRemoval::getEdgeIntersection(const Edge2D &edgeA, const Edge2D &edgeB, FVector2 &intersectionPoint)
{
    const float x1 = edgeA.start.X;
    const float y1 = edgeA.start.Y;
    const float x2 = edgeA.end.X;
    const float y2 = edgeA.end.Y;
    const float x3 = edgeB.start.X;
    const float y3 = edgeB.start.Y;
    const float x4 = edgeB.end.X;
    const float y4 = edgeB.end.Y;

    const float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    constexpr float EPSILON = 1e-6f;

    if (std::abs(denominator) < EPSILON)
    {
        return false; // Lines are parallel or coincident
    }

    const float numeratorA = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
    const float numeratorB = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2);
    const float t = numeratorA / denominator;
    const float u = numeratorB / denominator;

    if (t < -EPSILON || t > 1.0f + EPSILON || u < -EPSILON || u > 1.0f + EPSILON)
        return false; // Intersection not within the segments

    intersectionPoint.X = x1 + t * (x2 - x1);
    intersectionPoint.Y = y1 + t * (y2 - y1);
    return true;
}

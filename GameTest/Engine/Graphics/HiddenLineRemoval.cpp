#include "stdafx.h"
#include "HiddenLineRemoval.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/BoundingBox.h"
#include "Engine/Graphics/Edge.h"
#include <algorithm>
#include <cmath>
#include "Engine/Core/Events.h"
// Constructor
HiddenLineRemoval::HiddenLineRemoval(const std::vector<Triangle2D> &triangles) : m_triangles(triangles)
{

    sortTrianglesByDepth();
    initializeQuadtree();
    TestEvent.Notify(99);
    TestEvent2.Notify(100);
}

// Public method to remove hidden lines
std::vector<Edge2D> HiddenLineRemoval::removeHiddenLines() const
{
    std::vector<Edge2D> visibleEdges;
    visibleEdges.reserve(m_triangles.size() * 3);

    std::unordered_set<Edge2D, Edge2DHash> uniqueEdges;
    uniqueEdges.reserve(m_triangles.size() * 3);

    for (const auto &triangle : m_triangles)
    {
        processTriangle(triangle, uniqueEdges, visibleEdges);
    }

    return visibleEdges;
}

// Initialize the quadtree with the bounding box
void HiddenLineRemoval::initializeQuadtree()
{
    // Compute overall bounds using a single pass
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto &triangle : m_triangles)
    {
        minX = std::min({minX, triangle.v0.X, triangle.v1.X, triangle.v2.X});
        minY = std::min({minY, triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
        maxX = std::max({maxX, triangle.v0.X, triangle.v1.X, triangle.v2.X});
        maxY = std::max({maxY, triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
    }

    BoundingBox2D rootBounds(minX, minY, maxX, maxY);
    m_quadtree = std::make_unique<Quadtree>(rootBounds, 4, 4);
}

// Sort triangles based on average Z value (depth)
void HiddenLineRemoval::sortTrianglesByDepth()
{
    std::sort(m_triangles.begin(), m_triangles.end(),
              [](const Triangle2D &a, const Triangle2D &b) { return a.avgZ < b.avgZ; });
}

// Process a single triangle
void HiddenLineRemoval::processTriangle(const Triangle2D &triangle, std::unordered_set<Edge2D, Edge2DHash> &uniqueEdges,
                                        std::vector<Edge2D> &visibleEdges) const
{
    std::vector<Edge2D> segments;
    const std::vector<Edge2D> edges = createTriangleEdges(triangle);

    std::vector<Triangle2D> potentialOccluders = m_quadtree->queryTriangle(triangle);
    // We want to erase any triangles that share a vertex with the current triangle
    // Order of the occluders doesnt matter
    // We do not need to maintain order. So that means we could swap back and pop
    potentialOccluders.erase(
        std::remove_if(potentialOccluders.begin(), potentialOccluders.end(),
                       [&](const Triangle2D &occluder) { return sharesVertex(occluder, triangle); }),
        potentialOccluders.end());

    for (const auto &edge : edges)
    {
        if (uniqueEdges.insert(edge).second)
        {
            auto newSegments = processEdge(edge, potentialOccluders);
            for (auto &segment : newSegments)
            {
                segments.emplace_back(segment);
            }
        }
    }
    if (!segments.empty())
    {
        appendVisibleSegments(visibleEdges, segments);
    }
    m_quadtree->insert(triangle);
}

// Create edges for a triangle
std::vector<Edge2D> HiddenLineRemoval::createTriangleEdges(const Triangle2D &triangle)
{
    return {Edge2D(triangle.v0, triangle.v1), Edge2D(triangle.v1, triangle.v2), Edge2D(triangle.v2, triangle.v0)};
}

// Process an edge against potential occluders
std::vector<Edge2D> HiddenLineRemoval::processEdge(const Edge2D &edge,
                                                   const std::vector<Triangle2D> &potentialOccluders)
{
    std::vector<Edge2D> segments = {edge};
    for (const auto &occluder : potentialOccluders)
    {
        segments = clipSegmentsWithOccluder(segments, occluder);
        if (segments.empty())
            break;
    }
    return segments;
}

// Clip segments with an occluder triangle
std::vector<Edge2D> HiddenLineRemoval::clipSegmentsWithOccluder(const std::vector<Edge2D> &segments,
                                                                const Triangle2D &occluder)
{
    std::vector<Edge2D> clippedSegments;
    clippedSegments.reserve(segments.size());

    for (const auto &segment : segments)
    {
        auto clipped = clipEdgeAgainstTriangle(segment, occluder);
        for (const auto &edge : clipped)
        {
            clippedSegments.emplace_back(edge);
        }
    }
    return clippedSegments;
}

// Append visible segments to the main visible edges vector
void HiddenLineRemoval::appendVisibleSegments(std::vector<Edge2D> &visibleEdges, const std::vector<Edge2D> &segments)
{
    for (const auto &segment : segments)
    {
        visibleEdges.emplace_back(segment);
    }
}

// Clip an edge against a triangle
std::vector<Edge2D> HiddenLineRemoval::clipEdgeAgainstTriangle(const Edge2D &edge, const Triangle2D &triangle)
{
    std::vector<FVector2> intersectionPoints;
    intersectionPoints.reserve(3);

    Edge2D triEdges[3] = {Edge2D(triangle.v0, triangle.v1), Edge2D(triangle.v1, triangle.v2),
                          Edge2D(triangle.v2, triangle.v0)};

    for (const auto &triEdge : triEdges)
    {
        FVector2 intersectionPoint;
        if (getEdgeIntersection(edge, triEdge, intersectionPoint))
            intersectionPoints.emplace_back(intersectionPoint);
    }

    if (intersectionPoints.empty())
    {

        if (isPointInsideTriangle(edge.start, triangle) && isPointInsideTriangle(edge.end, triangle))
            return {}; // Edge is fully occluded
        else
            return {edge}; // Edge is fully outside the triangle
    }

    std::sort(intersectionPoints.begin(), intersectionPoints.end(), [&](const FVector2 &a, const FVector2 &b) {
        return (a - edge.start).LengthSquared() < (b - edge.start).LengthSquared();
    });

    // Reuse sortedPoints vector
    std::vector<FVector2> sortedPoints;
    sortedPoints.reserve(5); // start, 3 intersections, end
    sortedPoints.emplace_back(edge.start);
    for (const auto &point : intersectionPoints)
    {
        sortedPoints.emplace_back(point);
    }
    sortedPoints.emplace_back(edge.end);

    std::vector<Edge2D> clippedEdges;
    clippedEdges.reserve(sortedPoints.size() - 1);

    for (size_t i = 0; i < sortedPoints.size() - 1; ++i)
    {
        FVector2 midPoint = (sortedPoints[i] + sortedPoints[i + 1]) * 0.5f;
        if (!isPointInsideTriangle(midPoint, triangle))
        {
            Edge2D newEdge(sortedPoints[i], sortedPoints[i + 1]);
            if (newEdge.start != newEdge.end && (newEdge.end - newEdge.start).LengthSquared() >= 0.00001f)
            {
                clippedEdges.emplace_back(newEdge);
            }
        }
    }

    return clippedEdges;
}

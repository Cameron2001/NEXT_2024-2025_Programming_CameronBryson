// HiddenLineRemoval.cpp
#include "stdafx.h"
#include "HiddenLineRemoval.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/BoundingBox.h"
#include "Engine/Graphics/Edge.h"
#include <algorithm>
#include <cmath>
#include <concurrent_vector.h>
#include <ppl.h>

// Constructor
HiddenLineRemoval::HiddenLineRemoval(const std::vector<Triangle> &triangles) : m_triangles(triangles)
{
    sortTrianglesByDepth();
    initializeQuadtree();
}

// Public method to remove hidden lines
std::vector<Edge3D> HiddenLineRemoval::removeHiddenLines() const
{
    std::vector<Edge3D> visibleEdges;
    visibleEdges.reserve(m_triangles.size() * 3);

    std::unordered_set<Edge3D, Edge3DHash> uniqueEdges;
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

    // Add a small padding to avoid boundary issues
    constexpr float padding = 0.01f;
    minX -= padding;
    minY -= padding;
    maxX += padding;
    maxY += padding;

    BoundingBox2D rootBounds(minX, minY, maxX, maxY);
    m_quadtree = std::make_unique<Quadtree>(rootBounds, 16, 8);
}

// Sort triangles based on average Z value (depth)
void HiddenLineRemoval::sortTrianglesByDepth()
{
    std::sort(m_triangles.begin(), m_triangles.end(), [](const Triangle &a, const Triangle &b) {
        const float depthA = (a.v0.Z + a.v1.Z + a.v2.Z) / 3.0f;
        const float depthB = (b.v0.Z + b.v1.Z + b.v2.Z) / 3.0f;
        return depthA < depthB;
    });
}

// Process a single triangle
void HiddenLineRemoval::processTriangle(const Triangle &triangle, std::unordered_set<Edge3D, Edge3DHash> &uniqueEdges,
                                        std::vector<Edge3D> &visibleEdges) const
{
    std::vector<Edge3D> segments;
    const std::vector<Edge3D> edges = createTriangleEdges(triangle);

    for (const auto &edge : edges)
    {
        if (uniqueEdges.insert(edge).second)
        {
            std::vector<Triangle> potentialOccluders = m_quadtree->queryEdge(edge);
            auto newSegments = processEdge(edge, potentialOccluders);
            for (auto &segment : newSegments)
            {
                segments.emplace_back(segment);
            }
        }
    }
    if (segments.empty())
    {
        // For the life of me I cant figure out why this works better
        //  Adding fully occluded triangles to the quadtree works best for performance and reduces the most artifacts
        m_quadtree->insert(triangle);
    }
    else
    {
        appendVisibleSegments(visibleEdges, segments);
    }
}

// Create edges for a triangle
std::vector<Edge3D> HiddenLineRemoval::createTriangleEdges(const Triangle &triangle)
{
    return {Edge3D(triangle.v0, triangle.v1), Edge3D(triangle.v1, triangle.v2), Edge3D(triangle.v2, triangle.v0)};
}

// Process an edge against potential occluders
std::vector<Edge3D> HiddenLineRemoval::processEdge(const Edge3D &edge, const std::vector<Triangle> &potentialOccluders)
{
    if (edge.start == edge.end || (edge.end - edge.start).LengthSquared() < 0.00001f)
    {
        return {};
    }

    std::vector<Edge3D> segments = {edge};

    for (const auto &occluder : potentialOccluders)
    {
        if (isPointInsideTriangle(edge.start, occluder) && isPointInsideTriangle(edge.end, occluder))
        {
            return {}; // Edge is fully occluded
        }

        // Skip clipping if the triangle shares a vertex with the edge
        if (sharesVertex(occluder, edge))
        {
            continue;
        }

        segments = clipSegmentsWithOccluder(segments, occluder);
        if (segments.empty())
            break;
    }

    return segments;
}

// Clip segments with an occluder triangle
std::vector<Edge3D> HiddenLineRemoval::clipSegmentsWithOccluder(const std::vector<Edge3D> &segments,
                                                                const Triangle &occluder)
{
    std::vector<Edge3D> clippedSegments;
    clippedSegments.reserve(segments.size());

    for (const auto &segment : segments)
    {
        auto clipped = clipEdgeAgainstTriangle(segment, occluder);
        for (const auto &edge : clipped)
        {
            if (edge.start != edge.end)
            {
                clippedSegments.emplace_back(edge);
            }
        }
    }

    return clippedSegments;
}

// Append visible segments to the main visible edges vector
void HiddenLineRemoval::appendVisibleSegments(std::vector<Edge3D> &visibleEdges,
                                              const std::vector<Edge3D> &segments)
{
    for (const auto &segment : segments)
    {
        if (segment.start != segment.end)
        {
            visibleEdges.emplace_back(segment);
        }
    }
}

// Clip an edge against a triangle
std::vector<Edge3D> HiddenLineRemoval::clipEdgeAgainstTriangle(const Edge3D &edge, const Triangle &triangle)
{
    std::vector<FVector3> intersectionPoints;
    intersectionPoints.reserve(3);

    Edge3D triEdges[3] = {Edge3D(triangle.v0, triangle.v1), Edge3D(triangle.v1, triangle.v2),
                          Edge3D(triangle.v2, triangle.v0)};

    for (const auto &triEdge : triEdges)
    {
        FVector3 intersectionPoint;
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

    std::sort(intersectionPoints.begin(), intersectionPoints.end(), [&](const FVector3 &a, const FVector3 &b) {
        return (a - edge.start).LengthSquared() < (b - edge.start).LengthSquared();
    });

    // Reuse sortedPoints vector
    std::vector<FVector3> sortedPoints;
    sortedPoints.reserve(5); // start, 3 intersections, end
    sortedPoints.emplace_back(edge.start);
    for (const auto &point : intersectionPoints)
    {
        sortedPoints.emplace_back(point);
    }
    sortedPoints.emplace_back(edge.end);

    std::vector<Edge3D> clippedEdges;
    clippedEdges.reserve(sortedPoints.size() - 1);

    for (size_t i = 0; i < sortedPoints.size() - 1; ++i)
    {
        FVector3 midPoint = (sortedPoints[i] + sortedPoints[i + 1]) * 0.5f;
        if (!isPointInsideTriangle(midPoint, triangle))
            clippedEdges.emplace_back(sortedPoints[i], sortedPoints[i + 1]);
    }

    return clippedEdges;
}
// Get the intersection point of two edges if it exists
bool HiddenLineRemoval::getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB, FVector3 &intersectionPoint)
{
    const float x1 = edgeA.start.X;
    const float y1 = edgeA.start.Y;
    const float x2 = edgeA.end.X;
    const float y2 = edgeA.end.Y;
    const float x3 = edgeB.start.X;
    const float y3 = edgeB.start.Y;
    const float x4 = edgeB.end.X;
    const float y4 = edgeB.end.Y;

    const float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    constexpr float EPSILON = 1e-6f;

    if (fabs(denom) < EPSILON)
    {
        const bool startStart = edgeA.start == edgeB.start;
        const bool startEnd = edgeA.start == edgeB.end;
        const bool endStart = edgeA.end == edgeB.start;
        const bool endEnd = edgeA.end == edgeB.end;

        const int overlapCount = startStart + startEnd + endStart + endEnd;

        if (overlapCount > 1)
            return false;
        else if (overlapCount == 1)
        {
            if (startStart || startEnd)
                intersectionPoint = edgeA.start;
            else
                intersectionPoint = edgeA.end;
            return true;
        }
        else
            return false;
    }

    const float t_num = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
    const float u_num = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2);
    const float t = t_num / denom;
    const float u = u_num / denom;

    if (t < -EPSILON || t > 1.0f + EPSILON || u < -EPSILON || u > 1.0f + EPSILON)
        return false;

    const float intersectionX = x1 + t * (x2 - x1);
    const float intersectionY = y1 + t * (y2 - y1);
    const float intersectionZ = edgeA.start.Z + t * (edgeA.end.Z - edgeA.start.Z);

    intersectionPoint = FVector3(intersectionX, intersectionY, intersectionZ);
    return true;
}

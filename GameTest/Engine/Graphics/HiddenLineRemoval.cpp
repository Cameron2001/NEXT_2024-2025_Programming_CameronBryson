#include "stdafx.h"
#include "Edge.h"
#include "Triangle.h"
#include "HiddenLineRemoval.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <Engine/Math/BoundingBox.h>
#include <Engine/Math/Quadtree.h>
#include <Engine/Math/Vector2.h>
#include <Engine/Math/Vector3.h>
#include <limits>
#include <memory>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>
#include <concurrent_unordered_set.h>
#include <concurrent_vector.h>
HiddenLineRemoval::HiddenLineRemoval(const std::vector<Triangle> &triangles)
{
    m_triangles = triangles;
    sortTrianglesByDepth();
    initializeQuadtree();
}
std::vector<Edge3D> HiddenLineRemoval::removeHiddenLines()
{
    // What if we collect unique edges first.
    // Instead of just exiting early when a non unique is found
    // Issue is that we will have to do per edge and that doesnt work well with quadtree
    // Quad trees and many early exits work with triangles instead of standalone edges

    std::vector<Edge3D> visibleEdges;
    visibleEdges.reserve(m_triangles.size() * 3);

    std::unordered_set<Edge3D, Edge3DHash> uniqueEdges;
    uniqueEdges.reserve(m_triangles.size() * 3);
    for (const auto &triangle : m_triangles)
    {
        std::vector<Edge3D> edges = {
            {triangle.v0, triangle.v1}, {triangle.v1, triangle.v2}, {triangle.v2, triangle.v0}};
        bool allOcclued = true;
        for (const auto &edge : edges)
        {
            if (uniqueEdges.insert(edge).second)
            {
                std::vector<Triangle> potentialOccluders = m_quadtree->queryEdge(edge);
                if (processEdge(edge, potentialOccluders, visibleEdges))
                {
                    allOcclued = false;
                }
            }
        }
        if (allOcclued)
        {
            m_quadtree->insert(triangle);
        }
    }
    return std::vector<Edge3D>(visibleEdges.begin(), visibleEdges.end());
}

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

void HiddenLineRemoval::sortTrianglesByDepth()
{
    // Sort triangles based on average Z value (depth)
    std::sort(m_triangles.begin(), m_triangles.end(), [](const Triangle &a, const Triangle &b) {
        float depthA = (a.v0.Z + a.v1.Z + a.v2.Z) / 3.0f;
        float depthB = (b.v0.Z + b.v1.Z + b.v2.Z) / 3.0f;
        // float depthA = std::min({a.v0.Z, a.v1.Z, a.v2.Z});
        // float depthB = std::min({b.v0.Z, b.v1.Z, b.v2.Z});
        return depthA < depthB;
    });
}

// Corrected getEdgeIntersection function
inline bool HiddenLineRemoval::getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB,
                                                   FVector3 &intersectionPoint) const
{
    float x1 = edgeA.start.X, y1 = edgeA.start.Y;
    float x2 = edgeA.end.X, y2 = edgeA.end.Y;
    float x3 = edgeB.start.X, y3 = edgeB.start.Y;
    float x4 = edgeB.end.X, y4 = edgeB.end.Y;

    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    const float EPSILON = 1e-6f;

    if (fabs(denom) < EPSILON)
    {
        bool startStart = edgeA.start == edgeB.start;
        bool startEnd = edgeA.start == edgeB.end;
        bool endStart = edgeA.end == edgeB.start;
        bool endEnd = edgeA.end == edgeB.end;

        int overlapCount = startStart + startEnd + endStart + endEnd;

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

    float t_num = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
    float u_num = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2);
    float t = t_num / denom;
    float u = u_num / denom;

    if (t < -EPSILON || t > 1.0f + EPSILON || u < -EPSILON || u > 1.0f + EPSILON)
        return false;

    float intersectionX = x1 + t * (x2 - x1);
    float intersectionY = y1 + t * (y2 - y1);
    float intersectionZ = edgeA.start.Z + t * (edgeA.end.Z - edgeA.start.Z);

    intersectionPoint = FVector3(intersectionX, intersectionY, intersectionZ);
    return true;
}

std::vector<Edge3D> HiddenLineRemoval::clipEdgeAgainstTriangle(const Edge3D &edge, const Triangle &triangle) const
{
    std::vector<FVector3> intersectionPoints;
    intersectionPoints.reserve(3);

    Edge3D triEdges[3] = {{triangle.v0, triangle.v1}, {triangle.v1, triangle.v2}, {triangle.v2, triangle.v0}};

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
    sortedPoints.insert(sortedPoints.end(), intersectionPoints.begin(), intersectionPoints.end());
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
bool HiddenLineRemoval::processEdge(const Edge3D &edge, const std::vector<Triangle> &potentialOccluders,
                                    std::vector<Edge3D> &visibleEdges)
{
    if (edge.start == edge.end || (edge.end - edge.start).LengthSquared() < 1e-6f)
    {
        return false; // Degenerate or negligible edge
    }
    bool isVisable = false;
    std::vector<Edge3D> segments = {edge};
    for (const auto &occluder : potentialOccluders)
    {

        if (isPointInsideTriangle(edge.start, occluder) && isPointInsideTriangle(edge.end, occluder))
        {
            return false; // Edge is fully occluded
        }
        std::vector<Edge3D> tempSegments;
        tempSegments.reserve(segments.size());
        for (const auto &segment : segments)
        {
            auto clipped = clipEdgeAgainstTriangle(segment, occluder);
            tempSegments.insert(tempSegments.end(), clipped.begin(), clipped.end());
        }
        segments = std::move(tempSegments);
        if (segments.empty())
            break;
    }

    // Bulk insert visible segments to reduce overhead
    if (!segments.empty())
    {
        visibleEdges.insert(visibleEdges.end(), std::make_move_iterator(segments.begin()),
                            std::make_move_iterator(segments.end()));
        isVisable = true;
    }
    return isVisable;
}

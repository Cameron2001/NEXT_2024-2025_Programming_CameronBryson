#include "stdafx.h"
#include "Edge.h"
#include "Face.h"
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

HiddenLineRemoval::HiddenLineRemoval(const std::vector<Face> &triangles)
{
    m_triangles = triangles;
    sortTrianglesByDepth();
    initializeQuadtree();
}
std::vector<Edge3D> HiddenLineRemoval::removeHiddenLines()
{
    std::vector<Edge3D> visibleEdges;
    visibleEdges.reserve(m_triangles.size() * 3);

    std::unordered_set<Edge3D, Edge3DHash> processedEdges;
    processedEdges.reserve(m_triangles.size() * 3);

    for (const auto &triangle : m_triangles)
    {
        if (triangleArea(triangle) < 0.0002)
            continue;
        // query for potential occluders from the quadtree
        float minX = std::min({triangle.v0.X, triangle.v1.X, triangle.v2.X});
        float minY = std::min({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
        float maxX = std::max({triangle.v0.X, triangle.v1.X, triangle.v2.X});
        float maxY = std::max({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
        BoundingBox2D triangleBounds(minX, minY, maxX, maxY);
        std::vector<Face> potentialOccluders = m_quadtree->queryArea(triangleBounds);
        if (processTriangle(triangle, potentialOccluders, processedEdges, visibleEdges))
        {
            m_quadtree->insert(triangle);
        }
    }
    return visibleEdges;
}

void HiddenLineRemoval::initializeQuadtree()
{
    // Compute overall bounds using a single pass
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto &face : m_triangles)
    {
        minX = std::min({minX, face.v0.X, face.v1.X, face.v2.X});
        minY = std::min({minY, face.v0.Y, face.v1.Y, face.v2.Y});
        maxX = std::max({maxX, face.v0.X, face.v1.X, face.v2.X});
        maxY = std::max({maxY, face.v0.Y, face.v1.Y, face.v2.Y});
    }

    // Add a small padding to avoid boundary issues
    constexpr float padding = 0.01f;
    minX -= padding;
    minY -= padding;
    maxX += padding;
    maxY += padding;

    BoundingBox2D rootBounds(minX, minY, maxX, maxY);
    m_quadtree = std::make_unique<Quadtree>(rootBounds, 36, 8);
}

void HiddenLineRemoval::sortTrianglesByDepth()
{
    // Sort triangles based on average Z value (depth)
    std::sort(m_triangles.begin(), m_triangles.end(), [](const Face &a, const Face &b) {
        float depthA = (a.v0.Z + a.v1.Z + a.v2.Z) / 3.0f;
        float depthB = (b.v0.Z + b.v1.Z + b.v2.Z) / 3.0f;
        // float depthA = std::min({a.v0.Z, a.v1.Z, a.v2.Z});
        // float depthB = std::min({b.v0.Z, b.v1.Z, b.v2.Z});
        return depthA < depthB;
    });
}

// Corrected getEdgeIntersection function
bool HiddenLineRemoval::getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB, FVector3 &intersectionPoint) const
{
    // Extract coordinates
    float x1 = edgeA.start.X;
    float y1 = edgeA.start.Y;
    float x2 = edgeA.end.X;
    float y2 = edgeA.end.Y;

    float x3 = edgeB.start.X;
    float y3 = edgeB.start.Y;
    float x4 = edgeB.end.X;
    float y4 = edgeB.end.Y;

    // Compute denominator with epsilon
    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    const float EPSILON = 1e-6f;

    if (fabs(denom) < EPSILON)
    {
        // Lines are parallel or coincident
        bool startStart = edgeA.start == edgeB.start;
        bool startEnd = edgeA.start == edgeB.end;
        bool endStart = edgeA.end == edgeB.start;
        bool endEnd = edgeA.end == edgeB.end;

        int overlapCount = startStart + startEnd + endStart + endEnd;

        if (overlapCount > 1)
        {
            // Edges are coincident (all endpoints overlap)
            return false;
        }
        else if (overlapCount == 1)
        {
            // Edges intersect at a single endpoint
            if (startStart)
                intersectionPoint = edgeA.start;
            else if (startEnd)
                intersectionPoint = edgeA.start;
            else if (endStart)
                intersectionPoint = edgeA.end;
            else if (endEnd)
                intersectionPoint = edgeA.end;
            return true;
        }
        else
        {
            // Lines are parallel but do not share any endpoints
            return false;
        }
    }

    // Compute intersection parameters
    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    float u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denom;

    if (t < -EPSILON || t > 1.0f + EPSILON || u < -EPSILON || u > 1.0f + EPSILON)
        return false; // Intersection not within the segments

    // Clamp t and u to [0,1]
    t = std::max(0.0f, std::min(1.0f, t));
    u = std::max(0.0f, std::min(1.0f, u));

    // Calculate intersection point in 3D
    float intersectionX = x1 + t * (x2 - x1);
    float intersectionY = y1 + t * (y2 - y1);
    float intersectionZ = edgeA.start.Z + t * (edgeA.end.Z - edgeA.start.Z);

    intersectionPoint = FVector3(intersectionX, intersectionY, intersectionZ);
    return true;
}

std::pair<Edge3D, Edge3D> HiddenLineRemoval::splitEdge(const Edge3D &edge, const FVector3 &splitPoint) const
{
    Edge3D edgeA(edge.start, splitPoint);
    Edge3D edgeB(splitPoint, edge.end);
    return {edgeA, edgeB};
}
std::vector<Edge3D> HiddenLineRemoval::clipEdgeAgainstTriangle(const Edge3D &edge, const Face &triangle) const
{
    std::vector<FVector3> intersectionPoints;

    Edge3D triEdges[3] = {{triangle.v0, triangle.v1}, {triangle.v1, triangle.v2}, {triangle.v2, triangle.v0}};

    // Check intersection with each edge of the triangle
    for (const auto &triEdge : triEdges)
    {
        FVector3 intersectionPoint;
        if (getEdgeIntersection(edge, triEdge, intersectionPoint))
        {
            intersectionPoints.push_back(intersectionPoint);
        }
    }

    if (intersectionPoints.empty())
    {
        if (isPointInsideTriangle(edge.start, triangle) && isPointInsideTriangle(edge.end, triangle))
        {
            // Edge is fully occluded
            return {};
        }
        else
        {
            // Edge is fully outside the triangle
            return {edge};
        }
    }

    std::sort(intersectionPoints.begin(), intersectionPoints.end(), [&edge](const FVector3 &a, const FVector3 &b) {
        return (a - edge.start).LengthSquared() < (b - edge.start).LengthSquared();
    });

    // Include the start and end points
    std::vector<FVector3> sortedPoints = {edge.start};
    sortedPoints.insert(sortedPoints.end(), intersectionPoints.begin(), intersectionPoints.end());
    sortedPoints.push_back(edge.end);

    // Determine visibility of each segment between consecutive points
    std::vector<Edge3D> clippedEdges;
    for (size_t i = 0; i < sortedPoints.size() - 1; ++i)
    {
        FVector3 midPoint = (sortedPoints[i] + sortedPoints[i + 1]) * 0.5f;
        if (!isPointInsideTriangle(midPoint, triangle))
        {
            clippedEdges.emplace_back(sortedPoints[i], sortedPoints[i + 1]);
        }
    }

    return clippedEdges;
}

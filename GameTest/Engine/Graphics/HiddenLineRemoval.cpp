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
    initalizeQuadtree();
}
std::vector<Edge3D> HiddenLineRemoval::removeHiddenLines()
{
    std::vector<Edge3D> visibleEdges;
    visibleEdges.reserve(m_triangles.size() * 3);


    std::unordered_set<Edge3D, Edge3DHash> processedEdges;
    processedEdges.reserve(m_triangles.size() * 3);

    for (const auto &triangle : m_triangles)
    {
        // query for potential occluders from the quadtree
        float minX = std::min({triangle.v0.X, triangle.v1.X, triangle.v2.X});
        float minY = std::min({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
        float maxX = std::max({triangle.v0.X, triangle.v1.X, triangle.v2.X});
        float maxY = std::max({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
        BoundingBox2D triangleBounds(minX, minY, maxX, maxY);
        std::vector<Face> potentialOccluders = m_quadtree->query(triangleBounds);
        processTriangle(triangle, potentialOccluders, processedEdges, visibleEdges);
        m_quadtree->insert(triangle);
    }

    return visibleEdges;
}

void HiddenLineRemoval::initalizeQuadtree()
{
    // Compute overall bounds
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

    BoundingBox2D rootBounds(minX, minY, maxX, maxY);
    m_quadtree = std::make_unique<Quadtree>(rootBounds,100,6);
}

void HiddenLineRemoval::sortTrianglesByDepth()
{
    // Sort triangles based on average Z value (depth)
    std::sort(m_triangles.begin(), m_triangles.end(), [](const Face &a, const Face &b) {
        float depthA = (a.v0.Z + a.v1.Z + a.v2.Z) / 3.0f;
        float depthB = (b.v0.Z + b.v1.Z + b.v2.Z) / 3.0f;
        //float depthA = std::min({a.v0.Z, a.v1.Z, a.v2.Z});
        //float depthB = std::min({b.v0.Z, b.v1.Z, b.v2.Z});
        return depthA < depthB;
    });
}


bool HiddenLineRemoval::isPointOnEdge(const FVector3 &point, const Edge3D &edge) const
{
    FVector3 edgeVector = edge.end - edge.start;
    float edgeLengthSquared = edgeVector.LengthSquared();

    // Handle zero-length edges
    if (edgeLengthSquared < 1e-12f)
    {
        // Only the start point is considered on the edge
        return point == edge.start;
    }

    FVector3 pointVector = point - edge.start;
    float crossProduct = edgeVector.Cross(pointVector).LengthSquared();
    if (crossProduct > 1e-6f)
        return false;

    float dotProduct = pointVector.Dot(edgeVector);
    if (dotProduct < 0.0f || dotProduct > edgeLengthSquared)
        return false;

    return true;
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


bool HiddenLineRemoval::isPointInsideTriangle(const FVector3 &point, const Face &triangle) const
{
    FVector2 p(point.X, point.Y);
    FVector2 v0(triangle.v0.X, triangle.v0.Y);
    FVector2 v1(triangle.v1.X, triangle.v1.Y);
    FVector2 v2(triangle.v2.X, triangle.v2.Y);

    // Compute vectors
    float denom = (v1.Y - v2.Y) * (v0.X - v2.X) + (v2.X - v1.X) * (v0.Y - v2.Y);
    if (fabs(denom) < 1e-6f)
        return false; // Degenerate triangle

    float a = ((v1.Y - v2.Y) * (p.X - v2.X) + (v2.X - v1.X) * (p.Y - v2.Y)) / denom;
    float b = ((v2.Y - v0.Y) * (p.X - v2.X) + (v0.X - v2.X) * (p.Y - v2.Y)) / denom;
    float c = 1.0f - a - b;

    return (a >= 0.0f) && (b >= 0.0f) && (c >= 0.0f);
}


std::pair<Edge3D, Edge3D> HiddenLineRemoval::splitEdge(const Edge3D &edge, const FVector3 &splitPoint) const
{
    assert(isPointOnEdge(splitPoint, edge));
    Edge3D edgeA(edge.start, splitPoint);
    Edge3D edgeB(splitPoint, edge.end);
    return {edgeA, edgeB};
}
//split at intersections
// Figure out which half is visible
// Keep visible, get rid of invisible

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
            // Avoid duplicate intersection points
            if (std::find_if(intersectionPoints.begin(), intersectionPoints.end(), [&](const FVector3 &p) {
                    return (p - intersectionPoint).LengthSquared() < 1e-6f;
                }) == intersectionPoints.end())
            {
                intersectionPoints.push_back(intersectionPoint);
            }
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

void HiddenLineRemoval::processTriangle(const Face &triangle, const std::vector<Face> &potentialOccluders,
                                        std::unordered_set<Edge3D, Edge3DHash> &processedEdges,
                                        std::vector<Edge3D> &visibleEdges)
{
    Edge3D edges[3] = {{triangle.v0, triangle.v1}, {triangle.v1, triangle.v2}, {triangle.v2, triangle.v0}};
    
    for (const auto &edge : edges)
    {
        if (processedEdges.find(edge) != processedEdges.end())
        {
            continue;
        }
        processedEdges.insert(edge);
        std::vector<Edge3D> segments = {edge};
        for (const auto &occluder : potentialOccluders)
        {
            std::vector<Edge3D> tempSegments;
            tempSegments.reserve(segments.size());
            for (const auto &segment : segments)
            {
                auto clipped = clipEdgeAgainstTriangle(segment, occluder);
                tempSegments.insert(tempSegments.end(), clipped.begin(), clipped.end());
            }
            segments = std::move(tempSegments);
            if (segments.empty())
            {
                break;
            }
        }
        visibleEdges.insert(visibleEdges.end(), segments.begin(), segments.end());
    }

}

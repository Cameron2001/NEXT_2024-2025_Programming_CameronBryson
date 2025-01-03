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
HiddenLineRemoval::HiddenLineRemoval()
{
    m_intersectionPoints.reserve(3);
    m_sortedPoints.reserve(5);
    m_segments.reserve(20);
    m_clippedEdges.reserve(10);
    m_potentialOccluders.reserve(100);
}

std::vector<Edge2D> HiddenLineRemoval::removeHiddenLines(std::vector<Triangle2D> &triangles)
{
    std::sort(triangles.begin(), triangles.end(),
              [](const Triangle2D &a, const Triangle2D &b) { return a.avgZ < b.avgZ; });
    initializeQuadtree(triangles);

    m_visibleEdges.clear();
    m_uniqueEdges.clear();
    m_visibleEdges.reserve(triangles.size() * 3);
    m_uniqueEdges.reserve(triangles.size() * 3);

    for (const auto &triangle : triangles)
    {
        processTriangle(triangle);
    }

    return m_visibleEdges;
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

void HiddenLineRemoval::processTriangle(const Triangle2D &triangle)
{
    m_quadtree->queryTriangle(triangle, m_potentialOccluders);

    m_potentialOccluders.erase(
        std::remove_if(m_potentialOccluders.begin(), m_potentialOccluders.end(),
                       [&](const Triangle2D &occluder) { return sharesVertex(occluder, triangle); }),
        m_potentialOccluders.end());

    Edge2D edges[3];
    createTriangleEdges(triangle, edges);

    for (int i = 0; i < 3; ++i)
    {
        const Edge2D &edge = edges[i];
        if (m_uniqueEdges.insert(edge).second)
        {
            processEdge(edge, m_potentialOccluders);
        }
    }

    m_quadtree->insert(triangle);
    m_potentialOccluders.clear();
}

void HiddenLineRemoval::processEdge(const Edge2D &edge, const std::vector<Triangle2D> &occluders)
{
    m_segments.clear();
    m_segments.emplace_back(edge);

    for (const auto &occluder : occluders)
    {
        m_clippedEdges.clear();

        for (const auto &segment : m_segments)
        {
            clipEdgeAgainstTriangle(segment, occluder);
        }

        if (m_clippedEdges.empty())
        {
            m_segments.clear();
            break;
        }

        m_segments.swap(m_clippedEdges);
    }
    for (const auto &segment : m_segments)
    {
        m_visibleEdges.emplace_back(segment);
    }
}

void HiddenLineRemoval::clipEdgeAgainstTriangle(const Edge2D &edge, const Triangle2D &triangle)
{
    m_intersectionPoints.clear();
    m_sortedPoints.clear();

    Edge2D triEdges[3];
    createTriangleEdges(triangle, triEdges);

    for (int i = 0; i < 3; ++i)
    {
        FVector2 intersectionPoint;
        if (getEdgeIntersection(edge, triEdges[i], intersectionPoint))
        {
            m_intersectionPoints.emplace_back(intersectionPoint);
        }
    }

    if (m_intersectionPoints.empty())
    {
        if (isPointInsideTriangle(edge.start, triangle) && isPointInsideTriangle(edge.end, triangle))
        {
            // Edge is fully occluded
            return;
        }
        else
        {
            // Edge is fully visible
            m_clippedEdges.emplace_back(edge);
            return;
        }
    }

    m_sortedPoints.emplace_back(edge.start);
    for (const auto &point : m_intersectionPoints)

    {
        m_sortedPoints.emplace_back(point);
    }
    m_sortedPoints.emplace_back(edge.end);

    std::sort(m_sortedPoints.begin(), m_sortedPoints.end(), [&](const FVector2 &a, const FVector2 &b) {
        return (a - edge.start).LengthSquared() < (b - edge.start).LengthSquared();
    });

    for (size_t i = 0; i < m_sortedPoints.size() - 1; ++i)
    {
        FVector2 midPoint = (m_sortedPoints[i] + m_sortedPoints[i + 1]) * 0.5f;
        if (!isPointInsideTriangle(midPoint, triangle))
        {
            FVector2 delta = m_sortedPoints[i + 1] - m_sortedPoints[i];
            if (delta.LengthSquared() >= 1e-5f)
            {
                m_clippedEdges.emplace_back(m_sortedPoints[i], m_sortedPoints[i + 1]);
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
        return false;
    }

    const float numeratorA = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
    const float numeratorB = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2);
    const float t = numeratorA / denominator;
    const float u = numeratorB / denominator;

    if (t < -EPSILON || t > 1.0f + EPSILON || u < -EPSILON || u > 1.0f + EPSILON)
        return false;

    intersectionPoint.X = x1 + t * (x2 - x1);
    intersectionPoint.Y = y1 + t * (y2 - y1);
    return true;
}
bool HiddenLineRemoval::isPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle)
{
    float deltaX = point.X - triangle.v0.X;
    float deltaY = point.Y - triangle.v0.Y;

    bool isSideABPositive = (triangle.v1.X - triangle.v0.X) * deltaY - (triangle.v1.Y - triangle.v0.Y) * deltaX > 0;

    if (((triangle.v2.X - triangle.v0.X) * deltaY - (triangle.v2.Y - triangle.v0.Y) * deltaX > 0) == isSideABPositive)
        return false;

    if (((triangle.v2.X - triangle.v1.X) * (point.Y - triangle.v1.Y) -
             (triangle.v2.Y - triangle.v1.Y) * (point.X - triangle.v1.X) >
         0) != isSideABPositive)
        return false;

    return true;
}

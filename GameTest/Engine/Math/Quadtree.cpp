#include "stdafx.h"
#include "BoundingBox.h"
#include "Quadtree.h"
#include <Engine/Graphics/Edge.h>
#include <Engine/Graphics/Triangle.h>
#include <memory>
#include <utility>
#include <vector>

TriangleEntry::TriangleEntry(const Triangle2D &f) : triangle(f)
{
    const float minX = std::min({triangle.v0.x, triangle.v1.x, triangle.v2.x});
    const float minY = std::min({triangle.v0.y, triangle.v1.y, triangle.v2.y});
    const float maxX = std::max({triangle.v0.x, triangle.v1.x, triangle.v2.x});
    const float maxY = std::max({triangle.v0.y, triangle.v1.y, triangle.v2.y});
    bounds = BoundingBox2D(minX, minY, maxX, maxY);
}

Quadtree::Quadtree(const BoundingBox2D &bounds, int capacity, int maxDepth, int level)
    : m_bounds(bounds), m_capacity(capacity), m_maxDepth(maxDepth), m_level(level), m_divided(false)
{
}

bool Quadtree::Insert(const Triangle2D &triangle)
{
    TriangleEntry entry(triangle);

    if (!m_bounds.Intersects(entry.bounds))
    {
        return false;
    }

    if (static_cast<int>(m_triangles.size()) < m_capacity || m_level >= m_maxDepth)
    {
        m_triangles.emplace_back(entry);
        return true;
    }

    // Subdivide if not already divided
    if (!m_divided)
    {
        Subdivide();
    }

    // Determine the quadrant for the triangle's bounding box
    int quadrant = GetQuadrant(m_bounds, entry.bounds);
    if (quadrant != -1)
    {
        switch (quadrant)
        {
        case 0:
            return m_northWest->Insert(triangle);
        case 1:
            return m_northEast->Insert(triangle);
        case 2:
            return m_southWest->Insert(triangle);
        case 3:
            return m_southEast->Insert(triangle);
        default:
            break;
        }
    }

    // If the triangle spans multiple quadrants, keep it in the current node
    m_triangles.emplace_back(entry);
    return true;
}

void Quadtree::QueryArea(const BoundingBox2D &range, std::vector<Triangle2D> &found) const
{
    Query(range, found);
}

void Quadtree::QueryTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &found) const
{
    const float minX = std::min({triangle.v0.x, triangle.v1.x, triangle.v2.x});
    const float minY = std::min({triangle.v0.y, triangle.v1.y, triangle.v2.y});
    const float maxX = std::max({triangle.v0.x, triangle.v1.x, triangle.v2.x});
    const float maxY = std::max({triangle.v0.y, triangle.v1.y, triangle.v2.y});
    BoundingBox2D triangleBounds(minX, minY, maxX, maxY);

    Query(triangleBounds, found, triangle.avgZ);
}

void Quadtree::QueryTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &found, float maxAvgZ) const
{
    const float minX = std::min({triangle.v0.x, triangle.v1.x, triangle.v2.x});
    const float minY = std::min({triangle.v0.y, triangle.v1.y, triangle.v2.y});
    const float maxX = std::max({triangle.v0.x, triangle.v1.x, triangle.v2.x});
    const float maxY = std::max({triangle.v0.y, triangle.v1.y, triangle.v2.y});
    BoundingBox2D triangleBounds(minX, minY, maxX, maxY);

    Query(triangleBounds, found, maxAvgZ);
}

void Quadtree::QueryEdge(const Edge2D &edge, std::vector<Triangle2D> &found) const
{
    const float minX = std::min(edge.start.x, edge.end.x);
    const float minY = std::min(edge.start.y, edge.end.y);
    const float maxX = std::max(edge.start.x, edge.end.x);
    const float maxY = std::max(edge.start.y, edge.end.y);
    BoundingBox2D edgeBounds(minX, minY, maxX, maxY);

    Query(edgeBounds, found);
}

void Quadtree::Query(const BoundingBox2D &range, std::vector<Triangle2D> &found) const
{
    if (!m_bounds.Intersects(range))
    {
        return;
    }

    for (const auto &entry : m_triangles)
    {
        if (range.Intersects(entry.bounds))
        {
            found.emplace_back(entry.triangle);
        }
    }

    // Recursively query child quadrants
    if (m_divided)
    {
        m_northWest->Query(range, found);
        m_northEast->Query(range, found);
        m_southWest->Query(range, found);
        m_southEast->Query(range, found);
    }
}

void Quadtree::Query(const BoundingBox2D &range, std::vector<Triangle2D> &found, float maxAvgZ) const
{
    if (!m_bounds.Intersects(range))
    {
        return;
    }

    for (const auto &entry : m_triangles)
    {
        if (range.Intersects(entry.bounds) && entry.triangle.avgZ < maxAvgZ)
        {
            found.emplace_back(entry.triangle);
        }
    }

    // Recursively query child quadrants
    if (m_divided)
    {
        m_northWest->Query(range, found, maxAvgZ);
        m_northEast->Query(range, found, maxAvgZ);
        m_southWest->Query(range, found, maxAvgZ);
        m_southEast->Query(range, found, maxAvgZ);
    }
}

void Quadtree::Subdivide()
{
    BoundingBox2D nw = ComputeBox(m_bounds, 0);
    BoundingBox2D ne = ComputeBox(m_bounds, 1);
    BoundingBox2D sw = ComputeBox(m_bounds, 2);
    BoundingBox2D se = ComputeBox(m_bounds, 3);

    m_northWest = std::make_unique<Quadtree>(nw, m_capacity, m_maxDepth, m_level + 1);
    m_northEast = std::make_unique<Quadtree>(ne, m_capacity, m_maxDepth, m_level + 1);
    m_southWest = std::make_unique<Quadtree>(sw, m_capacity, m_maxDepth, m_level + 1);
    m_southEast = std::make_unique<Quadtree>(se, m_capacity, m_maxDepth, m_level + 1);

    m_divided = true;

    std::vector<TriangleEntry> remainingTriangles;
    remainingTriangles.reserve(m_triangles.size());

    for (const auto &entry : m_triangles)
    {
        int quadrant = GetQuadrant(m_bounds, entry.bounds);
        if (quadrant != -1)
        {
            switch (quadrant)
            {
            case 0:
                m_northWest->Insert(entry.triangle);
                break;
            case 1:
                m_northEast->Insert(entry.triangle);
                break;
            case 2:
                m_southWest->Insert(entry.triangle);
                break;
            case 3:
                m_southEast->Insert(entry.triangle);
                break;
            }
        }
        else
        {
            remainingTriangles.emplace_back(entry);
        }
    }

    m_triangles.swap(remainingTriangles);
}

BoundingBox2D Quadtree::ComputeBox(const BoundingBox2D &box, int quadrant) const
{
    const float x = box.minX;
    const float y = box.minY;
    const float halfWidth = (box.maxX - box.minX) / 2.0f;
    const float halfHeight = (box.maxY - box.minY) / 2.0f;

    switch (quadrant)
    {
    case 0: // Northwest
        return BoundingBox2D(x, y + halfHeight, x + halfWidth, box.maxY);
    case 1: // Northeast
        return BoundingBox2D(x + halfWidth, y + halfHeight, box.maxX, box.maxY);
    case 2: // Southwest
        return BoundingBox2D(x, y, x + halfWidth, y + halfHeight);
    case 3: // Southeast
        return BoundingBox2D(x + halfWidth, y, box.maxX, y + halfHeight);
    default:
        return BoundingBox2D();
    }
}

int Quadtree::GetQuadrant(const BoundingBox2D &nodeBox, const BoundingBox2D &valueBox) const
{
    const float midX = nodeBox.minX + (nodeBox.maxX - nodeBox.minX) / 2.0f;
    const float midY = nodeBox.minY + (nodeBox.maxY - nodeBox.minY) / 2.0f;

    const bool north = valueBox.minY >= midY;
    const bool south = valueBox.maxY <= midY;
    const bool west = valueBox.maxX <= midX;
    const bool east = valueBox.minX >= midX;

    if (north)
    {
        if (west)
            return 0; // Northwest
        else if (east)
            return 1; // Northeast
    }
    else if (south)
    {
        if (west)
            return 2; // Southwest
        else if (east)
            return 3; // Southeast
    }

    return -1; // Spans multiple quadrants
}

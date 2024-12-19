#include "stdafx.h"
#include "Quadtree.h"
#include <algorithm>
#include <memory>
#include <array>
triangleEntry::triangleEntry(const Triangle &f) : triangle(f)
{
    float minX = std::min({triangle.v0.X, triangle.v1.X, triangle.v2.X});
    float minY = std::min({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
    float maxX = std::max({triangle.v0.X, triangle.v1.X, triangle.v2.X});
    float maxY = std::max({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
    bounds = BoundingBox2D(minX, minY, maxX, maxY);
}

Quadtree::Quadtree(const BoundingBox2D &bounds, int capacity, int maxDepth, int level)
    : m_bounds(bounds), m_capacity(capacity), m_maxDepth(maxDepth), m_level(level), m_divided(false)
{
}

bool Quadtree::insert(const Triangle &triangle)
{
    triangleEntry entry(triangle);

    if (!m_bounds.intersects(entry.bounds))
    {
        return false;
    }

    if (static_cast<int>(m_triangles.size()) < m_capacity || m_level >= m_maxDepth)
    {
        m_triangles.emplace_back(entry);
        return true;
    }

    if (!m_divided)
    {
        subdivide();
    }

    int quadrant = getQuadrant(m_bounds, entry.bounds);
    if (quadrant != -1)
    {
        switch (quadrant)
        {
        case 0:
            return m_northWest->insert(triangle);
        case 1:
            return m_northEast->insert(triangle);
        case 2:
            return m_southWest->insert(triangle);
        case 3:
            return m_southEast->insert(triangle);
        default:
            break;
        }
    }

    m_triangles.emplace_back(entry);
    return true;
}

std::vector<Triangle> Quadtree::queryArea(const BoundingBox2D &range) const
{
    std::vector<Triangle> found;
    query(this, range, found);
    return found;
}

std::vector<Triangle> Quadtree::querytriangle(const Triangle &triangle) const
{
    // query for potential occluders from the quadtree
    float minX = std::min({triangle.v0.X, triangle.v1.X, triangle.v2.X});
    float minY = std::min({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
    float maxX = std::max({triangle.v0.X, triangle.v1.X, triangle.v2.X});
    float maxY = std::max({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y});
    BoundingBox2D triangleBounds(minX, minY, maxX, maxY);
    return queryArea(triangleBounds);
}

std::vector<Triangle> Quadtree::queryEdge(const Edge3D &edge) const
{
    float minX = std::min(edge.start.X, edge.end.X);
    float minY = std::min(edge.start.Y, edge.end.Y);
    float maxX = std::max(edge.start.X, edge.end.X);
    float maxY = std::max(edge.start.Y, edge.end.Y);
    BoundingBox2D edgeBounds(minX, minY, maxX, maxY);
    return queryArea(edgeBounds);
}

void Quadtree::query(const Quadtree *node, const BoundingBox2D &range, std::vector<Triangle> &found) const
{
    if (!node->m_bounds.intersects(range))
    {
        return;
    }

    for (const auto &entry : node->m_triangles)
    {
        if (range.intersects(entry.bounds))
        {
            found.push_back(entry.triangle);
        }
    }

    if (node->m_divided)
    {
        node->m_northWest->query(node->m_northWest.get(), range, found);
        node->m_northEast->query(node->m_northEast.get(), range, found);
        node->m_southWest->query(node->m_southWest.get(), range, found);
        node->m_southEast->query(node->m_southEast.get(), range, found);
    }
}

void Quadtree::subdivide()
{
    BoundingBox2D nw = computeBox(m_bounds, 0);
    BoundingBox2D ne = computeBox(m_bounds, 1);
    BoundingBox2D sw = computeBox(m_bounds, 2);
    BoundingBox2D se = computeBox(m_bounds, 3);

    m_northWest = std::make_unique<Quadtree>(nw, m_capacity, m_maxDepth, m_level + 1);
    m_northEast = std::make_unique<Quadtree>(ne, m_capacity, m_maxDepth, m_level + 1);
    m_southWest = std::make_unique<Quadtree>(sw, m_capacity, m_maxDepth, m_level + 1);
    m_southEast = std::make_unique<Quadtree>(se, m_capacity, m_maxDepth, m_level + 1);

    m_divided = true;

    // Redistribute existing triangles to children
    std::vector<triangleEntry> remainingtriangles;

    for (const auto &entry : m_triangles)
    {
        int quadrant = getQuadrant(m_bounds, entry.bounds);
        if (quadrant != -1)
        {
            switch (quadrant)
            {
            case 0:
                m_northWest->insert(entry.triangle);
                break;
            case 1:
                m_northEast->insert(entry.triangle);
                break;
            case 2:
                m_southWest->insert(entry.triangle);
                break;
            case 3:
                m_southEast->insert(entry.triangle);
                break;
            default:
                remainingtriangles.emplace_back(entry);
                break;
            }
        }
        else
        {
            remainingtriangles.emplace_back(entry);
        }
    }

    m_triangles.swap(remainingtriangles);
}
BoundingBox2D Quadtree::computeBox(const BoundingBox2D &box, int quadrant) const
{
    float x = box.minX;
    float y = box.minY;
    float halfWidth = (box.maxX - box.minX) / 2.0f;
    float halfHeight = (box.maxY - box.minY) / 2.0f;

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

// Determine the quadrant for a given triangle's bounding box
int Quadtree::getQuadrant(const BoundingBox2D &nodeBox, const BoundingBox2D &valueBox) const
{
    float midX = nodeBox.minX + (nodeBox.maxX - nodeBox.minX) / 2.0f;
    float midY = nodeBox.minY + (nodeBox.maxY - nodeBox.minY) / 2.0f;

    bool north = valueBox.minY >= midY;
    bool south = valueBox.maxY <= midY;
    bool west = valueBox.maxX <= midX;
    bool east = valueBox.minX >= midX;

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

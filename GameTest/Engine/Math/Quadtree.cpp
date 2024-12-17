#include "stdafx.h"
#include "Quadtree.h"
#include <algorithm>
#include <memory>
#include <array>
FaceEntry::FaceEntry(const Face &f) : face(f)
{
    float minX = std::min({face.v0.X, face.v1.X, face.v2.X});
    float minY = std::min({face.v0.Y, face.v1.Y, face.v2.Y});
    float maxX = std::max({face.v0.X, face.v1.X, face.v2.X});
    float maxY = std::max({face.v0.Y, face.v1.Y, face.v2.Y});
    bounds = BoundingBox2D(minX, minY, maxX, maxY);
}

Quadtree::Quadtree(const BoundingBox2D &bounds, int capacity, int maxDepth, int level)
    : m_bounds(bounds), m_capacity(capacity), m_maxDepth(maxDepth), m_level(level), m_divided(false)
{
}

bool Quadtree::insert(const Face &face)
{
    FaceEntry entry(face);

    if (!m_bounds.intersects(entry.bounds))
    {
        return false;
    }

    if (static_cast<int>(m_faces.size()) < m_capacity || m_level >= m_maxDepth)
    {
        m_faces.emplace_back(entry);
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
            return m_northWest->insert(face);
        case 1:
            return m_northEast->insert(face);
        case 2:
            return m_southWest->insert(face);
        case 3:
            return m_southEast->insert(face);
        default:
            break;
        }
    }

    m_faces.emplace_back(entry);
    return true;
}


std::vector<Face> Quadtree::queryArea(const BoundingBox2D &range) const
{
    std::vector<Face> found;
    query(this, range, found);
    return found;
}



void Quadtree::query(const Quadtree *node, const BoundingBox2D &range, std::vector<Face> &found) const
{
    if (!node->m_bounds.intersects(range))
    {
        return;
    }

    for (const auto &entry : node->m_faces)
    {
        if (range.intersects(entry.bounds))
        {
            found.push_back(entry.face);
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

    // Redistribute existing faces to children
    std::vector<FaceEntry> remainingFaces;

    for (const auto &entry : m_faces)
    {
        int quadrant = getQuadrant(m_bounds, entry.bounds);
        if (quadrant != -1)
        {
            switch (quadrant)
            {
            case 0:
                m_northWest->insert(entry.face);
                break;
            case 1:
                m_northEast->insert(entry.face);
                break;
            case 2:
                m_southWest->insert(entry.face);
                break;
            case 3:
                m_southEast->insert(entry.face);
                break;
            default:
                remainingFaces.emplace_back(entry);
                break;
            }
        }
        else
        {
            remainingFaces.emplace_back(entry);
        }
    }

    m_faces.swap(remainingFaces);
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

// Determine the quadrant for a given face's bounding box
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


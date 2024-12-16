#include "stdafx.h"
#include "Quadtree.h"
#include <algorithm>
#include <memory>

Quadtree::Quadtree(const BoundingBox2D &bounds, int capacity, int maxDepth, int level)
    : m_bounds(bounds), m_capacity(capacity), m_maxDepth(maxDepth), m_level(level), m_divided(false)
{
}

bool Quadtree::insert(const Face &face)
{
    // Compute the bounding box of the face
    float minX = std::min({face.v0.X, face.v1.X, face.v2.X});
    float minY = std::min({face.v0.Y, face.v1.Y, face.v2.Y});
    float maxX = std::max({face.v0.X, face.v1.X, face.v2.X});
    float maxY = std::max({face.v0.Y, face.v1.Y, face.v2.Y});
    BoundingBox2D faceBounds(minX, minY, maxX, maxY);

    if (!m_bounds.intersects(faceBounds))
    {
        return false;
    }

    if ((m_faces.size() < m_capacity) || (m_level >= m_maxDepth))
    {
        m_faces.push_back(face);
        return true;
    }

    if (!m_divided)
    {
        subdivide();
    }

    bool insertedIntoChild = false;
    insertedIntoChild |= m_northWest->insert(face);
    insertedIntoChild |= m_northEast->insert(face);
    insertedIntoChild |= m_southWest->insert(face);
    insertedIntoChild |= m_southEast->insert(face);

    if (!insertedIntoChild)
    {
        m_faces.push_back(face);
    }

    return true;
}


std::vector<Face> Quadtree::query(const BoundingBox2D &range) const
{
    std::vector<Face> found;

    if (!m_bounds.intersects(range))
    {
        return found;
    }

    for (const Face &face : m_faces)
    {
        float minX = std::min({face.v0.X, face.v1.X, face.v2.X});
        float minY = std::min({face.v0.Y, face.v1.Y, face.v2.Y});
        float maxX = std::max({face.v0.X, face.v1.X, face.v2.X});
        float maxY = std::max({face.v0.Y, face.v1.Y, face.v2.Y});
        BoundingBox2D faceBounds(minX, minY, maxX, maxY);

        if (range.intersects(faceBounds))
        {
            found.push_back(face);
        }
    }

    if (m_divided)
    {
        std::vector<Face> temp;

        temp = m_northWest->query(range);
        found.insert(found.end(), temp.begin(), temp.end());

        temp = m_northEast->query(range);
        found.insert(found.end(), temp.begin(), temp.end());

        temp = m_southWest->query(range);
        found.insert(found.end(), temp.begin(), temp.end());

        temp = m_southEast->query(range);
        found.insert(found.end(), temp.begin(), temp.end());
    }

    return found;
}

void Quadtree::subdivide()
{
    float x = m_bounds.minX;
    float y = m_bounds.minY;
    float w = (m_bounds.maxX - m_bounds.minX) / 2.0f;
    float h = (m_bounds.maxY - m_bounds.minY) / 2.0f;

    BoundingBox2D nwBounds(x, y + h, x + w, y + 2 * h);
    BoundingBox2D neBounds(x + w, y + h, x + 2 * w, y + 2 * h);
    BoundingBox2D swBounds(x, y, x + w, y + h);
    BoundingBox2D seBounds(x + w, y, x + 2 * w, y + h);

    // Create child nodes
    m_northWest = std::make_unique<Quadtree>(nwBounds, m_capacity, m_maxDepth, m_level + 1);
    m_northEast = std::make_unique<Quadtree>(neBounds, m_capacity, m_maxDepth, m_level + 1);
    m_southWest = std::make_unique<Quadtree>(swBounds, m_capacity, m_maxDepth, m_level + 1);
    m_southEast = std::make_unique<Quadtree>(seBounds, m_capacity, m_maxDepth, m_level + 1);

    m_divided = true;
}
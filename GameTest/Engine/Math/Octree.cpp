#include "stdafx.h"
#include "Octree.h"
#include <algorithm> // For std::max and std::minmax
#include <set>       // For std::set

ColliderEntry::ColliderEntry(const BoundingBox3D &bounds_, unsigned int entityID) : EntityID(entityID), bounds(bounds_)
{
}

Octree::Octree(const BoundingBox3D &bounds, int capacity, int maxDepth, int level)
    : m_bounds(bounds), m_capacity(capacity), m_maxDepth(maxDepth), m_level(level)
{
    for (auto &child : m_children)
    {
        child = nullptr;
    }
}

void Octree::insert(const SphereBoundsComponent &sphere, const TransformComponent &transform, unsigned int entityID)
{
    float scaledRadius = sphere.radius * std::max({transform.Scale.X, transform.Scale.Y, transform.Scale.Z});

    BoundingBox3D colliderBounds(transform.Position.X - scaledRadius, transform.Position.Y - scaledRadius,
                                 transform.Position.Z - scaledRadius, transform.Position.X + scaledRadius,
                                 transform.Position.Y + scaledRadius, transform.Position.Z + scaledRadius);

    insert(ColliderEntry(colliderBounds, entityID));
}

void Octree::insert(const BoxBoundsComponent &box, const TransformComponent &transform, unsigned int entityID)
{
    FVector3 scaledExtents(box.extents.X * transform.Scale.X, box.extents.Y * transform.Scale.Y,
                           box.extents.Z * transform.Scale.Z);

    BoundingBox3D colliderBounds(transform.Position.X - scaledExtents.X, transform.Position.Y - scaledExtents.Y,
                                 transform.Position.Z - scaledExtents.Z, transform.Position.X + scaledExtents.X,
                                 transform.Position.Y + scaledExtents.Y, transform.Position.Z + scaledExtents.Z);

    insert(ColliderEntry(colliderBounds, entityID));
}

void Octree::insert(const ColliderEntry &entry)
{
    // If the entry's bounds do not intersect this node's bounds, do not insert
    if (!m_bounds.intersects(entry.bounds))
    {
        return;
    }

    // Subdivide if necessary
    if (m_children[0] == nullptr && m_level < m_maxDepth)
    {
        if (m_colliders.size() >= static_cast<size_t>(m_capacity))
        {
            subdivide();
        }
    }

    bool insertedIntoChild = false;
    if (m_children[0] != nullptr)
    {
        for (auto &child : m_children)
        {
            if (child->m_bounds.intersects(entry.bounds))
            {
                child->insert(entry);
                insertedIntoChild = true;
                // Do not break; the entry can be in multiple children
            }
        }
    }

    // If not inserted into any child, store in this node
    if (!insertedIntoChild)
    {
        m_colliders.push_back(entry);
    }
}

void Octree::subdivide()
{
    const float x = m_bounds.minX;
    const float y = m_bounds.minY;
    const float z = m_bounds.minZ;
    const float midX = (m_bounds.minX + m_bounds.maxX) / 2.0f;
    const float midY = (m_bounds.minY + m_bounds.maxY) / 2.0f;
    const float midZ = (m_bounds.minZ + m_bounds.maxZ) / 2.0f;

    // Create 8 child nodes
    m_children[0] =
        std::make_unique<Octree>(BoundingBox3D(x, y, z, midX, midY, midZ), m_capacity, m_maxDepth, m_level + 1);
    m_children[1] = std::make_unique<Octree>(BoundingBox3D(midX, y, z, m_bounds.maxX, midY, midZ), m_capacity,
                                             m_maxDepth, m_level + 1);
    m_children[2] = std::make_unique<Octree>(BoundingBox3D(x, midY, z, midX, m_bounds.maxY, midZ), m_capacity,
                                             m_maxDepth, m_level + 1);
    m_children[3] = std::make_unique<Octree>(BoundingBox3D(midX, midY, z, m_bounds.maxX, m_bounds.maxY, midZ),
                                             m_capacity, m_maxDepth, m_level + 1);
    m_children[4] = std::make_unique<Octree>(BoundingBox3D(x, y, midZ, midX, midY, m_bounds.maxZ), m_capacity,
                                             m_maxDepth, m_level + 1);
    m_children[5] = std::make_unique<Octree>(BoundingBox3D(midX, y, midZ, m_bounds.maxX, midY, m_bounds.maxZ),
                                             m_capacity, m_maxDepth, m_level + 1);
    m_children[6] = std::make_unique<Octree>(BoundingBox3D(x, midY, midZ, midX, m_bounds.maxY, m_bounds.maxZ),
                                             m_capacity, m_maxDepth, m_level + 1);
    m_children[7] =
        std::make_unique<Octree>(BoundingBox3D(midX, midY, midZ, m_bounds.maxX, m_bounds.maxY, m_bounds.maxZ),
                                 m_capacity, m_maxDepth, m_level + 1);

    // Re-insert existing colliders into children
    std::vector<ColliderEntry> oldColliders = m_colliders;
    m_colliders.clear();
    for (const auto &entry : oldColliders)
    {
        insert(entry);
    }
}

void Octree::getPotentialCollisions(std::vector<std::pair<unsigned int, unsigned int>> &potentialCollisions) const
{
    std::set<std::pair<unsigned int, unsigned int>> collisionsSet;
    collectPotentialCollisions(collisionsSet);
    potentialCollisions.assign(collisionsSet.begin(), collisionsSet.end());
}

void Octree::collectPotentialCollisions(std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const
{
    // Check all pairs within this node
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_colliders.size(); ++j)
        {
            // Verify actual intersection
            if (m_colliders[i].bounds.intersects(m_colliders[j].bounds))
            {
                potentialCollisions.emplace(std::minmax(m_colliders[i].EntityID, m_colliders[j].EntityID));
            }
        }
    }

    // Check collisions between this node's colliders and descendants
    for (const auto &child : m_children)
    {
        if (child)
        {
            // Check collisions between this node's colliders and child's colliders
            for (const auto &entry : m_colliders)
            {
                child->collectCollisionsWithEntry(entry, potentialCollisions);
            }

            // Recursively collect potential collisions from children
            child->collectPotentialCollisions(potentialCollisions);
        }
    }
}


void Octree::collectCollisionsWithEntry(const ColliderEntry &entry,
                                        std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const
{
    // Check collisions between the given entry and colliders in this node
    for (const auto &collider : m_colliders)
    {
        // Verify actual intersection
        if (entry.bounds.intersects(collider.bounds))
        {
            potentialCollisions.emplace(std::minmax(entry.EntityID, collider.EntityID));
        }
    }

    // Recursively check in children
    for (const auto &child : m_children)
    {
        if (child)
        {
            child->collectCollisionsWithEntry(entry, potentialCollisions);
        }
    }
}

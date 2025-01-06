#include "stdafx.h"
#include "Octree.h"
#include <algorithm>
#include <set>

ColliderEntry::ColliderEntry(const BoundingSphere3D &bounds_, unsigned int entityID)
    : EntityID(entityID), sphereBounds(bounds_)
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

void Octree::Insert(const SphereBoundsComponent &sphere, const TransformComponent &transform, unsigned int entityID)
{
    // Calculate scaled radius considering the maximum scale component
    float scaledRadius = sphere.radius * std::max({transform.Scale.X, transform.Scale.Y, transform.Scale.Z});

    BoundingSphere3D colliderSphere(transform.Position, scaledRadius);

    ColliderEntry colliderEntry(colliderSphere, entityID);

    Insert(colliderEntry);
}

void Octree::Insert(const BoxBoundsComponent &box, const TransformComponent &transform, unsigned int entityID)
{
    // Calculate scaled half-extents
    FVector3 scaledExtents(box.extents.X * transform.Scale.X, box.extents.Y * transform.Scale.Y,
                           box.extents.Z * transform.Scale.Z);

    float radius = scaledExtents.Length();

    BoundingSphere3D colliderSphere(transform.Position, radius);

    ColliderEntry colliderEntry(colliderSphere, entityID);

    Insert(colliderEntry);
}

void Octree::Insert(const ColliderEntry &entry)
{
    // Check if the sphere intersects with this node's bounds (AABB)
    if (!m_bounds.IntersectsSphere(entry.sphereBounds))
    {
        return;
    }

    // Subdivide if necessary
    if (m_children[0] == nullptr && m_level < m_maxDepth)
    {
        if (m_colliders.size() >= static_cast<size_t>(m_capacity))
        {
            Subdivide();
        }
    }

    bool insertedIntoChild = false;
    if (m_children[0] != nullptr)
    {
        for (auto &child : m_children)
        {
            if (child->m_bounds.IntersectsSphere(entry.sphereBounds))
            {
                child->Insert(entry);
                insertedIntoChild = true;
                // Do not break; the entry can reside in multiple children
            }
        }
    }

    // If not inserted into any child, store in this node
    if (!insertedIntoChild)
    {
        m_colliders.push_back(entry);
    }
}

void Octree::Subdivide()
{
    const float x = m_bounds.minX;
    const float y = m_bounds.minY;
    const float z = m_bounds.minZ;
    const float midX = (m_bounds.minX + m_bounds.maxX) / 2.0f;
    const float midY = (m_bounds.minY + m_bounds.maxY) / 2.0f;
    const float midZ = (m_bounds.minZ + m_bounds.maxZ) / 2.0f;

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
        Insert(entry);
    }
}

void Octree::GetPotentialCollisions(std::vector<std::pair<unsigned int, unsigned int>> &potentialCollisions) const
{
    std::set<std::pair<unsigned int, unsigned int>> collisionsSet;
    CollectPotentialCollisions(collisionsSet);
    potentialCollisions.assign(collisionsSet.begin(), collisionsSet.end());
}

void Octree::CollectPotentialCollisions(std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const
{
    // Check all pairs within this node
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_colliders.size(); ++j)
        {
            // Sphere-Sphere intersection
            if (m_colliders[i].sphereBounds.Intersects(m_colliders[j].sphereBounds))
            {
                potentialCollisions.emplace(std::minmax(m_colliders[i].EntityID, m_colliders[j].EntityID));
            }
        }
    }

    for (const auto &child : m_children)
    {
        if (child)
        {
            // Check collisions between this node's colliders and child's colliders
            for (const auto &entry : m_colliders)
            {
                child->CollectCollisionsWithEntry(entry, potentialCollisions);
            }

            // Recursively collect potential collisions from children
            child->CollectPotentialCollisions(potentialCollisions);
        }
    }
}

void Octree::CollectCollisionsWithEntry(const ColliderEntry &entry,
                                        std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const
{
    for (const auto &collider : m_colliders)
    {
        // Sphere-Sphere intersection
        if (entry.sphereBounds.Intersects(collider.sphereBounds))
        {
            potentialCollisions.emplace(std::minmax(entry.EntityID, collider.EntityID));
        }
    }

    // Recursively check in children
    for (const auto &child : m_children)
    {
        if (child)
        {
            child->CollectCollisionsWithEntry(entry, potentialCollisions);
        }
    }
}

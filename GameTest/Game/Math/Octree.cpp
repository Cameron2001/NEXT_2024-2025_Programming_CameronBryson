#include "stdafx.h"
#include "Octree.h"
#include <algorithm>
#include <set>
ColliderEntry::ColliderEntry(const BoundingBox3D &bounds_, unsigned int entityID, bool isDynamic)
    : EntityID(entityID), boxBounds(bounds_), isDynamic(isDynamic)
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

void Octree::Insert(const SphereBoundsComponent &sphere, const TransformComponent &transform, unsigned int entityID, bool isDynamic)
{
    FVector3 minPoint = transform.position - FVector3(sphere.radius, sphere.radius, sphere.radius);
    FVector3 maxPoint = transform.position + FVector3(sphere.radius, sphere.radius, sphere.radius);
    BoundingBox3D aabb(minPoint, maxPoint);

    ColliderEntry colliderEntry(aabb, entityID, isDynamic);
    Insert(colliderEntry);
}

void Octree::Insert(const BoxBoundsComponent &box, const TransformComponent &transform, unsigned int entityID,
                    bool isDynamic)
{
    BoundingBox3D aabb = BoundingBox3D::CreateTransformedBox(box,transform);
    ColliderEntry colliderEntry(aabb, entityID, isDynamic);
    Insert(colliderEntry);
}

void Octree::Insert(const ColliderEntry &entry)
{
    if (!m_bounds.Intersects(entry.boxBounds))
    {
        return;
    }

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
            if (child->m_bounds.Intersects(entry.boxBounds))
            {
                child->Insert(entry);
                insertedIntoChild = true;
            }
        }
    }

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


void Octree::ClearDynamicColliders()
{
    m_colliders.erase(std::remove_if(m_colliders.begin(), m_colliders.end(),
                                     [](const ColliderEntry &entry) { return entry.isDynamic; }),
                      m_colliders.end());

    for (auto &child : m_children)
    {
        if (child)
        {
            child->ClearDynamicColliders();
        }
    }
}


void Octree::CollectPotentialCollisions(std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const
{
    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_colliders.size(); ++j)
        {
            if (m_colliders[i].boxBounds.Intersects(m_colliders[j].boxBounds))
            {
                potentialCollisions.emplace(std::minmax(m_colliders[i].EntityID, m_colliders[j].EntityID));
            }
        }
    }

    for (const auto &child : m_children)
    {
        if (child)
        {
            for (const auto &entry : m_colliders)
            {
                child->CollectCollisionsWithEntry(entry, potentialCollisions);
            }
            child->CollectPotentialCollisions(potentialCollisions);
        }
    }
}

void Octree::CollectCollisionsWithEntry(const ColliderEntry &entry,
                                        std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const
{
    for (const auto &collider : m_colliders)
    {
        if (entry.boxBounds.Intersects(collider.boxBounds))
        {
            potentialCollisions.emplace(std::minmax(entry.EntityID, collider.EntityID));
        }
    }

    for (const auto &child : m_children)
    {
        if (child)
        {
            child->CollectCollisionsWithEntry(entry, potentialCollisions);
        }
    }
}

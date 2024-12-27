#include "stdafx.h"
#include "Octree.h"

ColliderEntry::ColliderEntry(const BoundingBox3D &bounds_, unsigned int entityID) : bounds(bounds_), EntityID(entityID)
{
}

Octree::Octree(const BoundingBox3D &bounds, int capacity, int maxDepth, int level)
    : m_bounds(bounds), m_capacity(capacity), m_maxDepth(maxDepth), m_level(level), m_divided(false)
{
    for (auto &child : m_children)
    {
        child = nullptr;
    }
}

bool Octree::insert(const SphereBoundsComponent &sphere, const TransformComponent &transform, unsigned int entityID)
{
    // Compute the scaled radius
    float scaledRadius = sphere.radius * std::max({transform.Scale.X, transform.Scale.Y, transform.Scale.Z});

    // Compute the bounds of the sphere
    BoundingBox3D colliderBounds(transform.Position.X - scaledRadius, transform.Position.Y - scaledRadius,
                                 transform.Position.Z - scaledRadius, transform.Position.X + scaledRadius,
                                 transform.Position.Y + scaledRadius, transform.Position.Z + scaledRadius);

    return insert(colliderBounds, entityID);
}

bool Octree::insert(const BoxBoundsComponent &box, const TransformComponent &transform, unsigned int entityID)
{
    // Compute scaled extents
    FVector3 scaledExtents(box.extents.X * transform.Scale.X, box.extents.Y * transform.Scale.Y,
                           box.extents.Z * transform.Scale.Z);

    // Compute the bounds of the box
    BoundingBox3D colliderBounds(transform.Position.X - scaledExtents.X, transform.Position.Y - scaledExtents.Y,
                                 transform.Position.Z - scaledExtents.Z, transform.Position.X + scaledExtents.X,
                                 transform.Position.Y + scaledExtents.Y, transform.Position.Z + scaledExtents.Z);

    return insert(colliderBounds, entityID);
}
bool Octree::insert(const BoundingBox3D &colliderBounds, unsigned int entityID)
{
    // If the collider is not within this node's bounds, return false
    if (!m_bounds.intersects(colliderBounds))
    {
        return false;
    }

    // If capacity not reached or max depth, add collider here
    if (static_cast<int>(m_colliders.size()) < m_capacity || m_level >= m_maxDepth)
    {
        m_colliders.emplace_back(colliderBounds, entityID);
        return true;
    }

    // Subdivide if not already divided
    if (!m_divided)
    {
        subdivide();
    }

    // Determine the octant the collider belongs to
    int octant = getOctant(m_bounds, colliderBounds);
    if (octant != -1)
    {
        return m_children[octant]->insert(colliderBounds, entityID);
    }
    else
    {
        // Collider spans multiple octants; keep it at this level
        m_colliders.emplace_back(colliderBounds, entityID);
        return true;
    }
}
std::vector<unsigned int> Octree::queryArea(const BoundingBox3D &range) const
{
    std::vector<unsigned int> found;
    query(this, range, found);
    return found;
}

std::vector<unsigned int> Octree::querySphere(const SphereBoundsComponent &sphere,
                                              const TransformComponent &transform) const
{

    float scaledRadius = sphere.radius * std::max({transform.Scale.X, transform.Scale.Y, transform.Scale.Z});

    BoundingBox3D sphereBounds(transform.Position.X - scaledRadius, transform.Position.Y - scaledRadius,
                               transform.Position.Z - scaledRadius, transform.Position.X + scaledRadius,
                               transform.Position.Y + scaledRadius, transform.Position.Z + scaledRadius);

    return queryArea(sphereBounds);
}

std::vector<unsigned int> Octree::queryBox(const BoxBoundsComponent &box, const TransformComponent &transform) const
{

    FVector3 scaledExtents = FVector3(box.extents.X * transform.Scale.X, box.extents.Y * transform.Scale.Y,
                                      box.extents.Z * transform.Scale.Z);

    BoundingBox3D boxBounds(transform.Position.X - scaledExtents.X, transform.Position.Y - scaledExtents.Y,
                            transform.Position.Z - scaledExtents.Z, transform.Position.X + scaledExtents.X,
                            transform.Position.Y + scaledExtents.Y, transform.Position.Z + scaledExtents.Z);

    return queryArea(boxBounds);
}

void Octree::subdivide()
{
    // Create child nodes
    for (int i = 0; i < 8; ++i)
    {
        BoundingBox3D childBounds = computeBox(m_bounds, i);
        m_children[i] = std::make_unique<Octree>(childBounds, m_capacity, m_maxDepth, m_level + 1);
    }

    m_divided = true;

    // Redistribute existing colliders to children
    std::vector<ColliderEntry> remainingColliders;

    for (const auto &entry : m_colliders)
    {
        int octant = getOctant(m_bounds, entry.bounds);
        if (octant != -1)
        {
            if (m_children[octant]->insert(entry.bounds, entry.EntityID))
            {
                continue;
            }
        }
        remainingColliders.emplace_back(entry);
    }

    m_colliders.swap(remainingColliders);
}

void Octree::query(const Octree *node, const BoundingBox3D &range, std::vector<unsigned int> &found)
{
    if (!node->m_bounds.intersects(range))
    {
        return;
    }

    for (const auto &entry : node->m_colliders)
    {
        if (range.intersects(entry.bounds))
        {
            found.push_back(entry.EntityID);
        }
    }

    if (node->m_divided)
    {
        for (const auto &child : node->m_children)
        {
            if (child)
            {
                query(child.get(), range, found);
            }
        }
    }
}

BoundingBox3D Octree::computeBox(const BoundingBox3D &box, int octant)
{
    const float x = box.minX;
    const float y = box.minY;
    const float z = box.minZ;
    const float midX = (box.minX + box.maxX) / 2.0f;
    const float midY = (box.minY + box.maxY) / 2.0f;
    const float midZ = (box.minZ + box.maxZ) / 2.0f;

    switch (octant)
    {
    case 0: // Left Bottom Back
        return {x, y, z, midX, midY, midZ};
    case 1: // Right Bottom Back
        return {midX, y, z, box.maxX, midY, midZ};
    case 2: // Left Top Back
        return {x, midY, z, midX, box.maxY, midZ};
    case 3: // Right Top Back
        return {midX, midY, z, box.maxX, box.maxY, midZ};
    case 4: // Left Bottom Front
        return {x, y, midZ, midX, midY, box.maxZ};
    case 5: // Right Bottom Front
        return {midX, y, midZ, box.maxX, midY, box.maxZ};
    case 6: // Left Top Front
        return {x, midY, midZ, midX, box.maxY, box.maxZ};
    case 7: // Right Top Front
        return {midX, midY, midZ, box.maxX, box.maxY, box.maxZ};
    default:
        return {};
    }
}

int Octree::getOctant(const BoundingBox3D &nodeBox, const BoundingBox3D &valueBox)
{
    const float midX = (nodeBox.minX + nodeBox.maxX) / 2.0f;
    const float midY = (nodeBox.minY + nodeBox.maxY) / 2.0f;
    const float midZ = (nodeBox.minZ + nodeBox.maxZ) / 2.0f;

    bool inLeft = valueBox.maxX <= midX;
    bool inRight = valueBox.minX >= midX;
    bool inBottom = valueBox.maxY <= midY;
    bool inTop = valueBox.minY >= midY;
    bool inBack = valueBox.maxZ <= midZ;
    bool inFront = valueBox.minZ >= midZ;

    if ((inLeft || inRight) && (inBottom || inTop) && (inBack || inFront))
    {
        int octant = 0;
        if (inRight)
            octant |= 1;
        if (inTop)
            octant |= 2;
        if (inFront)
            octant |= 4;
        return octant;
    }

    return -1;
}

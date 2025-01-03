#pragma once
#include "BoundingSphere.h"
#include "BoundingBox.h"
#include <memory>
#include <vector>
#include <set> // For std::set
#include "Engine/Core/Components.h"

struct ColliderEntry
{
    unsigned int EntityID;
    BoundingSphere3D sphereBounds;

    ColliderEntry(const BoundingSphere3D &bounds_, unsigned int entityID);
};

class Octree
{
  public:
    Octree(const BoundingBox3D &bounds, int capacity = 4, int maxDepth = 10, int level = 0);

    void insert(const SphereBoundsComponent &sphere, const TransformComponent &transform, unsigned int entityID);
    void insert(const BoxBoundsComponent &box, const TransformComponent &transform, unsigned int entityID);

    void insert(const ColliderEntry &entry);

    void getPotentialCollisions(std::vector<std::pair<unsigned int, unsigned int>> &potentialCollisions) const;

  private:
    void subdivide();

    void collectPotentialCollisions(std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const;

    void collectCollisionsWithEntry(const ColliderEntry &entry,
                                    std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const;

    BoundingBox3D m_bounds;
    int m_capacity;
    int m_maxDepth;
    int m_level;

    std::vector<ColliderEntry> m_colliders;
    std::unique_ptr<Octree> m_children[8];
};

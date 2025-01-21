#pragma once
#include "BoundingSphere.h"
#include "BoundingBox.h"
#include <memory>
#include <vector>
#include <set> 
#include "Game/Core/Components.h"

struct ColliderEntry
{
    unsigned int EntityID;
    BoundingBox3D boxBounds;
    bool isDynamic = false;


    ColliderEntry(const BoundingBox3D &bounds_, unsigned int entityID, bool isDynamic);
};

class Octree
{
  public:
    Octree(const BoundingBox3D &bounds, int capacity = 4, int maxDepth = 10, int level = 0);

    void Insert(const SphereBoundsComponent &sphere, const TransformComponent &transform, unsigned int entityID, bool isDynamic);
    void Insert(const BoxBoundsComponent &box, const TransformComponent &transform, unsigned int entityID, bool isDynamic);

    void Insert(const ColliderEntry &entry);

    void GetPotentialCollisions(std::vector<std::pair<unsigned int, unsigned int>> &potentialCollisions) const;

    void ClearDynamicColliders();

  private:
    void Subdivide();

    void CollectPotentialCollisions(std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const;

    void CollectCollisionsWithEntry(const ColliderEntry &entry,
                                    std::set<std::pair<unsigned int, unsigned int>> &potentialCollisions) const;

    BoundingBox3D m_bounds;
    int m_capacity;
    int m_maxDepth;
    int m_level;

    std::vector<ColliderEntry> m_colliders;
    std::unique_ptr<Octree> m_children[8];
};

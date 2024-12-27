#pragma once
#include "BoundingBox.h"
#include <memory>
#include <vector>
#include <Engine/Core/Components.h>
struct ColliderEntry
{
    unsigned int EntityID;
    BoundingBox3D bounds;

    ColliderEntry(const BoundingBox3D &bounds_, unsigned int entityID);
};
class Octree
{
  public:
    Octree(const BoundingBox3D &bounds, int capacity = 4, int maxDepth = 10, int level = 0);

    bool insert(const SphereBoundsComponent &sphere, const TransformComponent &transform, unsigned int entityID);
    bool insert(const BoxBoundsComponent &box, const TransformComponent &transform, unsigned int entityID);

    bool insert(const BoundingBox3D &colliderBounds, unsigned int entityID);

    std::vector<unsigned int> queryArea(const BoundingBox3D &range) const;
    std::vector<unsigned int> querySphere(const SphereBoundsComponent &sphere,
                                          const TransformComponent &transform) const;
    std::vector<unsigned int> queryBox(const BoxBoundsComponent &box, const TransformComponent &transform) const;

  private:
    void subdivide();
    static void query(const Octree *node, const BoundingBox3D &range, std::vector<unsigned int> &found);
    static BoundingBox3D computeBox(const BoundingBox3D &box, int octant);
    static int getOctant(const BoundingBox3D &nodeBox, const BoundingBox3D &valueBox);

    BoundingBox3D m_bounds;
    int m_capacity;
    int m_maxDepth;
    int m_level;
    std::vector<ColliderEntry> m_colliders;
    bool m_divided;

    std::unique_ptr<Octree> m_children[8];
};

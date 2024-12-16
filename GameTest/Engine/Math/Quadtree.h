#pragma once
#include "BoundingBox.h"
#include "Engine/Graphics/Face.h"
#include <memory>
#include <vector>

class Quadtree
{
  public:
    Quadtree(const BoundingBox2D &bounds, int capacity = 4, int maxDepth = 10, int level = 0);

    bool insert(const Face &face);
    std::vector<Face> query(const BoundingBox2D &range) const;

  private:
    void subdivide();
    BoundingBox2D m_bounds;
    int m_capacity;
    int m_maxDepth;
    int m_level;
    std::vector<Face> m_faces;
    bool m_divided;

    std::unique_ptr<Quadtree> m_northWest;
    std::unique_ptr<Quadtree> m_northEast;
    std::unique_ptr<Quadtree> m_southWest;
    std::unique_ptr<Quadtree> m_southEast;

};

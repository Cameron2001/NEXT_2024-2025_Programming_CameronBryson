#pragma once

#include "BoundingBox.h"
#include "Engine/Graphics/Triangle.h"
#include <memory>
#include <utility>
#include <vector>
#include <Engine/Graphics/Edge.h>
struct triangleEntry
{
    Triangle2D triangle;
    BoundingBox2D bounds;
    triangleEntry(Triangle2D f);
};
class Quadtree
{
  public:
    Quadtree(const BoundingBox2D &bounds, int capacity = 4, int maxDepth = 10, int level = 0);
    bool insert(const Triangle2D &triangle);
    std::vector<Triangle2D> queryArea(const BoundingBox2D &range) const;
    std::vector<Triangle2D> querytriangle(const Triangle2D &triangle) const;
    std::vector<Triangle2D> queryEdge(const Edge2D &edge) const;

  private:
    void subdivide();
    static void query(const Quadtree *node, const BoundingBox2D &range, std::vector<Triangle2D> &found);
    static BoundingBox2D computeBox(const BoundingBox2D &box, int quadrant);

    static int getQuadrant(const BoundingBox2D &nodeBox, const BoundingBox2D &valueBox);

    BoundingBox2D m_bounds;
    int m_capacity;
    int m_maxDepth;
    int m_level;
    std::vector<triangleEntry> m_triangles;
    bool m_divided;

    std::unique_ptr<Quadtree> m_northWest;
    std::unique_ptr<Quadtree> m_northEast;
    std::unique_ptr<Quadtree> m_southWest;
    std::unique_ptr<Quadtree> m_southEast;
};

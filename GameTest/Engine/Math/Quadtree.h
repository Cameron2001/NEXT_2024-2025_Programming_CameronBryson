#pragma once

#include "BoundingBox.h"
#include "Engine/Graphics/Triangle.h"
#include <memory>
#include <vector>
#include <Engine/Graphics/Edge.h>

struct triangleEntry
{
    Triangle2D triangle;
    BoundingBox2D bounds;

    explicit triangleEntry(const Triangle2D &f);
};

class Quadtree
{
  public:
    Quadtree(const BoundingBox2D &bounds, int capacity = 4, int maxDepth = 10, int level = 0);

    bool insert(const Triangle2D &triangle);

    void queryArea(const BoundingBox2D &range, std::vector<Triangle2D> &found) const;
    void queryTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &found) const;
    void queryTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &found, float maxAvgZ) const;
    void queryEdge(const Edge2D &edge, std::vector<Triangle2D> &found) const;

  private:
    void subdivide();

    void query(const BoundingBox2D &range, std::vector<Triangle2D> &found) const;
    void query(const BoundingBox2D &range, std::vector<Triangle2D> &found, float maxAvgZ) const;

    BoundingBox2D computeBox(const BoundingBox2D &box, int quadrant) const;

    int getQuadrant(const BoundingBox2D &nodeBox, const BoundingBox2D &valueBox) const;

    BoundingBox2D m_bounds;
    int m_capacity;
    int m_maxDepth;
    int m_level;
    std::vector<triangleEntry> m_triangles;
    bool m_divided;

    // Child quadrants
    std::unique_ptr<Quadtree> m_northWest;
    std::unique_ptr<Quadtree> m_northEast;
    std::unique_ptr<Quadtree> m_southWest;
    std::unique_ptr<Quadtree> m_southEast;
};

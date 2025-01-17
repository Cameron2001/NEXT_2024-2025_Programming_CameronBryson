#pragma once

#include "BoundingBox.h"
#include "Game/Graphics/Triangle.h"
#include <memory>
#include <vector>
#include <Game/Graphics/Edge.h>

struct TriangleEntry
{
    Triangle2D triangle;
    BoundingBox2D bounds;

    explicit TriangleEntry(const Triangle2D &f);
};

class Quadtree
{
  public:
    Quadtree(const BoundingBox2D &bounds, int capacity = 4, int maxDepth = 10, int level = 0);

    bool Insert(const Triangle2D &triangle);

    void QueryArea(const BoundingBox2D &range, std::vector<Triangle2D> &found) const;
    void QueryTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &found) const;
    void QueryTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &found, float maxAvgZ) const;
    void QueryEdge(const Edge2D &edge, std::vector<Triangle2D> &found) const;

  private:
    void Subdivide();

    void Query(const BoundingBox2D &range, std::vector<Triangle2D> &found) const;
    void Query(const BoundingBox2D &range, std::vector<Triangle2D> &found, float maxAvgZ) const;

    BoundingBox2D ComputeBox(const BoundingBox2D &box, int quadrant) const;

    int GetQuadrant(const BoundingBox2D &nodeBox, const BoundingBox2D &valueBox) const;

    BoundingBox2D m_bounds;
    int m_capacity;
    int m_maxDepth;
    int m_level;
    std::vector<TriangleEntry> m_triangles;
    bool m_divided;

    // Child quadrants
    std::unique_ptr<Quadtree> m_northWest;
    std::unique_ptr<Quadtree> m_northEast;
    std::unique_ptr<Quadtree> m_southWest;
    std::unique_ptr<Quadtree> m_southEast;
};

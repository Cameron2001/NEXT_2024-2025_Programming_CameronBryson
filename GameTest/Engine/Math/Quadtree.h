#pragma once

#include "BoundingBox.h"
#include "Engine/Graphics/Face.h"
#include <memory>
#include <utility>
#include <vector>
#include <Engine/Graphics/Edge.h>
struct FaceEntry
{
    Face face;
    BoundingBox2D bounds;
    FaceEntry(const Face &f);
};
class Quadtree
{
  public:
    Quadtree(const BoundingBox2D &bounds, int capacity = 4, int maxDepth = 10, int level = 0);
    bool insert(const Face &face);
    std::vector<Face> queryArea(const BoundingBox2D &range) const;
    std::vector<Face> queryFace(const Face &face) const;
    std::vector<Face> queryEdge(const Edge3D &edge) const;

  private:
    void subdivide();
    void query(const Quadtree *node, const BoundingBox2D &range, std::vector<Face> &found) const;
    BoundingBox2D computeBox(const BoundingBox2D &box, int quadrant) const;

    int getQuadrant(const BoundingBox2D &nodeBox, const BoundingBox2D &valueBox) const;

    BoundingBox2D m_bounds;
    int m_capacity;
    int m_maxDepth;
    int m_level;
    std::vector<FaceEntry> m_faces;
    bool m_divided;

    std::unique_ptr<Quadtree> m_northWest;
    std::unique_ptr<Quadtree> m_northEast;
    std::unique_ptr<Quadtree> m_southWest;
    std::unique_ptr<Quadtree> m_southEast;
};

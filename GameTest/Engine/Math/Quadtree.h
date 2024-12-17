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
  private:
    void subdivide();
    void query(const Quadtree *node, const BoundingBox2D &range, std::vector<Face> &found) const;
    BoundingBox2D computeBox(const BoundingBox2D &box, int quadrant) const;

    int getQuadrant(const BoundingBox2D &nodeBox, const BoundingBox2D &valueBox) const;

    BoundingBox2D m_bounds;         ///< The bounding box of this quadtree node.
    int m_capacity;                 ///< Maximum number of faces before subdivision.
    int m_maxDepth;                 ///< Maximum allowed depth of the quadtree.
    int m_level;                    ///< Current depth level of this node.
    std::vector<FaceEntry> m_faces; ///< Faces contained in this node.
    bool m_divided;                 ///< Indicates whether this node has been subdivided.

    std::unique_ptr<Quadtree> m_northWest; 
    std::unique_ptr<Quadtree> m_northEast; 
    std::unique_ptr<Quadtree> m_southWest; 
    std::unique_ptr<Quadtree> m_southEast; 
};

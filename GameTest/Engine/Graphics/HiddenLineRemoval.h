#pragma once
#include "Edge.h"
#include "Face.h"
#include <Engine/Math/Quadtree.h>
#include <unordered_set>
#include <vector>
class HiddenLineRemoval
{
  public:
    HiddenLineRemoval(const std::vector<Face> &triangles);
    std::vector<Edge3D> removeHiddenLines();
    void runTests();
    bool testsPassed = false;
  private:
    std::vector<Face> m_triangles;
    std::unique_ptr<Quadtree> m_quadtree;
    void initalizeQuadtree();
    void sortTrianglesByDepth();
    
    bool isPointOnEdge(const FVector3 &point, const Edge3D &edge) const;
    bool getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB, FVector3& intersectionPoint) const;

    bool isPointInsideTriangle(const FVector3 &point, const Face &triangle) const;

    std::pair<Edge3D, Edge3D> splitEdge(const Edge3D &edge, const FVector3& splitPoint) const;
    std::vector<Edge3D> clipEdgeAgainstTriangle(const Edge3D &edge, const Face &triangle) const;
    void processTriangle(const Face &triangle, const std::vector<Face> &potentialOccluders,
                         std::unordered_set<Edge3D, Edge3DHash> &processedEdges, std::vector<Edge3D> &visibleEdges);

};  

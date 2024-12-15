#pragma once
#include "Face.h"
#include <vector>
#include "Edge.h"
class HiddenLineRemoval
{
  public:
    HiddenLineRemoval(const std::vector<Face> &triangles);
    std::vector<Edge3D> removeHiddenLines() const;
    void runTests();
    bool testsPassed = false;
  private:
    // Stores the input triangles, assumed to be sorted for hidden line processing
    std::vector<Face> m_triangles;

    // Helper function: Sorts triangles by depth (z-order) to process occlusion correctly
    void sortTrianglesByDepth();

    bool doTrianglesOverlap(const Face &t1, const Face &t2) const;
    
    bool isPointOnEdge(const FVector3 &point, const Edge3D &edge) const;
    bool getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB, FVector3& intersectionPoint) const;

    bool isPointInsideTriangle(const FVector3 &point, const Face &triangle) const;

    std::pair<Edge3D, Edge3D> splitEdge(const Edge3D &edge, const FVector3& splitPoint) const;
    std::vector<Edge3D> clipEdgeAgainstTriangle(const Edge3D &edge, const Face &triangle) const;
    // Tests
    void testIsPointInsideTriangle() const;
    void testSplitEdge() const;
    void testClipEdgeAgainstTriangle() const;
    void testGetEdgeIntersection() const;
    void testIsPointOnEdge() const;
    void testDoTrianglesOverlap() const;
    void testSortTrianglesByDepth() const;
    void testRemoveHiddenLines() const;

    void testRemoveHiddenLinesCube() const;

};  

#pragma once

#include "Edge.h"
#include "Triangle.h"
#include <Game/Math/Quadtree.h>
#include <memory>
#include <ppl.h>
#include <vector>
#include "Game/Core/Semaphore.h"
#include <mutex>
class FVector2;

struct BufferContext
{
    std::vector<Triangle2D> potentialOccluders;
    std::vector<Edge2D> segments;
    // we dont need a visable segments buffer

    BufferContext()
    {
        potentialOccluders.reserve(100);
        segments.reserve(100);
    }
    BufferContext(const BufferContext &) = delete;
    BufferContext &operator=(const BufferContext &) = delete;

    BufferContext(BufferContext &&) = default;
    BufferContext &operator=(BufferContext &&) = default;
    void clear()
    {
        potentialOccluders.clear();
        segments.clear();
    }
};

class HiddenLineRemoval
{
  public:
    HiddenLineRemoval();
    ~HiddenLineRemoval() = default;

    HiddenLineRemoval(const HiddenLineRemoval &) = delete;
    HiddenLineRemoval &operator=(const HiddenLineRemoval &) = delete;
    HiddenLineRemoval(HiddenLineRemoval &&) = default;
    HiddenLineRemoval &operator=(HiddenLineRemoval &&) = default;

    std::vector<Edge2D> RemoveHiddenLines(std::vector<Triangle2D> &triangles);

  private:
    void InitializeQuadtree(const std::vector<Triangle2D> &triangles);
    void ProcessTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &potentialOccluders,
                         std::vector<Edge2D> &segments);

    void ProcessEdge(const Edge2D &edge, const std::vector<Triangle2D> &occluders, std::vector<Edge2D> &segments);

    void ClipEdgeAgainstTriangle(const Edge2D &edge, const Triangle2D &triangle, std::vector<Edge2D> &clippedEdges);

    //No std optional in c++14 so instead of we return value via intersectionPoint param
    static bool GetEdgeIntersection(const Edge2D &edgeA, const Edge2D &edgeB, FVector2 &intersectionPoint);
    static bool IsPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle);
    static void CreateTriangleEdges(const Triangle2D &triangle, Edge2D edges[3]);
    static bool SharesVertex(const Triangle2D &triangleA, const Triangle2D &triangleB);

    Semaphore m_semaphore;
    concurrency::combinable<std::vector<Edge2D>> m_visibleEdges;
    std::vector<std::unique_ptr<BufferContext>> m_threadBuffers;
    std::mutex m_bufferMutex;
    std::vector<Edge2D> m_result;
    std::unique_ptr<Quadtree> m_quadtree;
    size_t m_bufferPoolSize = 4;
};

inline void HiddenLineRemoval::CreateTriangleEdges(const Triangle2D &triangle, Edge2D edges[3])
{
    edges[0] = Edge2D(triangle.v0, triangle.v1);
    edges[1] = Edge2D(triangle.v1, triangle.v2);
    edges[2] = Edge2D(triangle.v2, triangle.v0);
}

inline bool HiddenLineRemoval::SharesVertex(const Triangle2D &triangleA, const Triangle2D &triangleB)
{
    return (triangleA.v0 == triangleB.v0 || triangleA.v0 == triangleB.v1 || triangleA.v0 == triangleB.v2 ||
            triangleA.v1 == triangleB.v0 || triangleA.v1 == triangleB.v1 || triangleA.v1 == triangleB.v2 ||
            triangleA.v2 == triangleB.v0 || triangleA.v2 == triangleB.v1 || triangleA.v2 == triangleB.v2);
}

inline bool HiddenLineRemoval::IsPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle)
{
    float deltaX = point.x - triangle.v0.x;
    float deltaY = point.y - triangle.v0.y;

    bool isSideABPositive = (triangle.v1.x - triangle.v0.x) * deltaY - (triangle.v1.y - triangle.v0.y) * deltaX > 0;

    if (((triangle.v2.x - triangle.v0.x) * deltaY - (triangle.v2.y - triangle.v0.y) * deltaX > 0) == isSideABPositive)
        return false;

    if (((triangle.v2.x - triangle.v1.x) * (point.y - triangle.v1.y) -
             (triangle.v2.y - triangle.v1.y) * (point.x - triangle.v1.x) >
         0) != isSideABPositive)
        return false;

    return true;
}

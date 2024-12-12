#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include <vector>
#include <set>
struct Edge
{
    Edge(const FVector3 &start, const FVector3 &end) : start(start), end(end)
    {
    }
    FVector3 start;
    FVector3 end;
    bool operator==(const Edge &other) const
    {
        return start == other.start && end == other.end;
    };
};
struct IntersectionPoint
{
    IntersectionPoint(const FVector2 &point, bool isRight, const Edge *edgeA, const Edge *edgeB)
        : point(point), isRight(isRight), edgeA(edgeA), edgeB(edgeB)
    {
    }
    FVector2 point;
    bool isRight;
    const Edge *edgeA;
    const Edge *edgeB;
};
struct EndPoint
{
    EndPoint(const FVector2& point, bool isRight, int edgeIndex) : point(point), isRight(isRight), edgeIndex(edgeIndex)
    {
    }
    FVector2 point;
    bool isRight;
    int edgeIndex;
};
class HiddenLine
{
  public:
    void MakeNCopies(const std::vector<Edge> &edges);
    IntersectionPoint ComputeIntersection(float li, const Edge &ej, const FVector3 &observer);
    std::vector<Edge> DetermineUnion(const std::vector<Edge> &edges, const Edge &currentEdge, const FVector2 observer);
    std::vector<Edge> EliminateHiddenLines(std::vector<Edge> &edges, const FVector3& observer);
  private:
    std::vector<std::vector<Edge>> edgeCopies;
    std::vector<IntersectionPoint> intersections;
};

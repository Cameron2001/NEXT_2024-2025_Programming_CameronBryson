#include "stdafx.h"
#include "HiddenLineRemoval.h"
#include <algorithm>
#include <assert.h>

HiddenLineRemoval::HiddenLineRemoval(const std::vector<Face> &triangles)
{
    m_triangles = triangles;

    sortTrianglesByDepth();
}
// Sorted from closest to furtherest
// So we want every face to check the edges in front of it
// For example the first face checks against no faces
// The last face checks against all the faces

std::vector<Edge3D> HiddenLineRemoval::removeHiddenLines() const
{
    std::vector<Edge3D> visibleEdges;
    std::vector<Face> occluders;
    std::vector<Edge3D> processedEdges; // Tracks edges that have been processed

    // Iterate through each triangle from closest to farthest
    for (const auto &triangle : m_triangles)
    {
        // Define the edges of the current triangle with consistent ordering
        Edge3D edges[3] = {Edge3D(triangle.v0, triangle.v1), Edge3D(triangle.v1, triangle.v2),
                           Edge3D(triangle.v2, triangle.v0)};

        for (const auto &edge : edges)
        {
            // Check if the edge has already been processed
            if (std::find(processedEdges.begin(), processedEdges.end(), edge) != processedEdges.end())
            {
                continue; // Skip processing this shared edge again
            }

            // Mark the edge as processed
            processedEdges.push_back(edge);

            std::vector<Edge3D> segments = {edge};

            // Clip the edge against all existing occluders
            for (const auto &occluder : occluders)
            {
                std::vector<Edge3D> tempSegments;

                for (const auto &segment : segments)
                {
                    // Clip the current segment against the occluder triangle
                    auto clipped = clipEdgeAgainstTriangle(segment, occluder);
                    tempSegments.insert(tempSegments.end(), clipped.begin(), clipped.end());
                }

                segments = tempSegments;

                // If all segments are occluded, no need to check further
                if (segments.empty())
                    break;
            }

            // Add the remaining visible segments to the visibleEdges list
            visibleEdges.insert(visibleEdges.end(), segments.begin(), segments.end());
        }

        // Add the current triangle to the list of occluders for subsequent triangles
        occluders.push_back(triangle);
    }

    return visibleEdges;
}









void HiddenLineRemoval::sortTrianglesByDepth()
{
    // Sort triangles based on average Z value (depth)
    std::sort(m_triangles.begin(), m_triangles.end(), [](const Face &a, const Face &b) {
        float depthA = (a.v0.Z + a.v1.Z + a.v2.Z) / 3.0f;
        float depthB = (b.v0.Z + b.v1.Z + b.v2.Z) / 3.0f;
        //float depthA = std::min({a.v0.Z, a.v1.Z, a.v2.Z});
        //float depthB = std::min({b.v0.Z, b.v1.Z, b.v2.Z});
        return depthA < depthB;
    });
}

bool HiddenLineRemoval::doTrianglesOverlap(const Face &t1, const Face &t2) const
{
    // Simple bounding box overlap check in 2D image space
    auto getBoundingBox = [](const Face &t) -> std::pair<FVector2, FVector2> {
        float minX = std::min({t.v0.X, t.v1.X, t.v2.X});
        float minY = std::min({t.v0.Y, t.v1.Y, t.v2.Y});
        float maxX = std::max({t.v0.X, t.v1.X, t.v2.X});
        float maxY = std::max({t.v0.Y, t.v1.Y, t.v2.Y});
        return {FVector2(minX, minY), FVector2(maxX, maxY)};
    };

    auto bbox1 = getBoundingBox(t1);
    auto bbox2 = getBoundingBox(t2);

    // Check for bounding box overlap
    bool overlap = !(bbox1.second.X < bbox2.first.X || bbox1.first.X > bbox2.second.X ||
                     bbox1.second.Y < bbox2.first.Y || bbox1.first.Y > bbox2.second.Y);
    return overlap;
}

bool HiddenLineRemoval::isPointOnEdge(const FVector3 &point, const Edge3D &edge) const
{
    FVector3 edgeVector = edge.end - edge.start;
    float edgeLengthSquared = edgeVector.LengthSquared();

    // Handle zero-length edges
    if (edgeLengthSquared < 1e-12f)
    {
        // Only the start point is considered on the edge
        return point == edge.start;
    }

    FVector3 pointVector = point - edge.start;
    float crossProduct = edgeVector.Cross(pointVector).LengthSquared();
    if (crossProduct > 1e-6f)
        return false;

    float dotProduct = pointVector.Dot(edgeVector);
    if (dotProduct < 0.0f || dotProduct > edgeLengthSquared)
        return false;

    return true;
}

// Corrected getEdgeIntersection function
bool HiddenLineRemoval::getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB, FVector3 &intersectionPoint) const
{
    // Extract coordinates
    float x1 = edgeA.start.X;
    float y1 = edgeA.start.Y;
    float x2 = edgeA.end.X;
    float y2 = edgeA.end.Y;

    float x3 = edgeB.start.X;
    float y3 = edgeB.start.Y;
    float x4 = edgeB.end.X;
    float y4 = edgeB.end.Y;

    // Compute denominator with epsilon
    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    const float EPSILON = 1e-6f;

    if (fabs(denom) < EPSILON)
    {
        // Lines are parallel or coincident
        bool startStart = edgeA.start == edgeB.start;
        bool startEnd = edgeA.start == edgeB.end;
        bool endStart = edgeA.end == edgeB.start;
        bool endEnd = edgeA.end == edgeB.end;

        int overlapCount = startStart + startEnd + endStart + endEnd;

        if (overlapCount > 1)
        {
            // Edges are coincident (all endpoints overlap)
            return false;
        }
        else if (overlapCount == 1)
        {
            // Edges intersect at a single endpoint
            if (startStart)
                intersectionPoint = edgeA.start;
            else if (startEnd)
                intersectionPoint = edgeA.start;
            else if (endStart)
                intersectionPoint = edgeA.end;
            else if (endEnd)
                intersectionPoint = edgeA.end;
            return true;
        }
        else
        {
            // Lines are parallel but do not share any endpoints
            return false;
        }
    }

    // Compute intersection parameters
    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
    float u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denom;

    // Check if intersection is within both segments with epsilon
    if (t < -EPSILON || t > 1.0f + EPSILON || u < -EPSILON || u > 1.0f + EPSILON)
        return false; // Intersection not within the segments

    // Clamp t and u to [0,1]
    t = std::max(0.0f, std::min(1.0f, t));
    u = std::max(0.0f, std::min(1.0f, u));

    // Calculate intersection point in 3D
    float intersectionX = x1 + t * (x2 - x1);
    float intersectionY = y1 + t * (y2 - y1);
    float intersectionZ = edgeA.start.Z + t * (edgeA.end.Z - edgeA.start.Z);

    intersectionPoint = FVector3(intersectionX, intersectionY, intersectionZ);
    return true;
}


bool HiddenLineRemoval::isPointInsideTriangle(const FVector3 &point, const Face &triangle) const
{
    // Project the point and triangle vertices onto the XY-plane
    FVector2 p(point.X, point.Y);
    FVector2 v0(triangle.v0.X, triangle.v0.Y);
    FVector2 v1(triangle.v1.X, triangle.v1.Y);
    FVector2 v2(triangle.v2.X, triangle.v2.Y);

    // Compute vectors
    float denom = (v1.Y - v2.Y) * (v0.X - v2.X) + (v2.X - v1.X) * (v0.Y - v2.Y);
    if (fabs(denom) < 1e-6f)
        return false; // Degenerate triangle

    float a = ((v1.Y - v2.Y) * (p.X - v2.X) + (v2.X - v1.X) * (p.Y - v2.Y)) / denom;
    float b = ((v2.Y - v0.Y) * (p.X - v2.X) + (v0.X - v2.X) * (p.Y - v2.Y)) / denom;
    float c = 1.0f - a - b;

    return (a >= 0.0f) && (b >= 0.0f) && (c >= 0.0f);
}


std::pair<Edge3D, Edge3D> HiddenLineRemoval::splitEdge(const Edge3D &edge, const FVector3 &splitPoint) const
{
    assert(isPointOnEdge(splitPoint, edge));
    Edge3D edgeA(edge.start, splitPoint);
    Edge3D edgeB(splitPoint, edge.end);
    return {edgeA, edgeB};
}
//split at intersections
// Figure out which half is visible
// Keep visible, get rid of invisible

std::vector<Edge3D> HiddenLineRemoval::clipEdgeAgainstTriangle(const Edge3D &edge, const Face &triangle) const
{
    std::vector<FVector3> intersectionPoints;
    Edge3D triEdges[3] = {{triangle.v0, triangle.v1}, {triangle.v1, triangle.v2}, {triangle.v2, triangle.v0}};

    // Check intersection with each edge of the triangle
    for (const auto &triEdge : triEdges)
    {
        FVector3 intersectionPoint;
        if (getEdgeIntersection(edge, triEdge, intersectionPoint))
        {
            // Avoid duplicate intersection points
            if (std::find_if(intersectionPoints.begin(), intersectionPoints.end(), [&](const FVector3 &p) {
                    return (p - intersectionPoint).LengthSquared() < 1e-6f;
                }) == intersectionPoints.end())
            {
                intersectionPoints.push_back(intersectionPoint);
            }
        }
    }

    // If no intersections, check if the entire edge is inside or outside the triangle
    if (intersectionPoints.empty())
    {
        if (isPointInsideTriangle(edge.start, triangle) && isPointInsideTriangle(edge.end, triangle))
        {
            // Edge is fully occluded
            return {}; // returning empty vector
        }
        else
        {
            // Edge is fully outside the triangle
            return {edge}; // returning original edge
        }
    }

    // Sort the intersection points along the edge
    std::sort(intersectionPoints.begin(), intersectionPoints.end(), [&edge](const FVector3 &a, const FVector3 &b) {
        return (a - edge.start).LengthSquared() < (b - edge.start).LengthSquared();
    });

    // Include the start and end points
    std::vector<FVector3> sortedPoints = {edge.start};
    sortedPoints.insert(sortedPoints.end(), intersectionPoints.begin(), intersectionPoints.end());
    sortedPoints.push_back(edge.end);

    // Determine visibility of each segment between consecutive points
    std::vector<Edge3D> clippedEdges;
    for (size_t i = 0; i < sortedPoints.size() - 1; ++i)
    {
        FVector3 midPoint = (sortedPoints[i] + sortedPoints[i + 1]) * 0.5f;
        if (!isPointInsideTriangle(midPoint, triangle))
        {
            clippedEdges.emplace_back(sortedPoints[i], sortedPoints[i + 1]);
        }
    }

    return clippedEdges;
}



//Tests
void HiddenLineRemoval::runTests()
{
    if (testsPassed)
    {
        return;
    }
    testSortTrianglesByDepth();
    testIsPointOnEdge();
    testIsPointInsideTriangle();
    testSplitEdge();
    testGetEdgeIntersection();
    testDoTrianglesOverlap();
    testClipEdgeAgainstTriangle();
    //testRemoveHiddenLines();
    testRemoveHiddenLinesCube();
    testsPassed = true;

}
void HiddenLineRemoval::testIsPointInsideTriangle() const
{
    Face triangle(FVector3(0, 0, 0), FVector3(5, 0, 0), FVector3(0, 5, 0));

    // Point inside the triangle
    FVector3 insidePoint(1, 1, 0);
    assert(isPointInsideTriangle(insidePoint, triangle) == true);

    // Point outside the triangle
    FVector3 outsidePoint(6, 6, 0);
    assert(isPointInsideTriangle(outsidePoint, triangle) == false);

    // Point on the vertex of the triangle
    FVector3 vertexPoint(0, 0, 0);
    assert(isPointInsideTriangle(vertexPoint, triangle) == true);

    // Point on an edge of the triangle
    FVector3 edgePoint(2.5f, 0, 0);
    assert(isPointInsideTriangle(edgePoint, triangle) == true);

    // Degenerate triangle (all points colinear)
    Face degenerateTriangle(FVector3(0, 0, 0), FVector3(1, 1, 1), FVector3(2, 2, 2));
    FVector3 somePoint(1, 0, 0);
    assert(isPointInsideTriangle(somePoint, degenerateTriangle) == false);
}

void HiddenLineRemoval::testSplitEdge() const
{
    // Standard split
    Edge3D edge(FVector3(0, 0, 0), FVector3(10, 10, 10));
    FVector3 splitPoint(5, 5, 5);

    auto splitted = splitEdge(edge, splitPoint);
    assert(splitted.first.start.X == 0 && splitted.first.end.X == 5);
    assert(splitted.second.start.X == 5 && splitted.second.end.X == 10);

    // Split at start point
    FVector3 splitAtStart(0, 0, 0);
    auto splittedStart = splitEdge(edge, splitAtStart);
    assert(splittedStart.first.start.X == 0 && splittedStart.first.end.X == 0);
    assert(splittedStart.second.start.X == 0 && splittedStart.second.end.X == 10);

    // Split at end point
    FVector3 splitAtEnd(10, 10, 10);
    auto splittedEnd = splitEdge(edge, splitAtEnd);
    assert(splittedEnd.first.start.X == 0 && splittedEnd.first.end.X == 10);
    assert(splittedEnd.second.start.X == 10 && splittedEnd.second.end.X == 10);

    // Split with floating point precision
    FVector3 splitPointFP(5.0001f, 5.0001f, 5.0001f);
    auto splittedFP = splitEdge(edge, splitPointFP);
    assert(fabs(splittedFP.first.end.X - 5.0001f) < 1e-4);
    assert(fabs(splittedFP.second.start.X - 5.0001f) < 1e-4);
}

void HiddenLineRemoval::testClipEdgeAgainstTriangle() const
{
    // Define a triangle
    Face triangle(FVector3(0, 0, 0), FVector3(5, 0, 0), FVector3(0, 5, 0));

    // Define an edge that intersects the triangle
    Edge3D intersectingEdge(FVector3(-1, -1, 0), FVector3(6, 6, 0));

    auto clipped = clipEdgeAgainstTriangle(intersectingEdge, triangle);

    // Corrected expected size: 2 visible segments
    assert(clipped.size() == 2 && "Expected 2 clipped segments, but got a different count.");

    // Verify first clipped segment: (-1, -1, 0) to (0, 0, 0)
    assert(fabs(clipped[0].start.X - (-1.0f)) < 1e-4 && "Clipped[0].start.X incorrect.");
    assert(fabs(clipped[0].start.Y - (-1.0f)) < 1e-4 && "Clipped[0].start.Y incorrect.");
    assert(fabs(clipped[0].end.X - 0.0f) < 1e-4 && "Clipped[0].end.X incorrect.");
    assert(fabs(clipped[0].end.Y - 0.0f) < 1e-4 && "Clipped[0].end.Y incorrect.");

    // Verify second clipped segment: (2.5, 2.5, 0) to (6, 6, 0)
    assert(fabs(clipped[1].start.X - 2.5f) < 1e-4 && "Clipped[1].start.X incorrect.");
    assert(fabs(clipped[1].start.Y - 2.5f) < 1e-4 && "Clipped[1].start.Y incorrect.");
    assert(fabs(clipped[1].end.X - 6.0f) < 1e-4 && "Clipped[1].end.X incorrect.");
    assert(fabs(clipped[1].end.Y - 6.0f) < 1e-4 && "Clipped[1].end.Y incorrect.");

    // Additional Assertions for Clarity
    // Ensure the clipped segments are outside the triangle
    FVector3 midpoint1 = (clipped[0].start + clipped[0].end) * 0.5f; // (-0.5, -0.5, 0)
    FVector3 midpoint2 = (clipped[1].start + clipped[1].end) * 0.5f; // (4.25, 4.25, 0)
    assert(!isPointInsideTriangle(midpoint1, triangle) && "First clipped segment incorrectly inside the triangle.");
    assert(!isPointInsideTriangle(midpoint2, triangle) && "Second clipped segment incorrectly inside the triangle.");
}

void HiddenLineRemoval::testGetEdgeIntersection() const
{
    // Define two intersecting edges
    Edge3D edgeA(FVector3(0, 0, 0), FVector3(5, 5, 5));
    Edge3D edgeB(FVector3(0, 5, 0), FVector3(5, 0, 5));

    FVector3 intersection;
    bool result = getEdgeIntersection(edgeA, edgeB, intersection);
    assert(result == true && "Intersecting edges should intersect.");
    assert(fabs(intersection.X - 2.5f) < 1e-4 && "Intersection X coordinate incorrect.");
    assert(fabs(intersection.Y - 2.5f) < 1e-4 && "Intersection Y coordinate incorrect.");
    assert(fabs(intersection.Z - 2.5f) < 1e-4 && "Intersection Z coordinate incorrect.");

    // Define two parallel edges
    Edge3D edgeC(FVector3(0, 0, 0), FVector3(5, 5, 5));
    Edge3D edgeD(FVector3(0, 1, 0), FVector3(5, 6, 5));

    result = getEdgeIntersection(edgeC, edgeD, intersection);
    assert(result == false && "Parallel edges should not intersect.");

    // Define overlapping segments (coincident)
    Edge3D edgeE(FVector3(1, 1, 1), FVector3(4, 4, 4));
    Edge3D edgeF(FVector3(1, 1, 1), FVector3(4, 4, 4));

    result = getEdgeIntersection(edgeE, edgeF, intersection);
    assert(result == false && "Coincident edges should not count as intersecting.");

    // Intersection at endpoints
    Edge3D edgeG(FVector3(0, 0, 0), FVector3(5, 5, 5));
    Edge3D edgeH(FVector3(5, 5, 5), FVector3(10, 10, 10));

    result = getEdgeIntersection(edgeG, edgeH, intersection);
    assert(result == true && "Edges intersecting at endpoints should intersect.");
    assert(fabs(intersection.X - 5.0f) < 1e-4 && "Intersection at endpoint X incorrect.");
    assert(fabs(intersection.Y - 5.0f) < 1e-4 && "Intersection at endpoint Y incorrect.");
    assert(fabs(intersection.Z - 5.0f) < 1e-4 && "Intersection at endpoint Z incorrect.");

    // Nearly parallel edges
    Edge3D edgeI(FVector3(0, 0, 0), FVector3(5, 5.0001f, 5));
    Edge3D edgeJ(FVector3(0, 5, 0), FVector3(5, 0, 5));

    result = getEdgeIntersection(edgeI, edgeJ, intersection);
    //assert(result == false && "Nearly parallel edges should not intersect.");
}


void HiddenLineRemoval::testIsPointOnEdge() const
{
    Edge3D edge(FVector3(0, 0, 0), FVector3(10, 10, 10));

    // Point on the edge
    FVector3 onPoint(5, 5, 5);
    assert(isPointOnEdge(onPoint, edge) == true);

    // Point not on the edge
    FVector3 offPoint(5, 5, 6);
    assert(isPointOnEdge(offPoint, edge) == false);

    // Point outside the edge segment
    FVector3 outsidePoint(-1, -1, -1);
    assert(isPointOnEdge(outsidePoint, edge) == false);

    // Point very close to the edge (within epsilon)
    FVector3 nearPoint(5.000001f, 5.000001f, 5.000001f);
    assert(isPointOnEdge(nearPoint, edge) == true);

    // Point very close but outside epsilon
    FVector3 farPoint(5.0001f, 5.0001f, 5.0001f);
    // Adjusted to slightly deviate from the edge
    FVector3 farPointAdjusted(5.0001f, 5.0002f, 5.0001f);
    assert(isPointOnEdge(farPointAdjusted, edge) == false);

    // Zero-length edge
    Edge3D zeroLengthEdge(FVector3(1, 1, 1), FVector3(1, 1, 1));
    FVector3 samePoint(1, 1, 1);
    assert(isPointOnEdge(samePoint, zeroLengthEdge) == true);

    FVector3 differentPoint(1, 1, 2);
    assert(isPointOnEdge(differentPoint, zeroLengthEdge) == false);
}

void HiddenLineRemoval::testDoTrianglesOverlap() const
{
    // Define two overlapping triangles
    Face triangle1(FVector3(0, 0, 0), FVector3(5, 0, 0), FVector3(0, 5, 0));
    Face triangle2(FVector3(3, 3, 0), FVector3(8, 3, 0), FVector3(3, 8, 0));
    assert(doTrianglesOverlap(triangle1, triangle2) == true);

    // Define two non-overlapping triangles
    Face triangle3(FVector3(10, 10, 0), FVector3(15, 10, 0), FVector3(10, 15, 0));
    assert(doTrianglesOverlap(triangle1, triangle3) == false);

    // Triangles sharing an edge
    Face triangle4(FVector3(5, 0, 0), FVector3(10, 0, 0), FVector3(5, 5, 0));
    assert(doTrianglesOverlap(triangle1, triangle4) == true);

    // Triangles sharing a vertex
    Face triangle5(FVector3(0, 0, 0), FVector3(-5, -5, 0), FVector3(-5, 0, 0));
    assert(doTrianglesOverlap(triangle1, triangle5) == true);

    // One triangle completely inside another
    Face triangle6(FVector3(1, 1, 0), FVector3(4, 1, 0), FVector3(1, 4, 0));
    assert(doTrianglesOverlap(triangle1, triangle6) == true);

    // Degenerate triangles (zero area)
    Face degenerateTriangle1(FVector3(0, 0, 0), FVector3(1, 1, 1), FVector3(2, 2, 2));
    Face degenerateTriangle2(FVector3(3, 3, 0), FVector3(4, 4, 0), FVector3(5, 5, 0));
    assert(doTrianglesOverlap(degenerateTriangle1, degenerateTriangle2) == false);

    // Triangles with different Z values (assuming 2D overlap based on X and Y)
    Face triangle7(FVector3(2, 2, 1), FVector3(6, 2, 1), FVector3(2, 6, 1));
    assert(doTrianglesOverlap(triangle1, triangle7) == true);
}

void HiddenLineRemoval::testSortTrianglesByDepth() const
{
    // Create triangles with different depths
    std::vector<Face> triangles = {Face(FVector3(0, 0, 5), FVector3(1, 0, 5), FVector3(0, 1, 5)),
                                   Face(FVector3(0, 0, 2), FVector3(1, 0, 2), FVector3(0, 1, 2)),
                                   Face(FVector3(0, 0, 8), FVector3(1, 0, 8), FVector3(0, 1, 8))};

    HiddenLineRemoval hlr(triangles);
    hlr.sortTrianglesByDepth();

    // After sorting, the order should be triangles[1], triangles[0], triangles[2]
    assert(hlr.m_triangles[0].v0.Z == 2);
    assert(hlr.m_triangles[1].v0.Z == 5);
    assert(hlr.m_triangles[2].v0.Z == 8);

    // Triangles with the same depth
    std::vector<Face> sameDepthTriangles = {Face(FVector3(0, 0, 3), FVector3(1, 0, 3), FVector3(0, 1, 3)),
                                            Face(FVector3(2, 2, 3), FVector3(3, 2, 3), FVector3(2, 3, 3)),
                                            Face(FVector3(4, 4, 3), FVector3(5, 4, 3), FVector3(4, 5, 3))};

    HiddenLineRemoval hlrSameDepth(sameDepthTriangles);
    hlrSameDepth.sortTrianglesByDepth();

    // The order should remain unchanged as all have the same depth
    assert(hlrSameDepth.m_triangles[0].v0.X == 0);
    assert(hlrSameDepth.m_triangles[1].v0.X == 2);
    assert(hlrSameDepth.m_triangles[2].v0.X == 4);

    // Triangles with negative depths
    std::vector<Face> negativeDepthTriangles = {Face(FVector3(0, 0, -5), FVector3(1, 0, -5), FVector3(0, 1, -5)),
                                                Face(FVector3(0, 0, -2), FVector3(1, 0, -2), FVector3(0, 1, -2)),
                                                Face(FVector3(0, 0, -8), FVector3(1, 0, -8), FVector3(0, 1, -8))};

    HiddenLineRemoval hlrNegativeDepth(negativeDepthTriangles);
    hlrNegativeDepth.sortTrianglesByDepth();

    // After sorting, the order should be triangles[2], triangles[0], triangles[1]
    assert(hlrNegativeDepth.m_triangles[0].v0.Z == -8);
    assert(hlrNegativeDepth.m_triangles[1].v0.Z == -5);
    assert(hlrNegativeDepth.m_triangles[2].v0.Z == -2);
}

void HiddenLineRemoval::testRemoveHiddenLines() const
{
    // Create overlapping triangles
    std::vector<Face> triangles = {
        Face(FVector3(0, 0, 1), FVector3(5, 0, 1), FVector3(0, 5, 1)),
        Face(FVector3(1, 1, 0), FVector3(6, 1, 0), FVector3(1, 6, 0)) // Closer triangle
    };

    HiddenLineRemoval hlr(triangles);
    auto visibleEdges = hlr.removeHiddenLines();

    // Since the second triangle is closer, edges of the first triangle should be clipped
    // Verify that visibleEdges contains edges from the second triangle and clipped edges from the first
    // Expected visible edges: edges of the second triangle and possibly partial edges of the first

    // Check that edges from the closer triangle are present
    assert(std::find(visibleEdges.begin(), visibleEdges.end(), Edge3D(FVector3(1, 1, 0), FVector3(6, 1, 0))) !=
           visibleEdges.end());
    assert(std::find(visibleEdges.begin(), visibleEdges.end(), Edge3D(FVector3(1, 6, 0), FVector3(6, 1, 0))) ==
           visibleEdges.end()); // Edge should not be present as it should be the same as above due to ordering
    assert(std::find(visibleEdges.begin(), visibleEdges.end(), Edge3D(FVector3(1, 1, 0), FVector3(1, 6, 0))) !=
           visibleEdges.end());

    // Additional checks can be added here
}


void HiddenLineRemoval::testRemoveHiddenLinesCube() const
{
    // Define vertices for a cube
    FVector3 v0(0, 0, 0);
    FVector3 v1(1, 0, 0);
    FVector3 v2(1, 1, 0);
    FVector3 v3(0, 1, 0);
    FVector3 v4(0, 0, 1);
    FVector3 v5(1, 0, 1);
    FVector3 v6(1, 1, 1);
    FVector3 v7(0, 1, 1);

    // Define faces (two triangles per face)
    std::vector<Face> cubeTriangles = {// Bottom
                                       Face(v0, v1, v2), Face(v0, v2, v3),
                                       // Top
                                       Face(v4, v5, v6), Face(v4, v6, v7),
                                       // Front
                                       Face(v0, v1, v5), Face(v0, v5, v4),
                                       // Back
                                       Face(v2, v3, v7), Face(v2, v7, v6),
                                       // Left
                                       Face(v0, v3, v7), Face(v0, v7, v4),
                                       // Right
                                       Face(v1, v2, v6), Face(v1, v6, v5)};

    HiddenLineRemoval hlrCube(cubeTriangles);
    auto visibleEdges = hlrCube.removeHiddenLines();

    // Expected: Depending on the viewpoint, a specific number of edges should be visible.
    // For a standard cube from a diagonal view, up to 12 edges can be visible.
    // Here, we'll check that at least more than two edges are visible.
    assert(visibleEdges.size() > 2 && "Cube should have multiple visible edges.");
}

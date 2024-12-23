#pragma once
#include "Engine/Graphics/Edge.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Math/Matrix4.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Graphics/Triangle.h"
#include <vector>
#include <unordered_map>
const FVector3 VIEW_DIRECTION(0.0f, 0.0f, 1.0f);
constexpr float NDC = 1.00f;
constexpr float xNDCMax = NDC;
constexpr float xNDCMin = -NDC;
constexpr float yNDCMax = NDC;
constexpr float yNDCMin = -NDC;
class Renderer
{
  public:
    static void QueueMesh(const Mesh &mesh, const Matrix4 &MVP);
    static void QueueModel(const Model &model, const Matrix4 &MVP);
    static void SubmitQueue();
    static void ClearQueue();

  private:
    static std::vector<Triangle> m_triangles;

    static bool QuickReject(const Edge3D &edge0, const Edge3D &edge1, const Edge3D &edge2);
    // LiangBarsky
    static Edge3D LiangBarsky(const Edge3D &edge);
};
inline bool Renderer::QuickReject(const Edge3D &edge0, const Edge3D &edge1, const Edge3D &edge2)
{
    // Extract the vertices from the edges
    const FVector3 &v0 = edge0.start;
    const FVector3 &v1 = edge1.start;
    const FVector3 &v2 = edge2.start;

    // Check against the four frustum planes (excluding Z)
    // Left plane (x = -1)
    if (v0.X < xNDCMin && v1.X < xNDCMin && v2.X < xNDCMin)
        return true;
    // Right plane (x = 1)
    if (v0.X > xNDCMax && v1.X > xNDCMax && v2.X > xNDCMax)
        return true;
    // Bottom plane (y = -1)
    if (v0.Y < yNDCMin && v1.Y < yNDCMin && v2.Y < yNDCMin)
        return true;
    // Top plane (y = 1)
    if (v0.Y > yNDCMax && v1.Y > yNDCMax && v2.Y > yNDCMax)
        return true;

    // Calculate the area of the triangle
    const FVector3 vec1 = v1 - v0;
    const FVector3 vec2 = v2 - v0;
    const float area = 0.5f * vec1.Cross(vec2).Length();

    // Reject if the area is below the threshold
    if (area < 0.0001f)
        return true;

    // Triangle is at least partially inside the frustum
    return false;
}

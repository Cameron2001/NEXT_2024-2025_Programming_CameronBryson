#include "stdafx.h"
#include "GraphicsManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

GraphicsManager::GraphicsManager()
{
    LoadResources();
}

GraphicsManager::~GraphicsManager()
{
    UnloadResources();
}

void GraphicsManager::LoadResources()
{
    LoadModel("CubeOBJ", "assets/cube.obj");
    LoadModel("SphereOBJ", "assets/sphere.obj");
    LoadModel("FlagOBJ", "assets/flag.obj");
    LoadModel("AsteroidOBJ", "assets/asteroid.obj");
    LoadModel("ArrowOBJ", "assets/arrow2.obj");
    LoadModel("GolfCourseOBJ", "assets/golf_course.obj");
}

void GraphicsManager::UnloadResources()
{
}

void GraphicsManager::AddModel(const std::string &modelName, const Model &model)
{
    m_modelMap[modelName] = std::make_shared<Model>(model);
}

void GraphicsManager::LoadModel(const std::string &modelName, const std::string &path)
{
    std::ifstream objFile(path);
    if (!objFile.is_open())
    {
        std::cerr << "Failed to open file: " << path << '\n';
        return;
    }
    std::vector<Mesh> meshes;
    std::vector<FVector3> tempPositions;
    std::vector<Triangle3D> triangles;
    std::string currentMeshName = "Default";
    std::string line;
    while (std::getline(objFile, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;
        if (prefix == "o" || prefix == "g")
        {
            // If vertices and triangles have been collected, create a new mesh
            if (!triangles.empty())
            {
                meshes.emplace_back(triangles);
                triangles.clear();
            }
            lineStream >> currentMeshName;
        }
        else if (prefix == "v")
        {
            float x, y, z;
            lineStream >> x >> y >> z;
            tempPositions.emplace_back(x, y, z);
        }
        else if (prefix == "f")
        {
            unsigned int vertexIndex[3];

            for (int i = 0; i < 3; ++i)
            {
                lineStream >> vertexIndex[i];
                vertexIndex[i]--; // OBJ indices start at 1
            }

            // Create triangle with actual vertices
            triangles.emplace_back(tempPositions[vertexIndex[0]], tempPositions[vertexIndex[1]],
                                   tempPositions[vertexIndex[2]]);
        }
    }

    if (!triangles.empty())
    {
        meshes.emplace_back(triangles);
        triangles.clear();
    }

    AddModel(modelName, Model(meshes));
}

Model &GraphicsManager::GetModel(const std::string &modelName)
{
    return *m_modelMap[modelName];
}

void GraphicsManager::Shutdown()
{
}

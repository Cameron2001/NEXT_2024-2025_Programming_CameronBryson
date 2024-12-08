#include "stdafx.h"
#include "GraphicsManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void GraphicsManager::LoadResources()
{
    LoadModel("CubeOBJ", "assets/cube.obj");
    LoadModel("MonkeyOBJ", "assets/monkey.obj");
    LoadModel("SphereOBJ", "assets/sphere.obj");
    LoadModel("ShipOBJ", "assets/ship.obj");

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
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    std::vector<Mesh> meshes;
    std::vector<FVector3> tempPositions;
    std::vector<FVector3> tempNormals;
    std::vector<unsigned int> vertexIndices, normalIndices;
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
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
            // If vertices and faces have been collected, create a new mesh
            if (!vertices.empty() && !faces.empty())
            {
                meshes.emplace_back(vertices, faces);
                vertices.clear();
                faces.clear();
            }
            lineStream >> currentMeshName;
        }
        else if (prefix == "v")
        {
            float x, y, z;
            lineStream >> x >> y >> z;
            tempPositions.emplace_back(x, y, z);
        }
        else if (prefix == "vn")
        {
            float x, y, z;
            lineStream >> x >> y >> z;
            tempNormals.emplace_back(x, y, z);
        }
        else if (prefix == "f")
        {
            unsigned int vertexIndex[3]{}, normalIndex[3]{};
            char slash;
            for (int i = 0; i < 3; ++i)
            {
                lineStream >> vertexIndex[i] >> slash >> slash >> normalIndex[i];
                vertexIndices.push_back(vertexIndex[i] - 1);
                normalIndices.push_back(normalIndex[i] - 1);
            }
        }
    }
    for (size_t i = 0; i < vertexIndices.size(); ++i)
    {
        Vertex vertex(tempPositions[vertexIndices[i]]);
        vertex.normal = tempNormals[normalIndices[i]];
        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < vertexIndices.size(); i += 3)
    {
        faces.emplace_back(static_cast<unsigned int>(i), static_cast<unsigned int>(i + 1),
                           static_cast<unsigned int>(i + 2));
    }

    if (!vertices.empty() && !faces.empty())
    {
        meshes.emplace_back(vertices, faces);
        vertices.clear();
        faces.clear();
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
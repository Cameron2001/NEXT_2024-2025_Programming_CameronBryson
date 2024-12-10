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
            if (!faces.empty())
            {
                meshes.emplace_back(faces);
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
            unsigned int vertexIndex[3], normalIndex[3];
            char slash;

            // Parse face indices
            for (int i = 0; i < 3; ++i)
            {
                lineStream >> vertexIndex[i] >> slash >> slash >> normalIndex[i];
                vertexIndex[i]--; // OBJ indices start at 1
                normalIndex[i]--;
            }

            // Create vertices with positions and normals
            FVector3 vertex0 = tempPositions[vertexIndex[0]];
            FVector3 vertex1 = tempPositions[vertexIndex[1]];
            FVector3 vertex2 = tempPositions[vertexIndex[2]];

            FVector3 edge1 = vertex1 - vertex0;
            FVector3 edge2 = vertex2 - vertex0;

            FVector3 faceNormal = edge1.Cross(edge2);
            FVector3 vertexNormal =
                tempNormals[normalIndex[0]] + tempNormals[normalIndex[1]] + tempNormals[normalIndex[2]] / 3.0f;

            float dot = faceNormal.Dot(vertexNormal);
            if (dot < 0.0f)
            {
                faceNormal = faceNormal*-1.0f;
            }

            // Create face with actual vertices
            faces.emplace_back(tempPositions[vertexIndex[0]], tempPositions[vertexIndex[1]],
                                   tempPositions[vertexIndex[2]],faceNormal);
        }
    }

    if (!faces.empty())
    {
        meshes.emplace_back(faces);
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
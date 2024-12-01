#include "stdafx.h"
#include "RenderView.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Camera.h"

RenderView::RenderView()
{
}

void RenderView::Init(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<Camera> camera, std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray, std::shared_ptr<ComponentArray<ModelComponent>> modelComponentArray, std::shared_ptr<ComponentArray<TextComponent>> textComponentArray)
{
	m_graphicsManager = graphicsManager;
	m_camera = camera;
	m_transformComponentArray = transformComponentArray;
	m_modelComponentArray = modelComponentArray;
	m_textComponentArray = textComponentArray;

	std::vector<Vertex> cubeVertices = {
		Vertex{ FVector3(-1.0f, -1.0f, -1.0f) }, // Vertex 0
		Vertex{ FVector3(1.0f, -1.0f, -1.0f) },  // Vertex 1
		Vertex{ FVector3(1.0f, 1.0f, -1.0f) },   // Vertex 2
		Vertex{ FVector3(-1.0f, 1.0f, -1.0f) },  // Vertex 3
		Vertex{ FVector3(-1.0f, -1.0f, 1.0f) },  // Vertex 4
		Vertex{ FVector3(1.0f, -1.0f, 1.0f) },   // Vertex 5
		Vertex{ FVector3(1.0f, 1.0f, 1.0f) },    // Vertex 6
		Vertex{ FVector3(-1.0f, 1.0f, 1.0f) }    // Vertex 7
	};
	std::vector<unsigned int> cubeIndices = {
		0, 1, // Edge 0-1
		1, 2, // Edge 1-2
		2, 3, // Edge 2-3
		3, 0, // Edge 3-0
		4, 5, // Edge 4-5
		5, 6, // Edge 5-6
		6, 7, // Edge 6-7
		7, 4, // Edge 7-4
		0, 4, // Edge 0-4
		1, 5, // Edge 1-5
		2, 6, // Edge 2-6
		3, 7  // Edge 3-7
	};
	Mesh cubeMesh(cubeVertices, cubeIndices);
	Model cubeModel({ cubeMesh });
	graphicsManager->AddModel("Cube", cubeModel);
	//Create cube vertices
	//Create cube indices
	//Create cube mesh
	//Create cube model
	//Add cube model to graphics manager

	//Try to render cube
	//Only manually being created for now. Later will do via obj loader
}

void RenderView::Render()
{
	auto graphicsManager = m_graphicsManager.lock();
	auto camera = m_camera.lock();
	auto transformComponentArray = m_transformComponentArray.lock();
	auto modelComponentArray = m_modelComponentArray.lock();
	auto textComponentArray = m_textComponentArray.lock();

	if (!graphicsManager || !camera || !transformComponentArray || !modelComponentArray || !textComponentArray)
	{
		return;
	}
	const Matrix4& viewProjectionMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	auto transformModelIDs = transformComponentArray->GetEntityIntersection(modelComponentArray->GetEntities());
	for (auto ID : transformModelIDs)
	{
		auto & transform = transformComponentArray->GetItem(ID);
		auto & model = modelComponentArray->GetItem(ID);
		auto & modelData = graphicsManager->GetModel(model.modelName);
		auto & modelMatrix = Matrix4::CreateTranslationMatrix(transform.Position) * Matrix4::CreateEulerAngleMatrixXYZ(transform.Rotation) * Matrix4::CreateScaleMatrix(transform.Scale);
		auto & mvpMatrix = viewProjectionMatrix * modelMatrix;
		Renderer::RenderModel(modelData, mvpMatrix);
	}
}

void RenderView::Shutdown()
{
}

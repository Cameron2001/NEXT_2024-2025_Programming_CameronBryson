#include "stdafx.h"
#include "RenderView.h"

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


}

void RenderView::Shutdown()
{
}

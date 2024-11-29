#pragma once
class Scene
{
public:
	Scene();
	virtual ~Scene() = default;
	virtual void Init();
	virtual void LateInit();
	virtual void Update(const float dt);
	virtual void LateUpdate(const float dt);
	virtual void Render();
	virtual void LateRender();
	virtual void Shutdown();
	virtual void LateShutdown();
};


#pragma once
#include "Engine/Core/Scene.h"
class PlayScene : public Scene
{
public:
	PlayScene();
	virtual ~PlayScene() = default;
	virtual void Init() override;
	virtual void LateInit() override;
	virtual void Update(const float dt) override;
	virtual void LateUpdate(const float dt) override;
	virtual void Render() override;
	virtual void LateRender() override;
	virtual void Shutdown() override;
	virtual void LateShutdown() override;
private:
	void InitComponentArrays();
};


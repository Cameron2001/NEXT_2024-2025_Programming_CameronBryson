#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include "Game/GameComponents.h"
class PlayerController
{
public:
	PlayerController();
	~PlayerController() = default;
	void Init(std::shared_ptr<ComponentArray<PlayerComponent>> playerComponentArray, std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidbodyComponentArray);
	void Update(float dt);
private:
	std::weak_ptr<ComponentArray<PlayerComponent>> m_playerComponentArray;	
	std::weak_ptr<ComponentArray<RigidBodyComponent>> m_rigidbodyComponentArray;

};
//required compoenents:


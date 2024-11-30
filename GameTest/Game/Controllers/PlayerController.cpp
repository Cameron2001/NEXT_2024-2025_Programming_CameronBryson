#include "stdafx.h"
#include "PlayerController.h"
#include <App/app.h>

PlayerController::PlayerController()
{
}

void PlayerController::Init(std::shared_ptr<ComponentArray<PlayerComponent>> playerComponentArray, std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidbodyComponentArray)
{
	m_playerComponentArray = playerComponentArray;
	m_rigidbodyComponentArray = rigidbodyComponentArray;
}

void PlayerController::Update(float dt)
{
	auto playerComponentArray = m_playerComponentArray.lock();
	auto rigidbodyComponentArray = m_rigidbodyComponentArray.lock();
	if (!playerComponentArray || !rigidbodyComponentArray)
	{
		return;
	}
	bool W = App::IsKeyPressed('W');
	bool A = App::IsKeyPressed('A');
	bool S = App::IsKeyPressed('S');
	bool D = App::IsKeyPressed('D');
}

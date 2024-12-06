#include "stdafx.h"
#include "PlayerSystem.h"
#include <App/app.h>

PlayerSystem::PlayerSystem()
{
}

void PlayerSystem::Init(std::shared_ptr<ComponentArray<PlayerComponent>> playerComponentArray, std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidbodyComponentArray)
{
	m_playerComponentArray = playerComponentArray;
	m_rigidbodyComponentArray = rigidbodyComponentArray;
}

void PlayerSystem::Update(float dt)
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
	bool Q = App::IsKeyPressed('Q');
	bool E = App::IsKeyPressed('E');
	bool R = App::IsKeyPressed('R');
	bool F = App::IsKeyPressed('F');
	bool V = App::IsKeyPressed('V');
	const float rotationSpeed = 0.000001f;
	auto playerIDs = playerComponentArray->GetEntities();
	for (auto playerID : playerIDs)
	{
		auto & player = playerComponentArray->GetComponent(playerID);
		auto & rigidbody = rigidbodyComponentArray->GetComponent(playerID);
		if (W)
		{
			rigidbody.linearAcceleration += FVector3{ 0,1,0 }* player.moveSpeed * dt;
		}
		if (S)
		{
			rigidbody.linearAcceleration -= FVector3{ 0,1,0 }*player.moveSpeed * dt;
		}
		if (A)
		{
			rigidbody.linearAcceleration -= FVector3{ 1,0,0 }*player.moveSpeed * dt;
		}
		if (D)
		{
			rigidbody.linearAcceleration += FVector3{ 1,0,0 }*player.moveSpeed * dt;
		}
		if (Q)
		{
			rigidbody.linearAcceleration -= FVector3{ 0,0,1 }*player.moveSpeed * dt;
		}
		if (E)
		{
			rigidbody.linearAcceleration += FVector3{ 0,0,1 }*player.moveSpeed * dt;
		}
		if (R)
		{
			rigidbody.angularAcceleration += FVector3{ 0,0,1 }*rotationSpeed* dt;
		}
		if (F)
		{
			rigidbody.angularAcceleration += FVector3{ 0,1,0 }*rotationSpeed* dt;
		}
		if (V)
		{
			rigidbody.angularAcceleration += FVector3{ 1,0,0 }*rotationSpeed * dt;
		}
	}
}

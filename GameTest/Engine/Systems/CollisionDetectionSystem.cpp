#include "stdafx.h"
#include "CollisionDetectionSystem.h"

CollisionDetectionSystem::CollisionDetectionSystem()
{
}


void CollisionDetectionSystem::Init()
{
}

void CollisionDetectionSystem::Update(const float dt)
{
}

void CollisionDetectionSystem::Shutdown()
{
}

bool CollisionDetectionSystem::OOBvsOOB(Entity ID1, Entity ID2)
{
    return false;
}

bool CollisionDetectionSystem::SpherevsSphere(Entity ID1, Entity ID2)
{
    return false;
}

bool CollisionDetectionSystem::SpherevsOOB(Entity ID1, Entity ID2)
{
    return false;
}
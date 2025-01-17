#pragma once
#include <vector>
#include "Game/Core/Entity.h"

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void RemoveComponent(Entity entity) = 0;
    virtual bool HasComponent(Entity entity) = 0;
    virtual std::vector<Entity> GetEntities() = 0;
    virtual std::vector<Entity> GetEntityIntersection(std::vector<Entity> entities) = 0;
};
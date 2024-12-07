#pragma once
#include <vector>
using Entity = unsigned int;

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void RemoveComponent(Entity entity) = 0;
    virtual bool HasComponent(Entity entity) = 0;
    virtual std::vector<Entity> GetEntities() = 0;
    virtual std::vector<Entity> GetEntityIntersection(std::vector<Entity> entities) = 0;
};
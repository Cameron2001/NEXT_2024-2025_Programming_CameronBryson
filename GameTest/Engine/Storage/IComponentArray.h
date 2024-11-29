#pragma once
#include <vector>
using Entity = unsigned int;
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void RemoveItem(Entity entity) = 0;
	virtual bool HasItem(Entity entity) = 0;
	virtual std::vector<Entity> GetEntities() = 0;
	virtual std::vector<Entity> GetEntityIntersection(std::vector<Entity> entities) = 0;
};

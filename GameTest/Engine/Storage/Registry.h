#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Engine/Storage/ComponentArray.h"
class Registry
{
public:
	Registry();
	~Registry() = default;
	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	void ClearRegistry();

public:
	template<typename T>
	void CreateComponentArray();

	template<typename T, typename... Args>
	void AddComponent(Entity entity, Args&&... args);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	T& GetComponent(Entity entity);

	template<typename T>
	bool HasComponent(Entity entity);

	template<typename... T>
	std::vector<Entity> GetEntitiesWithComponents();

	template<typename T>
	
	std::shared_ptr<ComponentArray<T>> GetComponentArray();

	template<typename T>
	bool HasComponentArray();

private:
	std::vector<Entity> m_entities;
	std::vector<Entity> m_freeEntities;
	std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_componentArrays;

};

template<typename T>
inline void Registry::CreateComponentArray()
{
	m_componentArrays.try_emplace(std::type_index(typeid(T)), std::make_shared<ComponentArray<T>>());
}

template<typename T, typename ...Args>
inline void Registry::AddComponent(Entity entity, Args && ...args)
{
	GetComponentArray<T>()->AddComponent(entity, std::forward<Args>(args)...);
}

template<typename T>
inline void Registry::RemoveComponent(Entity entity)
{
	GetComponentArray<T>()->RemoveComponent(entity);
}

template<typename T>
inline T& Registry::GetComponent(Entity entity)
{
	return GetComponentArray<T>().GetComponent(entity);
}

template<typename T>
inline bool Registry::HasComponent(Entity entity)
{
	return GetComponentArray<T>().HasComponent(entity);
}

template<typename ...T>
inline std::vector<Entity> Registry::GetEntitiesWithComponents()
{
	std::vector<Entity> result;
	if (sizeof...(T) == 0)
	{
		return result;
	}
	std::vector<std::shared_ptr<IComponentArray>> arrays = { GetComponentArray<T>()... };
	result = arrays[0]->GetEntities();
	//would be best to initalize it as the smallest array
	for (size_t i = 1; i < arrays.size(); i++)
	{
		result = arrays[i]->GetEntityIntersection(result);
	}
	return result;
}

template<typename T>
inline std::shared_ptr<ComponentArray<T>> Registry::GetComponentArray()
{
	if (!HasComponentArray<T>())
	{
		CreateComponentArray<T>();
	}
	return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[std::type_index(typeid(T))]);
}

template<typename T>
inline bool Registry::HasComponentArray()
{
	return m_componentArrays.find(std::type_index(typeid(T))) != m_componentArrays.end();
}

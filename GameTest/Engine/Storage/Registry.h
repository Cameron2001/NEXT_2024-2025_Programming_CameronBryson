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

    template <typename T>
    void CreateComponentArray();

    template <typename T, typename... Args>
    void AddComponent(Entity entity, Args &&... args);

    template <typename T>
    void RemoveComponent(Entity entity);

    template <typename T>
    T &GetComponent(Entity entity);

    template <typename T>
    bool HasComponent(Entity entity);

    template <typename... T>
    std::vector<Entity> GetEntitiesWithComponents();

    template <typename T>

    std::shared_ptr<ComponentArray<T>> GetComponentArray();

    template <typename T>
    bool HasComponentArray();

private:
    std::vector<Entity> m_entities;
    std::vector<Entity> m_freeEntities;
    //std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_componentArrays;
    std::vector<std::shared_ptr<IComponentArray>> m_componentArrays;

private:
    struct ComponentCounter
    {
        static unsigned int counter;
    };

    template <typename T> static unsigned int GetComponentID()
    {
        static unsigned int id = ComponentCounter::counter++;
        return id;
    }


};

template <typename T> void Registry::CreateComponentArray()
{
    auto id = GetComponentID<T>();
    // Resize if needed
    if (id >= m_componentArrays.size())
    {
        m_componentArrays.resize(id + 1);
    }
    // Create new component array
    m_componentArrays[id] = std::make_shared<ComponentArray<T>>();
}
//just redirection
template <typename T, typename... Args>
void Registry::AddComponent(Entity entity, Args &&... args)
{
    GetComponentArray<T>()->AddComponent(entity, std::forward<Args>(args)...);
}
//just redirection
template <typename T>
void Registry::RemoveComponent(Entity entity)
{
    GetComponentArray<T>()->RemoveComponent(entity);
}
//just redirection
template <typename T>
T &Registry::GetComponent(Entity entity)
{
    return GetComponentArray<T>().GetComponent(entity);
}
//just redirection
template <typename T>
bool Registry::HasComponent(Entity entity)
{
    return GetComponentArray<T>().HasComponent(entity);
}

template <typename... T>
std::vector<Entity> Registry::GetEntitiesWithComponents()
{
    std::vector<Entity> result;
    if (sizeof...(T) == 0)
    {
        return result;
    }
    std::vector<std::shared_ptr<IComponentArray>> arrays = {GetComponentArray<T>()...};
    result = arrays[0]->GetEntities();
    //would be best to initalize it as the smallest array
    for (size_t i = 1; i < arrays.size(); i++)
    {
        result = arrays[i]->GetEntityIntersection(result);
    }
    return result;
}
//just redirection
template <typename T>
std::shared_ptr<ComponentArray<T>> Registry::GetComponentArray()
{
    auto typeID = GetComponentID<T>();
    if (!HasComponentArray<T>())
    {
        CreateComponentArray<T>();
    }
    return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeID]);
}

template <typename T> bool Registry::HasComponentArray()
{
    auto id = GetComponentID<T>();
    return id < m_componentArrays.size() && m_componentArrays[id] != nullptr;
}
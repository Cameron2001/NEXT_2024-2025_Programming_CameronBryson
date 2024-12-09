#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Engine/Storage/ComponentArray.h"
#include "View.h"

class Registry
{
public:
    Registry();
    ~Registry() = default;
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void ClearRegistry();

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
    ComponentArray<T>& GetComponentArray();

    template <typename T>
    bool HasComponentArray();

    template <typename... T>
    View<T...> CreateView()
    {
        return View<T...>(this);
    }

private:
    std::vector<Entity> m_entities;
    std::vector<Entity> m_freeEntities;
    std::vector<std::unique_ptr<IComponentArray>> m_componentArrays;

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

template <typename T, typename... Args>
void Registry::AddComponent(Entity entity, Args &&... args)
{
    GetComponentArray<T>().AddComponent(entity, std::forward<Args>(args)...);
}
//just redirection
template <typename T>
void Registry::RemoveComponent(Entity entity)
{
    GetComponentArray<T>().RemoveComponent(entity);
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
    if constexpr (sizeof...(T) == 0)
    {
        return std::vector<Entity>();
    }

    std::vector<IComponentArray *> arrays = {&GetComponentArray<T>()...};

    auto result = arrays[0]->GetEntities();
    for (size_t i = 1; i < arrays.size(); i++)
    {
        result = arrays[i]->GetEntityIntersection(result);
    }
    return result;
}
template <typename T>
ComponentArray<T>& Registry::GetComponentArray()
{
    auto id = GetComponentID<T>();
    if (id >= m_componentArrays.size())
    {
        m_componentArrays.resize(id + 1);
    }
    if (!m_componentArrays[id])
    {
        m_componentArrays[id] = std::make_unique<ComponentArray<T>>();
    }
    return *static_cast<ComponentArray<T> *>(m_componentArrays[id].get());
}

template <typename T> bool Registry::HasComponentArray()
{
    auto id = GetComponentID<T>();
    return id < m_componentArrays.size() && m_componentArrays[id] != nullptr;
}
#pragma once
#include "IComponentArray.h"
#include <algorithm>
#include <vector>
#include <iterator>
using Entity = unsigned int;
constexpr size_t MAX_ENTITIES = 10000;

template <typename T>
class ComponentArray final :
    public IComponentArray
{
public:
    ComponentArray();
    ~ComponentArray() override = default;

    template <typename... Args>
    void AddComponent(Entity entity, Args &&... args);
    void RemoveComponent(Entity entity) override;
    bool HasComponent(Entity entity) override;
    T &GetComponent(Entity entity);
    std::vector<Entity> GetEntities() override;
    std::vector<Entity> GetEntityIntersection(std::vector<Entity> entities) override;

private:
    std::vector<T> m_componentArray;
    std::vector<Entity> m_DenseArray;
    std::vector<Entity> m_SparseArray;
};

template <typename T>
ComponentArray<T>::ComponentArray()
{
    m_SparseArray.resize(MAX_ENTITIES, 0);
    m_DenseArray.reserve(MAX_ENTITIES);
    m_componentArray.reserve(MAX_ENTITIES);
}

template <typename T>
template <typename... Args>
void ComponentArray<T>::AddComponent(Entity entity, Args &&... args)
{
    m_DenseArray.emplace_back(entity);
    m_componentArray.emplace_back(std::forward<Args>(args)...);
    m_SparseArray[entity] = m_DenseArray.size() - 1;
}

template <typename T>
void ComponentArray<T>::RemoveComponent(Entity entity)
{
    Entity entityIndex = m_SparseArray[entity];
    Entity lastIndex = m_DenseArray.size() - 1;
    Entity lastEntity = m_DenseArray[lastIndex];

    std::swap(m_DenseArray[entityIndex], m_DenseArray[lastIndex]);
    std::swap(m_componentArray[entityIndex], m_componentArray[lastIndex]);

    m_DenseArray.pop_back();
    m_componentArray.pop_back();

    m_SparseArray[lastEntity] = entityIndex;
    m_SparseArray[entity] = 0; // invalid index, change to somehting ebetter later
}

template <typename T>
bool ComponentArray<T>::HasComponent(Entity entity)
{
    return m_SparseArray[entity] < m_DenseArray.size() && m_DenseArray[m_SparseArray[entity]] == entity;
}

template <typename T>
T &ComponentArray<T>::GetComponent(Entity entity)
{
    Entity index = m_SparseArray[entity];
    return m_componentArray[index];
}

template <typename T>
std::vector<Entity> ComponentArray<T>::GetEntities()
{
    return m_DenseArray;
}

template <typename T>
std::vector<Entity> ComponentArray<T>::GetEntityIntersection(std::vector<Entity> entities)
{
    auto IDs = GetEntities();

    if (!std::is_sorted(IDs.begin(), IDs.end()))
    {
        std::sort(IDs.begin(), IDs.end());
    }

    if (!std::is_sorted(entities.begin(), entities.end()))
    {
        std::sort(entities.begin(), entities.end());
    }

    std::vector<Entity> intersection;
    intersection.reserve(std::min(IDs.size(), entities.size()));
    std::set_intersection(IDs.begin(), IDs.end(), entities.begin(), entities.end(), std::back_inserter(intersection));

    return intersection;
}
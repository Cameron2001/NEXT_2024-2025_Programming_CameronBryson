#include "stdafx.h"
#include "Registry.h"
#include <numeric>

Registry::Registry()
{
    m_entities.reserve(MAX_ENTITIES);
    m_freeEntities.resize(MAX_ENTITIES - 1);
    std::iota(m_freeEntities.begin(), m_freeEntities.end(), 1);

}

Entity Registry::CreateEntity()
{
    Entity ID = m_freeEntities.back();
    m_freeEntities.pop_back();
    m_entities.push_back(ID);
    return ID;
}

void Registry::DestroyEntity(Entity entity)
{
    for (auto &componentArray : m_componentArrays)
    {
        if (componentArray.second->HasComponent(entity))
        {
            componentArray.second->RemoveComponent(entity);
        }
    }
    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
    m_freeEntities.emplace_back(entity);
}

void Registry::ClearRegistry()
{

}
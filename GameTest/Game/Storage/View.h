#pragma once
#include "Registry.h"
#include <tuple>
#include <utility>
#include <vector>
#include <iterator>
#include "ComponentArray.h"
#include <ppl.h>
#include "Game/Core/Entity.h"
class Registry;

template <typename... Components> class View
{
  public:
    explicit View(Registry *registry)
        : m_registry(registry), m_componentArrays(std::make_tuple(&m_registry->GetComponentArray<Components>()...))
    {
        Update();
    }

    class Iterator
    {
      public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::tuple<Entity, Components &...>;
        using pointer = value_type *;
        using reference = value_type &;

        Iterator(View *view, const size_t index) : m_view(view), m_index(index)
        {
        }

        value_type operator*()
        {
            Entity e = m_view->m_matchingEntities[m_index];
            return std::forward_as_tuple(e, m_view->GetComponent<Components>(e)...);
        }

        Iterator &operator++()
        {
            ++m_index;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator!=(const Iterator &other) const
        {
            return m_index != other.m_index;
        }

      private:
        View *m_view;
        size_t m_index;
    };

    class ConstIterator
    {
      public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::tuple<Entity, const Components &...>;
        using pointer = value_type *;
        using reference = value_type &;

        ConstIterator(const View *view, const size_t index) : m_view(view), m_index(index)
        {
        }

        value_type operator*() const
        {
            Entity e = m_view->m_matchingEntities[m_index];
            return std::forward_as_tuple(e, m_view->GetComponent<Components>(e)...);
        }

        ConstIterator &operator++()
        {
            ++m_index;
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator!=(const ConstIterator &other) const
        {
            return m_index != other.m_index;
        }

      private:
        const View *m_view;
        size_t m_index;
    };

    Iterator begin()
    {
        return Iterator(this, 0);
    }

    Iterator end()
    {
        return Iterator(this, m_matchingEntities.size());
    }

    ConstIterator begin() const
    {
        return ConstIterator(this, 0);
    }

    ConstIterator end() const
    {
        return ConstIterator(this, m_matchingEntities.size());
    }

    bool empty() const
    {
        return m_matchingEntities.empty();
    }

    size_t size() const
    {
        return m_matchingEntities.size();
    }

    Entity front() const
    {
        return m_matchingEntities.empty() ? Entity(0) : m_matchingEntities.front();
    }

    template <typename Func> void ForEach(Func &&func)
    {
        for (const Entity entity : m_matchingEntities)
        {
            func(entity, GetComponent<Components>(entity)...);
        }
    }

    template <typename Func> void ParallelForEach(Func &&func)
    {
        const size_t entityCount = m_matchingEntities.size();
        concurrency::parallel_for(static_cast<size_t>(0), entityCount, [&](const size_t i) {
            Entity e = m_matchingEntities[i];
            func(e, GetComponent<Components>(e)...);
        });
    }

    size_t GetEntityCount() const
    {
        return m_matchingEntities.size();
    }

    template <typename Component> ComponentArray<Component> *GetComponentArray() const
    {
        return std::get<ComponentArray<Component> *>(m_componentArrays);
    }

    void Update()
    {
        m_matchingEntities = m_registry->GetEntitiesWithComponents<Components...>();
    }

  private:
    Registry *m_registry;
    std::vector<Entity> m_matchingEntities;

    std::tuple<ComponentArray<Components> *...> m_componentArrays;

    template <typename Component> Component &GetComponent(Entity entity)
    {
        return std::get<ComponentArray<Component> *>(m_componentArrays)->GetComponent(entity);
    }
};

#pragma once
#include "Registry.h"
#include <tuple>
#include <utility>
#include <vector>
#include <ppl.h>
class Registry;
using Entity = unsigned int;

template <typename... Components> class View
{
  public:
    explicit View(Registry *registry) : m_registry(registry)
    {
        m_matchingEntities = m_registry->GetEntitiesWithComponents<Components...>();
    }

    class Iterator
    {
      public:
        using ValueType = std::tuple<Entity, Components &...>;

        Iterator(View *view, const size_t index) : m_view(view), m_index(index)
        {
        }

        ValueType operator*()
        {
            Entity e = m_view->m_matchingEntities[m_index];
            return std::forward_as_tuple(e, m_view->m_registry->template GetComponent<Components>(e)...);
        }

        Iterator &operator++()
        {
            ++m_index;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++m_index;
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

    Iterator begin()
    {
        return Iterator(this, 0);
    }
    Iterator end()
    {
        return Iterator(this, m_matchingEntities.size());
    }
    template <typename Func> void ParallelForEach(Func &&func)
    {
        const size_t entityCount = m_matchingEntities.size();
        concurrency::parallel_for(static_cast<size_t>(0), entityCount, [&](const size_t i) {
            Entity e = m_matchingEntities[i];
            func(std::forward_as_tuple(e, m_registry->template GetComponent<Components>(e)...));
        });
    }

  private:
    Registry *m_registry;
    std::vector<Entity> m_matchingEntities;
};

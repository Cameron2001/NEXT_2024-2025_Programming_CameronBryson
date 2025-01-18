#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <mutex>
#include "Game/Core/Event.h"

class EventManager
{
  public:
    EventManager() = default;
    ~EventManager() = default;

    template <typename EventType, typename T>
    void AddListener(std::shared_ptr<T> instance, void (T::*memberFn)(const EventType &))
    {
        
    }

    template <typename EventType, typename T>
    void RemoveListener(std::shared_ptr<T> instance, void (T::*memberFn)(const EventType &))
    {
       
    }

    template <typename EventType> void Notify(const EventType &eventData)
    {
       
    }

  private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> m_events;
    std::mutex m_mutex;
};

// File: GameTest/Game/Managers/EventManager.h
#pragma once
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <typeindex>
#include <typeinfo>
#include "Game/Core/Event.h" 

struct pair_hash
{
    std::size_t operator()(const std::pair<std::string, std::type_index> &key) const
    {
        return std::hash<std::string>()(key.first) ^ key.second.hash_code();
    }
};

class EventManager
{
  public:
    EventManager() = default;
    ~EventManager() = default;

    template <typename... Args, typename T>
    void AddListener(const std::string &identifier, std::shared_ptr<T> instance, void (T::*memberFn)(Args...))
    {
        auto event = GetOrCreateEvent<Args...>(identifier);
        event->AddListener(instance, memberFn);
    }

    template <typename... Args, typename T>
    void RemoveListener(const std::string &identifier, std::shared_ptr<T> instance, void (T::*memberFn)(Args...))
    {
        auto event = GetEvent<Args...>(identifier);
        if (event)
        {
            event->RemoveListener(instance, memberFn);
        }
    }

    template <typename... Args> void Notify(const std::string &identifier, Args... eventData)
    {
        auto event = GetEvent<Args...>(identifier);
        if (event)
        {
            event->Notify(eventData...);
        }
    }

  private:
    template <typename... Args> std::shared_ptr<Event<Args...>> GetOrCreateEvent(const std::string &identifier)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::pair<std::string, std::type_index> key = {identifier, typeid(Event<Args...>)};
        auto it = m_events.find(key);
        if (it != m_events.end())
        {
            return std::static_pointer_cast<Event<Args...>>(it->second);
        }
        else
        {
            auto event = std::make_shared<Event<Args...>>();
            m_events.emplace(key, event);
            return event;
        }
    }

    template <typename... Args> std::shared_ptr<Event<Args...>> GetEvent(const std::string &identifier)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::pair<std::string, std::type_index> key = {identifier, typeid(Event<Args...>)};
        auto it = m_events.find(key);
        if (it != m_events.end())
        {
            return std::static_pointer_cast<Event<Args...>>(it->second);
        }
        return nullptr;
    }
    //Do we want this to be shared ptr?
    std::unordered_map<std::pair<std::string, std::type_index>, std::shared_ptr<void>, pair_hash> m_events;
    std::mutex m_mutex;
};

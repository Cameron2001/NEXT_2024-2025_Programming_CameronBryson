#pragma once
#include <functional>
#include <vector>
#include <algorithm>

// Need to make this safer from dangling pointers and shit
template <typename... Args> class Event
{
  public:
    Event() = default;
    ~Event() = default;
    Event(const Event &) = delete;
    Event &operator=(const Event &) = delete;

    Event(Event &&) = default;
    Event &operator=(Event &&) = default;
    using ListenerFunc = std::function<void(Args...)>;

    struct Listener
    {
        void *owner;
        ListenerFunc func;

        bool operator==(const Listener &other) const
        {
            return owner == other.owner;
        }
    };

    template <typename T> void AddListener(T *instance, void (T::*memberFn)(Args...))
    {
        Listener listener{instance, [instance, memberFn](Args... args) { (instance->*memberFn)(args...); }};
        m_listeners.push_back(listener);
    }

    void RemoveListenersByOwner(void *owner)
    {
        m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(),
                                         [owner](const Listener &listener) { return listener.owner == owner; }),
                          m_listeners.end());
    }

    void Notify(Args... args)
    {
        for (auto &listener : m_listeners)
        {
            listener.func(std::forward<Args>(args)...);
        }
    }

  private:
    std::vector<Listener> m_listeners;
};

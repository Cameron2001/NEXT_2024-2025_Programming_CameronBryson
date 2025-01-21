#pragma once
#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
//Need to add comments here to explain
template <typename... Args> class Event
{
  public:
    using ListenerFunc = std::function<void(Args...)>;

    struct Listener
    {
        std::weak_ptr<void> owner;
        ListenerFunc func;

        bool expired() const
        {
            return owner.expired();
        }
    };

    template <typename T> void AddListener(std::shared_ptr<T> instance, void (T::*memberFn)(Args...))
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        Listener listener{};
        listener.owner = instance;
        std::weak_ptr<T> weakInstance = instance;

        listener.func = [weakInstance, memberFn](Args... args) {
            if (auto obj = weakInstance.lock())
            {
                (obj.get()->*memberFn)(args...);
            }
        };
        m_listeners.emplace_back(std::move(listener));
    }
    template <typename T> void RemoveListener(std::shared_ptr<T> instance, void (T::*memberFn)(Args...))
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(),
                                         [&](const Listener &listener) {
                                             auto shared = listener.owner.lock();
                                             return shared == instance;
                                         }),
                          m_listeners.end());
    }

    void Notify(Args... args)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        // Remove expired listeners
        m_listeners.erase(
            std::remove_if(m_listeners.begin(), m_listeners.end(), [](const Listener &l) { return l.expired(); }),
            m_listeners.end());

        for (auto &listener : m_listeners)
        {
            if (!listener.expired())
            {
                listener.func(args...);
            }
        }
    }

  private:
    std::vector<Listener> m_listeners;
    std::mutex m_mutex;
};

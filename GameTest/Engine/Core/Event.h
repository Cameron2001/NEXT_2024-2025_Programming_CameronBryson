#pragma once
#include <functional>
#include <vector>
#include <algorithm>
template <typename... Args>
class Event
{
public:
	using Listener = std::function<void(Args...)>;

	void AddListener(const Listener& listener)
	{
		m_listeners.push_back(listener);
	}
	template<typename... CallArgs>
	void Notify(CallArgs... args)
	{
		for (auto& listener : m_listeners)
		{
			listener(std::forward<CallArgs>(args)...);
		}
	}

private:
	std::vector<Listener> m_listeners;
	
};


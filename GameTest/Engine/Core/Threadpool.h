#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <future>

class Threadpool
{
  public:
    explicit Threadpool(const size_t numThreads) : stop(false)
    {
        for (size_t i = 0; i < numThreads; i++)
        {
            m_threads.emplace_back([this]() {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        m_condition.wait(lock, [this]() { return stop || !m_tasks.empty(); });
                        if (stop && m_tasks.empty())
                            return;
                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~Threadpool()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        stop = true;

        m_condition.notify_all();
        for (auto &thread : m_threads)
        {
            thread.join();
        }
    }
    Threadpool(const Threadpool &) = delete;
    Threadpool &operator=(const Threadpool &) = delete;
    Threadpool(Threadpool &&) = delete;
    Threadpool &operator=(Threadpool &&) = delete;

    template <typename Func, typename... Args>
    // return future
    auto QueueTask(Func &&function, Args &&...args) -> std::future<decltype(function(args...))>
    {
        using return_type = decltype(function(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(function), std::forward<Args>(args)...));
        auto result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            // adding task as void function
            m_tasks.emplace([task]() { (*task)(); });
        }
        m_condition.notify_one(); // notify about new task
        return result;
    }

  private:
    std::vector<std::thread> m_threads;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::queue<std::function<void()>> m_tasks;
    bool stop;
};

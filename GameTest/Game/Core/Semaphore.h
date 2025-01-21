#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore
{
  public:
    explicit Semaphore(const int count = 0);
    void Acquire();
    void Release();

  private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    int m_count;
};

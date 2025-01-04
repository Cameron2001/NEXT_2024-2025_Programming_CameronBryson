#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore
{
  public:
    explicit Semaphore(int count = 0);
    void acquire();
    void release();

  private:
    std::mutex mtx_;
    std::condition_variable cv_;
    int count_;
};

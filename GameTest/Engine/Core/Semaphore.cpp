#include "stdafx.h"
#include "Semaphore.h"

Semaphore::Semaphore(int count) : count_(count)
{
}

void Semaphore::acquire()
{
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [&]() { return count_ > 0; });
    --count_;
}

void Semaphore::release()
{
    std::unique_lock<std::mutex> lock(mtx_);
    ++count_;
    cv_.notify_one();
}

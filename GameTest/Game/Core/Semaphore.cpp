#include "stdafx.h"
#include "Semaphore.h"

Semaphore::Semaphore(const int count) : m_count(count)
{
}

void Semaphore::Acquire()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [&] { return m_count > 0; });
    --m_count;
}

void Semaphore::Release()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    ++m_count;
    m_condition.notify_one();
}

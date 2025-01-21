#pragma once
#include "Semaphore.h"
class SemaphoreGuard
{
  public:
    explicit SemaphoreGuard(Semaphore &semaphore);
    ~SemaphoreGuard();
    SemaphoreGuard(SemaphoreGuard &&other) noexcept;
    SemaphoreGuard &operator=(SemaphoreGuard &&other) noexcept;

  private:
    void Release();
    Semaphore *m_semaphore;
    bool m_owns;
};

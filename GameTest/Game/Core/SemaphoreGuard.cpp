#include "stdafx.h"
#include "SemaphoreGuard.h"

SemaphoreGuard::SemaphoreGuard(Semaphore &semaphore) : m_semaphore(&semaphore), m_owns(false)
{
    m_semaphore->Acquire();
    m_owns = true;
}
SemaphoreGuard::~SemaphoreGuard()
{
    Release();
}

SemaphoreGuard::SemaphoreGuard(SemaphoreGuard &&other) noexcept : m_semaphore(other.m_semaphore), m_owns(other.m_owns)
{
    other.m_semaphore = nullptr;
    other.m_owns = false;
}

SemaphoreGuard &SemaphoreGuard::operator=(SemaphoreGuard &&other) noexcept
{
    if (this != &other)
    {
        Release();
        m_semaphore = other.m_semaphore;
        m_owns = other.m_owns;
        other.m_semaphore = nullptr;
        other.m_owns = false;
    }
    return *this;
}

void SemaphoreGuard::Release()
{
    if (m_owns && m_semaphore)
    {
        m_semaphore->Release();
        m_owns = false;
    }
}

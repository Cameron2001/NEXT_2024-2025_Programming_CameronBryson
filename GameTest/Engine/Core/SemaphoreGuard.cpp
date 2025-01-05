#include "stdafx.h"
#include "SemaphoreGuard.h"

SemaphoreGuard::SemaphoreGuard(Semaphore &semaphore) : m_semaphore(&semaphore), m_owns(false)
{
    m_semaphore->acquire();
    m_owns = true;
}
SemaphoreGuard::~SemaphoreGuard()
{
    release();
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
        release();
        m_semaphore = other.m_semaphore;
        m_owns = other.m_owns;
        other.m_semaphore = nullptr;
        other.m_owns = false;
    }
    return *this;
}

void SemaphoreGuard::release()
{
    if (m_owns && m_semaphore)
    {
        m_semaphore->release();
        m_owns = false;
    }
}

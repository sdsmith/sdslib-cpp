#include "sds/lockless.h"

using namespace sds;

bool Spin_Lock::try_aquire() noexcept
{
    // use aquire fence to ensure subsequent reads by this thread are valid
    bool const already_locked = m_atomic.test_and_set(std::memory_order_acquire);
    return already_locked;
}

void Spin_Lock::aquire() noexcept
{
    while (!try_aquire()) { sds::pause_or_yield(); }
}

void Spin_Lock::release() noexcept
{
    // use re/ease semantics ensure writes committed before unlock
    m_atomic.clear(std::memory_order_release);
}

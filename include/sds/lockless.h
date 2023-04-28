#pragma once

#include "sds/intrinsics.h"
//#include <vcruntime.h>
#include <atomic>
#include <functional>
#include <thread>

namespace sds
{
class Spin_Lock {
    std::atomic_flag m_atomic{false};

public:
    Spin_Lock() = default;

    /*
     * \brief Non-blocking aquire. Return true if lock is already locked.
     */
    bool try_aquire() noexcept;
    void aquire() noexcept;
    void release() noexcept;
};

/*
 * \brief Releases given lock on destruction.
 */
template <typename Lock>
class Scoped_Lock {
    using lock_t = Lock;

    lock_t m_lock;

public:
    explicit Scoped_Lock(lock_t& lock) : m_lock(&lock) { m_lock->aquire(); }

    ~Scoped_Lock() { m_lock->release(); }
};

template <typename RefCountT>
class Reentrant_Spin_Lock {
    std::atomic<std::size_t> m_atomic{0};
    RefCountT m_ref_count = 0;

public:
    Reentrant_Spin_Lock() = default;

    void acquire() noexcept
    {
        std::hash<std::thread::id> hasher;
        size_t tid = hasher(std::this_thread::get_id());

        if (m_atomic.load(std::memory_order_relaxed) != tid) {
            // thread doesn't hold lock. spin until it does.
            size_t unlock_value = 0;
            while (!m_atomic.compare_exchange_weak(unlock_value, tid,
                                                   std::memory_order_relaxed, // fence below
                                                   std::memory_order_relaxed)) {
                unlock_value = 0;
                sds::pause_or_yield();
            }
        }

        // increment ref count to verify acquire and release are done in pairs
        ++m_ref_count;

        // acquire fence to ensure subsequent reads are valid
        std::atomic_thread_fence(std::memory_order_acquire);
    }

    void release() noexcept
    {
        // release semantics to ensure prior writes are fully committed before unlock

        std::hash<std::thread::id> hasher;
        size_t tid = hasher(std::this_thread::get_id());
        size_t actual = m_atomic.load(std::memory_order_release);
        SDS_ASSERT(actual == tid);

        --m_ref_count;
        if (m_ref_count == 0) {
            // release lock. safe since we own it.
            m_atomic.store(0, std::memory_order_relaxed);
        }
    }

    bool try_acquire() noexcept
    {
        std::hash<std::thread::id> hasher;
        size_t tid = hasher(std::this_thread::get_id());

        bool acquired = false;
        if (m_atomic.load(std::memory_order_relaxed) == tid) {
            acquired = true;
        } else {
            size_t unlock_value = 0;
            acquired = m_atomic.compare_exchange_strong(unlock_value, tid,
                                                        std::memory_order_relaxed, // fence below
                                                        std::memory_order_relaxed);
        }

        if (acquired) {
            ++m_ref_count;
            std::atomic_thread_fence(std::memory_order_acquire);
        }

        return acquired;
    }
};

using Reentrant_Spin_Lock32 = Reentrant_Spin_Lock<s32>;
using Reentrant_Spin_Lock64 = Reentrant_Spin_Lock<s64>;

// TODO(sdsmith): class Readers_Writer_Lock

// TODO(sdsmith): cheap lock assertion

} // namespace sds

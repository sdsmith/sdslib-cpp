#pragma once

#include "sds/details/common.h"

#include <immintrin.h>
#include <intrin.h>

namespace sds
{
/**
 * \brief Reduse CPU power or yield when pause is unavailable.
 */
void pause_or_yield() noexcept
{
    // TODO(sdsmith): AMD also supports pause

#if 1
    _mm_pause();
#else
    // TODO(sdsmith): for platforms that don't support pause
    std::this_thread::yield();
#endif
}
} // namespace sds

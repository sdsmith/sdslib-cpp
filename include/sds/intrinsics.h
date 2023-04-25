#pragma once

#include "sds/cast.h"
#include "sds/details/common.h"
#include "sds/string.h"

#include <immintrin.h>
#include <intrin.h>
#include <array>
#include <thread>
#include <type_traits>

namespace sds
{
enum class cpu_vendor : u8 {
    unknown = 0,
    intel,
    amd,
};

struct Cpu_Info {
    cpu_vendor vendor = cpu_vendor::unknown;
};

Cpu_Info cpuid() noexcept
{
    Cpu_Info cpu_info;
    SDS_ASSERT(cpu_info.vendor == cpu_vendor::unknown);
    std::array<s32, 4> info;

    // get highest valid func id + vendor name
    __cpuid(info.data(), 0);

    s32 const max_func_id = info[0];

    std::array<char, sizeof(s32) * 3 + 1> vendor_str;
    vendor_str.fill(0);

    type_pun<char>(vendor_str.data(), &info[1]);
    type_pun<char>(vendor_str.data(), &info[2]);
    type_pun<char>(vendor_str.data(), &info[3]);

    /*
     *reinterpret_cast<s32*>(vendor_str.data() + 0) = info[1];
     *reinterpret_cast<s32*>(vendor_str.data() + 4) = info[2];
     *reinterpret_cast<s32*>(vendor_str.data() + 8) = info[3];
     */

    if (sds::ascii_cmp(vendor_str.data(), "GenuineIntel")) {
        cpu_info.vendor = cpu_vendor::intel;
    } else if (sds::ascii_cmp(vendor_str.data(), "AuthenticAMD")) {
        cpu_info.vendor = cpu_vendor::amd;
    }

    if (cpu_info.vendor == cpu_vendor::unknown) { return cpu_info; }

    // TODO(sdsmith):
    SDS_ASSERT(false);
    return cpu_info;
}

/**
 * \brief Reduse power on intel cpus or yield when pause is unavailable.
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

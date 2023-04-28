#pragma once

#include <cstddef>
#include <cstdint>

namespace sds
{
using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s32f = int_fast32_t; /** At least 32bit, larger if faster */
using s64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u32f = uint_fast32_t; /** At least 32bit, larger if faster */
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using sz = s32; /** Size type (size_t equivalent) */
using ptrdiff = std::ptrdiff_t;
using byte = std::byte;

constexpr auto operator""_KB(u64 s) { return s * 1024; }

constexpr auto operator""_MB(u64 s) { return s * 1024_KB; }

constexpr auto operator""_GB(u64 s) { return s * 1024_MB; }

/*
 * NOTE(sdsmith): Want static assert available as soon as possible.
 */
#ifndef SDS_STATIC_ASSERT
/**
 * \def SDS_STATIC_ASSERT
 * \brief Assert used by the library. Defaults to \a static_assert.
 */
#    define SDS_STATIC_ASSERT(x) static_assert(x)
#    define SDS_STATIC_ASSERT_MSG(x, msg) static_assert(x, msg)
#endif

/**
 * \def SDS_OS_WINDOWS
 * \brief True if compiling on Windows.
 *
 * Usage: `#if SDS_OS_WINDOWS`
 */
/**
 * \def SDS_OS_LINUX
 * \brief True if compiling on Linux.
 *
 * Usage: `#if SDS_OS_LINUX`
 */
/**
 * \def SDS_OS_MAC
 * \brief True if compiling on Mac.
 *
 * Usage: `#if SDS_OS_MAC`
 */

#if defined(_WIN32)
#    define SDS_OS_WINDOWS 1
#elif defined(__linux__)
#    define SDS_OS_LINUX 1
#elif defined(__APPLE__)
#    define SDS_OS_MAC 1
#else
#    error Unknown operating system
#endif

#ifndef SDS_OS_WINDOWS
#    define SDS_OS_WINDOWS 0
#endif
#ifndef SDS_OS_LINUX
#    define SDS_OS_LINUX 0
#endif
#ifndef SDS_OS_MAC
#    define SDS_OS_MAC 0
#endif
SDS_STATIC_ASSERT_MSG(SDS_OS_WINDOWS + SDS_OS_LINUX + SDS_OS_MAC == 1,
                      "must have exactly one OS set");

/**
 * \def SDS_COMPILER_MSC
 * \brief Microsoft compiler compiling the project.
 *
 * Usage: `#if SDS_COMPILER_MSC`
 */
/**
 * \def SDS_COMPILER_GCC
 * \brief GNU GCC compiler compiling the project.
 *
 * Usage: `#if SDS_COMPILER_GCC`
 */
/**
 * \def SDS_COMPILER_CLANG
 * \brief Clang compiler compiling the project.
 *
 * Usage: `#if SDS_COMPILER_CLANG`
 */

#if defined(_MSC_VER)
#    define SDS_COMPILER_MSC 1
#elif defined(__GNUC__)
#    define SDS_COMPILER_GCC 1
#elif defined(__clang__)
#    define SDS_COMPILER_CLANG 1
#else
#    error Unknown compiler
#endif

#ifndef SDS_COMPILER_MSC
/** Microsoft C++ compiler */
#    define SDS_COMPILER_MSC 0
#endif
#ifndef SDS_COMPILER_GCC
#    define SDS_COMPILER_GCC 0
#endif
#ifndef SDS_COMPILER_CLANG
#    define SDS_COMPILER_CLANG 0
#endif
SDS_STATIC_ASSERT_MSG(SDS_COMPILER_MSC + SDS_COMPILER_GCC + SDS_COMPILER_CLANG == 1,
                      "must have exactly one compiler set");

/**
 * \def SDS_CPLUSPLUS
 * \brief Portable replacement for \a __cplusplus.
 *
 * Microsoft does it again...
 */
#ifndef SDS_CPLUSPLUS
#    define SDS_CPLUSPLUS_11 201103L
#    define SDS_CPLUSPLUS_14 201402L
#    define SDS_CPLUSPLUS_17 201703L
#    define SDS_CPLUSPLUS_20 202002L

/**
 * \def SDS_CPLUSPLUS
 * \brief Portable replacement for \a __cplusplus.
 *
 * MSVC incorrectly reports __cplusplus
 * ref: https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
 */
#    if SDS_COMPILER == SDS_COMPILER_MSC
#        define SDS_CPLUSPLUS _MSVC_LANG
#    else
#        define SDS_CPLUSPLUS __cplusplus
#    endif

/**
 * \def SDS_CPLUSPLUS_11_SUPPORTED
 * \brief True if compiling with C++11 support.
 */
/**
 * \def SDS_CPLUSPLUS_14_SUPPORTED
 * \brief True if compiling with C++14 support.
 */
/**
 * \def SDS_CPLUSPLUS_17_SUPPORTED
 * \brief True if compiling with C++17 support.
 */
/**
 * \def SDS_CPLUSPLUS_20_SUPPORTED
 * \brief True if compiling with C++20 support.
 */
#    if SDS_CPLUSPLUS >= SDS_CPLUSPLUS_11
#        define SDS_CPLUSPLUS_11_SUPPORTED 1
#    endif
#    if SDS_CPLUSPLUS >= SDS_CPLUSPLUS_14
#        define SDS_CPLUSPLUS_14_SUPPORTED 1
#    endif
#    if SDS_CPLUSPLUS >= SDS_CPLUSPLUS_17
#        define SDS_CPLUSPLUS_17_SUPPORTED 1
#    endif
#    if SDS_CPLUSPLUS >= SDS_CPLUSPLUS_20
#        define SDS_CPLUSPLUS_20_SUPPORTED 1
#    endif

#    ifndef SDS_CPLUSPLUS_11_SUPPORTED
#        define SDS_CPLUSPLUS_11_SUPPORTED 0
#    endif
#    ifndef SDS_CPLUSPLUS_14_SUPPORTED
#        define SDS_CPLUSPLUS_14_SUPPORTED 0
#    endif
#    ifndef SDS_CPLUSPLUS_17_SUPPORTED
#        define SDS_CPLUSPLUS_17_SUPPORTED 0
#    endif
#    ifndef SDS_CPLUSPLUS_20_SUPPORTED
#        define SDS_CPLUSPLUS_20_SUPPORTED 0
#    endif
SDS_STATIC_ASSERT_MSG(SDS_CPLUSPLUS_11 + SDS_CPLUSPLUS_14 + SDS_CPLUSPLUS_17 + SDS_CPLUSPLUS_20 !=
                          0,
                      "must have at least one c++ version set");
#endif

/**
 * \def SDS_FILENAME
 * \brief Name of the current file.
 */
#define SDS_FILENAME __FILE__

/**
 * \def SDS_FUNCNAME
 * \brief Portable function name.
 */
#if SDS_COMPILER_MSC
#    define SDS_FUNCNAME __FUNCTION__
#else
#    define SDS_FUNCNAME __FUNC__
#endif

/**
 * \def SDS_PATH_SEPARATOR
 * \brief Platform specific path separator.
 */
/**
 * \def SDS_PATH_SEPARATOR_STR
 * \brief Platform specific path separator as a string.
 *
 * Allows for automatic string literal merging.
 * Ex: `"hello" SDS_PATH_SEPARATOR_STR "world"` => `"hello/world"`
 */
#if SDS_OS_WINDOWS
// NOTE(sdsmith): Latest Windows versions also support '/', although many older Windows API
// functions do not.
#    define SDS_PATH_SEPARATOR '\\'
#    define SDS_PATH_SEPARATOR_STR "\\"
#else
#    define SDS_PATH_SEPARATOR '/'
#    define SDS_PATH_SEPARATOR_STR "/"
#endif

/**
 * \def SDS_RESTRICT
 * \brief Portable `restrict` keyword.
 *
 * NOTE: Don't lie to your compiler!
 *
 * ref: https://en.wikipedia.org/wiki/Restrict
 */
#ifndef SDS_RESTRICT
#    if SDS_COMPILER_MSC
#        define SDS_RESTRICT __restrict
#    elif SDS_COMPILER_GCC || SDS_COMPILER_CLANG
#        define SDS_RESTRICT __restrict__
#    else
#        error Unsupported compiler
#    endif
#endif

/**
 * \def SDS_INTERNAL
 * \brief Set internal linkage on symbol.
 *
 * Replaces the use of 'static' to define internal linkage, because 'static' is a poor keyword
 * choice.
 */
#define SDS_INTERNAL static

/**
 * \def SDS_LIKELY
 * \brief Portable `likely` branch attribute for if statements. Does not work on case statements.
 *
 * Usage: `if SDS_LIKELY(...) { ... }`
 */
/**
 * \def SDS_UNLIKELY
 * \brief Portable `unlikely` branch attribute for if statements. Does not work on case statements.
 *
 * Usage: `if SDS_UNLIKELY(...) { ... }`
 */

#ifndef SDS_LIKELY
#    if SDS_CPLUSPLUS_20_SUPPORTED
// C++20 supports the attribute
#        define SDS_LIKELY(x) (x) [[likely]]
#    elif SDS_COMPILER == SDS_COMPILER_GCC || SDS_COMPILER == SDS_COMPILER_CLANG
#        define SDS_LIKELY(x) (__builtin_expect(!!(x), 1))
#    else
#        define SDS_LIKELY(x) (x)
#    endif
#endif

#ifndef SDS_UNLIKELY
#    if SDS_CPLUSPLUS_20_SUPPORTED
// C++20 supports the attribute
#        define SDS_UNLIKELY(x) (x) [[unlikely]]
#    elif SDS_COMPILER_GCC || SDS_COMPILER_CLANG
#        define SDS_UNLIKELY(x) (__builtin_expect(!!(x), 0))
#    else
#        define SDS_UNLIKELY(x) (x)
#    endif
#endif

// TODO(sdsmith): @cpp20: The c++20 attribute [[likely/unlikely]] has a different syntax for usage
// on ifs vs switch cases. It also differs from usage with the previous __builtin_expect. This means
// to support switch cases portably there would need to be a separate marco from
// SDS_LIKELY/SDS_UNLIKELY and it would need to be duplicated; once on the switch condition and once
// on the case. This is annoying and fragile. There is no easy way to enfor ce that this happens or
// that the attributes match (ie likely on the conditional and unlikely on a case statement would be
// problematic!). In lieu of this, I'm going to not make SDS_LIKELY_CASE for now. One day we could
// switch to C++20 and be done with it.

} // namespace sds

 #pragma once

/**
 * \file types.h
 * \author Stewart Smith (stewart.dryden.smith@gmail.com)
 * \brief Project agnostic stuff.
 *
 * Q: Why are feature defines set to zero when not supported? Isn't that extra work?
 * A: Yes, if we just define macros as `#define FOO` it simplifies the code here. However,
 *  it allows typos to go by silently. Forcing the interface to be `#if FOO` requires that
 *  FOO is defined, meaning it can't fail silently. This file follows the convention of
 *  defining 1 for feature enabled and 0 for not for this reason.
 */

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

/**
 * \def SDS_STR
 * \brief Convert macro value to a string.
 *
 * Ex usage:
 *  #define FOO somevalue
 *  #pragma message "Value: " SDS_STR(FOO)
 *      => "Value: somevalue"
 */
#define SDS_I_STR(X) #X
#define SDS_STR(X) SDS_I_STR(X)

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
/**
 * \def SDS_COMPILER_MINGW32
 * \brief Clang compiler compiling the project.
 *
 * Usage: `#if SDS_COMPILER_MINGW32`
 */
/**
 * \def SDS_COMPILER_MING64
 * \brief Clang compiler compiling the project.
 *
 * Usage: `#if SDS_COMPILER_MINGW64`
 */
#if defined(_MSC_VER)
#    define SDS_COMPILER_MSC 1
#elif defined(__clang__)
    // IMPORTANT: Always check clang before gcc.
    // There's a weird case with MingW's clang compliation, where it
    // defines both __clang__ and __GNUC__.
#   define SDS_COMPILER_CLANG 1
#elif defined(__GNUC__)
#    define SDS_COMPILER_GCC 1
#elif defined(__MINGW32__)
#    define SDS_COMPILER_MINGW32 1
#elif defined(__MINGW64__)
#    define SDS_COMPILER_MINGW64 1
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
#ifndef SDS_COMPILER_MINGW32
#    define SDS_COMPILER_MINGW32 0
#endif
#ifndef SDS_COMPILER_MINGW64
#    define SDS_COMPILER_MINGW64 0
#endif
SDS_STATIC_ASSERT_MSG(SDS_COMPILER_MSC + SDS_COMPILER_GCC + SDS_COMPILER_CLANG + SDS_COMPILER_MINGW32 + SDS_COMPILER_MINGW64 == 1,
                      "must have exactly one compiler set");

//
// CPU Architecture
//
// ref: https://sourceforge.net/p/predef/wiki/Architectures/
//
#if defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86)
// Intel x86
#   define SDS_ARCH_X86 1
#elif defined(__x86_64__) || defined(__x86_64__) || defined(_M_X64) || defined(__amd64) || defined(__amd64__) || defined(_M_AMD64)
#   define SDS_ARCH_AMD64 1
#else
#   error Unknown CPU architecture
#endif

#ifndef SDS_ARCH_X86
#   define SDS_ARCH_X86 0
#endif
#ifndef SDS_ARCH_AMD64
#   define SDS_ARCH_AMD64 0
#endif
SDS_STATIC_ASSERT_MSG(SDS_ARCH_X86 + SDS_ARCH_AMD64 == 1,
                      "must have exactly one architecture set");

/**
 * \def SDS_32_BIT
 * \brief Set if compiling in 32-bit.
 */
/**
 * \def SDS_64_BIT
 * \brief Set if compiling in 64-bit.
 */
#if SDS_OS_WINDOWS
#   if _WIN64
#       define SDS_64_BIT 1
#   else
#       define SDS_32_BIT 1
    #endif
#else
#   error Unsupported OS
#endif

#ifndef SDS_64_BIT
#   define SDS_64_BIT 0
#endif
#ifndef SDS_32_BIT
#   define SDS_32_BIT 0
#endif
SDS_STATIC_ASSERT_MSG(SDS_32_BIT + SDS_64_BIT == 1,
                      "must have exactly one set");

/**
 * \def SDS_CPP
 * \brief Portable replacement for \a __cplusplus.
 *
 * Microsoft does it again...
 */
#ifndef SDS_CPP
#    define SDS_CPP_11 201103L
#    define SDS_CPP_14 201402L
#    define SDS_CPP_17 201703L
#    define SDS_CPP_20 202002L

/**
 * \def SDS_CPP
 * \brief Portable replacement for \a __cplusplus.
 *
 * MSVC incorrectly reports __cplusplus
 * ref: https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
 */
#    if SDS_COMPILER_MSC
#        define SDS_CPP _MSVC_LANG
#    else
#        define SDS_CPP __cplusplus
#    endif

/**
 * \def SDS_CPP_11_SUPPORTED
 * \brief True if compiling with C++11 support.
 */
/**
 * \def SDS_CPP_14_SUPPORTED
 * \brief True if compiling with C++14 support.
 */
/**
 * \def SDS_CPP_17_SUPPORTED
 * \brief True if compiling with C++17 support.
 */
/**
 * \def SDS_CPP_20_SUPPORTED
 * \brief True if compiling with C++20 support.
 */
#    if SDS_CPP >= SDS_CPP_11
#        define SDS_CPP_11_SUPPORTED 1
#    endif
#    if SDS_CPP >= SDS_CPP_14
#        define SDS_CPP_14_SUPPORTED 1
#    endif
#    if SDS_CPP >= SDS_CPP_17
#        define SDS_CPP_17_SUPPORTED 1
#    endif
#    if SDS_CPP >= SDS_CPP_20
#        define SDS_CPP_20_SUPPORTED 1
#    endif

#    ifndef SDS_CPP_11_SUPPORTED
#        define SDS_CPP_11_SUPPORTED 0
#    endif
#    ifndef SDS_CPP_14_SUPPORTED
#        define SDS_CPP_14_SUPPORTED 0
#    endif
#    ifndef SDS_CPP_17_SUPPORTED
#        define SDS_CPP_17_SUPPORTED 0
#    endif
#    ifndef SDS_CPP_20_SUPPORTED
#        define SDS_CPP_20_SUPPORTED 0
#    endif
SDS_STATIC_ASSERT_MSG(SDS_CPP_11 + SDS_CPP_14 + SDS_CPP_17 + SDS_CPP_20 !=
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
#elif defined(SDS_COMPILER_CLANG)
#   define SDS_FUNCNAME __func__
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
#    elif defined(SDS_COMPILER_GCC) || defined(SDS_COMPILER_CLANG)
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
#    if SDS_CPP_20_SUPPORTED
// C++20 supports the attribute
#        define SDS_LIKELY(x) (x) [[likely]]
#    elif SDS_COMPILER_GCC || SDS_COMPILER_CLANG
#        define SDS_LIKELY(x) (__builtin_expect(!!(x), 1))
#    else
#        define SDS_LIKELY(x) (x)
#    endif
#endif

#ifndef SDS_UNLIKELY
#    if SDS_CPP_20_SUPPORTED
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

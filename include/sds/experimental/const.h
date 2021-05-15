#pragma once

/**
 * \def SDS_CONSTEXPR
 * \brief Define constexpr literals.
 *
 * ref: https://mpark.github.io/programming/2017/05/26/constexpr-function-parameters/
 */

#if 0 // TODO(sdsmith): C++ < 17
#define SDS_I_CONSTEXPR(...)                                                   \
  union {                                                                      \
    static constexpr auto value() { return __VA_ARGS__; }                      \
  }

#define SDS_CONSTEXPR(...) \
    [] { using R = SDS_I_CONSTEXPR(__VA_ARGS__); return R{}; }()
#endif

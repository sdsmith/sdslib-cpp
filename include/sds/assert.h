#pragma once


/**
 * \def STATIC_ASSERT(expr)
 * \brief Static assertion.
 *
 * \param expr Expression.
 */

/**
 * \def STATIC_ASSERT_MSG(expr, msg)
 * \brief Static assertion with a custom message on failure.
 *
 * \param expr Expression.
 * \param msg Error message.
 */
#define _ASSERT_GLUE(a, b) a##b
#define ASSERT_GLUE(a, b) _ASSERT_GLUE(a, b)

#ifdef __cplusplus
#   if __cplusplus >= 201103L // c++11
#       define STATIC_ASSERT(expr)                              \
           static_assert(expr, "static assert failed:" #expr)
#       define STATIC_ASSERT_MSG(expr, msg) static_assert(expr, msg);
#   else
        // no static_assert prior to c++11
        template <bool> class Static_Assert;
        template <> class Static_Assert<true> {};
#       define STATIC_ASSERT(expr)                                      \
           enum {                                                       \
               ASSERT_GLUE(g_assert_fail_, __LINE__) = sizeof(Static_Assert<!!(expr)>) \
           }
#       define STATIC_ASSERT_MSG(expr, msg) STATIC_ASSERT(expr)
#   endif
#endif

#undef ASSERT_GLUE
#undef _ASSERT_GLUE

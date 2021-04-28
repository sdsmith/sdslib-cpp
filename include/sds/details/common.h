#pragma once

#include "sds/config.h"

#ifndef SDS_ASSERT
#    include <cassert>
/**
 * \def SDS_ASSERT
 * \brief Assert used by the library. Defaults to using cassert. Define \a
 * SDS_ASSERT to use a custom assertion.
 */
#    define SDS_ASSERT(x) assert(x)
#endif

#ifndef SDS_STATIC_ASSERT
/**
 * \def SDS_STATIC_ASSERT
 * \brief Assert used by the library. Defaults to \a static_assert.
 */
#    define SDS_STATIC_ASSERT(x) static_assert(x)
#    define SDS_STATIC_ASSERT_MSG(x, msg) static_assert(x, msg)
#endif

#include "sds/details/types.h"

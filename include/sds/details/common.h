#pragma once

#ifndef SDS_ASSERT
#   include <cassert>
/**
 * \def SDS_ASSERT
 * \brief Assert used by the library. Defualts to using cassert. Define \a
 * SDS_ASSERT to use a custom assertion.
 */
#   define SDS_ASSERT(x) assert(x)
#endif

#include "sds/details/types.h"

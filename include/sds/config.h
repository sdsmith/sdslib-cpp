#pragma once

/**
 * \file config.h
 * \brief Library configuration. Set these defines defore including the library to configure its
 * behaviour.
 */

/**
 * \def SDS_USE_RTTI_FEATURES
 * \brief Enable runtime type info (RTTI) dependent parts of the library.
 *
 * Expensive. Off by default.
 */
#ifndef SDS_USE_RTTI_FEATURES
#    define SDS_USE_RTTI_FEATURES 0
#else
#    define SDS_USE_RTTI_FEATURES 1
#endif

/**
 * \def SDS_USE_COMPILER_BUILTINS
 * \brief Use compiler builtins where available instead of hand rolled implementations.
 */
#ifndef SDS_USE_COMPILER_BUILTINS
#    define SDS_USE_COMPILER_BUILTINS 1
#else
#    define SDS_USE_COMPILER_BUILTINS 0
#endif

/**
 * \def SDS_INCLUDE_SLT_FEATURES
 * \brief Include features that would only be relevant if using the C++ Standard Library and are not
 * required by the library implementation.
 *
 * Disable if not using the standard library or the standard template library.
 */
#ifndef SDS_INCLUDE_STL_FEATURES
#    define SDS_INCLUDE_STL_FEATURES 1
#else
#    define SDS_INCLUDE_STL_FEATURES 0
#endif

/**
 * \def SDS_USE_STD_ITERATOR_CATEGORIES
 * \brief Use C++ standard library iterator categories.
 */
#ifndef SDS_USE_STD_ITERATOR_CATEGORIES
#    define SDS_USE_STD_ITERATOR_CATEGORIES 1
#else
#    define SDS_USE_STD_ITERATOR_CATEGORIES 0
#endif

/**
 * \def SDS_ASSERT
 * \brief Assert used by the library. Defaults to using cassert. Define \a
 * SDS_ASSERT to use a custom assertion.
 */
#ifndef SDS_ASSERT
#    include <cassert>
#    define SDS_ASSERT(x) assert(x)
#endif

/**
 * \def SDS_FORWARD_TYPES_TO_NAMESPACE
 * \brief Alias sds::* type declarations to the given namespace if set.
 */
#ifndef SDS_FORWARD_TYPES_TO_NAMESPACE
#endif

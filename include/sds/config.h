#pragma once

/**
 * \file config.h
 * \brief Library configuration.
 */

/**
 * \def SDS_USE_RTTI_FEATURES
 * \brief Enable runtime type info (RTTI) dependent parts of the library.
 *
 * Expensive. Off by default.
 */
#ifndef SDS_USE_RTTI_FEATURES
#    define SDS_USE_RTTI_FEATURES 0
#endif

/**
 * \def SDS_USE_COMPILER_BUILTINS
 * \brief Use compiler builtins where available instead of hand rolled implementations.
 */
#ifndef SDS_USE_COMPILER_BUILTINS
#    define SDS_USE_COMPILER_BUILTINS 1
#endif

/**
 * \def SDS_INCLUDE_CPPSTD_FEATURES
 * \brief Include features that would only be relevant if using the C++ Standard Library and are not
 * required by the library implementation.
 *
 * Disable if not using the standard library or the standard template library.
 */
#ifndef SDS_INCLUDE_CPPSTD_FEATURES
#    define SDS_INCLUDE_CPPSTD_FEATURES 1
#endif

/**
 * \def SDS_USE_STD_ITERATOR_CATEGORIES
 * \brief Use C++ standard library iterator categories.
 */
#ifndef SDS_USE_STD_ITERATOR_CATEGORIES
#    define SDS_USE_STD_ITERATOR_CATEGORIES 0
#endif

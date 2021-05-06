/*
 * \file config.h
 * \brief Library configuration.
 */

#ifndef SDS_USE_RTTI_FEATURES
/*
 * \def SDS_USE_RTTI_FEATURES
 * \brief Enable runtime type info (RTTI) dependent parts of the library.
 *
 * Expensive. Off by default.
 */
#define SDS_USE_RTTI_FEATURES 0
#endif

#ifndef SDS_USE_COMPILER_BUILTINS
/*
 * \def SDS_USE_COMPILER_BUILTINS
 * \brief Use compiler builtins where available instead of hand rolled implementations.
 */
#define SDS_USE_COMPILER_BUILTINS 1
#endif

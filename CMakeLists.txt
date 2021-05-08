cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(sdslib CXX)

message(STATUS "Build sdslib")

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# ---------------------------------------------------------------------------------------
# Set SDSLIB_MASTER_PROJECT to ON if we are building sdslib
# ---------------------------------------------------------------------------------------
# Check if sdslib is being used directly or via add_subdirectory, but allow overriding
if (NOT DEFINED SDSLIB_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(SDSLIB_MASTER_PROJECT ON)
    else()
        set(SDSLIB_MASTER_PROJECT OFF)
    endif()
endif()


# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
if (CMAKE_SYSTEM_NAME MATCHES "CYGWIN" OR CMAKE_SYSTEM_NAME MATCHES "MSYS")
    set(CMAKE_CXX_EXTENSIONS ON)
endif()

if (SDSLIB_MASTER_PROJECT EQUAL ON)
    set(cxx_flags "")
    set(cxx_flags_debug "")
    set(cxx_flags_release "")
    set(exe_linker_flags "")
    set(exe_linker_flags_debug "")
    set(exe_linker_flags_release "")

    if (NOT MSVC)
        string(CONCAT cxx_flags
            "-fPIC "
            "-Werror "
            "-Wall "
            "-Wextra "
            "-Weffc++ "
            "-Wfloat-equal "
            "-Wpointer-arith "
            "-Wcast-align "
            "-Wstrict-overflow=4 "
            "-Wwrite-strings "
            "-Wno-unused-result "

            # Remove noisy warnings
            "-Wcast-qual "
            "-Wswitch-default "
            "-Wno-switch-enum "
            "-Wconversion "
            "-Wunreachable-code "
            )

        string(CONCAT cxx_flags_debug
            "-gdwarf "
        )

        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            # Add code coverage
            string(CONCAT cxx_flags_debug
                ${cxx_flags_debug}
                "-fprofile-instr-generate "
                "-fcoverage-mapping "
            )
        endif()
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_flags}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${cxx_flags_debug}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${cxx_flags_release}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${exe_linker_flags}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${exe_linker_flags_debug}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${exe_linker_flags_release}")
endif()

option(SDSLIB_BUILD_TESTS "Build tests" ${SDSLIB_MASTER_PROJECT})

message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# ---------------------------------------------------------------------------------------
# Static library
# ---------------------------------------------------------------------------------------
set(SDSLIB_HEADERS
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/array.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/array/carray.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/array/dynamic_array.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/array/make_array.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/bit.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/bitfield.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/comparison.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/config.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/details/common.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/intrinsics.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/list/linked_list.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/lockless.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/sds/string.h"
)

set(SDSLIB_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/src/bit.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/lockless.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/string.cpp"
)

add_library(sdslib STATIC ${SDSLIB_HEADERS} ${SDSLIB_SOURCES})
target_include_directories(sdslib PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
set_target_properties(sdslib PROPERTIES DEBUG_POSTFIX d)

# ---------------------------------------------------------------------------------------
# Build binaries
# ---------------------------------------------------------------------------------------
if (SDSLIB_BUILD_TESTS)
    message(STATUS "Generating tests")
    enable_testing()
    add_subdirectory(test)
endif()

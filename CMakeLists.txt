cmake_minimum_required(VERSION 3.16)
project(sdslib)

## Googletest
#
# ref: https://github.com/google/googletest/blob/master/googletest/README.md

# Download and unpack googletest at configure time
configure_file(cmake/CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
  EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

## Compiler config
#

# Generic flags
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO(Sdsmith): gtest doesn't play well with -Wundef, -Wswitch-enum
set(GENERIC_CXX_FLAGS "-fPIC -Werror -Wall -Wextra -Weffc++ -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-overflow=4 -Wwrite-strings -Wno-unused-result")
# Remove noisy warnings
set(GENERIC_CXX_FLAGS "${GENERIC_CXX_FLAGS} -Wcast-qual -Wswitch-default -Wno-switch-enum -Wconversion -Wunreachable-code")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  # Include c-preprocessor debug info
  set(GENERIC_CXX_FLAGS "${GENERIC_CXX_FLAGS} -gdwarf")

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Add code coverage
    set(GENERIC_CXX_FLAGS "${GENERIC_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
  endif()
endif()

# Compiler specific flags
#
set(CLANG_CXX_FLAGS "${GENERIC_CXX_FLAGS}")

set(GCC_CXX_FLAGS "${GENERIC_CXX_FLAGS}")

# Finalize flags
#
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")
endif()


## Source
#

set(HEADERS
  ${CMAKE_CURRENT_LIST_DIR}/src/carray.h
  ${CMAKE_CURRENT_LIST_DIR}/src/dynamic_array.h
  ${CMAKE_CURRENT_LIST_DIR}/src/make_array.h
  ${CMAKE_CURRENT_LIST_DIR}/src/list/linked_list.h
)

set(SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/src/stub.cpp # TODO(sdsmith): temp until there is actually a source file.
)

set(TEST_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/test/carray_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/test/dynamic_array_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/test/list/linked_list_test.cpp
)

## Create static library
#

foreach(header ${HEADERS})
  file(RELATIVE_PATH _header_path "${CMAKE_CURRENT_LIST_DIR}/inc" "${header}")
  configure_file("${header}" "${PROJECT_BINARY_DIR}/out/inc/${_header_path}" COPYONLY)
endforeach()
add_library(sdslib STATIC ${SOURCES})

## Create test executable
#

message(STATUS "CMAKE_BUILD_TYPE:      ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_FLAGS:       ${GCC_CXX_FLAGS}")

add_executable(sdslib_test ${TEST_SOURCES} ${SOURCES})
target_link_libraries(sdslib_test gtest_main)
target_include_directories(sdslib_test PUBLIC src)
target_compile_definitions(sdslib_test PUBLIC WITH_GTEST)
enable_testing()
add_test(NAME sdslib_test COMMAND sdslib_test)

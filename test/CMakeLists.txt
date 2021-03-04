cmake_minimum_required(VERSION 3.15)
project(sdslib_test CXX)

# ---------------------------------------------------------------------------------------
# Googletest setup
# ---------------------------------------------------------------------------------------
# ref: https://github.com/google/googletest/blob/master/googletest/README.md

# Download and unpack googletest at configure time
configure_file(cmake/CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if (result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if (result)
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

# ---------------------------------------------------------------------------------------
# Build binaries
# ---------------------------------------------------------------------------------------
set(SDSLIB_TEST_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/array/carray_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/array/dynamic_array_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/array/make_array_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/list/linked_list_test.cpp
)

enable_testing()

add_executable(sdslib_test ${SDSLIB_TEST_SOURCES})
target_link_libraries(sdslib_test PRIVATE sdslib gtest_main)
target_include_directories(sdslib_test PUBLIC src)
target_compile_definitions(sdslib_test PUBLIC WITH_GTEST)
add_test(NAME sdslib_test COMMAND sdlib_test)

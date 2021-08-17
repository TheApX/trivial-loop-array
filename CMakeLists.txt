cmake_minimum_required(VERSION 3.14)

project(trivial_loop_array)

set(CMAKE_CXX_STANDARD 20)

# Suppress long and annoying warning.
set(GCC_NO_PSABI_WARNING "-Wno-psabi")
add_definitions(${GCC_NO_PSABI_WARNING})

# Main library: trivial_loop_array
set(trivial_loop_array_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/trivial_loop_array.hpp
)
add_library(trivial_loop_array STATIC
    ${trivial_loop_array_SRCS}
)
set_target_properties(trivial_loop_array PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(trivial_loop_array PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Fetch googletest
find_package(Threads REQUIRED)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/master.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Tests
enable_testing()
set(trivial_loop_array_test_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/test/trivial_loop_array_test.cpp
)
add_executable(trivial_loop_array_test ${trivial_loop_array_test_SRCS})
target_link_libraries(trivial_loop_array_test
    trivial_loop_array
    gtest_main
    gmock_main
)
add_test(NAME trivial_loop_array_test
         COMMAND trivial_loop_array_test)

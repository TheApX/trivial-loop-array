#include <trivial_loop_array.hpp>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

static_assert(std::is_trivial<theapx::trivial_loop_array<int, 10>>::value);

using ::testing::Eq;
using ::testing::IsTrue;

TEST(TrivialLoopArray, EmptyInitializer) {
  theapx::trivial_loop_array<int, 10> array{};

  EXPECT_THAT(array.size(), Eq(0));
  EXPECT_THAT(array.empty(), IsTrue());
}

TEST(TrivialLoopArray, DesignatedInitializer) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  EXPECT_THAT(array.size(), Eq(3));
  EXPECT_THAT(array[0], Eq(1));
  EXPECT_THAT(array[1], Eq(2));
  EXPECT_THAT(array[2], Eq(3));
}

TEST(TrivialLoopArray, WrapsAround) {
  theapx::trivial_loop_array<int, 6> array{
      .data = {4, 0, 0, 1, 2, 3},
      .start_index = 3,
      .array_size = 4,
  };

  EXPECT_THAT(array.size(), Eq(4));
  EXPECT_THAT(array[0], Eq(1));
  EXPECT_THAT(array[1], Eq(2));
  EXPECT_THAT(array[2], Eq(3));
  EXPECT_THAT(array[3], Eq(4));
}

TEST(TrivialLoopArray, ResizeUp) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  array.resize(5);

  EXPECT_THAT(array.size(), Eq(5));
  EXPECT_THAT(array[0], Eq(1));
  EXPECT_THAT(array[1], Eq(2));
  EXPECT_THAT(array[2], Eq(3));
  EXPECT_THAT(array[3], Eq(0));
  EXPECT_THAT(array[4], Eq(0));
}

TEST(TrivialLoopArray, ResizeDown) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  array.resize(2);

  EXPECT_THAT(array.size(), Eq(2));
  EXPECT_THAT(array[0], Eq(1));
  EXPECT_THAT(array[1], Eq(2));
}

TEST(TrivialLoopArray, RangeLoop) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  int n = 1;
  for (int& v : array) {
    EXPECT_THAT(v, Eq(n));
    ++n;
  }
}

TEST(TrivialLoopArray, RangeLoopConst) {
  theapx::trivial_loop_array<int, 10> mutable_array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  const theapx::trivial_loop_array<int, 10>& array = mutable_array;

  int n = 1;
  for (const int& v : array) {
    EXPECT_THAT(v, Eq(n));
    ++n;
  }
}

TEST(TrivialLoopArray, PushBack) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  array.push_back(100);

  EXPECT_THAT(array.size(), Eq(4));
  EXPECT_THAT(array[0], Eq(1));
  EXPECT_THAT(array[1], Eq(2));
  EXPECT_THAT(array[2], Eq(3));
  EXPECT_THAT(array[3], Eq(100));
}

TEST(TrivialLoopArray, PushBackWrapsAround) {
  theapx::trivial_loop_array<int, 5> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  array.push_back(100);

  EXPECT_THAT(array.size(), Eq(4));
  EXPECT_THAT(array[0], Eq(1));
  EXPECT_THAT(array[1], Eq(2));
  EXPECT_THAT(array[2], Eq(3));
  EXPECT_THAT(array[3], Eq(100));
}

TEST(TrivialLoopArray, PushFront) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  array.push_front(100);

  EXPECT_THAT(array.size(), Eq(4));
  EXPECT_THAT(array[0], Eq(100));
  EXPECT_THAT(array[1], Eq(1));
  EXPECT_THAT(array[2], Eq(2));
  EXPECT_THAT(array[3], Eq(3));

  EXPECT_THAT(array.start_index, Eq(1));
}

TEST(TrivialLoopArray, PushFrontWrapsAround) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {1, 2, 3},
      .start_index = 0,
      .array_size = 3,
  };

  array.push_front(100);

  EXPECT_THAT(array.size(), Eq(4));
  EXPECT_THAT(array[0], Eq(100));
  EXPECT_THAT(array[1], Eq(1));
  EXPECT_THAT(array[2], Eq(2));
  EXPECT_THAT(array[3], Eq(3));

  EXPECT_THAT(array.start_index, Eq(array.capacity() - 1));
}

TEST(TrivialLoopArray, FrontBack) {
  theapx::trivial_loop_array<int, 10> mutable_array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  const theapx::trivial_loop_array<int, 10>& const_array = mutable_array;

  EXPECT_THAT(mutable_array.front(), Eq(1));
  EXPECT_THAT(mutable_array.back(), Eq(3));
  EXPECT_THAT(const_array.front(), Eq(1));
  EXPECT_THAT(const_array.back(), Eq(3));
}

TEST(TrivialLoopArray, ElemetsAreArrayMatcher) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  EXPECT_THAT(array, testing::ElementsAreArray(array));
}

TEST(TrivialLoopArray, Create) {
  theapx::trivial_loop_array<int, 10> array{
      .data = {0, 0, 1, 2, 3},
      .start_index = 2,
      .array_size = 3,
  };

  auto test_array = theapx::trivial_loop_array<int, 10>::Create({1, 2, 3});
  EXPECT_THAT(test_array, testing::ElementsAreArray(array));
}

}  // namespace

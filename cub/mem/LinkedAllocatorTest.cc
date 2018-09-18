#include <cub/algo/loop.h>
#include <cub/mem/LinkedAllocator.h>
#include <cub/repo/array/ArraySize.h>
#include <cut/cut.hpp>

USING_CUM_NS
USING_CUB_NS

namespace {
  int array[] = { 0, 1, 2, 3, 4 };
}

FIXTURE(LinkedAllocatorTest) {
  LinkedAllocatorTest()
      : allocator(array, MAX_ALLOC_NUM) {
  }

  TEST("should has free slot num when init") {
    ASSERT_THAT(allocator.isBusyListEmpty(), be_true());
    ASSERT_THAT(allocator.isFreeListEmpty(), be_false());
  }

  TEST("should alloc ok when has free slots") {
    FOREACH(i, (int)MAX_ALLOC_NUM) {
      auto x = allocator.alloc();
      ASSERT_THAT(x != nullptr, be_true());
      ASSERT_THAT(*x, eq(i));
    }

    auto x = allocator.alloc();
    ASSERT_THAT(x == nullptr, be_true());
  }

  TEST("should alloc ok when free slots") {
    FOREACH(i, MAX_ALLOC_NUM - 1) {
      allocator.alloc();
    }

    auto x = allocator.alloc();
    ASSERT_THAT(x != nullptr, be_true());

    auto y = allocator.alloc();
    ASSERT_THAT(y == nullptr, be_true());

    ASSERT_THAT(allocator.dealloc(*x), eq(CUB_SUCCESS));

    y = allocator.alloc();
    ASSERT_THAT(y != nullptr, be_true());
    ASSERT_THAT(*y, eq(2));
  }

  enum { MAX_ALLOC_NUM = 3 };
  LinkedAllocator<int, ARR_SIZE(array)> allocator;
};


#include <cut/cut.hpp>
#include <cub/algo/search.h>
#include <cub/repo/array/ArraySize.h>

USING_CUM_NS
USING_CUB_NS

namespace {
  int a[] = { 1, 3, 5, 7 };
}

FIXTURE(BinarySearchTest) {
  TEST("should find correct when key is in the middle position") {
    int *r = binary_search(std::begin(a), std::end(a), 5);
    ASSERT_THAT(*r, eq(5));
  }

  TEST("should return the end when key not find") {
    ASSERT_THAT(binary_search(std::begin(a), std::end(a), 6), eq(std::end(a)));
  }
};

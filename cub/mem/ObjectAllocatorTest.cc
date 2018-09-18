#include <cut/cut.hpp>
#include <cub/mem/ObjectAllocator.h>
#include <cub/algo/loop.h>

USING_CUM_NS
USING_CUB_NS

namespace {
  struct Foo {
    Foo(int a) : x(a) {
    }

    int getValue() const {
      return x;
    }

    DECL_OPERATOR_NEW();

  private:
    int x;
  };

  const uint16_t MAX_SLOT_NUM = 2;
}

DEF_OBJ_ALLOCATOR(Foo, MAX_SLOT_NUM);

FIXTURE(ObjectAllocatorTest) {
  TEST("should have correct free slot when init") {
    ASSERT_THAT(FooAllocator.getFreeSlotNum(), eq(MAX_SLOT_NUM));
  }

  TEST("should alloc OK when has free slot") {
    Foo* foo = new Foo(3);
    ASSERT_THAT(foo != nullptr, be_true());
    ASSERT_THAT(FooAllocator.getFreeSlotNum(), eq(MAX_SLOT_NUM - 1));

    delete foo;
    ASSERT_THAT(FooAllocator.getFreeSlotNum(), eq(MAX_SLOT_NUM));
  }

  TEST("should not alloc when has no free slot") {
    Foo* foos[MAX_SLOT_NUM] = { nullptr };

    FOREACH(i, MAX_SLOT_NUM) {
      foos[i] = new Foo(0);
    }

    Foo* foo = new Foo(0);
    ASSERT_THAT(foo == nullptr, be_true());

    FOREACH(i, MAX_SLOT_NUM) {
      ASSERT_THAT(foos[i] != nullptr, be_true());
      delete foos[i];
    }

    foo = new Foo(0);
    ASSERT_THAT(foo != nullptr, be_true());
    delete foo;
  }
};


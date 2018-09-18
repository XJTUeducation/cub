#include <cut/cut.hpp>
#include <cub/repo/number/RingNumber.h>

USING_CUM_NS
USING_CUB_NS

FIXTURE(RingNumberTest) {
  TEST("should compare equal when ring number init equal") {
    RingNumber<uint8_t, 10> r1(1);
    RingNumber<uint8_t, 10> r2(11);

    ASSERT_THAT(r1 == r2, be_true());
  }

  TEST("should compare unequal when ring number init unequal") {
    RingNumber<uint8_t, 10> r1(2);
    RingNumber<uint8_t, 10> r2(11);

    ASSERT_THAT(r1 == r2, be_false());
  }

  TEST("should compare equal when ring number add to equal") {
    RingNumber<uint8_t, 10> r1(2);
    RingNumber<uint8_t, 10> r2(11);

    ASSERT_THAT(r1 == (++r2), be_true());
  }

  TEST("should compare equal when ring number move to equal") {
    RingNumber<uint8_t, 10> r1(4);
    RingNumber<uint8_t, 10> r2(11);

    ASSERT_THAT((r1 << 2) == (r2 >> 11), be_true());
  }
};

#include <cut/cut.hpp>
#include <cub/network/ByteOrder.h>

USING_CUM_NS
USING_CUB_NS

FIXTURE(ByteOrder) {
  TEST("network order of S8 should be equal to the original one") {
    int8_t v = -2;

    ASSERT_THAT(hton(v), eq(v));
  }

  TEST("network order of uint8_t should be equal to the original one") {
    uint8_t v = 2;

    ASSERT_THAT(hton(v), eq(v));
  }

  TEST("network order of S16 should be equal to network order") {
    int16_t v = -274;

    ASSERT_THAT(ntohs(hton(v)), eq(v));
  }

  TEST("network order of uint16_t should be equal to network order") {
    uint16_t v = 274;

    ASSERT_THAT(ntohs(hton(v)), eq(v));
  }

  TEST("network order of S32 should be equal to network order") {
    int32_t v = -274932234;

    ASSERT_THAT(ntohl(hton(v)), eq(v));
  }

  TEST("network order of uint32_t should be equal to network order") {
    uint32_t v = 274343432;

    ASSERT_THAT(ntohl(hton(v)), eq(v));
  }
};

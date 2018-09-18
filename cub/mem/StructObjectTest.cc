#include <cut/cut.hpp>
#include <cub/mem/StructObject.h>

USING_CUM_NS
USING_CUB_NS

namespace {
  struct Msg {
    uint32_t id;
    uint32_t transNum;
  };
}

FIXTURE(StructObjectTest) {

  TEST("should be zero when init") {
    StructObject<Msg> msg;
    ASSERT_THAT(msg.id, eq(0));
    ASSERT_THAT(msg.transNum, eq(0));

    memset(&msg, 0xff, sizeof(msg));
    ASSERT_THAT(msg.id, eq(0xffffffff));
    ASSERT_THAT(msg.transNum, eq(0xffffffff));
  }

  TEST("should be used as pointer") {
    StructObject<Msg> msg;
    ASSERT_THAT(msg->id, eq(0));
    ASSERT_THAT(msg->transNum, eq(0));

    memset(msg.getPointer(), 0x0ff, sizeof(msg));
    ASSERT_THAT((*msg).id, eq(0xffffffff));
    ASSERT_THAT((*msg).transNum, eq(0xffffffff));
  }
};

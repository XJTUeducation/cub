#include <cut/cut.hpp>
#include <cub/mem/AutoMsg.h>

USING_CUM_NS
USING_CUB_NS

namespace {
  struct LargeMsg {
    uint8_t msgBuff[16 * 1024];
  };
}

FIXTURE(AutoMsgTest) {
  TEST("should alloc auto msg not in stack") {
    AutoMsg<LargeMsg> msg;
  }
};


#include <cut/cut.hpp>
#include <cub/mem/StructWrapper.h>

USING_CUM_NS
USING_CUB_NS

namespace {
  struct PlainMsg {
    uint32_t id;
    uint32_t transNum;
  };

  STRUCT_WRAPPER(DomainEvent, PlainMsg) {
    enum {
      MIN_ID = 0, MAX_ID = 20,
    };

    bool isValid() const {
      return (MIN_ID <= id) && (id <= MAX_ID);
    }
  };
}

FIXTURE(StructWrapperTest) {
  TEST("should use the method when wrappered") {
    PlainMsg msg { 2, 4 };

    DomainEvent& event = DomainEvent::by(msg);

    ASSERT_THAT(event.isValid(), be_true());
    ASSERT_THAT(event.transNum, eq(4));
  }
};


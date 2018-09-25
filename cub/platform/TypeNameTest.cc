#include "cub/platform/TypeName.h"
#include <cut/cut.hpp>

USING_CUT_NS
USING_CUM_NS

CUB_NS_BEGIN

using namespace std::string_literals;

FIXTURE(TypeNameTest) {
  TEST("primitive type") {
    ASSERT_EQ("int"s, TypeName<int>::value());
  }

  struct Dummy {};

  TEST("self-defined type") {
    ASSERT_EQ("cub::TypeNameTest::Dummy"s, TypeName<Dummy>::value());
  }
};

CUB_NS_END

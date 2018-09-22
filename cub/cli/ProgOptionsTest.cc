#include "cub/cli/ProgOptions.h"
#include <cut/cut.hpp>

USING_CUT_NS
USING_CUM_NS

CUB_NS_BEGIN

FIXTURE(ProgOptionsTest) {
  int v1 = 10;
  float v2 = 12.0;
  bool v3 = false;
  std::string v4 = "v4";

  ProgOptions options {{
      option("v1", &v1, "integer option"),
      option("v2", &v2, "float option"),
      option("v3", &v3, "boolean option"),
      option("v4", &v4, "string option"),
  }};

  TEST("usage") {
    std::string text;
    options.usage("prog", text);
    std::cout << text << std::endl;
  }

  TEST("parse ok") {
    const char* argv[] = {
        "prog",
        "--v1=20",
        "--v2=20.0",
        "--v3=1",
        "--v4=something"
    };
    ASSERT_TRUE(options.parse(5, argv));
    ASSERT_THAT(v1, eq(20));
    ASSERT_THAT(v2, close_to(20.0, 0.001));
    ASSERT_TRUE(v3);
    ASSERT_THAT("something", eq(v4));
  }

  TEST("parse failed: invalid int") {
    const char* argv[] = {
        "prog",
        "--v1=str",
        "--v2=20.0",
        "--v3=1",
        "--v4=something"
    };
    ASSERT_FALSE(options.parse(5, argv));
  }

  TEST("parse failed: invalid float") {
    const char* argv[] = {
        "prog",
        "--v1=20",
        "--v2=str",
        "--v3=1",
        "--v4=something"
    };
    ASSERT_FALSE(options.parse(5, argv));
  }

  TEST("parse failed: invalid bool") {
    const char* argv[] = {
        "prog",
        "--v1=20",
        "--v2=20.0",
        "--v3=true",  // true -> 1, false -> 0
        "--v4=something"
    };
    ASSERT_FALSE(options.parse(5, argv));
    ASSERT_THAT(v1, eq(20));
    ASSERT_THAT(v2, close_to(20.0, 0.001));
  }
};

CUB_NS_END

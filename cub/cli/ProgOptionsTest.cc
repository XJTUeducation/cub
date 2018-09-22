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
};

CUB_NS_END

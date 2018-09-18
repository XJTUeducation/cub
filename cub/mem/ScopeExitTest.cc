#include <cut/cut.hpp>
#include <cub/mem/ScopeExit.h>

USING_CUM_NS

FIXTURE(ScopeExitTest) {
  TEST("should_execute_the_code_when_exit_scope") {
    bool exited = false;
    {
      SCOPE_EXIT([&exited]() {exited = true;});
    }
    ASSERT_THAT(exited, be_true());
  }
};

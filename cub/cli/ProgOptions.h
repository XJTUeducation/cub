#ifndef H587CE253_444D_4E03_98D9_EA686D11D883
#define H587CE253_444D_4E03_98D9_EA686D11D883

#include <vector>
#include "cub/cli/Option.h"

CUB_NS_BEGIN

struct ProgOptions {
  ProgOptions(std::vector<Option*>&& options);
  ~ProgOptions();

  bool parse(int argc, const char** argv);
  void usage(const std::string& prog, std::string&) const;

private:
  std::vector<Option*> options;
};

CUB_NS_END

#endif

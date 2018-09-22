#include "cub/cli/ProgOptions.h"
#include "cub/string/StringPrintf.h"
#include "cub/algo/loop.h"

CUB_NS_BEGIN

ProgOptions::ProgOptions(std::vector<Option*>&& options)
  : options(std::move(options)) {}

ProgOptions::~ProgOptions() {
  foreach([](auto option){
    delete option;
  });
}

bool ProgOptions::parse(int* argc, char** argv) {
  FOREACH_FROM(i, 1, int(*argc)) {
    for (auto option : options)
      if (!option->parse(argv[i]))
        return false;
  }
  return true;
}

void ProgOptions::usage(const std::string& prog, std::string& text) const {
  text.append("usage: ").append(prog).append("\nFlags:\n");
  foreach([&text](auto option) {
    text += option->help();
  });
}

CUB_NS_END

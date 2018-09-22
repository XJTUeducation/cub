#include "cub/cli/ProgOptions.h"
#include "cub/string/StringPrintf.h"
#include "cub/algo/loop.h"
#include "cub/algo/range.h"
#include <iostream>

CUB_NS_BEGIN

ProgOptions::ProgOptions(std::vector<Option*>&& options)
  : options(std::move(options)) {}

ProgOptions::~ProgOptions() {
  each(options, [](auto option){
    delete option;
  });
}

bool ProgOptions::parse(int argc, const char** argv) {
  for(auto i=1; i!=argc; ++i) {
    auto failed = [i, argv](auto option) {
      return !option->parse(argv[i]);
    };
    if (all_of(options, failed)) {
      std::cerr << "Unknown option: " << argv[i] << std::endl;
      return false;
    }
  }
  return true;
}

void ProgOptions::usage(const std::string& prog, std::string& text) const {
  text.append("usage: ").append(prog).append("\nFlags:\n");
  each(options, [&text](auto option) {
    text += option->help();
  });
}

CUB_NS_END

#ifndef HCDEB97D2_E132_493B_85E9_A57AA728C6E2
#define HCDEB97D2_E132_493B_85E9_A57AA728C6E2

#include "cub/cub.h"

CUB_NS_BEGIN

struct LibraryLoader {
  explicit LibraryLoader(const char* name);
  void* getSymbol(const char* symbol) const;

private:
  void* handle = nullptr;
};

CUB_NS_END

#endif

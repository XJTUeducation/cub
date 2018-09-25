#include "cub/platform/LibraryLoader.h"
#include <dlfcn.h>

CUB_NS_BEGIN

LibraryLoader::LibraryLoader(const char* filename)
  : handle(dlopen(filename, RTLD_NOW | RTLD_LOCAL)) {}

void* LibraryLoader::getSymbol(const char* symbol) const {
  if (handle == nullptr) {
    return nullptr;
  } else if (auto result = dlsym(handle, symbol)) {
    return result;
  } else {
    return nullptr;
  }
}

CUB_NS_END

#include "cub/platform/TypeName.h"
#include <memory>
#include <cxxabi.h>
#include <cstdlib>

CUB_NS_BEGIN

std::string demangle(const std::type_info& info) {
  auto status = 0;
  std::unique_ptr<char, void (*)(void*)> res {
    abi::__cxa_demangle(info.name(), nullptr, nullptr, &status), std::free };
  return status == 0 ? res.get() : info.name();
}

CUB_NS_END

#include "cub/cli/Option.h"
#include "cub/string/StringPrintf.h"
#include "cub/string/StringUtils.h"
#include "cub/string/Scanner.h"
#include <stdio.h>
#include <functional>

CUB_NS_BEGIN

namespace {
  using Saver = std::function<bool(StringView)>;

  struct OptionImpl : Option {
    OptionImpl(const char* name, Saver saver, std::string&& usage)
      : name(name), saver(saver), usage(std::move(usage)) {
    }

  private:
    const std::string& help() const override {
      return usage;
    }

    bool parse(StringView arg) override {
      StringView rhs;
      return Scanner(arg)
          .literal("--")
          .literal(name)
          .literal("=")
          .result(&rhs) && saver(rhs);
    }

  private:
    const char* name;
    Saver saver;
    std::string usage;
  };

  template <typename T, typename Parsed = T>
  Saver saver(T* dst, const char* format) {
    return [dst, format](StringView value) {
      Parsed parsed;
      char extra;
      if (sscanf(value.data(), format, &parsed, &extra) == 1) {
        *dst = parsed;
        return true;
      }
      return false;
    };
  }

  template <typename T>
  std::string format(T* t) {
    return std::to_string(*t);
  }

  template <>
  std::string format(std::string* t) {
    using namespace std::string_literals;
    return "\""s += *t += "\""s;
  }

  std::string help(const char* name, const char* type,
      const std::string& init, const std::string& usage) {
    auto flag = stringprintf("--%s=%s", name, init.c_str());
    return stringprintf("\t%-33s\t%s\t%s\n", flag.c_str(), type, usage.c_str());
  }
}

#define OPTION(type, saver) \
  new OptionImpl(name, saver, help(name, #type, format(dst), usage))

Option* option(const char* name, int* dst, const std::string& usage) {
  return OPTION(int, saver(dst, "%d%c"));
}

Option* option(const char* name, float* dst, const std::string& usage) {
  return OPTION(float, saver(dst, "%f%c"));
}

Option* option(const char* name, bool* dst, const std::string& usage) {
  return OPTION(bool, (saver<bool, int>(dst, "%d%c")));
}

Option* option(const char* name, std::string* dst, const std::string& usage) {
  Saver saver = [dst](StringView value) {
    *dst = std::string(value);
    return true;
  };
  return OPTION(std::string, saver);
}

CUB_NS_END

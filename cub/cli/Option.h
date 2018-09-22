#ifndef H77F84DB1_A8B6_4AEE_B353_C9358BB990A8
#define H77F84DB1_A8B6_4AEE_B353_C9358BB990A8

#include "cub/string/StringView.h"
#include "cub/base/Keywords.h"

CUB_NS_BEGIN

DEF_INTERFACE(Option) {
  virtual bool parse(StringView arg) = 0;
  virtual const std::string& help() const = 0;
};

Option* option(const char* name, int* dst, const char* usage);
Option* option(const char* name, float* dst, const char* usage);
Option* option(const char* name, bool* dst, const char* usage);
Option* option(const char* name, std::string* dst, const char* usage);

CUB_NS_END

#endif

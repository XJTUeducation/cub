#ifndef RRTX_4775_BBGR_DRE2_BR64_BHDT0_12BVF
#define RRTX_4775_BBGR_DRE2_BR64_BHDT0_12BVF

#include <string>
#include <cub/string/CharSpec.h>
#include "cub/string/StringUtils.h"

CUB_NS_BEGIN

struct Scanner {
  explicit Scanner(StringView);

  Scanner& one(CharSpec);
  Scanner& optional(StringView);
  Scanner& literal(StringView);
  Scanner& any(CharSpec);
  Scanner& many(CharSpec);

  Scanner& restartCapture();
  Scanner& stopCapture();

  Scanner& eos();

  Scanner& until(char);
  Scanner& escapedUntil(char);

  char peek(char = '\0') const;
  int empty() const;
  bool result(StringView* remaining = nullptr, StringView* capture = nullptr);

private:
  void untilImpl(char end_ch, bool escaped);
  Scanner& error();

private:
  StringView cur;
  const char* start = nullptr;
  const char* end = nullptr;
  bool error_ = false;
};

CUB_NS_END

#endif

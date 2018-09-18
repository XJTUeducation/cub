#include <cub/log/log.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

CUB_NS_BEGIN

///////////////////////////////////////////////////////////////////////////
namespace {
  unsigned int infraPrintf(uint8_t level, const void * format, ...) {
    return 0;
  }

  const char* getBaseName(const char* absPath) {
    if (absPath == 0)
      return "";

    auto p = absPath + strlen(absPath);
    while ((p != absPath) && (*(p - 1) != '/')) {
      p--;
    }
    return p;
  }

  void logTrace(const char* file, uint32_t line, uint8_t prnLevel,
      const char* fmt, va_list paramList) {
    const int BUFF_SIZE = 512;
    static char errInfo[BUFF_SIZE];

    if (fmt != nullptr) {
      vsnprintf(errInfo, BUFF_SIZE, fmt, paramList);
    }

    infraPrintf(prnLevel, "%s:%lu: %s ", getBaseName(file), line, errInfo);
  }
}

#define DEF_LOG_TRACE(name, level)                                        \
void log_##name(const char* file, uint32_t line, const char* fmt, ...) {  \
  va_list valist;                                                         \
  va_start(valist, fmt);                                                  \
  logTrace(file, line, level, fmt, valist);                               \
  va_end(valist);                                                         \
}

#define DEBUG_LEVEL     (uint8_t)0x01
#define INFO_LEVEL      (uint8_t)0x02
#define WARN_LEVEL      (uint8_t)0x04
#define ERROR_LEVEL     (uint8_t)0x08
#define FATAL_LEVEL     (uint8_t)0x10

DEF_LOG_TRACE(fatal, FATAL_LEVEL);
DEF_LOG_TRACE(error, ERROR_LEVEL);
DEF_LOG_TRACE(warn,  WARN_LEVEL);
DEF_LOG_TRACE(info,  INFO_LEVEL);
DEF_LOG_TRACE(debug, DEBUG_LEVEL);

CUB_NS_END

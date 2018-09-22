#ifndef HFCA7AD3D_A09C_4E40_BD19_99C964A2C6F1
#define HFCA7AD3D_A09C_4E40_BD19_99C964A2C6F1

#include <stdarg.h>
#include <string>

#include "cub/cub.h"

CUB_NS_BEGIN

std::string stringprintf(const char* format, ...);
void appendf(std::string& dst, const char* format, ...);

CUB_NS_END

#endif
#ifndef H06C055F5_A7AE_4F10_A614_572CCA9817C6
#define H06C055F5_A7AE_4F10_A614_572CCA9817C6

#include <string>
#include "cub/base/Status.h"

CUB_NS_BEGIN

struct FileSystem {
  Status exists(const std::string& fname);
};

CUB_NS_END

#endif

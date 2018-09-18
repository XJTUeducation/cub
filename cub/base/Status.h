#ifndef H5712E60C_DEF5_4A3B_93C0_C074CD80C63B
#define H5712E60C_DEF5_4A3B_93C0_C074CD80C63B

#include <cub/cub.h>
#include <stdint.h>

CUB_NS_BEGIN

using Status = uint32_t ;

enum : Status {
  CUB_RESERVED_FAILURE = 0x80000000
};

inline bool isSuccStatus(Status status) {
  return (status & CUB_RESERVED_FAILURE) == 0;
}

inline bool isFailStatus(Status status) {
  return !isSuccStatus(status);
}

inline constexpr Status succStatus(Status status) {
  return status;
}

inline constexpr Status failStatus(Status status) {
  return status | CUB_RESERVED_FAILURE;
}

CUB_NS_END

enum : uint16_t {
  CUB_INVALID_U16 = 0xFFFF
};

enum : uint32_t {
  CUB_INVALID_U32 = 0xFFFFFFFF
};

/////////////////////////////////////////////////////////////////

enum CubStatus : cub::Status {
  CUB_SUCCESS = cub::succStatus(0),
  CUB_FATAL_BUG = cub::failStatus(0x7FFFFFFE),
  CUB_FAILURE = cub::failStatus(0x7FFFFFFF)
};

/////////////////////////////////////////////////////////////////
#define __CUB_FAILED(result)   cub::isFailStatus(result)

#define CUB_IS_SUCC_STATUS(status) cub::isSuccStatus(status)

#endif

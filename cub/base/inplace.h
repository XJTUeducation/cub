#ifndef HA62D918E_DA16_4172_8E8E_6C33650D7B70
#define HA62D918E_DA16_4172_8E8E_6C33650D7B70

#include <cub/cub.h>

CUB_NS_BEGIN

struct inplace_t {
  explicit inplace_t() = default;
};

extern const inplace_t inplace;

CUB_NS_END

#endif

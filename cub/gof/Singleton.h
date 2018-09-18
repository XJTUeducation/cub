#ifndef H671141B2_B247_440B_8E83_476558110FFF
#define H671141B2_B247_440B_8E83_476558110FFF

#include <cub/cub.h>
#include <cub/base/Uncloneable.h>

CUB_NS_BEGIN

template<typename T>
struct Singleton {
  static T& inst() {
    static T t;
    return t;
  }

  DISALLOW_COPY_AND_ASSIGN(Singleton)

protected:
  Singleton() {}
};

#define DEF_SINGLETON(object) \
  struct object : ::CUB_NS::Singleton<object>

CUB_NS_END

#endif


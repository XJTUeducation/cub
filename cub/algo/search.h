#ifndef H1BEBC79D_5BA8_2341_AD8C_94F820EC7D5B
#define H1BEBC79D_5BA8_2341_AD8C_94F820EC7D5B

#include <cub/cub.h>
#include <algorithm>

CUB_NS_BEGIN

template<typename Iterator, typename T>
Iterator binary_search(Iterator first, Iterator last, const T& val) {
  auto iter = std::lower_bound(first, last, val);
  if (iter != last && *iter == val)
    return iter;
  else
    return last;
}

CUB_NS_END

#endif


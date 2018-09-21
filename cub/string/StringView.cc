#include <cub/string/StringView.h>
#include <algorithm>
#include <iostream>

CUB_NS_BEGIN

const size_t StringView::npos = -1;

std::ostream& operator<<(std::ostream& o, StringView piece) {
  o.write(piece.data(), piece.size());
  return o;
}

size_t StringView::find(char c, size_t pos) const {
  if (pos >= size_) {
    return npos;
  }
  auto result = static_cast<const char*>(memchr(data_ + pos, c, size_ - pos));
  return result != nullptr ? result - data_ : npos;
}

size_t StringView::rfind(char c, size_t pos) const {
  if (size_ == 0)
    return npos;
  for (auto p = data_ + std::min(pos, size_ - 1); p >= data_; p--) {
    if (*p == c) {
      return p - data_;
    }
  }
  return npos;
}

StringView StringView::substr(size_t pos, size_t n) const {
  if (pos > size_)
    pos = size_;
  if (n > size_ - pos)
    n = size_ - pos;
  return {data_ + pos, n};
}

int StringView::compare(StringView b) const {
  auto min_len = (size_ < b.size_) ? size_ : b.size_;
  auto r = memcmp(data_, b.data_, min_len);
  if (r == 0) {
    if (size_ < b.size_)
      r = -1;
    else if (size_ > b.size_)
      r = 1;
  }
  return r;
}

CUB_NS_END

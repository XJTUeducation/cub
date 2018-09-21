#ifndef HA2265080_5BC3_4D0C_AAB0_B6D41881089C
#define HA2265080_5BC3_4D0C_AAB0_B6D41881089C

#include <iosfwd>
#include <string>
#include <stddef.h>
#include <string.h>
#include <type_traits>

#include <cub/cub.h>

CUB_NS_BEGIN

struct StringView {
  using size_type = size_t;

  StringView()
    : StringView(nullptr, 0) {
  }

  StringView(const std::string& s)
    : StringView(s.data(), s.size()) {
  }

  StringView(const char* s)
    : StringView(s, strlen(s)) {
  }

  StringView(const char* d, size_t n)
    : data_(d), size_(n) {
  }

  const char* data() const {
    return data_;
  }

  size_t size() const {
    return size_;
  }

  bool empty() const {
    return size_ == 0;
  }

  using const_iterator = const char*;
  using iterator = const char*;

  iterator begin() const {
    return data_;
  }

  iterator end() const {
    return data_ + size_;
  }

  static const size_t npos;

  char operator[](size_t n) const {
    return data_[n];
  }

  void remove_prefix(size_t n) {
    data_ += n;
    size_ -= n;
  }

  void remove_suffix(size_t n) {
    size_ -= n;
  }

  size_t find(char c, size_t pos = 0) const;
  size_t rfind(char c, size_t pos = npos) const;

  StringView substr(size_t pos, size_t n = npos) const;

  int compare(StringView b) const;

  // Converts to like `std::basic_string`.
  template<typename S>
  explicit operator S() const {
    static_assert(
        std::is_same<char, typename S::value_type>::value,
        "Type mismatch: S must be a string with character type char.");
    static_assert(
        std::is_same<std::char_traits<char>, typename S::traits_type>::value,
        "Type mismatch: S must be a string with traits type "
        "std::char_traits<char>.");
    if (!data())
      return {};
    return {data(), size()};
  }

private:
  const char* data_;
  size_t size_;
};

inline bool operator==(StringView x, StringView y) {
  return x.size() == y.size() &&
      memcmp(x.data(), y.data(), x.size()) == 0;
}

inline bool operator!=(StringView x, StringView y) {
  return !(x == y);
}

inline bool operator<(StringView x, StringView y) {
  return x.compare(y) < 0;
}

inline bool operator>(StringView x, StringView y) {
  return x.compare(y) > 0;
}

inline bool operator<=(StringView x, StringView y) {
  return x.compare(y) <= 0;
}

inline bool operator>=(StringView x, StringView y) {
  return x.compare(y) >= 0;
}

std::ostream& operator<<(std::ostream& o, StringView piece);

CUB_NS_END

#endif

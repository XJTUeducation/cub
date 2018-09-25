#ifndef HA2265080_5BC3_4D0C_AAB0_B6D41881089C
#define HA2265080_5BC3_4D0C_AAB0_B6D41881089C

#include <iosfwd>
#include <string>
#include <vector>
#include <stddef.h>
#include <string.h>
#include <type_traits>

#include <cub/cub.h>

CUB_NS_BEGIN

struct StringView {
  using size_type = size_t;
  using const_iterator = const char*;
  using iterator = const char*;

  static const size_t npos;

  StringView();
  StringView(const std::string&);
  StringView(const char* s);
  StringView(const char* d, size_t n);

  const char* data() const;
  bool empty() const;

  size_t size() const;
  char operator[](size_t n) const;

  iterator begin() const;
  iterator end() const;

  int compare(StringView b) const;

  size_t find(char c, size_t pos = 0) const;
  size_t rfind(char c, size_t pos = npos) const;

  StringView substr(size_t pos, size_t n = npos) const;

  std::string to_s() const;
  explicit operator std::string() const;

  bool contains(StringView substr) const;
  bool starts(StringView prefix) const;
  bool ends(StringView suffix) const;

  void removePrefix(size_t n);
  void removeSuffix(size_t n);

  bool consumePrefix(StringView prefix);
  bool consumeSuffix(StringView suffix);

  size_t ltrim();
  size_t rtrim();
  size_t trim();

  std::string lower() const;
  std::string upper() const;

  void split(StringView delims, std::vector<std::string>&) const;

private:
  template <typename Pred, typename Action>
  bool consumeSubstr(StringView substr, Pred pred, Action action);

  template <typename F>
  std::string map(F f) const;

private:
  const char* data_;
  size_t size_;
};

bool operator==(StringView x, StringView y);
bool operator!=(StringView x, StringView y);
bool operator<(StringView x, StringView y);
bool operator>(StringView x, StringView y);
bool operator<=(StringView x, StringView y);
bool operator>=(StringView x, StringView y);

std::ostream& operator<<(std::ostream& o, StringView);

CUB_NS_END

#endif

#include <cub/string/StringUtils.h>
#include <algorithm>

CUB_NS_BEGIN

bool StringUtils::contains(StringView str, StringView sub) {
  return std::search(str.begin(), str.end(),
                     sub.begin(), sub.end()) != str.end();
}

bool StringUtils::starts(StringView text, StringView prefix) {
  return prefix.empty() ||
      (text.size() >= prefix.size() &&
       memcmp(text.data(), prefix.data(), prefix.size()) == 0);
}

bool StringUtils::ends(StringView text, StringView suffix) {
  return suffix.empty() ||
      (text.size() >= suffix.size() &&
       memcmp(text.data() + (text.size() - suffix.size()),
              suffix.data(), suffix.size()) == 0);
}

bool StringUtils::consumePrefix(StringView& s, StringView expected) {
  if (starts(s, expected)) {
    s.remove_prefix(expected.size());
    return true;
  }
  return false;
}

bool StringUtils::consumeSuffix(StringView& s, StringView expected) {
  if (ends(s, expected)) {
    s.remove_suffix(expected.size());
    return true;
  }
  return false;
}

CUB_NS_END

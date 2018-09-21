#ifndef HED06F7BD_D7BB_4F0C_A9E4_CB029148549A
#define HED06F7BD_D7BB_4F0C_A9E4_CB029148549A

#include <cub/string/StringView.h>

CUB_NS_BEGIN

struct StringUtils {
  static bool consumePrefix(StringView& s, StringView prefix);
  static bool consumeSuffix(StringView& s, StringView suffix);

  static bool starts(StringView text, StringView prefix);
  static bool ends(StringView text, StringView suffix);
  static bool contains(StringView str, StringView sub);
};

CUB_NS_END

#endif

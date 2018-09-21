#include "cub/string/Scanner.h"

CUB_NS_BEGIN

Scanner::Scanner(StringView source) : cur(source) {
  restartCapture();
}

Scanner& Scanner::one(CharSpec spec) {
  if (cur.empty() || !spec(cur[0])) {
    return error();
  }
  cur.remove_prefix(1);
  return *this;
}

Scanner& Scanner::optional(StringView s) {
  StringUtils::consumePrefix(cur, s);
  return *this;
}

Scanner& Scanner::literal(StringView s) {
  if (!StringUtils::consumePrefix(cur, s)) {
    error_ = true;
  }
  return *this;
}

Scanner& Scanner::any(CharSpec spec) {
  while (!cur.empty() && spec(cur[0])) {
    cur.remove_prefix(1);
  }
  return *this;
}

Scanner& Scanner::many(CharSpec clz) {
  return one(clz).any(clz);
}

Scanner& Scanner::restartCapture() {
  start = cur.data();
  end = nullptr;
  return *this;
}

Scanner& Scanner::stopCapture() {
  end = cur.data();
  return *this;
}

Scanner& Scanner::eos() {
  if (!cur.empty())
    error_ = true;
  return *this;
}

Scanner& Scanner::until(char end_ch) {
  untilImpl(end_ch, false);
  return *this;
}

Scanner& Scanner::escapedUntil(char end_ch) {
  untilImpl(end_ch, true);
  return *this;
}

char Scanner::peek(char default_value) const {
  return cur.empty() ? default_value : cur[0];
}

void Scanner::untilImpl(char end_ch, bool escaped) {
  for (;;) {
    if (cur.empty()) {
      error();
      return;
    }
    const char ch = cur[0];
    if (ch == end_ch) {
      return;
    }

    cur.remove_prefix(1);
    if (escaped && ch == '\\') {
      // Escape character, skip next character.
      if (cur.empty()) {
        error();
        return;
      }
      cur.remove_prefix(1);
    }
  }
}

Scanner& Scanner::error() {
  error_ = true;
  return *this;
}

bool Scanner::result(StringView* remaining, StringView* capture) {
  if (error_) {
    return false;
  }
  if (remaining != nullptr) {
    *remaining = cur;
  }
  if (capture != nullptr) {
    const char* last = end == nullptr ? cur.data() : end;
    *capture = StringView(start, last - start);
  }
  return true;
}

CUB_NS_END

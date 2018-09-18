#ifndef H5712E6CD_DEFF_4A3B_93C0_C074CD80C63B
#define H5712E6CD_DEFF_4A3B_93C0_C074CD80C63B

#include <cub/cub.h>
#include <stdint.h>
#include <memory.h>

CUB_NS_BEGIN

template<size_t SIZE>
struct FixSizeBuff {
  FixSizeBuff() {
  }

  explicit FixSizeBuff(const uint8_t* buf) {
    ::memcpy(buff, buf, SIZE);
  }

  FixSizeBuff(const FixSizeBuff& rhs) {
    ::memcpy(buff, rhs.buff, SIZE);
  }

  FixSizeBuff& operator=(const FixSizeBuff& rhs) {
    ::memcpy(buff, rhs.buff, SIZE);
    return *this;
  }

  bool operator==(const FixSizeBuff& rhs) const {
    return ::memcmp(buff, rhs.buff, SIZE) == 0;
  }

  bool operator!=(const FixSizeBuff& rhs) const {
    return !(*this == rhs);
  }

  size_t size() const {
    return SIZE;
  }

  operator const uint8_t*() const {
    return buff;
  }

  operator uint8_t*() {
    return buff;
  }

  void update(const uint8_t* buf) {
    ::memcpy(buff, buf, SIZE);
  }

  void copyTo(uint8_t* buffDes) const {
    ::memcpy(buffDes, buff, SIZE);
  }

  const uint8_t* getContent() const {
    return buff;
  }

  uint8_t* getBuffer() const {
    return (uint8_t*) buff;
  }

  bool matches(const uint8_t* content) const {
    return ::memcmp(content, buff, SIZE) == 0;
  }

protected:
  uint8_t buff[SIZE];
};

CUB_NS_END

#endif


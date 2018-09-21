#ifndef HC6783643_ACBC_4DC1_A5B1_F6A3FDBEA123
#define HC6783643_ACBC_4DC1_A5B1_F6A3FDBEA123

#include <atomic>
#include <memory>

#include <cub/cub.h>
#include <cub/base/Uncloneable.h>

CUB_NS_BEGIN

struct RefCounted {
  RefCounted() : ref_(1) {}

  void ref() const {
    ref_.fetch_add(1, std::memory_order_relaxed);
  }

  bool unref() const {
    if (exactlyOne() || ref_.fetch_sub(1) == 1) {
      delete this;
      return true;
    } else {
      return false;
    }
  }

  bool exactlyOne() const {
    return ref_.load(std::memory_order_acquire) == 1;
  }

protected:
  virtual ~RefCounted() {}

private:
  mutable std::atomic_int_fast32_t ref_;
  DISALLOW_COPY_AND_ASSIGN(RefCounted)
};

struct ScopedUnref {
  explicit ScopedUnref(RefCounted* obj) : obj(obj) {}

  ~ScopedUnref() {
    if (obj) obj->unref();
  }

private:
  RefCounted* obj;
  DISALLOW_COPY_AND_ASSIGN(ScopedUnref)
};

CUB_NS_END

#endif

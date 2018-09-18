#ifndef H49422F70_25E4_4BC5_B62E_49094085B51D
#define H49422F70_25E4_4BC5_B62E_49094085B51D

#include <array>
#include <functional>
#include <initializer_list>
#include <new>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <cub/cub.h>

CUB_NS_BEGIN

// A value of type Optional<T> holds either a value of T or
// an "empty" value.  When it holds a value of T, it stores it as a direct
// subobject, so sizeof(Optional<T>) is approximately sizeof(T)+1. The interface
// is based on the upcoming std::experimental::Optional<T>, and
// Optional<T> is designed to be cheaply drop-in replaceable by
// std::experimental::Optional<T>, once it is rolled out.
//
// This implementation is based on the specification in N4335 Section 5:
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4335.html
//
// Differences between Optional<T> and
// std::experimental::Optional<T> include:
//    - Optional<T> is basically a proper subset of
//         std::experimental::Optional<T>.
//    - constexpr not used. (dependency on some differences between C++11 and
//         C++14.)
//    - noexcept not used.
//    - exceptions not used - in lieu of exceptions we use CHECK-failure.
//
// std::Optional<T> might not quite be a drop-in replacement for
// std::experimental::Optional<T> because the standards committee is considering
// changes to the semantics of relational operators as part of the process of
// turning std::experimental::Optional<T> into std::Optional<T>.  The best way
// of making sure you aren't affected by those changes is to make sure that your
// type T defines all of the operators consistently. (x <= y is exactly
// equivalent to !(x > y), etc.)
//
// Synopsis:
//
//     #include "cub/base/Optional.h"
//
//     using cub::Optional;
//     using cub::nullopt;
//     using cub::in_place;
//     using cub::make_Optional;
//
//     Optional<string> f() {
//       string result;
//       if (...) {
//          ...
//          result = ...;
//          return result;
//       } else {
//          ...
//          return nullopt;
//       }
//     }
//
//     int main() {
//         Optional<string> optstr = f();
//         if (optstr) {
//            // non-empty
//            print(optstr.value());
//         } else {
//            // empty
//            error();
//         }
//     }
template<typename T>
class Optional;

struct in_place_t {
};
extern const in_place_t in_place;

struct nullopt_t {
  // It must not be default-constructible to avoid ambiguity for opt = {}.
  explicit constexpr nullopt_t(int /*unused*/) {
  }
};
extern const nullopt_t nullopt;

// See comment above first declaration.
template<typename T>
struct Optional {
  using value_type = T;

  Optional() {
  }
  Optional(nullopt_t) {
  }

  Optional(const Optional& src) {
    if (src) {
      construct(src.reference());
    }
  }

  Optional(Optional&& src) noexcept {
    if (src) {
      construct(std::move(src.reference()));
    }
  }

  Optional(const T& src) {
    construct(src);
  }

  Optional(T&& src) {
    construct(std::move(src));
  }

  // Optional<T>(in_place, arg1, arg2, arg3) constructs a non-empty Optional
  // with an in-place constructed value of T(arg1,arg2,arg3).
  template<typename ... Args>
  explicit Optional(in_place_t, Args&&... args) {
    construct(std::forward<Args>(args)...);
  }

  // Optional<T>(in_place, {arg1, arg2, arg3}) constructs a non-empty Optional
  // with an in-place list-initialized value of T({arg1, arg2, arg3}).
  template<class U, typename ... Args>
  explicit Optional(in_place_t, std::initializer_list<U> il, Args&&... args) {
    construct(il, std::forward<Args>(args)...);
  }

  ~Optional() {
    clear();
  }

  Optional& operator=(nullopt_t) {
    clear();
    return *this;
  }

  Optional& operator=(const Optional& src) {
    if (src) {
      operator=(src.reference());
    } else {
      clear();
    }
    return *this;
  }

  Optional& operator=(Optional&& src) noexcept {
    if (src) {
      operator=(std::move(src.reference()));
    } else {
      clear();
    }
    return *this;
  }

  Optional& operator=(const T& src) {
    if (*this) {
      reference() = src;
    } else {
      construct(src);
    }
    return *this;
  }

  Optional& operator=(T&& src) {
    if (*this) {
      reference() = std::move(src);
    } else {
      construct(std::move(src));
    }
    return *this;
  }

  // Emplace reconstruction.  (Re)constructs the underlying T in-place with the
  // given arguments forwarded:
  //
  // Optional<Foo> opt;
  // opt.emplace(arg1,arg2,arg3);  (Constructs Foo(arg1,arg2,arg3))
  //
  // If the Optional is non-empty, and the `args` refer to subobjects of the
  // current object, then behaviour is undefined.  This is because the current
  // object will be destructed before the new object is constructed with `args`.
  //
  template<typename ... Args>
  void emplace(Args&&... args) {
    clear();
    construct(std::forward<Args>(args)...);
  }

  // Emplace reconstruction with initializer-list.  See immediately above.
  template<class U, class ... Args>
  void emplace(std::initializer_list<U> il, Args&&... args) {
    clear();
    construct(il, std::forward<Args>(args)...);
  }

  // Swap, standard semantics.
  void swap(Optional& src) {
    if (*this) {
      if (src) {
        using std::swap;
        swap(reference(), src.reference());
      } else {
        src.construct(std::move(reference()));
        destruct();
      }
    } else {
      if (src) {
        construct(std::move(src.reference()));
        src.destruct();
      }
    }
  }

  // You may use `*opt`, and `opt->m`, to access the underlying T value and T's
  // member `m`, respectively.  If the Optional is empty, behaviour is
  // undefined.
  const T* operator->() const {
    //DCHECK(engaged_);
    return pointer();
  }
  T* operator->() {
    //DCHECK(engaged_);
    return pointer();
  }
  const T& operator*() const {
    //DCHECK(engaged_);
    return reference();
  }
  T& operator*() {
    //DCHECK(engaged_);
    return reference();
  }

  operator bool() const {
    return engaged_;
  }

  // Use `opt.value()` to get a reference to underlying value.  The constness
  // and lvalue/rvalue-ness of `opt` is preserved to the view of the T
  // subobject.
  const T& value() const & {
    //CHECK(*this) << "Bad Optional access";
    return reference();
  }

  T& value() & {
    //CHECK(*this) << "Bad Optional access";
    return reference();
  }

  T&& value() && {  // NOLINT(build/c++11)
    //CHECK(*this) << "Bad Optional access";
    return std::move(reference());
  }

  const T&& value() const && {  // NOLINT(build/c++11)
    //CHECK(*this) << "Bad Optional access";
    return std::move(reference());
  }

  // Use `opt.value_or(val)` to get either the value of T or the given default
  // `val` in the empty case.
  template<class U>
  T value_or(U&& val) const & {
    if (*this) {
      return reference();
    } else {
      return static_cast<T>(std::forward<U>(val));
    }
  }

  template<class U>
  T value_or(U&& val) && {
    if (*this) {
      return std::move(reference());
    } else {
      return static_cast<T>(std::forward<U>(val));
    }
  }

private:
  const T* pointer() const {
    return static_cast<const T*>(static_cast<const void*>(&storage_));
  }

  T* pointer() {
    return static_cast<T*>(static_cast<void*>(&storage_));
  }

  const T& reference() const {
    return *pointer();
  }

  T& reference() {
    return *pointer();
  }

  // Construct inner T in place with given `args`.
  // Precondition: engaged_ is false
  // Postcondition: engaged_ is true
  template<class ... Args>
  void construct(Args&&... args) {
    engaged_ = true;
    new (pointer()) T(std::forward<Args>(args)...);
  }

  // Destruct inner T.
  // Precondition: engaged_ is true
  // Postcondition: engaged_ is false
  void destruct() {
    //DCHECK(engaged_);
    pointer()->T::~T();
    engaged_ = false;
    //DCHECK(!engaged_);
  }

  // Destroy inner T if engaged.
  // Postcondition: engaged_ is false
  void clear() {
    if (engaged_) {
      destruct();
    }
    //DCHECK(!engaged_);
  }

  // The internal storage for a would-be T value, constructed and destroyed
  // with placement new and placement delete.
  typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;

  // Whether or not this Optional is non-empty.
  bool engaged_ = false;

  // T constraint checks. You can't have an Optional of nullopt_t, in_place_t or
  // a reference.
  static_assert(
      !std::is_same<nullopt_t, typename std::remove_cv<T>::type>::value,
      "Optional<nullopt_t> is not allowed.");
  static_assert(
      !std::is_same<in_place_t, typename std::remove_cv<T>::type>::value,
      "Optional<in_place_t> is not allowed.");
  static_assert(!std::is_reference<T>::value,
      "Optional<reference> is not allowed.");
};

// make_Optional(v) creates a non-empty Optional<T> where the type T is deduced
// from v.  Can also be explicitly instantiated as make_Optional<T>(v).
template<typename T>
Optional<typename std::decay<T>::type> make_optional(T&& v) {
  return Optional<typename std::decay<T>::type>(std::forward<T>(v));
}

// All combinations of the six comparisons between Optional<T>, T and nullopt.
// The empty value is considered less than all non-empty values, and equal to
// itself.
template<typename T>
bool operator==(const Optional<T>& lhs, const Optional<T>& rhs) {
  if (lhs) {
    if (rhs) {
      return *lhs == *rhs;
    } else {
      return false;
    }
  } else {
    if (rhs) {
      return false;
    } else {
      return true;
    }
  }
}

template<typename T>
bool operator!=(const Optional<T>& lhs, const Optional<T>& rhs) {
  return !(lhs == rhs);
}

template<typename T>
bool operator<(const Optional<T>& lhs, const Optional<T>& rhs) {
  if (!rhs) {
    return false;
  } else if (!lhs) {
    return true;
  } else {
    return *lhs < *rhs;
  }
}

template<typename T>
bool operator>(const Optional<T>& lhs, const Optional<T>& rhs) {
  return rhs < lhs;
}

template<typename T>
bool operator<=(const Optional<T>& lhs, const Optional<T>& rhs) {
  return !(rhs < lhs);
}

template<typename T>
bool operator>=(const Optional<T>& lhs, const Optional<T>& rhs) {
  return !(lhs < rhs);
}

template<typename T>
bool operator==(const Optional<T>& lhs, nullopt_t rhs) {
  return !lhs;
}

template<typename T>
bool operator==(nullopt_t lhs, const Optional<T>& rhs) {
  return !rhs;
}

template<typename T>
bool operator!=(const Optional<T>& lhs, nullopt_t rhs) {
  return static_cast<bool>(lhs);
}

template<typename T>
bool operator!=(nullopt_t lhs, const Optional<T>& rhs) {
  return static_cast<bool>(rhs);
}

template<typename T>
bool operator<(const Optional<T>& lhs, nullopt_t rhs) {
  return false;
}

template<typename T>
bool operator<(nullopt_t lhs, const Optional<T>& rhs) {
  return static_cast<bool>(rhs);
}

template<typename T>
bool operator<=(const Optional<T>& lhs, nullopt_t rhs) {
  return !lhs;
}

template<typename T>
bool operator<=(nullopt_t lhs, const Optional<T>& rhs) {
  return true;
}

template<typename T>
bool operator>(const Optional<T>& lhs, nullopt_t rhs) {
  return static_cast<bool>(lhs);
}

template<typename T>
bool operator>(nullopt_t lhs, const Optional<T>& rhs) {
  return false;
}

template<typename T>
bool operator>=(const Optional<T>& lhs, nullopt_t rhs) {
  return true;
}

template<typename T>
bool operator>=(nullopt_t lhs, const Optional<T>& rhs) {
  return !rhs;
}

template<typename T>
bool operator==(const Optional<T>& lhs, const T& rhs) {
  return static_cast<bool>(lhs) ? *lhs == rhs : false;
}

template<typename T>
bool operator==(const T& lhs, const Optional<T>& rhs) {
  return static_cast<bool>(rhs) ? lhs == *rhs : false;
}

template<typename T>
bool operator!=(const Optional<T>& lhs, const T& rhs) {
  return static_cast<bool>(lhs) ? !(*lhs == rhs) : true;
}

template<typename T>
bool operator!=(const T& lhs, const Optional<T>& rhs) {
  return static_cast<bool>(rhs) ? !(lhs == *rhs) : true;
}

template<typename T>
bool operator<(const Optional<T>& lhs, const T& rhs) {
  return static_cast<bool>(lhs) ? *lhs < rhs : true;
}

template<typename T>
bool operator<(const T& lhs, const Optional<T>& rhs) {
  return static_cast<bool>(rhs) ? lhs < *rhs : false;
}

template<typename T>
bool operator>(const Optional<T>& lhs, const T& rhs) {
  return static_cast<bool>(lhs) ? rhs < *lhs : false;
}

template<typename T>
bool operator>(const T& lhs, const Optional<T>& rhs) {
  return static_cast<bool>(rhs) ? *rhs < lhs : true;
}

template<typename T>
bool operator<=(const Optional<T>& lhs, const T& rhs) {
  return !(lhs > rhs);
}

template<typename T>
bool operator<=(const T& lhs, const Optional<T>& rhs) {
  return !(lhs > rhs);
}

template<typename T>
bool operator>=(const Optional<T>& lhs, const T& rhs) {
  return !(lhs < rhs);
}

template<typename T>
bool operator>=(const T& lhs, const Optional<T>& rhs) {
  return !(lhs < rhs);
}

// Swap, standard semantics.
template<typename T>
inline void swap(Optional<T>& a, Optional<T>& b) {
  a.swap(b);
}

CUB_NS_END

#endif

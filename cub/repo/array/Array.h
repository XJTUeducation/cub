#ifndef H5C910113_6FE0_4972_8A42_F9CC943F5B74
#define H5C910113_6FE0_4972_8A42_F9CC943F5B74

#include <cub/base/StaticAssert.h>
#include <cub/base/EqHelper.h>
#include <cub/utils/DefaultConstructor.h>
#include <cub/mem/Placement.h>
#include <type_traits>

CUB_NS_BEGIN

template<typename T, size_t N>
struct Array {
  using Elem = Placement<T>;

  struct Iterator {
    Iterator() : elem(0) {
    }

    Iterator(Elem* elem) : elem(elem) {
    }

    Iterator(const Iterator& rhs) : elem(rhs.elem) {
    }

    Iterator& operator=(const Iterator& rhs) {
      elem = rhs.elem;
      return *this;
    }

    __INLINE_EQUALS(Iterator) {
      return (elem == rhs.elem);
    }

    void reset() {
      elem = 0;
    }

    bool isNull() const {
      return elem == 0;
    }

    T* operator->() {
      return getValue();
    }

    T& operator*() {
      return *getValue();
    }

    T* getValue() const {
      return &(elem->getRef());
    }

    Iterator operator++(int) {
      Iterator i = *this;
      elem++;
      return i;
    }

    Iterator& operator++() {
      elem++;
      return *this;
    }

  private:
    Elem* elem;
  };

  Array() {
    STATIC_ASSERT(N > 0);
    STATIC_ASSERT(std::is_default_constructible<T>::value);
    DefaultConstructor<true>::construct(elems);
  }

  template<typename ... ARGS>
  Array(ARGS&&... args) {
    for (size_t i = 0; i < N; ++i) {
      new (elems[i].alloc()) T(std::forward<ARGS>(args)...);
    }
  }

  size_t size() const {
    return N;
  }

  bool empty() const {
    return size() == 0;
  }

  const T& operator[](size_t index) const {
    return elems[index].getRef();
  }

  T& operator[](size_t index) {
    return elems[index].getRef();
  }

  template<typename ... ARGS>
  void emplace(size_t index, ARGS&&... args) {
    if (index >= N)
      return;
    elems[index].destroy();
    new (elems[index].alloc()) T(std::forward<ARGS>(args)...);
  }

  Iterator begin() {
    return {&elems[0]};
  }

  Iterator end() {
    return {&elems[0] + N};
  }

private:
  Placement<T> elems[N];
};

/////////////////////////////////////////////////////////////
#define ARRAY_FOREACH_FROM(i, from, items) \
   for(auto i=from; i!=items.end(); ++i)

#define ARRAY_FOREACH(i, items) \
   ARRAY_FOREACH_FROM(i, items.begin(), items)

CUB_NS_END

#endif

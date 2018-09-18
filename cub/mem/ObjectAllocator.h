#ifndef H38247538_297F_4A80_94D3_8A289788465E
#define H38247538_297F_4A80_94D3_8A289788465E

#include <cub/algo/loop.h>
#include <cub/base/Assertions.h>

CUB_NS_BEGIN

template<typename T, uint16_t MAX_NUM>
struct ObjectAllocator {
  ObjectAllocator() : freeHead(nullptr), freeTail(nullptr), freeCounter(0) {
    FOREACH(i, MAX_NUM) {
      freeEntity(entities[i]);
    }
  }

  uint16_t getFreeSlotNum() const {
    if (freeCounter < 0 || freeCounter > MAX_NUM) {
      return CUB_INVALID_U16;
    }
    return freeCounter;
  }

  bool hasFreeSlot() const {
    return freeHead != nullptr;
  }

  void* alloc() {
    return hasFreeSlot() ? doAlloc() : nullptr;
  }

  void free(void* p) {
    if (p == nullptr)
      return;

    CUB_ASSERT_TRUE_VOID(within(p));
    CUB_ASSERT_TRUE_VOID(
        (((char* )p - (char* )entities) % sizeof(Entity)) == 0);

    freeEntity(*(Entity*) p);
  }

  bool within(void* p) const {
    return p >= (void*) entities && p < (void*) &(entities[MAX_NUM]);
  }

private:
  union Entity {
    char obj[sizeof(T)];
    Entity* next;
  };

private:

  void* doAlloc() {
    auto p = freeHead;
    freeHead = freeHead->next;
    if (freeHead == nullptr) {
      freeTail = nullptr;
    }
    freeCounter--;
    return p;
  }

  void freeEntity(Entity& entity) {
    if (freeHead == 0) {
      freeHead = &entity;
    } else {
      CUB_ASSERT_VALID_PTR_VOID(freeTail);
      freeTail->next = &entity;
    }

    freeTail = &entity;
    freeTail->next = nullptr;
    freeCounter++;
  }

  bool isFree(const void* p) const {
    Entity* _header = freeHead;
    while (_header != nullptr) {
      if (_header == p)
        return true;
      _header = _header->next;
    }
    return false;
  }

private:
  Entity entities[MAX_NUM];
  Entity* freeHead;
  Entity* freeTail;
  uint16_t freeCounter;
};

//////////////////////////////////////////////////////////////////////////
#define DECL_OPERATOR_NEW()              \
void* operator new(size_t) throw();      \
void operator delete(void* p)

//////////////////////////////////////////////////////////////////////////
#define DEF_OPERATOR_NEW(p_type, allocator)                     \
    void* p_type::operator new(size_t) throw()                  \
    {                                                           \
       return allocator.alloc();                                \
    }                                                           \
    void p_type::operator delete(void* p)                       \
    {                                                           \
        allocator.free(p);                                      \
    }

//////////////////////////////////////////////////////////////////////////
#define DEF_GENERIC_OPERATOR_NEW(p_type, allocator)             \
    template <typename T>                                       \
    void* p_type<T>::operator new(size_t) throw()               \
    {                                                           \
       return allocator.alloc();                                \
    }                                                           \
    template <typename T>                                       \
    void p_type<T>::operator delete(void* p)                    \
    {                                                           \
        allocator.free(p);                                      \
    }

//////////////////////////////////////////////////////////////////////////
#define DEF_OBJ_ALLOCATOR(p_type, p_num)                        \
namespace {                                                     \
  ::CUB_NS::ObjectAllocator<p_type, p_num> p_type##Allocator;   \
}                                                               \
DEF_OPERATOR_NEW(p_type, p_type##Allocator)

CUB_NS_END

#endif


#ifndef H38247538_297F_4A80_94D3_8A289788462A
#define H38247538_297F_4A80_94D3_8A289788462A

#include <cub/cub.h>

CUB_NS_BEGIN

template<typename MSG>
struct AutoMsg {
  AutoMsg() : msg(new MSG{}){
  }

  ~AutoMsg() {
    if (msg != nullptr) {
      delete msg;
      msg = nullptr;
    }
  }

  MSG* operator->() {
    return msg;
  }

  const MSG* operator->() const {
    return msg;
  }

  MSG& operator*() {
    return *msg;
  }

  const MSG& operator*() const {
    return *msg;
  }


  const MSG* getPointer() const {
    return msg;
  }

  MSG* getPointer() {
    return msg;
  }

  const MSG& getRef() const {
    return *msg;
  }

  MSG& getRef() {
    return *msg;
  }

private:
  MSG* msg;
};

CUB_NS_END

#endif


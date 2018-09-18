#ifndef H38247538_297F_4A80_94D3_8B289888462A
#define H38247538_297F_4A80_94D3_8B289888462A

#include <cub/cub.h>
#include <arpa/inet.h>

CUB_NS_BEGIN

inline uint8_t  hton(uint8_t  v) { return v;         }
inline uint16_t hton(uint16_t v) { return htons(v);  }
inline uint32_t hton(uint32_t v) { return htonl(v);  }

inline int8_t  hton(int8_t  v) { return v;         }
inline int16_t hton(int16_t v) { return htons(v);  }
inline int32_t hton(int32_t v) { return htonl(v);  }

CUB_NS_END

#endif //CUB_BYTEORDER_H_H

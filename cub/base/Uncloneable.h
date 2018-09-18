#ifndef H671141B2_B247_440B_8E83_476558110FFE
#define H671141B2_B247_440B_8E83_476558110FFE

#define DISALLOW_COPY_AND_ASSIGN(xclass) \
DISALLOW_ASSIGN(xclass) 						     \
DISALLOW_COPY(xclass)

#define DISALLOW_ASSIGN(xclass) \
  xclass& operator=(const xclass&) = delete;

#define DISALLOW_COPY(xclass) \
  xclass(const xclass&) = delete;

#endif


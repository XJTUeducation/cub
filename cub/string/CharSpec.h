#ifndef HACEBA5AA_27B2_403A_B1EA_AA8056B4F282
#define HACEBA5AA_27B2_403A_B1EA_AA8056B4F282

#include <cub/cub.h>
#include <functional>
#include <vector>

CUB_NS_BEGIN

using CharSpec = std::function<bool(char)>;

CharSpec all();
CharSpec never();

using CType = int(*)(int);

CharSpec atom(CType);
CharSpec ch(char c);

CharSpec is_and(std::vector<CharSpec>&&);
CharSpec is_or(std::vector<CharSpec>&&);
CharSpec is_not(CharSpec);

#define DECL_ATOM_SPEC(name) CharSpec name()

DECL_ATOM_SPEC(alnum);
DECL_ATOM_SPEC(alpha);
DECL_ATOM_SPEC(cntrl);
DECL_ATOM_SPEC(digit);
DECL_ATOM_SPEC(lower);
DECL_ATOM_SPEC(graph);
DECL_ATOM_SPEC(print);
DECL_ATOM_SPEC(punct);
DECL_ATOM_SPEC(space);
DECL_ATOM_SPEC(upper);
DECL_ATOM_SPEC(xdigit);

DECL_ATOM_SPEC(dot);
DECL_ATOM_SPEC(dash);
DECL_ATOM_SPEC(plus);
DECL_ATOM_SPEC(minus);
DECL_ATOM_SPEC(zero);
DECL_ATOM_SPEC(slash);
DECL_ATOM_SPEC(underscore);

#undef DECL_ATOM_SPEC

CUB_NS_END

#endif

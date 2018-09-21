#ifndef H67177F30_1107_45A0_A764_8E764C4693D7
#define H67177F30_1107_45A0_A764_8E764C4693D7

#include <string>

struct Path {
  explicit Path(const std::string& path);

  Path& join(const std::string&);

private:
  std::string path;
};

#endif

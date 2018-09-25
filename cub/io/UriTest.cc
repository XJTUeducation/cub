#include "cub/io/Uri.h"
#include <cut/cut.hpp>

USING_CUT_NS
USING_CUM_NS

CUB_NS_BEGIN

FIXTURE(UriTest) {

  void assertUri(const std::string& str,
      StringView scheme, StringView host, StringView path) {
    Uri uri(str);

    StringView s, h, p;
    uri.get(s, h, p);

    ASSERT_EQ(scheme, s);
    ASSERT_EQ(host,   h);
    ASSERT_EQ(path,   p);

    Uri other(s, h, p);
    ASSERT_EQ(str, other.to_s());
  }

  TEST("parse") {
    assertUri("http://foo", "http", "foo", "");
    assertUri("/encrypted/://foo", "", "", "/encrypted/://foo");
    assertUri("/usr/local/foo", "", "", "/usr/local/foo");
    assertUri("file:///usr/local/foo", "file", "", "/usr/local/foo");
    assertUri("local.file:///usr/local/foo", "local.file", "",
                     "/usr/local/foo");
    assertUri("a-b:///foo", "", "", "a-b:///foo");
    assertUri(":///foo", "", "", ":///foo");
    assertUri("9dfd:///foo", "", "", "9dfd:///foo");
    assertUri("file:", "", "", "file:");
    assertUri("file:/", "", "", "file:/");
    assertUri("hdfs://localhost:8020/path/to/file", "hdfs",
                     "localhost:8020", "/path/to/file");
    assertUri("hdfs://localhost:8020", "hdfs", "localhost:8020", "");
    assertUri("hdfs://localhost:8020/", "hdfs", "localhost:8020", "/");
  }
};



CUB_NS_END
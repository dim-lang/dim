// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "container/LinkedHashMap.h"
#include "Log.h"
#include "boost/preprocessor/stringize.hpp"
#include "catch2/catch.hpp"
#include "container/LinkedHashMap.hpp"
#include <string>

#define CONSTRUCTOR_UNIT(a, b)                                                 \
  do {                                                                         \
    LinkedHashMap<a, b> hm1;                                                   \
    REQUIRE(hm1.size() == 0);                                                  \
    REQUIRE(hm1.bucket() == 0);                                                \
    REQUIRE(hm1.empty());                                                      \
    REQUIRE(hm1.loadFactor() == 0.0);                                          \
    CINFO("CONSTRUCTOR_UNIT: a:{}, b:{}, hm1.begin:{}, hm1.end: {}",           \
          BOOST_PP_STRINGIZE(a), BOOST_PP_STRINGIZE(b),                        \
          hm1.begin().toString(), hm1.end().toString());                       \
    REQUIRE(hm1.begin() == hm1.end());                                         \
                                                                               \
    LinkedHashMap<a, b> hm2(100);                                              \
    REQUIRE(hm2.size() == 0);                                                  \
    REQUIRE(hm2.bucket() == 100);                                              \
    REQUIRE(hm2.empty());                                                      \
    REQUIRE(hm2.loadFactor() == 0.0);                                          \
    REQUIRE(hm2.begin() == hm2.end());                                         \
    CINFO("CONSTRUCTOR_UNIT: a:{}, b:{}, hm2.begin:{}, hm2.end: {}",           \
          BOOST_PP_STRINGIZE(a), BOOST_PP_STRINGIZE(b),                        \
          hm2.begin().toString(), hm2.end().toString());                       \
  } while (0)

struct LHMTester {
  std::string name;
  int age;
  int score;
};

namespace std {

template <> struct hash<LHMTester> {
public:
  std::size_t operator()(const LHMTester &t) const {
    std::hash<int> h1;
    std::hash<std::string> h2;
    return h1(t.age) ^ h1(t.score) ^ h2(t.name);
  }
};

} // namespace std

TEST_CASE("container/LinkedHashMap", "[container/LinkedHashMap]") {
  SECTION("constructor") {
    CONSTRUCTOR_UNIT(char, unsigned char);
    CONSTRUCTOR_UNIT(unsigned char, char);
    CONSTRUCTOR_UNIT(short, unsigned short);
    CONSTRUCTOR_UNIT(unsigned short, short);
    CONSTRUCTOR_UNIT(int, unsigned int);
    CONSTRUCTOR_UNIT(unsigned int, int);
    CONSTRUCTOR_UNIT(long, unsigned long);
    CONSTRUCTOR_UNIT(unsigned long, long);
    CONSTRUCTOR_UNIT(long long, unsigned long long);
    CONSTRUCTOR_UNIT(unsigned long long, long long);
    CONSTRUCTOR_UNIT(std::string, int);
  }
}

#undef CONSTRUCTOR_UNIT

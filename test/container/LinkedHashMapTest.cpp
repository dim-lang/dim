// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "container/LinkedHashMap.h"
#include "Log.h"
#include "Random.h"
#include "boost/preprocessor/stringize.hpp"
#include "catch2/catch.hpp"
#include "container/LinkedHashMap.hpp"
#include <string>

template <typename A, typename B> void testConstructor(A a, B b) {
  LinkedHashMap<A, B> hm1;
  REQUIRE(hm1.size() == 0);
  REQUIRE(hm1.bucket() == 0);
  REQUIRE(hm1.empty());
  REQUIRE(hm1.load() == 0.0);
  REQUIRE(hm1.begin() == hm1.end());
  REQUIRE(hm1.threshold() == 4.0);
  CINFO("testConstructor: a:{}, b:{}, hm1.begin:{}, hm1.end: {}",
        BOOST_PP_STRINGIZE(a), BOOST_PP_STRINGIZE(b), hm1.begin().toString(),
        hm1.end().toString());

  LinkedHashMap<A, B> hm2(100);
  REQUIRE(hm2.size() == 0);
  REQUIRE(hm2.bucket() == 100);
  REQUIRE(hm2.empty());
  REQUIRE(hm2.load() == 0.0);
  REQUIRE(hm2.begin() == hm2.end());
  REQUIRE(hm2.threshold() == 4.0);
  CINFO("testConstructor: a:{}, b:{}, hm2.begin:{}, hm2.end: {}",
        BOOST_PP_STRINGIZE(a), BOOST_PP_STRINGIZE(b), hm2.begin().toString(),
        hm2.end().toString());
}

template <typename A> void testInsert(A a, A b) {
  LinkedHashMap<A, B> hm1;
  int c = 0;
  for (int i = (int)a; i < (int)b; i++) {
    if (i % 2 == 0) {
      hm1.insert((A)i, (A)i);
    } else {
      hm1.insert(std::make_pair((A)i, (A)i));
    }
    ++c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    REQUIRE(!hm1.empty());
    REQUIRE(hm1.load() <= hm1.threshold());
    REQUIRE(hm1[(A)i] == (A)i);
  }
}

template <> void testInsert(std::string a, std::string b) {
  LinkedHashMap<A, B> hm1;
  int c = 0;
  for (int i = 0; i < 64; i++) {
    if (i % 2 == 0) {
      hm1.insert((A)i, (A)i);
    } else {
      hm1.insert(std::make_pair((A)i, (A)i));
    }
    ++c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    REQUIRE(!hm1.empty());
    REQUIRE(hm1.load() <= hm1.threshold());
    REQUIRE(hm1[(A)i] == (A)i);
  }
}

struct LHMTester {
  std::string name;
  int age;
  int score;
  bool operator==(const LHMTester &other) const {
    return name == other.name && age == other.age && score == other.score;
  }
  bool operator!=(const LHMTester &other) const {
    return name != other.name || age != other.age || score != other.score;
  }
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
    testConstructor((char)1, (unsigned char)1);
    testConstructor((unsigned char)1, (char)1);
    testConstructor((short)1, (unsigned short)1);
    testConstructor((unsigned short)1, (short)1);
    testConstructor((int)1, (unsigned int)1);
    testConstructor((unsigned int)1, (int)1);
    testConstructor((long)1, (unsigned long)1);
    testConstructor((unsigned long)1, (long)1);
    testConstructor((long long)1, (unsigned long long)1);
    testConstructor((unsigned long long)1, (long long)1);
    testConstructor(std::string("1"), (int)1);
    testConstructor(LHMTester(), (int)1);
  }
  SECTION("insert") {
    testInsert((char)1, (char)1);
    testInsert((unsigned char)1, (unsigned char)1);
    testInsert((short)1, (short)1);
    testInsert((unsigned short)1, (unsigned short)1);
    testInsert((int)1, (int)1);
    testInsert((unsigned int)1, (unsigned int)1);
    testInsert((long)1, (long)1);
    testInsert((unsigned long)1, (unsigned long)1);
    testInsert((long long)1, (long long)1);
    testInsert((unsigned long long)1, (unsigned long long)1);
    testInsert(std::string("1"), std::string("1"));
    testInsert(LHMTester(), (int)1);
  }
}

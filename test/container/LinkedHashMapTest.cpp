// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "container/LinkedHashMap.h"
#include "Log.h"
#include "Random.h"
#include "boost/preprocessor/stringize.hpp"
#include "catch2/catch.hpp"
#include "container/LinkedHashMap.hpp"
#include <string>

#define BUCKET_MIN 8
#define BUCKET_STEP 8
#define TEST_MAX 1024

static RandomNumber<int> randomBucket(1, 1024);
static RandomString randomString;
static RandomNumber<int> randomInt;

struct LHMTester {
  std::string name;
  int age;
  int score;

  LHMTester() : name("name"), age(1), score(1) {}
  LHMTester(const std::string &a_name, int a_age, int a_score)
      : name(a_name), age(a_age), score(a_score) {}

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

template <typename A, typename B> void testConstructor(A a, B b) {
  LinkedHashMap<A, B> hm1;
  REQUIRE(hm1.size() == 0);
  REQUIRE(hm1.bucket() == 0);
  REQUIRE(hm1.empty());
  REQUIRE(hm1.load() == 0.0);
  REQUIRE(hm1.begin() == hm1.end());
  CINFO("testConstructor: a:{}, b:{}, hm1.begin:{}, hm1.end: {}",
        BOOST_PP_STRINGIZE(a), BOOST_PP_STRINGIZE(b), hm1.begin().toString(),
        hm1.end().toString());

  int r = randomBucket.next();
  LinkedHashMap<A, B> hm2(r);
  REQUIRE(hm2.size() == 0);
  REQUIRE(hm2.bucket() >= BUCKET_MIN);
  REQUIRE(hm2.bucket() >= r);
  REQUIRE(hm2.bucket() <= r + BUCKET_STEP);
  REQUIRE(hm2.empty());
  REQUIRE(hm2.load() == 0.0);
  REQUIRE(hm2.begin() == hm2.end());
  CINFO("testConstructor: a:{}, b:{}, hm2.begin:{}, hm2.end: {}",
        BOOST_PP_STRINGIZE(a), BOOST_PP_STRINGIZE(b), hm2.begin().toString(),
        hm2.end().toString());
}

template <typename A> void testInsert(A a, A b) {
  LinkedHashMap<A, A> hm1;
  int c = 0;
  for (int i = (A)a; i < (A)b; i++) {
    if (i % 2 == 0) {
      REQUIRE(hm1.find((A)i) == hm1.end());
      hm1.insert((A)i, (A)i);
    } else {
      REQUIRE(hm1.find((A)i) == hm1.end());
      hm1.insert(std::make_pair((A)i, (A)i));
    }
    ++c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    REQUIRE(!hm1.empty());
    REQUIRE(hm1.load() <= 4.0);
    REQUIRE(hm1[(A)i] == (A)i);
  }
}

template <> void testInsert(std::string a, std::string b) {
  LinkedHashMap<std::string, std::string> hm1;
  int c = 0;
  for (int i = 0; i < TEST_MAX; i++) {
    std::string kv = randomString.nextAlnum(i + 1);
    if (i % 2 == 0) {
      REQUIRE(hm1.find(kv) == hm1.end());
      hm1.insert(kv, kv);
    } else {
      REQUIRE(hm1.find(kv) == hm1.end());
      hm1.insert(std::make_pair(kv, kv));
    }
    ++c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    REQUIRE(!hm1.empty());
    REQUIRE(hm1.load() <= 4.0);
    REQUIRE(hm1[kv] == kv);
  }
}

template <> void testInsert(LHMTester a, LHMTester b) {
  LinkedHashMap<LHMTester, LHMTester> hm1;
  int c = 0;
  for (int i = 0; i < TEST_MAX; i++) {
    std::string p = randomString.nextAlnum(i + 1);
    int q = randomInt.next();
    if (i % 2 == 0) {
      REQUIRE(hm1.find(LHMTester(p, q, q)) == hm1.end());
      hm1.insert(LHMTester(p, q, q), LHMTester(p, q, q));
    } else {
      REQUIRE(hm1.find(LHMTester(p, q, q)) == hm1.end());
      hm1.insert(std::make_pair(LHMTester(p, q, q), LHMTester(p, q, q)));
    }
    ++c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    REQUIRE(!hm1.empty());
    REQUIRE(hm1.load() <= 4.0);
    REQUIRE(hm1[LHMTester(p, q, q)] == LHMTester(p, q, q));
  }
}

TEST_CASE("container/LinkedHashMap", "[container/LinkedHashMap]") {
  SECTION("nextBucket") {
    detail::LinkedHt<int, int, std::hash<int>, std::equal_to<int>> ht;
    for (int i = 0; i < TEST_MAX; i++) {
      int r = randomInt.next();
      REQUIRE(ht.nextBucket(r) % 8 == 0);
      REQUIRE(ht.nextBucket(r) >= r);
      REQUIRE(ht.nextBucket(r) < r + 8);
    }
  }
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
    testInsert((char)0, (char)127);
    testInsert((unsigned char)0, (unsigned char)127);
    testInsert((short)0, (short)128);
    testInsert((unsigned short)0, (unsigned short)128);
    testInsert((int)0, (int)128);
    testInsert((unsigned int)0, (unsigned int)128);
    testInsert((long)0, (long)128);
    testInsert((unsigned long)0, (unsigned long)128);
    testInsert((long long)0, (long long)128);
    testInsert((unsigned long long)0, (unsigned long long)128);
    testInsert(std::string("1"), std::string("1"));
    testInsert(LHMTester(), LHMTester());
  }
}

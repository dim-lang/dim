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

static RandomInt<int> randomBucket(1, 1024);
static RandomString randomString;
static RandomInt<int> randomInt;

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
  REQUIRE(hm1.rbegin() == hm1.rend());
  CINFO("testConstructor: a:{}, b:{}, hm1.begin:{}, hm1.end:{}, "
        "hm1.rbegin():{}, hm1.rend():{}",
        BOOST_PP_STRINGIZE(a), BOOST_PP_STRINGIZE(b), hm1.begin().toString(),
        hm1.end().toString(), hm1.rbegin().toString(), hm1.rend().toString());

  int r = randomBucket.next();
  LinkedHashMap<A, B> hm2(r);
  REQUIRE(hm2.size() == 0);
  REQUIRE(hm2.bucket() >= BUCKET_MIN);
  REQUIRE(hm2.bucket() >= r);
  REQUIRE(hm2.bucket() <= r + BUCKET_STEP);
  REQUIRE(hm2.empty());
  REQUIRE(hm2.load() == 0.0);
  REQUIRE(hm2.begin() == hm2.end());
  CINFO("testConstructor: a:{}, b:{}, hm2.begin:{}, hm2.end:{}, hm2.rbegin:{}, "
        "hm2.rend:{}",
        BOOST_PP_STRINGIZE(a), BOOST_PP_STRINGIZE(b), hm2.begin().toString(),
        hm2.end().toString(), hm2.rbegin().toString(), hm2.rend().toString());
}

template <typename A> void testInsert(A a, A b) {
  LinkedHashMap<A, A> hm1;
  int c = 0;
  for (int i = (A)a; i < (int)b; i++) {
    REQUIRE(hm1.find((A)i) == hm1.end());
    if (i % 2 == 0) {
      hm1.insert((A)i, (A)i);
    } else {
      hm1.insert(std::make_pair((A)i, (A)i));
    }
    auto p = hm1.find((A)i);
    REQUIRE(p != hm1.end());
    REQUIRE(p->first == (A)i);
    REQUIRE(p->second == (A)i);
    REQUIRE(hm1.exist((A)i));
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
    REQUIRE(hm1.find(kv) == hm1.end());
    if (i % 2 == 0) {
      hm1.insert(kv, kv);
    } else {
      hm1.insert(std::make_pair(kv, kv));
    }
    auto p = hm1.find(kv);
    REQUIRE(p != hm1.end());
    REQUIRE(p->first == kv);
    REQUIRE(p->second == kv);
    REQUIRE(hm1.exist(kv));
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
    LHMTester lhm(p, q, q);
    REQUIRE(hm1.find(lhm) == hm1.end());
    if (i % 2 == 0) {
      hm1.insert(lhm, lhm);
    } else {
      hm1.insert(std::make_pair(lhm, lhm));
    }
    auto u = hm1.find(lhm);
    REQUIRE(u != hm1.end());
    REQUIRE(u->first == lhm);
    REQUIRE(u->second == lhm);
    REQUIRE(hm1.exist(lhm));
    ++c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    REQUIRE(!hm1.empty());
    REQUIRE(hm1.load() <= 4.0);
    REQUIRE(hm1[lhm] == lhm);
  }
}

void testRemove(int a, int b) {
  LinkedHashMap<int, int> hm1(100);
  int c = 0;
  for (int i = a; i < b; i++) {
    REQUIRE(hm1.find(i) == hm1.end());
    hm1.insert(i, i);
    auto p = hm1.find(i);
    REQUIRE(p != hm1.end());
    REQUIRE(p->first == i);
    REQUIRE(p->second == i);
    ++c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    REQUIRE(!hm1.empty());
    REQUIRE(hm1.load() <= 4.0);
    REQUIRE(hm1[i] == i);
  }
  for (int i = a; i < b; i++) {
    if (hm1.find(i) == hm1.end()) {
      CINFO("i: {}", i);
    }
    REQUIRE(hm1.find(i) != hm1.end());
    if (i % 2 == 0) {
      hm1.remove(i);
    } else {
      auto k = hm1.find(i);
      hm1.remove(k);
    }
    REQUIRE(hm1.find(i) == hm1.end());
    --c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    if (i < (int)b - 1) {
      REQUIRE(!hm1.empty());
    } else {
      REQUIRE(hm1.empty());
    }
    REQUIRE(hm1.load() <= 4.0);
  }
}

void testRemove(std::string a, std::string b) {
  LinkedHashMap<std::string, std::string> hm1(100);
  std::vector<std::string> keyList;
  int c = 0;
  for (int i = 0; i < TEST_MAX; i++) {
    std::string kv = randomString.nextAlnum(i + 1);
    keyList.push_back(kv);
    REQUIRE(hm1.find(kv) == hm1.end());
    hm1.insert(kv, kv);
    REQUIRE(hm1.find(kv) != hm1.end());
    ++c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    REQUIRE(!hm1.empty());
    REQUIRE(hm1.load() <= 4.0);
    REQUIRE(hm1[kv] == kv);
  }
  for (int i = 0; i < TEST_MAX; i++) {
    std::string kv = keyList[i];
    REQUIRE(hm1.find(kv) != hm1.end());
    if (i % 2 == 0) {
      hm1.remove(kv);
    } else {
      auto k = hm1.find(kv);
      hm1.remove(k);
    }
    REQUIRE(hm1.find(kv) == hm1.end());
    --c;
    REQUIRE(hm1.size() == c);
    REQUIRE(hm1.bucket() > 0);
    if (i < TEST_MAX - 1) {
      REQUIRE(!hm1.empty());
    } else {
      REQUIRE(hm1.empty());
    }
    REQUIRE(hm1.load() <= 4.0);
  }
}

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
  SECTION("remove") {
    testRemove(0, 1024);
    testRemove(std::string("1"), std::string("1"));
  }
  SECTION("clear/release") {
    LinkedHashMap<int, int> h1;
    for (int i = 0; i < TEST_MAX; i++) {
      h1.insert(i, i);
    }
    h1.clear();
    REQUIRE(h1.empty());
    REQUIRE(h1.bucket() > 0);
    h1.release();
    REQUIRE(h1.empty());
    REQUIRE(h1.bucket() == 0);
  }
  SECTION("foreach") {
    LinkedHashMap<int, int> hm1;
    LinkedHashMap<std::string, std::string> hm2;
    LinkedHashMap<LHMTester, LHMTester> hm3;
    int i;
    for (i = 0; i < TEST_MAX; i++) {
      hm1.insert(i, i);
      hm2.insert(std::to_string(i), std::to_string(i));
      hm3.insert(LHMTester(std::to_string(i), i, i),
                 LHMTester(std::to_string(i), i, i));
    }
    LinkedHashMap<int, int>::Iterator p;
    LinkedHashMap<int, int>::RIterator p1;
    for (p = hm1.begin(), i = 0; p != hm1.end(); p++, i++) {
      REQUIRE(i < TEST_MAX);
      REQUIRE(p->first == i);
      REQUIRE(p->second == i);
    }
    for (p1 = hm1.rbegin(), i = TEST_MAX - 1; p1 != hm1.rend(); p1++, i--) {
      REQUIRE(i >= 0);
      REQUIRE(p1->first == i);
      REQUIRE(p1->second == i);
    }
    LinkedHashMap<std::string, std::string>::Iterator q;
    LinkedHashMap<std::string, std::string>::RIterator q1;
    for (q = hm2.begin(), i = 0; q != hm2.end(); q++, i++) {
      REQUIRE(i < TEST_MAX);
      REQUIRE(q->first == std::to_string(i));
      REQUIRE(q->second == std::to_string(i));
    }
    for (q1 = hm2.rbegin(), i = TEST_MAX - 1; q1 != hm2.rend(); q1++, i--) {
      REQUIRE(i >= 0);
      REQUIRE(q1->first == std::to_string(i));
      REQUIRE(q1->second == std::to_string(i));
    }
    LinkedHashMap<LHMTester, LHMTester>::Iterator u;
    LinkedHashMap<LHMTester, LHMTester>::RIterator u1;
    for (u = hm3.begin(), i = 0; u != hm3.end(); u++, i++) {
      REQUIRE(i < TEST_MAX);
      REQUIRE(u->first == LHMTester(std::to_string(i), i, i));
      REQUIRE(u->second == LHMTester(std::to_string(i), i, i));
    }
    for (u1 = hm3.rbegin(), i = TEST_MAX - 1; u1 != hm3.rend(); u1++, i--) {
      REQUIRE(i >= 0);
      REQUIRE(u1->first == LHMTester(std::to_string(i), i, i));
      REQUIRE(u1->second == LHMTester(std::to_string(i), i, i));
    }
  }
  SECTION("insertOrAssign") {
    LinkedHashMap<int, int> hm1;
    LinkedHashMap<std::string, std::string> hm2;
    LinkedHashMap<LHMTester, LHMTester> hm3;

    int a = 100;
    std::string b = "hello";
    LHMTester c("jack", 10, 20);

    hm1.insertOrAssign(a, a);
    REQUIRE(hm1.size() == 1);
    REQUIRE(hm1.exist(a));
    REQUIRE(hm1[a] == a);
    hm1.insertOrAssign(a, a);
    REQUIRE(hm1.size() == 1);
    REQUIRE(hm1.exist(a));
    REQUIRE(hm1[a] == a);

    hm2.insertOrAssign(b, b);
    REQUIRE(hm2.size() == 1);
    REQUIRE(hm2[b] == b);
    REQUIRE(hm2.exist(b));
    hm2.insertOrAssign(b, b);
    REQUIRE(hm2.size() == 1);
    REQUIRE(hm2[b] == b);
    REQUIRE(hm2.exist(b));

    hm3.insertOrAssign(c, c);
    REQUIRE(hm3.size() == 1);
    REQUIRE(hm3[c] == c);
    REQUIRE(hm3.exist(c));
    hm3.insertOrAssign(c, c);
    REQUIRE(hm3.size() == 1);
    REQUIRE(hm3[c] == c);
    REQUIRE(hm3.exist(c));
  }
}

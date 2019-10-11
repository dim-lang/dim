// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "Random.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

#define TEST_MAX 1024

#define assertEquals(cs, ss)                                                   \
  do {                                                                         \
    std::string tmp(ss);                                                       \
    REQUIRE((cs).capacity() >= (tmp).length());                                \
    REQUIRE(!(cs).empty());                                                    \
    REQUIRE((cs).size() == (tmp).length());                                    \
    REQUIRE((void *)(cs).head() != nullptr);                                   \
    REQUIRE((void *)(cs).tail() == (cs).head() + (tmp).length());              \
    REQUIRE(std::memcmp((cs).head(), (tmp).data(), (cs).size()) == 0);         \
  } while (0)

#define assertEmpty(cs)                                                        \
  do {                                                                         \
    REQUIRE((cs).empty());                                                     \
    REQUIRE((cs).size() == 0);                                                 \
    REQUIRE((void *)(cs).head() == (cs).tail());                               \
  } while (0)

static bool testStringEq(const fastype::Cowstr &a, const std::string &b) {
  return std::memcmp(a.head(), b.data(), a.size()) == 0 &&
         a.size() == (int)b.length();
}

// @return   first is replace(all)
//           second is replace(all)'s count
//           third is replaceFirst
//           fourth is replaceFirst's count
static std::tuple<std::string, int, std::string, int>
testReplace(const std::string &src, const std::string &from,
            const std::string &to) {
  std::string all, first;
  int alln = 0, firstn = 0, i = 0;
  bool done = false;
  while (i < src.length() - from.length() + 1) {
    if (std::memcmp(src.data() + i, from.data(), from.length()) == 0) {
      alln++;
      all = all + to;
      if (!done) {
        first = first + to;
        done = true;
        firstn++;
      } else {
        first = first + src.substr(i, from.length());
      }
      i += from.length();
    } else {
      all = all + src[i];
      first = first + src[i];
      i++;
    }
  }
  if (i < src.length()) {
    all = all + src.substr(i);
    first = first + src.substr(i);
  }
  return std::make_tuple(all, alln, first, firstn);
}

TEST_CASE("Cowstr", "[Cowstr]") {
  fastype::Random::initialize();

  SECTION("Empty Constructor") {
    fastype::Cowstr cs;
    assertEmpty(cs);
    REQUIRE((cs).capacity() == 0);
  }

  SECTION("Capacity Constructor") {
    for (int i = 0; i < TEST_MAX; i++) {
      fastype::Cowstr cs(i);
      assertEmpty(cs);
      REQUIRE((cs).capacity() >= i);

      std::string t = fastype::Random::nextAlphaNumeric(i + 1);
      cs = cs.concat(t);
      assertEquals(cs, t);
    }
  }

  SECTION("concat/concatHead") {
    for (int i = 0; i < TEST_MAX; i++) {
      {
        fastype::Cowstr cs(i);
        assertEmpty(cs);
        REQUIRE((cs).capacity() >= i);

        std::string t1 = fastype::Random::nextAlphaNumeric(i + 1);
        cs = cs.concat(t1);
        assertEquals(cs, t1);

        fastype::Cowstr cs1 = cs + t1;
        assertEquals(cs1, t1 + t1);

        fastype::Cowstr ss1(i);
        ss1 += t1;
        assertEquals(ss1, t1);

        std::string t2 = fastype::Random::nextAlphaNumeric(i * 2 + 1);
        cs = cs.concat(t2);
        assertEquals(cs, t1 + t2);

        fastype::Cowstr cs2 = cs + t2;
        assertEquals(cs2, t1 + t2 + t2);

        std::string t3 = fastype::Random::nextAlphaNumeric(i + 1);
        fastype::Cowstr ss2(i);
        ss2 += cs;
        ss2 += t3;
        assertEquals(ss2, t1 + t2 + t3);
      }

      {
        std::string t1 = fastype::Random::nextAlphaNumeric(i + 1);
        std::string t2 = fastype::Random::nextAlphaNumeric(i + 1);
        fastype::Cowstr c1(t1);
        fastype::Cowstr c2(t2);
        fastype::Cowstr c3 = c1.concat(c2);
        assertEquals(c3, t1 + t2);

        fastype::Cowstr c4 = c1 + c2;
        assertEquals(c4, t1 + t2);
        REQUIRE(c3.size() == c4.size());
        REQUIRE(std::memcmp(c3.head(), c4.head(), c3.size()) == 0);

        fastype::Cowstr c5;
        c5 += c1;
        c5 += c2;
        assertEquals(c5, t1 + t2);
      }

      {
        std::string t1 = fastype::Random::nextAlphaNumeric(i + 1);
        std::string t2 = fastype::Random::nextAlphaNumeric(i + 1);
        fastype::Cowstr c1(t1);
        fastype::Cowstr c2(t2);
        fastype::Cowstr c3 = c1.concatHead(c2);
        assertEquals(c3, t2 + t1);
      }
    }
  }

  SECTION("replace/replaceFirst") {
    {
      std::string s1 = "helloworldgoodbyeworld";
      std::string s2 = "hellookgoodbyeok";
      fastype::Cowstr c1(s1);
      fastype::Cowstr c2 = c1.replace("world", "ok");
      REQUIRE(c2.size() == (int)s2.length());
      REQUIRE(std::memcmp(c2.head(), s2.data(), c2.size()) == 0);
    }

    {
      std::string s1 = "helloworldgoodbyeworld";
      std::string s2 = "hellogoodbye";
      fastype::Cowstr c1(s1);
      fastype::Cowstr c2 = c1.replace("world", "");
      REQUIRE(c2.size() == (int)s2.length());
      REQUIRE(std::memcmp(c2.head(), s2.data(), c2.size()) == 0);
    }

    {
      std::string s1 = "helloworldgoodbyeworld";
      std::string s2 = "hellogoodbyeworld";
      fastype::Cowstr c1(s1);
      fastype::Cowstr c2 = c1.replaceFirst("world", "");
      REQUIRE(c2.size() == (int)s2.length());
      REQUIRE(std::memcmp(c2.head(), s2.data(), c2.size()) == 0);
    }

    {
      std::string s1 = "helloworldgoodbyeworld";
      std::string s2 = "hellocoolgoodbyeworld";
      fastype::Cowstr c1(s1);
      fastype::Cowstr c2 = c1.replaceFirst("world", "cool");
      REQUIRE(c2.size() == (int)s2.length());
      REQUIRE(std::memcmp(c2.head(), s2.data(), c2.size()) == 0);
    }

    {
      for (int i = 0; i < TEST_MAX; i++) {
        std::string s1 = fastype::Random::nextAlphaNumeric(i + 1);
        char cc = fastype::Random::nextAlphaNumericChar();
        fastype::Cowstr c1(s1);

        std::tuple<std::string, int, std::string, int> replacement1 =
            testReplace(s1, std::string("") + cc, "");
        std::string s21 = std::get<0>(replacement1);
        std::string s31 = std::get<2>(replacement1);

        fastype::Cowstr c21 = c1.replace(cc, "");
        fastype::Cowstr c22 = c1.replaceFirst(cc, "");
        REQUIRE(c21.size() == (int)s21.length());
        REQUIRE(std::memcmp(c21.head(), s21.data(), c21.size()) == 0);
        REQUIRE(c22.size() == (int)s31.length());
        REQUIRE(std::memcmp(c22.head(), s31.data(), c22.size()) == 0);

        std::tuple<std::string, int, std::string, int> replacement2 =
            testReplace(s1, std::string("") + cc, "world");
        std::string s22 = std::get<0>(replacement2);
        std::string s32 = std::get<2>(replacement2);

        fastype::Cowstr c31 = c1.replace(cc, "world");
        fastype::Cowstr c32 = c1.replaceFirst(cc, "world");
        REQUIRE(c31.size() == (int)s22.length());
        REQUIRE(std::memcmp(c31.head(), s22.data(), c31.size()) == 0);
        REQUIRE(c32.size() == (int)s32.length());
        REQUIRE(std::memcmp(c32.head(), s32.data(), c32.size()) == 0);
      }
    }

    {
      for (int i = 0; i < TEST_MAX; i++) {
        std::string ss1 = fastype::Random::nextAlphaNumeric(i + 1);
        std::string ss2 = fastype::Random::nextAlphaNumeric(
            std::min<int>(ss1.length(), i + 1));

        std::tuple<std::string, int, std::string, int> replacement =
            testReplace(ss1, ss2, "world");
        std::string s2 = std::get<0>(replacement);
        std::string s3 = std::get<2>(replacement);

        fastype::Cowstr c1(ss1);
        fastype::Cowstr c2 = c1.replace(ss2, "world");
        fastype::Cowstr c3 = c1.replaceFirst(ss2, "world");

        if (c2.size() != (int)s2.length()) {
          F_INFO_MSG("you can stop here");
        }
        REQUIRE(c2.size() == (int)s2.length());
        REQUIRE(std::memcmp(c2.head(), s2.data(), c2.size()) == 0);
        REQUIRE(c3.size() == (int)s3.length());
        REQUIRE(std::memcmp(c3.head(), s3.data(), c3.size()) == 0);
      }
    }
  }

  SECTION("split") {
    for (int i = 0; i < TEST_MAX; i++) {
      std::string s = fastype::Random::nextAlphaNumeric(i + 1);
      char splitC = fastype::Random::nextAlphaNumericChar();
      std::string splitS =
          fastype::Random::nextAlphaNumeric(std::min<int>(i + 1, s.length()));

      fastype::Cowstr c1(s);
      std::vector<fastype::Cowstr> c2 = c1.split(fastype::Cowstr(splitC));
      std::vector<fastype::Cowstr> c3 = c1.split(fastype::Cowstr(splitS));
      int n2 = std::accumulate(
          c2.begin(), c2.end(), 0,
          [](int v, const fastype::Cowstr &str) { return v + str.size(); });
      int n3 = std::accumulate(
          c3.begin(), c3.end(), 0,
          [](int v, const fastype::Cowstr &str) { return v + str.size(); });
      if (!c2.empty()) {
        REQUIRE(n2 <= c1.size());
      }
      if (!c3.empty()) {
        REQUIRE(n3 <= c1.size());
      }
      int k = 0;
      for (int j = 0; j < c2.size(); j++) {
        REQUIRE(std::memcmp(c2[j].head(), c1.head() + k, c2[j].size()) == 0);
        k += c2[j].size() + 1;
      }
      k = 0;
      for (int j = 0; j < c3.size(); j++) {
        REQUIRE(std::memcmp(c3[j].head(), c1.head() + k, c3[j].size()) == 0);
        k += c3[j].size() + splitS.length();
      }
    }

    {
      fastype::Cowstr cc("boo:and:foobar");
      std::string r = "o";
      std::vector<fastype::Cowstr> cr = cc.split(r);
      REQUIRE(cr.size() == 5);
      REQUIRE(testStringEq(cr[0], "b"));
      REQUIRE(testStringEq(cr[1], ""));
      REQUIRE(testStringEq(cr[2], ":and:f"));
      REQUIRE(testStringEq(cr[3], ""));
      REQUIRE(testStringEq(cr[4], "bar"));
    }
    {
      fastype::Cowstr cc("boo:and:foobar");
      std::string r = "";
      std::vector<fastype::Cowstr> cr = cc.split(r);
      REQUIRE(cr.size() == 14);
      REQUIRE(testStringEq(cr[0], "b"));
      REQUIRE(testStringEq(cr[1], "o"));
      REQUIRE(testStringEq(cr[2], "o"));
      REQUIRE(testStringEq(cr[3], ":"));
      REQUIRE(testStringEq(cr[4], "a"));
      REQUIRE(testStringEq(cr[5], "n"));
      REQUIRE(testStringEq(cr[6], "d"));
      REQUIRE(testStringEq(cr[7], ":"));
      REQUIRE(testStringEq(cr[8], "f"));
      REQUIRE(testStringEq(cr[9], "o"));
      REQUIRE(testStringEq(cr[10], "o"));
      REQUIRE(testStringEq(cr[11], "b"));
      REQUIRE(testStringEq(cr[12], "a"));
      REQUIRE(testStringEq(cr[13], "r"));
    }
    {
      fastype::Cowstr cc("boo:and:foobar");
      std::string r = "b";
      std::vector<fastype::Cowstr> cr = cc.split(r);
      REQUIRE(cr.size() == 3);
      REQUIRE(testStringEq(cr[0], ""));
      REQUIRE(testStringEq(cr[1], "oo:and:foo"));
      REQUIRE(testStringEq(cr[2], "ar"));
    }
    {
      fastype::Cowstr cc("boo:and:foobar");
      std::string r = "r";
      std::vector<fastype::Cowstr> cr = cc.split(r);
      REQUIRE(cr.size() == 2);
      REQUIRE(testStringEq(cr[0], "boo:and:fooba"));
      REQUIRE(testStringEq(cr[1], ""));
    }
    {
      fastype::Cowstr cc("boo:and:foobar");
      std::string r = "c";
      std::vector<fastype::Cowstr> cr = cc.split(r);
      REQUIRE(cr.size() == 1);
      REQUIRE(cr[0] == cc.stdstr());
    }
    {
      fastype::Cowstr cc("5|6|7||8|9||");
      std::string r = "|";
      std::vector<fastype::Cowstr> cr = cc.split(r);
      REQUIRE(cr.size() == 8);
      REQUIRE(testStringEq(cr[0], "5"));
      REQUIRE(testStringEq(cr[1], "6"));
      REQUIRE(testStringEq(cr[2], "7"));
      REQUIRE(testStringEq(cr[3], ""));
      REQUIRE(testStringEq(cr[4], "8"));
      REQUIRE(testStringEq(cr[5], "9"));
      REQUIRE(testStringEq(cr[6], ""));
      REQUIRE(testStringEq(cr[7], ""));
    }
    {
      fastype::Cowstr cc("");
      std::string r = "a";
      std::vector<fastype::Cowstr> cr = cc.split(r);
      REQUIRE(cr.size() == 1);
      REQUIRE(testStringEq(cr[0], ""));
    }
  }
}

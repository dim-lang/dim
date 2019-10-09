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

#define TEST_MAX 128

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

// @return   first is replace(all)
//           second is replace(all)'s count
//           third is replaceFirst
//           fourth is replaceFirst's count
static std::tuple<std::string, int, std::string, int>
myReplace(const std::string &src, const std::string &from,
          const std::string &to) {
  int n1 = 0, n2 = 0, i;
  std::string s1, s2;
  bool firstDone = false;
  for (i = 0; i < src.length() - from.length() + 1;) {
    if (std::memcmp(src.data() + i, from.data(), from.length()) == 0) {
      n1++;
      s1 = s1 + to;
      i += from.length();
      if (!firstDone) {
        s2 = s2 + to;
        firstDone = true;
        n2++;
      } else {
        s2 = s2 + src[i];
      }
    } else {
      s1 = s1 + src[i];
      s2 = s2 + src[i];
      i++;
    }
  }
  return std::make_tuple(s1, n1, s2, n2);
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
            myReplace(s1, std::string("") + cc, "");
        std::string s21 = std::get<0>(replacement1);
        int n21 = std::get<1>(replacement1);
        (void)n21;
        std::string s31 = std::get<2>(replacement1);
        int n31 = std::get<3>(replacement1);
        (void)n31;

        fastype::Cowstr c21 = c1.replace(cc, "");
        fastype::Cowstr c22 = c1.replaceFirst(cc, "");
        REQUIRE(c21.size() == (int)s21.length());
        REQUIRE(std::memcmp(c21.head(), s21.data(), c21.size()) == 0);
        REQUIRE(c22.size() == (int)s31.length());
        REQUIRE(std::memcmp(c22.head(), s31.data(), c22.size()) == 0);

        std::tuple<std::string, int, std::string, int> replacement2 =
            myReplace(s1, std::string("") + cc, "world");
        std::string s22 = std::get<0>(replacement2);
        int n22 = std::get<1>(replacement2);
        (void)n22;
        std::string s32 = std::get<2>(replacement2);
        int n32 = std::get<3>(replacement2);
        (void)n32;

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
            myReplace(ss1, ss2, "world");
        std::string s2 = std::get<0>(replacement);
        int n2 = std::get<1>(replacement);
        (void)n2;
        std::string s3 = std::get<2>(replacement);
        int n3 = std::get<3>(replacement);
        (void)n3;

        fastype::Cowstr c1(ss1);
        fastype::Cowstr c2 = c1.replace(ss2, "world");
        fastype::Cowstr c3 = c1.replaceFirst(ss2, "world");

        REQUIRE(c2.size() == (int)s2.length());
        REQUIRE(std::memcmp(c2.head(), s2.data(), c2.size()) == 0);
        REQUIRE(c3.size() == (int)s3.length());
        REQUIRE(std::memcmp(c3.head(), s3.data(), c3.size()) == 0);
      }
    }
  }
}

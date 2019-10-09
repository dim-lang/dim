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
  }
}

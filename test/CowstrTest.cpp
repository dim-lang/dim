// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Cowstr.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <utility>

#define HELLO_WORLD "hello world"
#define GOODBYE_WORLD "goodbye world"

#define TEST_MIN 16
#define TEST_MAX 1025

static std::string randomString(int n) {
  std::stringstream ss;
  std::string candidates = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghikjlmnopqrstu"
                           "vwxyz1234567890,.;:~!@#$%^&*()-+";
  for (int i = 0; i < n; i++) {
    ss << candidates[std::rand() % candidates.length()];
  }
  return ss.str();
};

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
    REQUIRE((void *)(cs).head() == nullptr);                                   \
    REQUIRE((void *)(cs).tail() == nullptr);                                   \
  } while (0)

TEST_CASE("Cowstr", "[Cowstr]") {

  SECTION("Empty Constructor") {
    fastype::Cowstr cs;
    assertEmpty(cs);
    REQUIRE((cs).capacity() == 0);
  }

  SECTION("Capacity Constructor") {
    for (int i = TEST_MIN; i < TEST_MAX; i++) {
      fastype::Cowstr cs(i);
      assertEmpty(cs);
      REQUIRE((cs).capacity() >= i);

      std::string t = randomString(i);
      cs = cs.concat(t);
      assertEquals(cs, t);
    }
  }

  SECTION("Concat") {
    for (int i = TEST_MIN; i < TEST_MAX; i++) {
      {
        fastype::Cowstr cs(i);
        assertEmpty(cs);
        REQUIRE((cs).capacity() >= i);

        std::string t1 = randomString(i);
        cs = cs.concat(t1);
        assertEquals(cs, t1);

        fastype::Cowstr cs1 = cs + t1;
        assertEquals(cs1, t1 + t1);

        fastype::Cowstr ss1(i);
        ss1 += t1;
        assertEquals(ss1, t1);

        std::string t2 = randomString(i * 2);
        cs = cs.concat(t2);
        assertEquals(cs, t1 + t2);

        fastype::Cowstr cs2 = cs + t2;
        assertEquals(cs2, t1 + t2 + t2);

        std::string t3 = randomString(i);
        fastype::Cowstr ss2(i);
        ss2 += cs;
        ss2 += t3;
        assertEquals(ss2, t1 + t2 + t3);
      }

      {
        std::string t1 = randomString(i);
        std::string t2 = randomString(2);
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
    }
  }
}

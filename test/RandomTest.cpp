// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Random.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>

#define TEST_MAX 16384

template <typename T> void testInt(T a, T b) {
  RandomInt<T> r1;
  for (int i = 0; i < TEST_MAX; i++) {
    T t = r1.next();
    REQUIRE(t >= 0);
    REQUIRE(t < std::numeric_limits<T>::max());
  }
  RandomInt<T> r2(a, b);
  for (int i = 0; i < TEST_MAX; i++) {
    T t = r2.next();
    REQUIRE(t >= a);
    REQUIRE(t < b);
  }
}

#define TEST_LITERAL(f, g)                                                     \
  do {                                                                         \
    RandomChar rc;                                                             \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      char c = rc.f();                                                         \
      REQUIRE(g(c));                                                           \
    }                                                                          \
    RandomString rs;                                                           \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      std::string s = rs.f(i + 1);                                             \
      for (int j = 0; j < s.length(); j++) {                                   \
        REQUIRE(g(s[j]));                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

static bool testIsAscii(int c) { return c >= 0 && c < 128; }

TEST_CASE("Random", "[Random]") {
  SECTION("integer") {
    testInt<short>(-1023, 193);
    testInt<unsigned short>(38, 193);
    testInt<int>(1023, 9331193);
    testInt<unsigned int>(138, 44193);
    testInt<long>(-94L, 9393L);
    testInt<unsigned long>(8138L, 4483193L);
    testInt<long long>(-9394LL, 129393LL);
    testInt<unsigned long long>(288138LL, 844483193LL);
  }

  SECTION("literal") {
    {
      RandomChar rc;
      for (int i = 0; i < TEST_MAX; i++) {
        char c = rc.nextAlpha();
        REQUIRE(std::isalpha(c));
      }
      RandomString rs;
      for (int i = 0; i < TEST_MAX; i++) {
        std::string s = rs.nextAlpha(i + 1);
        for (int j = 0; j < s.length(); j++) {
          REQUIRE(std::isalpha(s[j]));
        }
      }
    }
    TEST_LITERAL(nextAlnum, std::isalnum);
    TEST_LITERAL(nextDigit, std::isdigit);
    TEST_LITERAL(nextHex, std::isxdigit);
    TEST_LITERAL(nextPunct, std::ispunct);
    TEST_LITERAL(nextPrint, std::isprint);
    TEST_LITERAL(nextSpace, std::isspace);
    TEST_LITERAL(nextCtrl, std::iscntrl);
    TEST_LITERAL(nextAscii, testIsAscii);
  }
}

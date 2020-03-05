// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Random.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>

#define TEST_MAX 16384

#define TEST_INT(T, a, b)                                                      \
  do {                                                                         \
    RandomInt<T> rand;                                                         \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      T t = rand.next();                                                       \
      REQUIRE(t >= 0);                                                         \
      REQUIRE(t < std::numeric_limits<T>::max());                              \
    }                                                                          \
    RandomInt<T> rand(a, b);                                                   \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      T t = rand.next();                                                       \
      REQUIRE(t >= a);                                                         \
      REQUIRE(t < b);                                                          \
    }                                                                          \
  } while (0)

#define assertLiteral(U, V, P)                                                 \
  {                                                                            \
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {                \
      UChar c = U();                                                           \
      icu::UnicodeString s = V(i + 1);                                         \
      REQUIRE(P(c));                                                           \
      for (int j = 0; j < s.length(); j++) {                                   \
        REQUIRE(P((char)s.charAt(j)));                                         \
      }                                                                        \
    }                                                                          \
  }

#define TEST_LITERAL(f, p)                                                     \
  do {                                                                         \
    RandomChar rc;                                                             \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      char c = rc.f();                                                         \
      REQUIRE(p(c));                                                           \
    }                                                                          \
    RandomString rs;                                                           \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      std::string s = rs.f(i + 1);                                             \
      for (int j = 0; j < s.length(); j++) {                                   \
        REQUIRE(p(s[j]));                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

TEST_CASE("Random", "[Random]") {
  Random::initialize();

  SECTION("integer") {
    TEST_INT(short, -1023, 193);
    TEST_INT(unsigned short, 38, 193);
    TEST_INT(int, 1023, 9331193);
    TEST_INT(unsigned int, 138, 44193);
    TEST_INT(long, -94L, 9393L);
    TEST_INT(unsigned long, 8138L, 4483193L);
    TEST_INT(long long, -9394LL, 129393LL);
    TEST_INT(unsigned long long, 288138LL, 844483193LL);
  }

  SECTION("literal") {
    TEST_LITERAL(nextAlpha, std::isalpha);
    TEST_LITERAL(nextAlnum, std::isalnum);
    TEST_LITERAL(nextDigit, std::isdigit);
    TEST_LITERAL(nextHex, std::isxdigit);
    TEST_LITERAL(nextPunct, std::ispunct);
    TEST_LITERAL(nextPrint, std::isprint);
    TEST_LITERAL(nextWhitespace, std::isspace);
    TEST_LITERAL(nextCtrl, std::iscntrl);
    TEST_LITERAL(nextAscii, std::isascii);
  }
}

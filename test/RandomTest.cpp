// Copyright 2019- <collie.org>
// Apache License Version 2.0

#include "Random.h"
#include "Log.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>

#define TEST_MAX 1024

#define TEST_INT(T, a, b)                                                      \
  do {                                                                         \
    RandomInt<T> r1;                                                           \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      T t = r1.next();                                                         \
      REQUIRE(t >= (T)0);                                                      \
      REQUIRE(t <= std::numeric_limits<T>::max());                             \
    }                                                                          \
    RandomInt<T> r2(a, b);                                                     \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      T t = r2.next();                                                         \
      REQUIRE(t >= (T)a);                                                      \
      REQUIRE(t <= (T)b);                                                      \
    }                                                                          \
  } while (0)

#define TEST_REAL(T, a, b)                                                     \
  do {                                                                         \
    RandomReal<T> r1;                                                          \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      T t = r1.next();                                                         \
      REQUIRE(t >= (T)0.0);                                                    \
      REQUIRE(t <= std::numeric_limits<T>::max());                             \
    }                                                                          \
    RandomReal<T> r2(a, b);                                                    \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      T t = r2.next();                                                         \
      REQUIRE(t >= (T)a);                                                      \
      REQUIRE(t <= (T)b);                                                      \
    }                                                                          \
  } while (0)

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
      for (int j = 0; j < (int)s.length(); j++) {                              \
        REQUIRE(g(s[j]));                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

static bool testIsAscii(int c) { return c >= 0 && c < 128; }

TEST_CASE("Random", "[Random]") {
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
  SECTION("real") {
    TEST_REAL(float, 1.28, 8.29);
    TEST_REAL(double, -0.183, 821.89e+4);
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
        for (int j = 0; j < (int)s.length(); j++) {
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

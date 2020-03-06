// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Random.h"
#include "Log.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>

#define TEST_MAX 1024

#define TEST_NUMBER(T, a, b)                                                   \
  do {                                                                         \
    RandomNumber<T> r1;                                                        \
    for (int i = 0; i < TEST_MAX; i++) {                                       \
      T t = r1.next();                                                         \
      REQUIRE(t >= (T)0);                                                      \
      REQUIRE(t <= std::numeric_limits<T>::max());                             \
    }                                                                          \
    RandomNumber<T> r2(a, b);                                                  \
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
      for (int j = 0; j < s.length(); j++) {                                   \
        REQUIRE(g(s[j]));                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

static bool testIsAscii(int c) { return c >= 0 && c < 128; }

TEST_CASE("Random", "[Random]") {
  SECTION("integer") {
    TEST_NUMBER(short, -1023, 193);
    TEST_NUMBER(unsigned short, 38, 193);
    TEST_NUMBER(int, 1023, 9331193);
    TEST_NUMBER(unsigned int, 138, 44193);
    TEST_NUMBER(long, -94L, 9393L);
    TEST_NUMBER(unsigned long, 8138L, 4483193L);
    TEST_NUMBER(long long, -9394LL, 129393LL);
    TEST_NUMBER(unsigned long long, 288138LL, 844483193LL);
    TEST_NUMBER(float, 1.28, 8.29);
    TEST_NUMBER(double, -0.183, 821.89e+4);
  }

  SECTION("real") {}

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

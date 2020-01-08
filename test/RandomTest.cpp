// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Random.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>

#define TEST_MAX 16384 * 7

#define assertValid(i, IntType, Name)                                          \
  do {                                                                         \
    IntType a = BOOST_PP_CAT(Random::next, Name)();                            \
    IntType b = BOOST_PP_CAT(Random::next, Name)(i);                           \
    IntType c =                                                                \
        BOOST_PP_CAT(Random::next, Name)(std::min(a, b), std::max(a, b));      \
    REQUIRE(a >= 0);                                                           \
    REQUIRE(a < std::numeric_limits<IntType>::max());                          \
    REQUIRE(b >= 0);                                                           \
    if (i > 0) {                                                               \
      REQUIRE(b < i);                                                          \
    }                                                                          \
    REQUIRE(c >= std::min(a, b));                                              \
    REQUIRE(c < std::max(a, b));                                               \
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

TEST_CASE("Random", "[Random]") {
  Random::initialize();

  SECTION("int") {
    INFO("int");
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      assertValid(i, int, Int);
    }
  }

  SECTION("unsigned int") {
    INFO("unsigned int");
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      assertValid(i, unsigned int, UInt);
    }
  }

  SECTION("long") {
    INFO("long");
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      assertValid(i, long, Long);
    }
  }

  SECTION("unsigned long") {
    INFO("unsigned long");
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      assertValid(i, unsigned long, ULong);
    }
  }

  SECTION("long long") {
    INFO("long long");
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      assertValid(i, long long, LLong);
    }
  }

  SECTION("unsigned long long") {
    INFO("unsigned long long");
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      assertValid(i, unsigned long long, ULLong);
    }
  }

  SECTION("alpha") {
    // alpha
    assertLiteral(Random::nextAlphaChar, Random::nextAlpha, std::isalpha);
    // digit
    assertLiteral(Random::nextDigitChar, Random::nextDigit, std::isdigit);
    // hex
    assertLiteral(Random::nextHexChar, Random::nextHex, std::isxdigit);
    // alpha numeric
    assertLiteral(Random::nextAlphaNumericChar, Random::nextAlphaNumeric,
                  std::isalnum);
    // punctuation
    assertLiteral(Random::nextPunctuationChar, Random::nextPunctuation,
                  std::ispunct);
    // printable
    assertLiteral(Random::nextPrintableChar, Random::nextPrintable,
                  std::isprint);
    // control
    assertLiteral(Random::nextControlChar, Random::nextControl, std::iscntrl);
  }
}

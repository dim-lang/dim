// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Random.h"
#include "Logging.h"
#include "catch2/catch.hpp"
#include <algorithm>
#include <boost/preprocessor/cat.hpp>
#include <cctype>
#include <cstddef>
#include <cstdint>

#define TEST_MAX 16384 * 7

#define assertValid(i, IntType, Name)                                          \
  do {                                                                         \
    IntType a = BOOST_PP_CAT(fastype::Random::next, Name)();                   \
    IntType b = BOOST_PP_CAT(fastype::Random::next, Name)(i);                  \
    IntType c = BOOST_PP_CAT(fastype::Random::next, Name)(std::min(a, b),      \
                                                          std::max(a, b));     \
    REQUIRE(a >= 0);                                                           \
    REQUIRE(a < std::numeric_limits<IntType>::max());                          \
    REQUIRE(b >= 0);                                                           \
    if (i > 0) {                                                               \
      REQUIRE(b < i);                                                          \
    }                                                                          \
    REQUIRE(c >= std::min(a, b));                                              \
    REQUIRE(c < std::max(a, b));                                               \
  } while (0)

TEST_CASE("Random", "[Random]") {
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
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      char c = fastype::Random::nextAlphaChar();
      std::string s = fastype::Random::nextAlpha(i + 1);
      REQUIRE(std::isalpha(c));
      std::for_each(s.begin(), s.end(),
                    [](const char &t) { REQUIRE(std::isalpha(t)); });
    }
  }

  SECTION("digit") {
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      char c = fastype::Random::nextDigitChar();
      std::string s = fastype::Random::nextDigit(i + 1);
      REQUIRE(std::isdigit(c));
      std::for_each(s.begin(), s.end(),
                    [](const char &t) { REQUIRE(std::isdigit(t)); });
    }
  }

  SECTION("hex") {
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      char c = fastype::Random::nextHexChar();
      std::string s = fastype::Random::nextHex(i + 1);
      REQUIRE(std::isxdigit(c));
      std::for_each(s.begin(), s.end(),
                    [](const char &t) { REQUIRE(std::isxdigit(t)); });
    }
  }

  SECTION("alpha numeric") {
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      char c = fastype::Random::nextAlphaNumericChar();
      std::string s = fastype::Random::nextAlphaNumeric(i + 1);
      REQUIRE(std::isalnum(c));
      std::for_each(s.begin(), s.end(),
                    [](const char &t) { REQUIRE(std::isalnum(t)); });
    }
  }

  SECTION("punctuation") {
    for (int i = 0; i < TEST_MAX; i = std::max(i * 3, i + 1)) {
      char c = fastype::Random::nextPunctuationChar();
      std::string s = fastype::Random::nextPunctuation(i + 1);
      REQUIRE(std::ispunct(c));
      std::for_each(s.begin(), s.end(),
                    [](const char &t) { REQUIRE(std::ispunct(t)); });
    }
  }
}

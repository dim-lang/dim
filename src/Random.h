// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include <boost/preprocessor/cat.hpp>
#include <cstdint>
#include <limits>
#include <random>
#include <string>
#include <unicode/uUChar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class Random {
public:
  static void initialize();

#define F_DECL_RAND(Type, Name)                                                \
  static Type BOOST_PP_CAT(next, Name)(Type right =                            \
                                           std::numeric_limits<Type>::max());  \
  static Type BOOST_PP_CAT(next, Name)(Type left, Type right)

  // @return   [0, right)
  // @return   [left, right)
  F_DECL_RAND(short, Short);
  F_DECL_RAND(unsigned short, UShort);
  F_DECL_RAND(int, Int);
  F_DECL_RAND(unsigned int, UInt);
  F_DECL_RAND(long, Long);
  F_DECL_RAND(unsigned long, ULong);
  F_DECL_RAND(long long, LLong);
  F_DECL_RAND(unsigned long long, ULLong);

#undef F_DECL_RAND

  static icu::UnicodeString nextAlpha(int len = 1);
  static icu::UnicodeString nextAlphaNumeric(int len = 1);
  static icu::UnicodeString nextDigit(int len = 1);
  static icu::UnicodeString nextHex(int len = 1);
  static icu::UnicodeString nextPunctuation(int len = 1);
  static icu::UnicodeString nextPrintable(int len = 1);
  static icu::UnicodeString nextWhitespace(int len = 1);
  static icu::UnicodeString nextControl(int len = 1);
  static icu::UnicodeString nextAscii(int len = 1);
  static icu::UnicodeString nextString(const UChar *candidates, int n,
                                       int len = 1);
  static icu::UnicodeString nextString(const icu::UnicodeString &candidates,
                                       int len = 1);

  static UChar nextAlphaChar();
  static UChar nextAlphaNumericChar();
  static UChar nextDigitChar();
  static UChar nextHexChar();
  static UChar nextPunctuationChar();
  static UChar nextPrintableChar();
  static UChar nextWhitespaceChar();
  static UChar nextControlChar();
  static UChar nextAsciiChar();
  static UChar nextChar(const UChar *candidates, int n);
  static UChar nextChar(const icu::UnicodeString &candidates);

private:
  static UChar nextAsciiChar(const std::vector<std::pair<int, int>> &range,
                             int rangeLength);
  static icu::UnicodeString
  nextAsciiString(const std::vector<std::pair<int, int>> &range,
                  int rangeLength, int len);

  static std::random_device &device();
  static std::mt19937 &engine();
  static std::uniform_int_distribution<long> &longDistribution();
  static std::uniform_int_distribution<unsigned long> &ulongDistribution();
  static std::uniform_int_distribution<long long> &llongDistribution();
  static std::uniform_int_distribution<unsigned long long> &
  ullongDistribution();

  static std::random_device *device_;
  static std::mt19937 *engine_;
  static std::uniform_int_distribution<long> *longDist_;
  static std::uniform_int_distribution<unsigned long> *ulongDist_;
  static std::uniform_int_distribution<long long> *llongDist_;
  static std::uniform_int_distribution<unsigned long long> *ullongDist_;
};

} // namespace fastype

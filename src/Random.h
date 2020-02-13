// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Log.h"
#include "boost/preprocessor/cat.hpp"
#include "unicode/uchar.h"
#include "unicode/unistr.h"
#include "unicode/ustdio.h"
#include "unicode/ustring.h"
#include "unicode/utypes.h"
#include <cstdint>
#include <cstring>
#include <limits>
#include <random>
#include <string>

class Random {
public:
  static void initialize();

#define FDECL_RAND(Type, Name)                                                 \
  static Type BOOST_PP_CAT(next, Name)(Type right =                            \
                                           std::numeric_limits<Type>::max());  \
  static Type BOOST_PP_CAT(next, Name)(Type left, Type right)

  /**
   * generate API like:
   *
   * @return  random integer range in [0, right)
   * static int nextInt(int right = std::numeric_limits<int>::max());
   *
   * @return  random integer range in [left, right)
   * static int nextInt(int left, int right);
   */

  FDECL_RAND(short, Short);
  FDECL_RAND(unsigned short, UShort);
  FDECL_RAND(int, Int);
  FDECL_RAND(unsigned int, UInt);
  FDECL_RAND(long, Long);
  FDECL_RAND(unsigned long, ULong);
  FDECL_RAND(long long, LLong);
  FDECL_RAND(unsigned long long, ULLong);

#undef FDECL_RAND

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
  static UChar nextCharImpl(const std::vector<std::pair<int, int>> &range,
                            int n);
  static icu::UnicodeString
  nextStringImpl(const std::vector<std::pair<int, int>> &range, int n, int len);

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

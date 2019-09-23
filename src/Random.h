// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include <boost/preprocessor/cat.hpp>
#include <cstdint>
#include <limits>
#include <random>
#include <string>

namespace fastype {

class Random {
public:
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

  static std::string nextAlpha(int len = 1);
  static std::string nextAlphaNumeric(int len = 1);
  static std::string nextDigit(int len = 1);
  static std::string nextHex(int len = 1);
  static std::string nextPunctuation(int len = 1);
  static std::string nextPrintable(int len = 1);
  static std::string nextControl(int len = 1);
  static std::string nextAscii(int len = 1);
  static std::string nextString(const char *candidates, int n, int len = 1);
  static std::string nextString(const std::string &candidates, int len = 1);

  static char nextAlphaChar();
  static char nextAlphaNumericChar();
  static char nextDigitChar();
  static char nextHexChar();
  static char nextPunctuationChar();
  static char nextPrintableChar();
  static char nextControlChar();
  static char nextAsciiChar();
  static char nextChar(const char *candidates, int n);
  static char nextChar(const std::string &candidates);

private:
  static char nextAsciiChar(const std::vector<std::pair<int, int>> &range,
                            int rangeLength);
  static std::string
  nextAsciiString(const std::vector<std::pair<int, int>> &range,
                  int rangeLength, int len);

  static std::random_device device_;
  static std::mt19937 engine_;
  static std::uniform_int_distribution<long> long_;
  static std::uniform_int_distribution<unsigned long> ulong_;
  static std::uniform_int_distribution<long long> llong_;
  static std::uniform_int_distribution<unsigned long long> ullong_;
};

} // namespace fastype

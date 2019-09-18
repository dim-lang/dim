// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include <boost/preprocessor/cat.hpp>
#include <cmath>
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

  // @return   [0.0F, 1.0F)
  static float nextFloat();
  static double nextDouble();

#undef F_DECL_RAND

  static std::string nextAlpha(int limit);

  static std::string nextAlphaNumeric(int limit);

  static std::string nextDigit(int limit);

  static std::string nextString(int limit, const char *candidates, int c);

  static std::string nextString(int limit, const std::string &candidates);

private:
  static std::random_device device_;
  static std::mt19937 engine_;
  static std::uniform_int_distribution<long> long_;
  static std::uniform_int_distribution<unsigned long> ulong_;
  static std::uniform_int_distribution<long long> llong_;
  static std::uniform_int_distribution<unsigned long long> ullong_;
  static std::uniform_real_distribution<float> float_;
  static std::uniform_real_distribution<double> double_;
};

} // namespace fastype

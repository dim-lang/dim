// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <limits>

namespace fastype {

class Random {
public:
  static int nextInt(int start = std::numeric_limits<int>::min(),
                     int limit = std::numeric_limits<int>::max());

  static unsigned int
  nextUInt(unsigned int start = std::numeric_limits<unsigned int>::min(),
           unsigned int limit = std::numeric_limits<unsigned int>::max());

  static long nextLong(long start = std::numeric_limits<long>::min(),
                       long limit = std::numeric_limits<long>::max());

  static unsigned long
  nextULong(unsigned long start = std::numeric_limits<unsigned long>::min(),
            unsigned long limit = std::numeric_limits<unsigned long>::max());

  static float nextFloat(float start = std::numeric_limits<float>::min(),
                         float limit = std::numeric_limits<float>::max());

  static double nextDouble(double start = std::numeric_limits<double>::min(),
                           double limit = std::numeric_limits<double>::max());
};

} // namespace fastype

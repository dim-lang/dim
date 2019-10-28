// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Math.h"
#include <cmath>

namespace fastype {

bool Approximate::eq(float a, float b, float epsilon) {
  return std::fabs(a - b) <=
         ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) *
          epsilon);
}

bool Approximate::gt(float a, float b, float epsilon) {
  return (a - b) >
         ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) *
          epsilon);
}

bool Approximate::lt(float a, float b, float epsilon) {
  return (b - a) >
         ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) *
          epsilon);
}

} // namespace fastype

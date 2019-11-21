// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Approximate.h"
#include <cmath>
#include <cstdlib>

//#define F_ABS(x) ((x < 0.0) ? -x : x)
#define F_ABS(x) std::abs(x)
#define F_MAX(a, b) ((a) > (b) ? (a) : (b))
#define F_MIN(a, b) ((a) < (b) ? (a) : (b))

namespace fastype {

bool Approximate::eq(float a, float b, float epsilon) {
  return F_ABS(a - b) <=
         ((F_ABS(a) < F_ABS(b) ? F_ABS(b) : F_ABS(a)) * epsilon);
}

bool Approximate::eq(double a, double b, double epsilon) {
  return F_ABS(a - b) <=
         ((F_ABS(a) < F_ABS(b) ? F_ABS(b) : F_ABS(a)) * epsilon);
}

bool Approximate::gt(float a, float b, float epsilon) {
  return (a - b) > ((F_ABS(a) < F_ABS(b) ? F_ABS(b) : F_ABS(a)) * epsilon);
}

bool Approximate::gt(double a, double b, double epsilon) {
  return (a - b) > ((F_ABS(a) < F_ABS(b) ? F_ABS(b) : F_ABS(a)) * epsilon);
}

bool Approximate::lt(float a, float b, float epsilon) {
  return (b - a) > ((F_ABS(a) < F_ABS(b) ? F_ABS(b) : F_ABS(a)) * epsilon);
}

bool Approximate::lt(double a, double b, double epsilon) {
  return (b - a) > ((F_ABS(a) < F_ABS(b) ? F_ABS(b) : F_ABS(a)) * epsilon);
}

} // namespace fastype

#undef F_ABS
#undef F_MAX
#undef F_MIN

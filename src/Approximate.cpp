// Copyright 2019- <rsc-lang>
// Apache License Version 2.0

#include "Approximate.h"
#include <cmath>
#include <cstdlib>

//#define FABS(x) ((x < 0.0) ? -x : x)
#define FABS(x) std::abs(x)
#define FMAX(a, b) ((a) > (b) ? (a) : (b))
#define FMIN(a, b) ((a) < (b) ? (a) : (b))

bool Approximate::eq(float a, float b, float epsilon) {
  return FABS(a - b) <= ((FABS(a) < FABS(b) ? FABS(b) : FABS(a)) * epsilon);
}

bool Approximate::eq(double a, double b, double epsilon) {
  return FABS(a - b) <= ((FABS(a) < FABS(b) ? FABS(b) : FABS(a)) * epsilon);
}

bool Approximate::gt(float a, float b, float epsilon) {
  return (a - b) > ((FABS(a) < FABS(b) ? FABS(b) : FABS(a)) * epsilon);
}

bool Approximate::gt(double a, double b, double epsilon) {
  return (a - b) > ((FABS(a) < FABS(b) ? FABS(b) : FABS(a)) * epsilon);
}

bool Approximate::lt(float a, float b, float epsilon) {
  return (b - a) > ((FABS(a) < FABS(b) ? FABS(b) : FABS(a)) * epsilon);
}

bool Approximate::lt(double a, double b, double epsilon) {
  return (b - a) > ((FABS(a) < FABS(b) ? FABS(b) : FABS(a)) * epsilon);
}

#undef FABS
#undef FMAX
#undef FMIN

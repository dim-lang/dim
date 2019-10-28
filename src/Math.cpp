// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Math.h"

namespace fastype {

bool Approximate::eq(float a, float b, float epsilon) {
  return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool Approximate::gt(float a, float b, float epsilon) {
  return (a - b) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool Approximate::lt(float a, float b, float epsilon) {
  return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

} // namespace fastype

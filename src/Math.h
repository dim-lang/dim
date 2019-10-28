// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cmath>

namespace fastype {

class Approximate {
public:
  static bool eq(float a, float b, float epsilon);

  static bool gt(float a, float b, float epsilon);

  static bool lt(float a, float b, float epsilon);
};

} // namespace fastype

// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

namespace fastype {

class Util {
public:
  // in range include: a <= x <= b
  static bool inRangeInc(int x, int a, int b);

  // in range exclude: a < x < b
  static bool inRangeExc(int x, int a, int b);

  // out range include: x <= a, x >= b
  static bool outRangeInc(int x, int a, int b);

  // out range exclude: x < a, x > b
  static bool outRangeExc(int x, int a, int b);
};

} // namespace fastype

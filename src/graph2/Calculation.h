// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "view/Area.h"

namespace fastype {

class Calculation {
public:
  static bool inRangeInc(int x, int a, int y);  // in range include
  static bool inRangeExc(int x, int a, int y);  // in range exclude
  static bool outRangeInc(int x, int a, int y); // out range include
  static bool outRangeExc(int x, int a, int y); // out range exclude

  static bool insideInc(const Area &a, const Area &b); // inside include
  static bool insideExc(const Area &a, const Area &b); // inside include
  static bool outsideInc(int x, int a, int y);         // outside exclude
  static bool outsideExc(int x, int a, int y);         // outside exclude
};

} // namespace fastype

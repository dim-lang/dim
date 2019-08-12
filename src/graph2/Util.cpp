// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "graph2/Util.h"

namespace fastype {

bool Util::inRangeInc(int x, int a, int b) { return a <= x && x <= b; }

bool Util::inRangeExc(int x, int a, int b) { return a < x && x < b; }

bool Util::outRangeInc(int x, int a, int b) { return x <= a || x >= b; }

bool Util::outRangeExc(int x, int a, int b) { return x < a || x > b; }

} // namespace fastype

// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "graph2/Dot2.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

Dot2::Dot2() : x_(-1), y_(-1) {}

Dot2::Dot2(int x, int y) : x_(x), y_(y) {}

const int &Dot2::x() const { return x_; }

Dot2 &Dot2::setX(int x) {
  std::swap(x_, x);
  return *this;
}

const int &Dot2::y() const { return y_; }

Dot2 &Dot2::setY(int y) {
  std::swap(y_, y);
  return *this;
}

bool Dot2::operator==(const Dot2 &other) const {
  return x_ == other.x_ && y_ == other.y_;
}

bool Dot2::operator!=(const Dot2 &other) const { return !(*this == other); }

std::string Dot2::toString() const {
  return fmt::format("[ @Dot2 x_:{} y_:{} ]", x_, y_);
}

} // namespace fastype

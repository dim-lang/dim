// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "graph/Dot.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

Dot::Dot() : x_(-1), y_(-1) {}

Dot::Dot(int x, int y) : x_(x), y_(y) {}

const int &Dot::x() const { return x_; }

Dot &Dot::setX(int x) {
  std::swap(x_, x);
  return *this;
}

const int &Dot::y() const { return y_; }

Dot &Dot::setY(int y) {
  std::swap(y_, y);
  return *this;
}

bool Dot::operator==(const Dot &other) const {
  return x_ == other.x_ && y_ == other.y_;
}

bool Dot::operator!=(const Dot &other) const { return !(*this == other); }

std::string Dot::toString() const {
  return fmt::format("[ @Dot x_:{} y_:{} ]", x_, y_);
}

} // namespace fastype

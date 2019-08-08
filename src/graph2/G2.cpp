// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "graph2/G2.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

G2::G2() : Logging("G2"), x_(-1), y_(-1) {}

G2::G2(int x, int y) : Logging("G2"), x_(x), y_(y) {}

const int &G2::x() const { return x_; }

G2 &G2::setX(int x) {
  std::swap(x_, x);
  return *this;
}

const int &G2::y() const { return y_; }

G2 &G2::setY(int y) {
  std::swap(y_, y);
  return *this;
}

bool G2::operator==(const G2 &other) const {
  return x_ == other.x_ && y_ == other.y_;
}

bool G2::operator!=(const G2 &other) const { return !(*this == other); }

std::string G2::toString() const {
  return fmt::format("[ @G2 x_:{} y_:{} ]", x_, y_);
}

} // namespace fastype

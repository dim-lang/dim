// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "G2.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

G2::G2() : Logging("G2"), x_(-1), y_(-1) {}

G2::G2(int x, int y) : Logging("G2"), x_(x), y_(y) {}

const int &G2::x() const { return x_; }

int G2::setX(int x) { return std::exchange(x_, x); }

const int &G2::y() const { return y_; }

int G2::setY(int y) { return std::exchange(y_, y); }

bool G2::operator==(const G2 &other) const {
  return x_ == other.x_ && y_ == other.y_;
}

bool G2::operator!=(const G2 &other) const { return !(*this == other); }

std::string G2::toString() const {
  return fmt::format("[ @G2 x_:{} y_:{} ]", x_, y_);
}

} // namespace fastype

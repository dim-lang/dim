// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Position.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

Position::Position() : dot2_() {}

Position::Position(int x, int y) : dot2_(x, y) {
  F_CHECKF(x >= 0, "x {} >= 0", x);
  F_CHECKF(y >= 0, "y {} >= 0", y);
}

const int &Position::x() const { return dot2_.x(); }

void Position::setX(int x) { dot2_.setX(x); }

const int &Position::y() const { return dot2_.y(); }

void Position::setY(int y) { dot2_.setY(y); }

bool Position::operator==(const Position &other) const {
  return dot2_ == other.dot2_;
}

bool Position::operator!=(const Position &other) const {
  return dot2_ != other.dot2_;
}

Position Position::operator-(const Vec &v) const {
  return Position(dot2_.x() - v.x(), dot2_.y() - v.y());
}

Position &Position::operator-=(const Vec &v) {
  dot2_.setX(dot2_.x() - v.x());
  dot2_.setY(dot2_.y() - v.y());
  return *this;
}

Position Position::operator+(const Vec &v) const {
  return Position(dot2_.x() + v.x(), dot2_.y() + v.y());
}

Position &Position::operator+=(const Vec &v) {
  dot2_.setX(dot2_.x() + v.x());
  dot2_.setY(dot2_.y() + v.y());
  return *this;
}

std::string Position::toString() const {
  return fmt::format("[ @Position x_:{} y_:{} ]", dot2_.x(), dot2_.y());
}

} // namespace fastype

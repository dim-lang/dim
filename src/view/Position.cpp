// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Position.h"
#include "Logging.h"
#include "Profile.h"
#include <fmt/format.h>
#include <utility>

namespace fastype {

Position::Position() : dot_() {}

Position::Position(int x, int y) : dot_(x, y) {
  F_CHECKF(x >= 0, "x {} >= 0", x);
  F_CHECKF(y >= 0, "y {} >= 0", y);
}

const int &Position::x() const { return dot_.x(); }

void Position::setX(int x) { dot_.setX(x); }

const int &Position::y() const { return dot_.y(); }

void Position::setY(int y) { dot_.setY(y); }

bool Position::operator==(const Position &other) const {
  return dot_ == other.dot_;
}

bool Position::operator!=(const Position &other) const {
  return dot_ != other.dot_;
}

Position Position::operator-(const Vec &v) const {
  return Position(dot_.x() - v.x(), dot_.y() - v.y());
}

Position &Position::operator-=(const Vec &v) {
  dot_.setX(dot_.x() - v.x());
  dot_.setY(dot_.y() - v.y());
  return *this;
}

Position Position::operator+(const Vec &v) const {
  return Position(dot_.x() + v.x(), dot_.y() + v.y());
}

Position &Position::operator+=(const Vec &v) {
  dot_.setX(dot_.x() + v.x());
  dot_.setY(dot_.y() + v.y());
  return *this;
}

std::string Position::toString() const {
  return fmt::format("[ @Position x_:{} y_:{} ]", dot_.x(), dot_.y());
}

} // namespace fastype

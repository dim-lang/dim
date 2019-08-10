// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Position.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

Position::Position() : g2_() {}

Position::Position(int x, int y) : g2_(x, y) {
  F_CHECKF(x >= 0, "x {} >= 0", x);
  F_CHECKF(y >= 0, "y {} >= 0", y);
}

const int &Position::x() const { return g2_.x(); }

void Position::setX(int x) { g2_.setX(x); }

const int &Position::y() const { return g2_.y(); }

void Position::setY(int y) { g2_.setY(y); }

bool Position::operator==(const Position &other) const {
  return g2_ == other.g2_;
}

bool Position::operator!=(const Position &other) const {
  return g2_ != other.g2_;
}

Position Position::operator-(const Vec &v) const {
  return Position(g2_.x() - v.x(), g2_.y() - v.y());
}

Position &Position::operator-=(const Vec &v) {
  g2_.setX(g2_.x() - v.x());
  g2_.setY(g2_.y() - v.y());
  return *this;
}

Position Position::operator+(const Vec &v) const {
  return Position(g2_.x() + v.x(), g2_.y() + v.y());
}

Position &Position::operator+=(const Vec &v) {
  g2_.setX(g2_.x() + v.x());
  g2_.setY(g2_.y() + v.y());
  return *this;
}

std::string Position::toString() const {
  return fmt::format("[ @Position x_:{} y_:{} ]", g2_.x(), g2_.y());
}

} // namespace fastype

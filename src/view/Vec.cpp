// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Vec.h"
#include "G2.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

Vec::Vec() : g2_() {}

Vec::Vec(int x, int y) : g2_(x, y) {}

Vec::Vec(const Position &a, const Position &b)
    : g2_(b.x() - a.x(), b.y() - a.y()) {}

const int &Vec::x() const { return g2_.x(); }

void Vec::setX(int x) { g2_.setX(x); }

const int &Vec::y() const { return g2_.y(); }

void Vec::setY(int y) { g2_.setY(y); }

bool Vec::operator==(const Vec &other) const { return g2_ == other.g2_; }

bool Vec::operator!=(const Vec &other) const { return g2_ != other.g2_; }

std::string Vec::toString() const {
  return fmt::format("[ @Vec x_:{} y_:{} ]", g2_.x(), g2_.y());
}

} // namespace fastype

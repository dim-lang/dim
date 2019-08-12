// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Vec.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

Vec::Vec() : dot2_() {}

Vec::Vec(int x, int y) : dot2_(x, y) {}

const int &Vec::x() const { return dot2_.x(); }

void Vec::setX(int x) { dot2_.setX(x); }

const int &Vec::y() const { return dot2_.y(); }

void Vec::setY(int y) { dot2_.setY(y); }

bool Vec::operator==(const Vec &other) const { return dot2_ == other.dot2_; }

bool Vec::operator!=(const Vec &other) const { return dot2_ != other.dot2_; }

std::string Vec::toString() const {
  return fmt::format("[ @Vec x_:{} y_:{} ]", dot2_.x(), dot2_.y());
}

} // namespace fastype

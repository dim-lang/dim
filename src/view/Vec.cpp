// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Vec.h"
#include "Logging.h"
#include <fmt/format.h>
#include <utility>

namespace fastype {

Vec::Vec() : dot_() {}

Vec::Vec(int x, int y) : dot_(x, y) {}

const int &Vec::x() const { return dot_.x(); }

void Vec::setX(int x) { dot_.setX(x); }

const int &Vec::y() const { return dot_.y(); }

void Vec::setY(int y) { dot_.setY(y); }

bool Vec::operator==(const Vec &other) const { return dot_ == other.dot_; }

bool Vec::operator!=(const Vec &other) const { return dot_ != other.dot_; }

std::string Vec::toString() const {
  return fmt::format("[ @Vec x_:{} y_:{} ]", dot_.x(), dot_.y());
}

} // namespace fastype

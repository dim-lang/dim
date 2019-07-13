// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "fmt/format.h"
#include <algorithm>
#include <utility>

namespace fastype {

bool Line::operator==(const Line &other) const {
  return lineNumber_ == other.lineNumber_;
}

bool Line::operator!=(const Line &other) const { return !(*this == other); }

bool Line::operator>(const Line &other) const {
  return lineNumber_ > other.lineNumber_;
}

bool Line::operator>=(const Line &other) const {
  return *this == other || *this > other;
}

bool Line::operator<(const Line &other) const {
  return lineNumber_ < other.lineNumber_;
}

bool Line::operator<=(const Line &other) const {
  return *this == other || *this < other;
}

bool Line::unsetLeft() const { return left_ == LineBound::undefined(); }

bool Line::unsetRight() const { return right_ == LineBound::undefined(); }

bool Line::unset() const {
  return left_ == LineBound::undefined() && right_ == LineBound::undefined();
}

const Line &Line::undefined() {
  static Line undef(-1);
  return undef;
}

int32_t Line::lineNumber() const { return lineNumber_; }

int32_t Line::setLineNumber(int32_t lineNumber_) {
  return std::exchange(lineNumber_, lineNumber_);
}

const LineBound &Line::left() const { return left_; }

LineBound Line::setLeft(const LineBound &left) {
  return std::exchange(left_, left);
}

const LineBound &Line::right() const { return right_; }

LineBound Line::setRight(const LineBound &right) {
  return std::exchange(right_, right);
}

std::string Line::toString() const {
  return fmt::format("[ @Line lineNumber: {}, left: {}, right: {}]",
                     lineNumber_, left_.toString(), right_.toString());
}

Line::Line(int32_t lineNumber, const LineBound &left, const LineBound &right)
    : Logging("Line"), lineNumber_(lineNumber), left_(left), right_(right) {}

Line::Line(int32_t lineNumber, const LineBound &left)
    : Logging("Line"), lineNumber_(lineNumber), left_(left), right_() {}

Line::Line(int32_t lineNumber)
    : Logging("Line"), lineNumber_(lineNumber), left_(), right_() {}

} // namespace fastype

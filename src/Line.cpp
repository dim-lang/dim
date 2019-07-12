// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include "fmt/format.h"
#include <algorithm>
#include <utility>

namespace fastype {

Line &Line::operator++() {
  Line tmp(*this);
  // if has next line, or set endline
  *this = (lineNumber_ + 1 < fp_->lineList_.size())
              ? fp_->lineList_[lineNumber_ + 1]
              : undefinedLine();
  return tmp;
}

Line &Line::operator--() {
  Line tmp(*this);
  // if has previous line, or set endline
  *this = (lineNumber_ - 1 >= 0) ? fp_->lineList_[lineNumber_ - 1]
                                 : undefinedLine();
  return tmp;
}

bool Line::sameFile(const Line &other) const { return fp_ == other.fp_; }

bool Line::operator==(const Line &other) const {
  return fp_ == other.fp_ && lineNumber_ == other.lineNumber_;
}

bool Line::operator!=(const Line &other) const { return !(*this == other); }

bool Line::operator>(const Line &other) const {
  return fp_ == other.fp_ && lineNumber_ > other.lineNumber_;
}

bool Line::operator>=(const Line &other) const {
  return *this == other || *this > other;
}

bool Line::operator<(const Line &other) const {
  return fp_ == other.fp_ && lineNumber_ < other.lineNumber_;
}

bool Line::operator<=(const Line &other) const {
  return *this == other || *this < other;
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
  return fmt::format("[ @Line fp: {}, lineNumber: {}, left: {}, right: {}]",
                     (void *)fp_.get(), lineNumber_, left_.toString(),
                     right_.toString());
}

Line::Line(std::shared_ptr<File> fp, int32_t lineNumber, const LineBound &left,
           const LineBound &right)
    : fp_(fp), lineNumber_(lineNumber), left_(left), right_(right) {}

Line::Line(std::shared_ptr<File> fp, int32_t lineNumber, const LineBound &left)
    : fp_(fp), lineNumber_(lineNumber), left_(left), right_() {}

Line::Line(std::shared_ptr<File> fp, int32_t lineNumber)
    : fp_(fp), lineNumber_(lineNumber), left_(), right_() {}

const Line &Line::undefinedLine() {
  static Line undef(std::shared_ptr<File>(nullptr), -1);
  return undef;
}

} // namespace fastype

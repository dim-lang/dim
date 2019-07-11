// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Line.h"
#include <algorithm>
#include <utility>

namespace fastype {

Line &Line::operator++() {
  Line tmp(fp, lineNumber, startBuffer, startByte, endBuffer, endByte);
  // if has next line, or set endline
  *this = (lineNumber + 1 < fp->lineList.size()) ? fp->lineList[lineNumber + 1]
                                                 : getEndLine();
  return tmp;
}

Line &Line::operator--() {
  Line tmp(fp, lineNumber, startBuffer, startByte, endBuffer, endByte);
  // if has previous line, or set endline
  *this = (lineNumber - 1 >= 0) ? fp->lineList[lineNumber - 1] : getEndLine();
  return tmp;
}

bool Line::sameFile(const Line &other) const { return fp == other.fp; }

bool Line::operator==(const Line &other) const {
  return fp == other.fp && lineNumber == other.lineNumber;
}

bool Line::operator!=(const Line &other) const { return !(*this == other); }

bool Line::operator>(const Line &other) const {
  return fp == other.fp && lineNumber > other.lineNumber;
}

bool Line::operator>=(const Line &other) const {
  return *this == other || *this > other;
}

bool Line::operator<(const Line &other) const {
  return fp == other.fp && lineNumber < other.lineNumber;
}

bool Line::operator<=(const Line &other) const {
  return *this == other || *this < other;
}

int32_t Line::lineNumber() const { return lineNumber_; }

int32_t Line::setLineNumber(int32_t lineNumber) {
  return std::exchange(lineNumber_, lineNumber);
}

LineBound &Line::left() const { return left_; }

LineBound Line::setLeft(const LineBound &left) {
  return std::exchange(left_, left);
}

LineBound &Line::right() const { return right_; }

LineBound Line::setRight(const LineBound &right) {
  return std::exchange(right_, right);
}

Line::Line(std::shared_ptr<File> fp, int32_t lineNumber, const LineBound &left,
           const LineBound &right)
    : fp(fp), lineNumber(lineNumber), left_(left), right_(right) {}

Line::Line(std::shared_ptr<File> fp, int32_t lineNumber, const LineBound &left)
    : fp(fp), lineNumber(lineNumber), left_(left), right_() {}

Line::Line(std::shared_ptr<File> fp, int32_t lineNumber)
    : fp(fp), lineNumber(lineNumber), left_(), right_() {}

const Line &Line::undefinedLine() {
  static Line undef(std::shared_ptr<File>(nullptr), -1);
  return undef;
}

} // namespace fastype

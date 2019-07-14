// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "Position.h"
#include "Util.h"
#include "exceptions/NoSuchOrderException.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

Position::Position() : row_(-1), col_(-1) {}

Position::Position(int32_t row, int32_t column) : row_(row), col_(column) {}

int32_t Position::row() const { return row_; }

int32_t Position::setRow(int32_t row) { return std::exchange(row_, row); }

int32_t Position::column() const { return col_; }

int32_t Position::setColumn(int32_t column) {
  return std::exchange(col_, column);
}

bool Position::operator==(const Position &other) const {
  return row_ == other.row_ && col_ == other.col_;
}

bool Position::operator!=(const Position &other) const {
  return !(*this == other);
}

bool Position::operator>(const Position &other) const {
  if (row_ != other.row_ && col_ != other.col_) {
    F_THROW_EXF(NoSuchOrderException, "this: {} other: {}", toString(),
                other.toString());
  }
  if (row_ == other.row_) {
    return col_ > other.col_;
  } else {
    return row_ > other.row_;
  }
}

bool Position::operator>=(const Position &other) const {
  return *this > other || *this == other;
}

bool Position::operator<(const Position &other) const {
  if (row_ != other.row_ && col_ != other.col_) {
    F_THROW_EXF(NoSuchOrderException, "this: {} other: {}", toString(),
                other.toString());
  }
  if (row_ == other.row_) {
    return col_ < other.col_;
  } else {
    return row_ < other.row_;
  }
}

bool Position::operator<=(const Position &other) const {
  return *this < other || *this == other;
}

std::string Position::toString() const {
  return fmt::format("[ @Position row_: {}, col_: {}]", row_, col_);
}

} // namespace fastype

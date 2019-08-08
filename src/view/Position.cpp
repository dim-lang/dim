// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Position.h"
#include "G2.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include <utility>

namespace fastype {

Position::Position() : g2_() {}

Position::Position(int row, int column) : g2_(row, column) {
  F_CHECKF(row >= 0, "row {} >= 0", row);
  F_CHECKF(column >= 0, "col {} >= 0", column);
}

const int &Position::row() const { return g2_.x(); }

Position &Position::setRow(int row) {
  g2_.setX(row);
  return *this;
}

const int &Position::column() const { return g2_.y(); }

Position &Position::setColumn(int column) {
  g2_.setY(column);
  return *this;
}

bool Position::operator==(const Position &other) const {
  return g2_ == other.g2_;
}

bool Position::operator!=(const Position &other) const {
  return g2_ != other.g2_;
}

std::string Position::toString() const {
  return fmt::format("[ @Position row_:{} col_:{} ]", g2_.x(), g2_.y());
}

} // namespace fastype

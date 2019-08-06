// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "view/Position.h"
#include "Logging.h"
#include "Profile.h"
#include "fmt/format.h"
#include "view/G2.h"
#include <utility>

namespace fastype {

Position::Position() : g2_() {}

Position::Position(int row, int column) : g2_(row, col) {
  F_CHECKF(row >= 0, "row {} >= 0", row);
  F_CHECKF(column >= 0, "col {} >= 0", column);
}

const int &Position::row() const { return g2_.x(); }

int Position::setRow(int row) { return g2_.setX(row); }

const int &Position::column() const { return g2_.y(); }

int Position::setColumn(int column) { return g2_.setY(column); }

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

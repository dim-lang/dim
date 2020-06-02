// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Position.h"

#define INVALID -1

Position::Position(int firstLine, int firstColumn, int lastLine, int lastColumn)
    : firstLine_(firstLine), firstColumn_(firstColumn), lastLine_(lastLine),
      lastColumn_(lastColumn) {}

const int &Position::firstLine() const { return firstLine_; }

const int &Position::firstColumn() const { return firstColumn_; }

const int &Position::lastLine() const { return lastLine_; }

const int &Position::lastColumn() const { return lastColumn_; }

Position &Position::updateFirstLine(int value) {
  firstLine_ =
      firstLine_ < 0 ? value : (firstLine_ > value ? value : firstLine_);
  return *this;
}

Position &Position::updateFirstColumn(int value) {
  firstColumn_ =
      firstColumn_ < 0 ? value : (firstColumn_ > value ? value : firstColumn_);
  return *this;
}

Position &Position::updateLastLine(int value) {
  lastLine_ = lastLine_ < 0 ? value : (lastLine_ > value ? lastLine_ : value);
  return *this;
}

Position &Position::updateLastColumn(int value) {
  lastColumn_ =
      lastColumn_ < 0 ? value : (lastColumn_ > value ? lastColumn_ : value);
  return *this;
}

const Position &Position::invalid() {
  static Position inv(-1, -1, -1, -1);
  return inv;
}

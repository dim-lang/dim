// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Position.h"
#include "Log.h"

#define INVALID -1
#define IS_INVALID(x) (x < 0)

Position::Position()
    : firstLine_(INVALID), firstColumn_(INVALID), lastLine_(INVALID),
      lastColumn_(INVALID) {}

Position::Position(int firstLine, int firstColumn, int lastLine, int lastColumn)
    : firstLine(firstLine), firstColumn(firstColumn), lastLine(lastLine),
      lastColumn(lastColumn) {}

Position &Position::updatePosition(const Position &position) {
  updateFirstLine(position.firstLine());
  updateFirstColumn(position.firstColumn());
  updateLastLine(position.lastLine());
  updateLastColumn(position.lastColumn());
  return *this;
}

Position &Position::updatePosition(const Position *position) {
  LOG_ASSERT(position, "position is null");
  updateFirstLine(position->firstLine());
  updateFirstColumn(position->firstColumn());
  updateLastLine(position->lastLine());
  updateLastColumn(position->lastColumn());
  return *this;
}

Position &Position::updateFirstLine(int value) {
  firstLine_ = IS_INVALID(firstLine_)
                   ? value
                   : (firstLine_ > value ? value : firstLine_);
  return *this;
}

Position &Position::updateFirstColumn(int value) {
  firstColumn_ = IS_INVALID(firstColumn_)
                     ? value
                     : (firstColumn_ > value ? value : firstColumn_);
  return *this;
}

Position &Position::updateLastLine(int value) {
  lastLine_ =
      IS_INVALID(lastLine_) ? value : (lastLine_ > value ? lastLine_ : value);
  return *this;
}

Position &Position::updateLastColumn(int value) {
  lastColumn_ = IS_INVALID(lastColumn_)
                    ? value
                    : (lastColumn_ > value ? lastColumn_ : value);
  return *this;
}

#undef INVALID
#undef IS_INVALID

// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Position.h"
#include "Log.h"

#define INVALID -1
#define IS_INVALID(x) (x < 0)

Position::Position()
    : firstLine(INVALID), firstColumn(INVALID), lastLine(INVALID),
      lastColumn(INVALID) {}

Position::Position(int firstLine, int firstColumn, int lastLine, int lastColumn)
    : firstLine(firstLine), firstColumn(firstColumn), lastLine(lastLine),
      lastColumn(lastColumn) {}

Position &Position::updatePosition(const Position &position) {
  updateFirstLine(position.firstLine);
  updateFirstColumn(position.firstColumn);
  updateLastLine(position.lastLine);
  updateLastColumn(position.lastColumn);
  return *this;
}

Position &Position::updatePosition(const Position *position) {
  LOG_ASSERT(position, "position is null");
  updateFirstLine(position->firstLine);
  updateFirstColumn(position->firstColumn);
  updateLastLine(position->lastLine);
  updateLastColumn(position->lastColumn);
  return *this;
}

Position &Position::updateFirstLine(int value) {
  if (IS_INVALID(value)) {
    return *this;
  }
  firstLine =
      IS_INVALID(firstLine) ? value : (firstLine > value ? value : firstLine);
  return *this;
}

Position &Position::updateFirstColumn(int value) {
  if (IS_INVALID(value)) {
    return *this;
  }
  firstColumn = IS_INVALID(firstColumn)
                    ? value
                    : (firstColumn > value ? value : firstColumn);
  return *this;
}

Position &Position::updateLastLine(int value) {
  if (IS_INVALID(value)) {
    return *this;
  }
  lastLine =
      IS_INVALID(lastLine) ? value : (lastLine > value ? lastLine : value);
  return *this;
}

Position &Position::updateLastColumn(int value) {
  if (IS_INVALID(value)) {
    return *this;
  }
  lastColumn = IS_INVALID(lastColumn)
                   ? value
                   : (lastColumn > value ? lastColumn : value);
  return *this;
}

#undef INVALID
#undef IS_INVALID

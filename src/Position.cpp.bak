// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Position.h"
#include "Exception.h"
#include "Log.h"
#include "fmt/format.h"

#define INVALID (-1)
#define IS_INVALID(x) ((x) < 0)

Position::Position()
    : firstLine(INVALID), firstColumn(INVALID), lastLine(INVALID),
      lastColumn(INVALID) {}

Position::Position(int a_firstLine, int a_firstColumn, int a_lastLine,
                   int a_lastColumn)
    : firstLine(a_firstLine), firstColumn(a_firstColumn), lastLine(a_lastLine),
      lastColumn(a_lastColumn) {
  EX_ASSERT(!IS_INVALID(firstLine), "firstLine {} must be valid", firstLine);
  EX_ASSERT(!IS_INVALID(firstColumn), "firstColumn {} must be valid",
            firstColumn);
  EX_ASSERT(!IS_INVALID(lastLine), "lastLine {} must be valid", lastLine);
  EX_ASSERT(!IS_INVALID(lastColumn), "lastColumn {} must be valid", lastColumn);
}

bool Position::invalid(int i) { return IS_INVALID(i); }

bool Position::invalid(const Position &p) {
  return IS_INVALID(p.firstLine) || IS_INVALID(p.firstColumn) ||
         IS_INVALID(p.lastLine) || IS_INVALID(p.lastColumn);
}

Position &Position::update(const Position &position) {
  updateFirstLine(position.firstLine);
  updateFirstColumn(position.firstColumn);
  updateLastLine(position.lastLine);
  updateLastColumn(position.lastColumn);
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

std::string Position::toString() const {
  return fmt::format("{}:{}-{}:{}", firstLine, firstColumn, lastLine,
                     lastColumn);
}

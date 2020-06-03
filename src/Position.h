// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once

class Position {
public:
  Position();
  Position(int firstLine, int firstColumn, int lastLine, int lastColumn);
  virtual ~Position() = default;
  Position(const Position &) = default;
  Position &operator=(const Position &) = default;
  virtual Position &updatePosition(const Position &position);
  virtual Position &updatePosition(const Position *position);
  virtual Position &updateFirstLine(int value);
  virtual Position &updateFirstColumn(int value);
  virtual Position &updateLastLine(int value);
  virtual Position &updateLastColumn(int value);

  int firstLine;
  int firstColumn;
  int lastLine;
  int lastColumn;
};

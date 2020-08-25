// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include <string>

class Position {
public:
  Position();
  Position(int a_firstLine, int a_firstColumn, int a_lastLine,
           int a_lastColumn);
  virtual ~Position() = default;
  Position(const Position &) = default;
  Position &operator=(const Position &) = default;
  virtual Position &update(const Position &position);
  virtual Position &updateFirstLine(int value);
  virtual Position &updateFirstColumn(int value);
  virtual Position &updateLastLine(int value);
  virtual Position &updateLastColumn(int value);
  virtual std::string toString() const;

  static bool invalid(int i);
  static bool invalid(const Position &p);

  int firstLine;
  int firstColumn;
  int lastLine;
  int lastColumn;
};

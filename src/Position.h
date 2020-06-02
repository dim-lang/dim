// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once

class Position {
public:
  Position(int firstLine, int firstColumn, int lastLine, int lastColumn);
  virtual ~Position() = default;
  Position(const Position &) = default;
  Position &operator=(const Position &) = default;
  virtual const int &firstLine() const;
  virtual const int &firstColumn() const;
  virtual const int &lastLine() const;
  virtual const int &lastColumn() const;
  virtual Position &updateFirstLine(int value);
  virtual Position &updateFirstColumn(int value);
  virtual Position &updateLastLine(int value);
  virtual Position &updateLastColumn(int value);
  static const Position &invalid();

protected:
  int firstLine_;
  int firstColumn_;
  int lastLine_;
  int lastColumn_;
};

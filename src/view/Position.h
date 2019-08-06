// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "view/G2.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace fastype {

class Position : public Stringify {
public:
  Position();
  Position(int row, int col);

  Position(const Position &) = default;
  Position &operator=(const Position &) = default;
  Position(Position &&) = default;
  Position &operator=(Position &&) = default;
  virtual ~Position() = default;

  const int &row() const;
  int setRow(int row);
  const int &column() const;
  int setColumn(int column);

  // compare two position
  bool operator==(const Position &other) const;
  bool operator!=(const Position &other) const;

  virtual std::string toString() const;

private:
  G2 g2_;
};

} // namespace fastype

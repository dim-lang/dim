// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace fastype {

class Position : public Stringify {
public:
  Position();
  Position(int32_t row, int32_t col);
  virtual ~Position() = default;

  int32_t row() const;
  int32_t setRow(int32_t row);
  int32_t column() const;
  int32_t setColumn(int32_t col);

  bool operator==(const Position &other) const;
  bool operator!=(const Position &other) const;

  // > >= < <= compares one row or one column
  // otherwise throw NoSuchOrderException
  bool operator>(const Position &other) const;
  bool operator>=(const Position &other) const;
  bool operator<(const Position &other) const;
  bool operator<=(const Position &other) const;

  virtual std::string toString() const;

private:
  int32_t row_;
  int32_t col_;
};

} // namespace fastype

// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Logging.h"
#include "Stringify.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace fastype {

class Position : public Stringify, Logging {
public:
  Position();
  Position(int row, int col);
  Position(const Position &) = default;
  Position &operator=(const Position &) = default;
  Position(Position &&) = default;
  Position &operator=(Position &&) = default;
  virtual ~Position() = default;

  int &row();
  int setRow(int row);
  int &column();
  int setColumn(int col);

  bool operator==(const Position &other) const;
  bool operator!=(const Position &other) const;

  virtual std::string toString() const;

private:
  int row_;
  int col_;
};

} // namespace fastype

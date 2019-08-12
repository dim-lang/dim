// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "graph2/Dot2.h"

namespace fastype {

class Index {
public:
  Index();
  Index(int row, int column);
  virtual ~Index() = default;

  Index(const Index &) = default;
  Index &operator=(const Index &) = default;
  Index(Index &&) = default;
  Index &operator=(Index &&) = default;

  int row() const;
  void setRow(int row);
  int column() const;
  void setColumn(int column);

private:
  Dot2 dot2_;
};

} // namespace fastype

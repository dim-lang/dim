// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "G2.h"

namespace fastype {

class Cursor {
public:
  Cursor();
  Cursor(const Cursor &) = default;
  Cursor &operator=(const Cursor &) = default;
  Cursor(Cursor &&) = default;
  Cursor &operator=(Cursor &&) = default;
  virtual ~Cursor() = default;

private:
  G2 g2_;
};

} // namespace fastype

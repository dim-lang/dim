// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include <string>

namespace fastype {

class Term {
public:
  virtual ~Term() = default;

  virtual void show(const std::string &fileName) = 0;

  static Sptr<Term> open(const std::string &termName);
};

} // namespace fastype

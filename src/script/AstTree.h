// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"

namespace fastype {

class AstTree {
public:
  virtual ~AstTree() = default;
  virtual Sptr<AstTree> child(int i) = 0;
  virtual int childrenNumber() = 0;
  virtual std::string toString() const = 0;
};

} // namespace fastype

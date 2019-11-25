// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"

namespace fastype {

class Ast : public Stringify {
public:
  enum AstType {
    INTEGER_CONSTANT = 1,
    FLOATING_CONSTANT = 2,
    BOOLEAN_CONSTANT = 3,
    STRING_CONSTANT = 4,
    IDENTIFIER_CONSTANT = 5,
    BINARY_OP = 6,
    UNARY_OP = 7,
    COMPOUND_STATEMENT = 8,
    ASSIGNMENT_STATEMENT = 9,
    VARIABLE = 10,
    EMPTY = 11,
  };

  virtual ~Ast() = default;
  // virtual Sptr<Ast> child(int i) const = 0;
  // virtual int childrenNumber() const = 0;
  virtual AstType type() const = 0;
  virtual std::string toString() const = 0;
};

} // namespace fastype

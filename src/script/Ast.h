// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"

namespace fastype {

class Ast : public Stringify {
public:
  enum AstType {
    PROGRAM = 1,
    STATEMENT_LIST,
    DECLARATION,
    STATEMENT,
    VARIABLE_DECLARATION,
    FUNCTION_DECLARATION,
    CLASS_DECLARATION,
    COMPOUND_STATEMENT,
    ASSIGNMENT_STATEMENT,
    EMPTY_STATEMENT,
    RETURN_STATEMENT,
    BINARY_OP,
    UNARY_OP,
    VARIABLE,
    INTEGER_CONSTANT,
    FLOATING_CONSTANT,
    BOOLEAN_CONSTANT,
    STRING_CONSTANT,
  };

  virtual ~Ast() = default;
  // virtual Sptr<Ast> child(int i) const = 0;
  // virtual int childrenNumber() const = 0;
  virtual AstType type() const = 0;
  virtual std::string toString() const = 0;
};

} // namespace fastype

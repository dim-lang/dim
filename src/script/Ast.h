// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include <fmt/format.h>
#include <memory>

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

  virtual ~Ast();
  virtual AstType type() const = 0;
  virtual std::string toString() const;
};

} // namespace fastype

namespace std {

template <> class hash<std::shared_ptr<fastype::Ast>> {
public:
  size_t operator()(const std::shared_ptr<fastype::Ast> &s) const {
    return (size_t)s.get();
  }
};

}; // namespace std

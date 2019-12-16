// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Ast.h"

namespace fastype {

const std::vector<Type> &Ast::astTypes() {
  static const std::vector<Type> types = {
      Type::T_PROGRAM,
      Type::T_STATEMENT_LIST,
      Type::T_DECLARATION,
      Type::T_STATEMENT,
      Type::T_VARIABLE_DECLARATION,
      Type::T_FUNCTION_DECLARATION,
      Type::T_CLASS_DECLARATION,
      Type::T_COMPOUND_STATEMENT,
      Type::T_ASSIGNMENT_STATEMENT,
      Type::T_EMPTY_STATEMENT,
      Type::T_RETURN_STATEMENT,
      Type::T_BINARY_OP,
      Type::T_UNARY_OP,
      Type::T_VARIABLE,
      Type::T_INTEGER_CONSTANT,
      Type::T_FLOATING_CONSTANT,
      Type::T_BOOLEAN_CONSTANT,
      Type::T_STRING_CONSTANT,
  };
  return types;
}

} // namespace fastype

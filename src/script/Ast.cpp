// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Ast.h"

namespace fastype {

const std::vector<int> &Ast::astTypes() {
  static const std::vector<int> types = {
      F_TYPE_PROGRAM,
      F_TYPE_STATEMENT_LIST,
      F_TYPE_DECLARATION,
      F_TYPE_STATEMENT,
      F_TYPE_VARIABLE_DECLARATION,
      F_TYPE_FUNCTION_DECLARATION,
      F_TYPE_CLASS_DECLARATION,
      F_TYPE_COMPOUND_STATEMENT,
      F_TYPE_ASSIGNMENT_STATEMENT,
      F_TYPE_EMPTY_STATEMENT,
      F_TYPE_RETURN_STATEMENT,
      F_TYPE_BINARY_OP,
      F_TYPE_UNARY_OP,
      F_TYPE_VARIABLE,
      F_TYPE_INTEGER_CONSTANT,
      F_TYPE_FLOATING_CONSTANT,
      F_TYPE_BOOLEAN_CONSTANT,
      F_TYPE_STRING_CONSTANT,
  };
  return types;
}

} // namespace fastype

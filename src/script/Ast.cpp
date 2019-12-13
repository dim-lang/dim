// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Ast.h"

namespace fastype {

const std::vector<AstType> &Ast::astTypes() {
  static const std::vector<Ast::AstType> types = {
      Ast::AstType::PROGRAM,
      Ast::AstType::STATEMENT_LIST,
      Ast::AstType::DECLARATION,
      Ast::AstType::STATEMENT,
      Ast::AstType::VARIABLE_DECLARATION,
      Ast::AstType::FUNCTION_DECLARATION,
      Ast::AstType::CLASS_DECLARATION,
      Ast::AstType::COMPOUND_STATEMENT,
      Ast::AstType::ASSIGNMENT_STATEMENT,
      Ast::AstType::EMPTY_STATEMENT,
      Ast::AstType::RETURN_STATEMENT,
      Ast::AstType::BINARY_OP,
      Ast::AstType::UNARY_OP,
      Ast::AstType::VARIABLE,
      Ast::AstType::INTEGER_CONSTANT,
      Ast::AstType::FLOATING_CONSTANT,
      Ast::AstType::BOOLEAN_CONSTANT,
      Ast::AstType::STRING_CONSTANT,
  };
  return types;
}

int Ast::astTypeValue(AstType t) {}

icu::UnicodeString Ast::astTypeName(AstType t) {}

AstType Ast::astTypeFromValue(int value) {}

AstType Ast::astTypeFromName(const icu::UnicodeString &name) {}

} // namespace fastype

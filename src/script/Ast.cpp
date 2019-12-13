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

int Ast::astTypeValue(AstType t) {
  switch (t) {
  case Ast::AstType::PROGRAM:
    return 1;
  case Ast::AstType::STATEMENT_LIST:
    return 2;
  case Ast::AstType::DECLARATION:
    return 3;
  case Ast::AstType::STATEMENT:
    return 4;
  case Ast::AstType::VARIABLE_DECLARATION:
    return 5;
  case Ast::AstType::FUNCTION_DECLARATION:
    return 6;
  case Ast::AstType::CLASS_DECLARATION:
    return 7;
  case Ast::AstType::COMPOUND_STATEMENT:
    return 8;
  case Ast::AstType::ASSIGNMENT_STATEMENT:
    return 9;
  case Ast::AstType::EMPTY_STATEMENT:
    return 10;
  case Ast::AstType::RETURN_STATEMENT:
    return 11;
  case Ast::AstType::BINARY_OP:
    return 12;
  case Ast::AstType::UNARY_OP:
    return 13;
  case Ast::AstType::VARIABLE:
    return 14;
  case Ast::AstType::INTEGER_CONSTANT:
    return 15;
  case Ast::AstType::FLOATING_CONSTANT:
    return 16;
  case Ast::AstType::BOOLEAN_CONSTANT:
    return 17;
  case Ast::AstType::STRING_CONSTANT:
    return 18;
  }
  F_THROW(NotFoundException, "astTypeValue not found! t: {}", t);
}

icu::UnicodeString Ast::astTypeName(AstType t) {
  switch (t) {
  case Ast::AstType::PROGRAM:
    return UNICODE_STRING_SIMPLE("PROGRAM");
  case Ast::AstType::STATEMENT_LIST:
    return UNICODE_STRING_SIMPLE("STATEMENT_LIST");
  case Ast::AstType::DECLARATION:
    return UNICODE_STRING_SIMPLE("DECLARATION");
  case Ast::AstType::STATEMENT:
    return UNICODE_STRING_SIMPLE("STATEMENT");
  case Ast::AstType::VARIABLE_DECLARATION:
    return UNICODE_STRING_SIMPLE("VARIABLE_DECLARATION");
  case Ast::AstType::FUNCTION_DECLARATION:
    return UNICODE_STRING_SIMPLE("FUNCTION_DECLARATION");
  case Ast::AstType::CLASS_DECLARATION:
    return UNICODE_STRING_SIMPLE("CLASS_DECLARATION");
  case Ast::AstType::COMPOUND_STATEMENT:
    return UNICODE_STRING_SIMPLE("COMPOUND_STATEMENT");
  case Ast::AstType::ASSIGNMENT_STATEMENT:
    return UNICODE_STRING_SIMPLE("ASSIGNMENT_STATEMENT");
  case Ast::AstType::EMPTY_STATEMENT:
    return UNICODE_STRING_SIMPLE("EMPTY_STATEMENT");
  case Ast::AstType::RETURN_STATEMENT:
    return UNICODE_STRING_SIMPLE("RETURN_STATEMENT");
  case Ast::AstType::BINARY_OP:
    return UNICODE_STRING_SIMPLE("BINARY_OP");
  case Ast::AstType::UNARY_OP:
    return UNICODE_STRING_SIMPLE("UNARY_OP");
  case Ast::AstType::VARIABLE:
    return UNICODE_STRING_SIMPLE("VARIABLE");
  case Ast::AstType::INTEGER_CONSTANT:
    return UNICODE_STRING_SIMPLE("INTEGER_CONSTANT");
  case Ast::AstType::FLOATING_CONSTANT:
    return UNICODE_STRING_SIMPLE("FLOATING_CONSTANT");
  case Ast::AstType::BOOLEAN_CONSTANT:
    return UNICODE_STRING_SIMPLE("BOOLEAN_CONSTANT");
  case Ast::AstType::STRING_CONSTANT:
    return UNICODE_STRING_SIMPLE("STRING_CONSTANT");
  }
  F_THROW(NotFoundException, "astTypeName not found! t: {}", t);
}

AstType Ast::astTypeFromValue(int value) {
  for (auto t : types()) {
    if (astTypeValue(t) == value) {
      return t;
    }
  }
  F_THROW(NotFoundException, "astTypeValue not found! value: {}", value);
}

AstType Ast::astTypeFromName(const icu::UnicodeString &name) {
  for (auto t : types()) {
    if (astTypeName(t) == value) {
      return t;
    }
  }
  std::string _1;
  F_THROW(NotFoundException, "astTypeName not found! name: {}",
          name.toUTF8String(_1));
}

} // namespace fastype

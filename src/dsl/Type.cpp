// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "dsl/Type.h"
#include "Exception.h"
#include "Hasher.h"

namespace dsl {

std::string Type::name(int value) {
  switch (value) {
    // token type
  case FDSL_EOF:
    return "FDSL_EOF";
  case FDSL_INTEGER:
    return "FDSL_INTEGER";
  case FDSL_FLOATING:
    return "FDSL_FLOATING";
  case FDSL_OPERATOR:
    return "FDSL_OPERATOR";
  case FDSL_COMPARATOR:
    return "FDSL_COMPARATOR";
  case FDSL_ASSIGNMENT:
    return "FDSL_ASSIGNMENT";
  case FDSL_BOOLEAN:
    return "FDSL_BOOLEAN";
  case FDSL_IDENTIFIER:
    return "FDSL_IDENTIFIER";
  case FDSL_PUNCTUATION:
    return "FDSL_PUNCTUATION";
  case FDSL_KEYWORD:
    return "FDSL_KEYWORD";
  case FDSL_STRING:
    return "FDSL_STRING";

    // ast type
  case FDSL_PROGRAM:
    return "FDSL_PROGRAM";
  case FDSL_STATEMENT_LIST:
    return "FDSL_STATEMENT_LIST";
  case FDSL_DECLARATION:
    return "FDSL_DECLARATION";
  case FDSL_STATEMENT:
    return "FDSL_STATEMENT";
  case FDSL_VARIABLE_DECLARATION:
    return "FDSL_VARIABLE_DECLARATION";
  case FDSL_FUNCTION_DECLARATION:
    return "FDSL_FUNCTION_DECLARATION";
  case FDSL_CLASS_DECLARATION:
    return "FDSL_CLASS_DECLARATION";
  case FDSL_COMPOUND_STATEMENT:
    return "FDSL_COMPOUND_STATEMENT";
  case FDSL_ASSIGNMENT_STATEMENT:
    return "FDSL_ASSIGNMENT_STATEMENT";
  case FDSL_EMPTY_STATEMENT:
    return "FDSL_EMPTY_STATEMENT";
  case FDSL_RETURN_STATEMENT:
    return "FDSL_RETURN_STATEMENT";
  case FDSL_BINARY_OP:
    return "FDSL_BINARY_OP";
  case FDSL_UNARY_OP:
    return "FDSL_UNARY_OP";
  case FDSL_VARIABLE:
    return "FDSL_VARIABLE";
  case FDSL_INTEGER_CONSTANT:
    return "FDSL_INTEGER_CONSTANT";
  case FDSL_FLOATING_CONSTANT:
    return "FDSL_FLOATING_CONSTANT";
  case FDSL_BOOLEAN_CONSTANT:
    return "FDSL_BOOLEAN_CONSTANT";
  case FDSL_STRING_CONSTANT:
    return "FDSL_STRING_CONSTANT";

    // symbol type
  case FDSL_BUILTIN_TYPE_SYMBOL:
    return "FDSL_BUILTIN_TYPE_SYMBOL";
  case FDSL_VARIABLE_SYMBOL:
    return "FDSL_VARIABLE_SYMBOL";
  case FDSL_FUNCTION_SYMBOL:
    return "FDSL_FUNCTION_SYMBOL";
  default:
    FTHROW("type value not found:{}", value);
  }
}

std::string Type::nameUTF8(int value) {
  std::string _1;
  return name(value).toUTF8String(_1);
}

int Type::value(const std::string &name) {
  static const std::unordered_map<std::string, int> typeMapping = {
      // token type
      {"FDSL_EOF", FDSL_EOF},
      {"FDSL_INTEGER", FDSL_INTEGER},
      {"FDSL_FLOATING", FDSL_FLOATING},
      {"FDSL_OPERATOR", FDSL_OPERATOR},
      {"FDSL_COMPARATOR", FDSL_COMPARATOR},
      {"FDSL_ASSIGNMENT", FDSL_ASSIGNMENT},
      {"FDSL_BOOLEAN", FDSL_BOOLEAN},
      {"FDSL_IDENTIFIER", FDSL_IDENTIFIER},
      {"FDSL_PUNCTUATION", FDSL_PUNCTUATION},
      {"FDSL_KEYWORD", FDSL_KEYWORD},
      {"FDSL_STRING", FDSL_STRING},

      // ast type
      {"FDSL_PROGRAM", FDSL_PROGRAM},
      {"FDSL_STATEMENT_LIST", FDSL_STATEMENT_LIST},
      {"FDSL_DECLARATION", FDSL_DECLARATION},
      {"FDSL_STATEMENT", FDSL_STATEMENT},
      {"FDSL_VARIABLE_DECLARATION", FDSL_VARIABLE_DECLARATION},
      {"FDSL_FUNCTION_DECLARATION", FDSL_FUNCTION_DECLARATION},
      {"FDSL_CLASS_DECLARATION", FDSL_CLASS_DECLARATION},
      {"FDSL_COMPOUND_STATEMENT", FDSL_COMPOUND_STATEMENT},
      {"FDSL_ASSIGNMENT_STATEMENT", FDSL_ASSIGNMENT_STATEMENT},
      {"FDSL_EMPTY_STATEMENT", FDSL_EMPTY_STATEMENT},
      {"FDSL_RETURN_STATEMENT", FDSL_RETURN_STATEMENT},
      {"FDSL_BINARY_OP", FDSL_BINARY_OP},
      {"FDSL_UNARY_OP", FDSL_UNARY_OP},
      {"FDSL_VARIABLE", FDSL_VARIABLE},
      {"FDSL_INTEGER_CONSTANT", FDSL_INTEGER_CONSTANT},
      {"FDSL_FLOATING_CONSTANT", FDSL_FLOATING_CONSTANT},
      {"FDSL_BOOLEAN_CONSTANT", FDSL_BOOLEAN_CONSTANT},
      {"FDSL_STRING_CONSTANT", FDSL_STRING_CONSTANT},

      // symbol type
      {"FDSL_BUILTIN_TYPE_SYMBOL", FDSL_BUILTIN_TYPE_SYMBOL},
      {"FDSL_VARIABLE_SYMBOL", FDSL_VARIABLE_SYMBOL},
      {"FDSL_FUNCTION_SYMBOL", FDSL_FUNCTION_SYMBOL},
  };
  auto it = typeMapping.find(name);
  if (it == typeMapping.end()) {
    FTHROW("dsl type name not found:{}", name);
  }
  return it->second;
}

} // namespace dsl

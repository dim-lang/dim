// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Type.h"
#include "Hasher.h"
#include "exception/ScriptException.h"

namespace fastype {

Type::Type(const icu::UnicodeString &name, int typeId)
    : name_(name), value_(typeId) {}

const Type &Type::fromValue(int value) {
  switch (value) {
    // token type
  case F_TYPE_EOF:
    return T_EOF;
  case F_TYPE_INTEGER:
    return T_INTEGER;
  case F_TYPE_FLOATING:
    return T_FLOATING;
  case F_TYPE_OPERATOR:
    return T_OPERATOR;
  case F_TYPE_COMPARATOR:
    return T_COMPARATOR;
  case F_TYPE_ASSIGNMENT:
    return T_ASSIGNMENT;
  case F_TYPE_BOOLEAN:
    return T_BOOLEAN;
  case F_TYPE_IDENTIFIER:
    return T_IDENTIFIER;
  case F_TYPE_PUNCTUATION:
    return T_PUNCTUATION;
  case F_TYPE_KEYWORD:
    return T_KEYWORD;
  case F_TYPE_STRING:
    return T_STRING;

    // ast type
  case F_TYPE_PROGRAM:
    return T_PROGRAM;
  case F_TYPE_STATEMENT_LIST:
    return T_STATEMENT_LIST;
  case F_TYPE_DECLARATION:
    return T_DECLARATION;
  case F_TYPE_STATEMENT:
    return T_STATEMENT;
  case F_TYPE_VARIABLE_DECLARATION:
    return T_VARIABLE_DECLARATION;
  case F_TYPE_FUNCTION_DECLARATION:
    return T_FUNCTION_DECLARATION;
  case F_TYPE_CLASS_DECLARATION:
    return T_CLASS_DECLARATION;
  case F_TYPE_COMPOUND_STATEMENT:
    return T_COMPOUND_STATEMENT;
  case F_TYPE_ASSIGNMENT_STATEMENT:
    return T_ASSIGNMENT_STATEMENT;
  case F_TYPE_EMPTY_STATEMENT:
    return T_EMPTY_STATEMENT;
  case F_TYPE_RETURN_STATEMENT:
    return T_RETURN_STATEMENT;
  case F_TYPE_BINARY_OP:
    return T_BINARY_OP;
  case F_TYPE_UNARY_OP:
    return T_UNARY_OP;
  case F_TYPE_VARIABLE:
    return T_VARIABLE;
  case F_TYPE_INTEGER_CONSTANT:
    return T_INTEGER_CONSTANT;
  case F_TYPE_FLOATING_CONSTANT:
    return T_FLOATING_CONSTANT;
  case F_TYPE_BOOLEAN_CONSTANT:
    return T_BOOLEAN_CONSTANT;
  case F_TYPE_STRING_CONSTANT:
    return T_STRING_CONSTANT;
  default:
    F_THROW(ScriptException, "type value not found:{}", value);
  }
}

const Type &Type::fromName(const icu::UnicodeString &name) {
  static const std::unordered_map<icu::UnicodeString, Type> typeMapping = {
      // token type
      {UNICODE_STRING_SIMPLE("EOF"), T_EOF},
      {UNICODE_STRING_SIMPLE("INTEGER"), T_INTEGER},
      {UNICODE_STRING_SIMPLE("FLOATING"), T_FLOATING},
      {UNICODE_STRING_SIMPLE("OPERATOR"), T_OPERATOR},
      {UNICODE_STRING_SIMPLE("COMPARATOR"), T_COMPARATOR},
      {UNICODE_STRING_SIMPLE("ASSIGNMENT"), T_ASSIGNMENT},
      {UNICODE_STRING_SIMPLE("BOOLEAN"), T_BOOLEAN},
      {UNICODE_STRING_SIMPLE("IDENTIFIER"), T_IDENTIFIER},
      {UNICODE_STRING_SIMPLE("PUNCTUATION"), T_PUNCTUATION},
      {UNICODE_STRING_SIMPLE("KEYWORD"), T_KEYWORD},
      {UNICODE_STRING_SIMPLE("STRING"), T_STRING},
      // ast type
      {UNICODE_STRING_SIMPLE("PROGRAM"), T_PROGRAM},
      {UNICODE_STRING_SIMPLE("STATEMENT_LIST"), T_STATEMENT_LIST},
      {UNICODE_STRING_SIMPLE("DECLARATION"), T_DECLARATION},
      {UNICODE_STRING_SIMPLE("STATEMENT"), T_STATEMENT},
      {UNICODE_STRING_SIMPLE("VARIABLE_DECLARATION"), T_VARIABLE_DECLARATION},
      {UNICODE_STRING_SIMPLE("FUNCTION_DECLARATION"), T_FUNCTION_DECLARATION},
      {UNICODE_STRING_SIMPLE("CLASS_DECLARATION"), T_CLASS_DECLARATION},
      {UNICODE_STRING_SIMPLE("COMPOUND_STATEMENT"), T_COMPOUND_STATEMENT},
      {UNICODE_STRING_SIMPLE("ASSIGNMENT_STATEMENT"), T_ASSIGNMENT_STATEMENT},
      {UNICODE_STRING_SIMPLE("EMPTY_STATEMENT"), T_EMPTY_STATEMENT},
      {UNICODE_STRING_SIMPLE("RETURN_STATEMENT"), T_RETURN_STATEMENT},
      {UNICODE_STRING_SIMPLE("BINARY_OP"), T_BINARY_OP},
      {UNICODE_STRING_SIMPLE("UNARY_OP"), T_UNARY_OP},
      {UNICODE_STRING_SIMPLE("VARIABLE"), T_VARIABLE},
      {UNICODE_STRING_SIMPLE("INTEGER_CONSTANT"), T_INTEGER_CONSTANT},
      {UNICODE_STRING_SIMPLE("FLOATING_CONSTANT"), T_FLOATING_CONSTANT},
      {UNICODE_STRING_SIMPLE("BOOLEAN_CONSTANT"), T_BOOLEAN_CONSTANT},
      {UNICODE_STRING_SIMPLE("STRING_CONSTANT"), T_STRING_CONSTANT},
  };
  std::unordered_map<icu::UnicodeString, Type>::const_iterator it =
      typeMapping.find(name);
  if (it == typeMapping.end()) {
    std::string _1;
    F_THROW(ScriptException, "type name not found:{}", name.toUTF8String(_1));
  }
  return it->second;
}

int Type::value() const { return value_; }

const icu::UnicodeString &Type::name() const { return name_; }

std::string Type::nameUTF8() const {
  std::string _1;
  return name_.toUTF8String(_1);
}

std::string Type::toString() const {
  return fmt::format("[ @Type name_:{}, value_:{} ]", nameUTF8(), value_);
}

bool Type::operator==(const Type &t) const { return compare(t) == 0; }

bool Type::operator!=(const Type &t) const { return compare(t) != 0; }

bool Type::operator<(const Type &t) const { return compare(t) < 0; }

bool Type::operator<=(const Type &t) const { return compare(t) <= 0; }

bool Type::operator>(const Type &t) const { return compare(t) > 0; }

bool Type::operator>=(const Type &t) const { return compare(t) >= 0; }

int Type::compare(const Type &t) const { return value_ - t.value_; }

// token type
const Type Type::T_EOF(UNICODE_STRING_SIMPLE("EOF"), F_TYPE_EOF);
const Type Type::T_INTEGER(UNICODE_STRING_SIMPLE("INTEGER"), F_TYPE_INTEGER);
const Type Type::T_FLOATING(UNICODE_STRING_SIMPLE("FLOATING"), F_TYPE_FLOATING);
const Type Type::T_OPERATOR(UNICODE_STRING_SIMPLE("OPERATOR"), F_TYPE_OPERATOR);
const Type Type::T_COMPARATOR(UNICODE_STRING_SIMPLE("COMPARATOR"),
                              F_TYPE_COMPARATOR);
const Type Type::T_ASSIGNMENT(UNICODE_STRING_SIMPLE("ASSIGNMENT"),
                              F_TYPE_ASSIGNMENT);
const Type Type::T_BOOLEAN(UNICODE_STRING_SIMPLE("BOOLEAN"), F_TYPE_BOOLEAN);
const Type Type::T_IDENTIFIER(UNICODE_STRING_SIMPLE("IDENTIFIER"),
                              F_TYPE_IDENTIFIER);
const Type Type::T_PUNCTUATION(UNICODE_STRING_SIMPLE("PUNCTUATION"),
                               F_TYPE_PUNCTUATION);
const Type Type::T_KEYWORD(UNICODE_STRING_SIMPLE("KEYWORD"), F_TYPE_KEYWORD);
const Type Type::T_STRING(UNICODE_STRING_SIMPLE("STRING"), F_TYPE_STRING);

// ast type
const Type Type::T_PROGRAM(UNICODE_STRING_SIMPLE("PROGRAM"), F_TYPE_PROGRAM);
const Type Type::T_STATEMENT_LIST(UNICODE_STRING_SIMPLE("STATEMENT_LIST"),
                                  F_TYPE_STATEMENT_LIST);
const Type Type::T_DECLARATION(UNICODE_STRING_SIMPLE("DECLARATION"),
                               F_TYPE_DECLARATION);
const Type Type::T_STATEMENT(UNICODE_STRING_SIMPLE("STATEMENT"),
                             F_TYPE_STATEMENT);
const Type
    Type::T_VARIABLE_DECLARATION(UNICODE_STRING_SIMPLE("VARIABLE_DECLARATION"),
                                 F_TYPE_VARIABLE_DECLARATION);
const Type
    Type::T_FUNCTION_DECLARATION(UNICODE_STRING_SIMPLE("FUNCTION_DECLARATION"),
                                 F_TYPE_FUNCTION_DECLARATION);
const Type Type::T_CLASS_DECLARATION(UNICODE_STRING_SIMPLE("CLASS_DECLARATION"),
                                     F_TYPE_CLASS_DECLARATION);
const Type
    Type::T_COMPOUND_STATEMENT(UNICODE_STRING_SIMPLE("COMPOUND_STATEMENT"),
                               F_TYPE_COMPOUND_STATEMENT);
const Type
    Type::T_ASSIGNMENT_STATEMENT(UNICODE_STRING_SIMPLE("ASSIGNMENT_STATEMENT"),
                                 F_TYPE_ASSIGNMENT_STATEMENT);
const Type Type::T_EMPTY_STATEMENT(UNICODE_STRING_SIMPLE("EMPTY_STATEMENT"),
                                   F_TYPE_EMPTY_STATEMENT);
const Type Type::T_RETURN_STATEMENT(UNICODE_STRING_SIMPLE("RETURN_STATEMENT"),
                                    F_TYPE_RETURN_STATEMENT);
const Type Type::T_BINARY_OP(UNICODE_STRING_SIMPLE("BINARY_OP"),
                             F_TYPE_BINARY_OP);
const Type Type::T_UNARY_OP(UNICODE_STRING_SIMPLE("UNARY_OP"), F_TYPE_UNARY_OP);
const Type Type::T_VARIABLE(UNICODE_STRING_SIMPLE("VARIABLE"), F_TYPE_VARIABLE);
const Type Type::T_INTEGER_CONSTANT(UNICODE_STRING_SIMPLE("INTEGER_CONSTANT"),
                                    F_TYPE_INTEGER_CONSTANT);
const Type Type::T_FLOATING_CONSTANT(UNICODE_STRING_SIMPLE("FLOATING_CONSTANT"),
                                     F_TYPE_FLOATING_CONSTANT);
const Type Type::T_BOOLEAN_CONSTANT(UNICODE_STRING_SIMPLE("BOOLEAN_CONSTANT"),
                                    F_TYPE_BOOLEAN_CONSTANT);
const Type Type::T_STRING_CONSTANT(UNICODE_STRING_SIMPLE("STRING_CONSTANT"),
                                   F_TYPE_STRING_CONSTANT);

} // namespace fastype

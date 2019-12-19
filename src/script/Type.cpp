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
    return TP_EOF;
  case F_TYPE_INTEGER:
    return TP_INTEGER;
  case F_TYPE_FLOATING:
    return TP_FLOATING;
  case F_TYPE_OPERATOR:
    return TP_OPERATOR;
  case F_TYPE_COMPARATOR:
    return TP_COMPARATOR;
  case F_TYPE_ASSIGNMENT:
    return TP_ASSIGNMENT;
  case F_TYPE_BOOLEAN:
    return TP_BOOLEAN;
  case F_TYPE_IDENTIFIER:
    return TP_IDENTIFIER;
  case F_TYPE_PUNCTUATION:
    return TP_PUNCTUATION;
  case F_TYPE_KEYWORD:
    return TP_KEYWORD;
  case F_TYPE_STRING:
    return TP_STRING;

    // ast type
  case F_TYPE_PROGRAM:
    return TP_PROGRAM;
  case F_TYPE_STATEMENT_LIST:
    return TP_STATEMENT_LIST;
  case F_TYPE_DECLARATION:
    return TP_DECLARATION;
  case F_TYPE_STATEMENT:
    return TP_STATEMENT;
  case F_TYPE_VARIABLE_DECLARATION:
    return TP_VARIABLE_DECLARATION;
  case F_TYPE_FUNCTION_DECLARATION:
    return TP_FUNCTION_DECLARATION;
  case F_TYPE_CLASS_DECLARATION:
    return TP_CLASS_DECLARATION;
  case F_TYPE_COMPOUND_STATEMENT:
    return TP_COMPOUND_STATEMENT;
  case F_TYPE_ASSIGNMENT_STATEMENT:
    return TP_ASSIGNMENT_STATEMENT;
  case F_TYPE_EMPTY_STATEMENT:
    return TP_EMPTY_STATEMENT;
  case F_TYPE_RETURN_STATEMENT:
    return TP_RETURN_STATEMENT;
  case F_TYPE_BINARY_OP:
    return TP_BINARY_OP;
  case F_TYPE_UNARY_OP:
    return TP_UNARY_OP;
  case F_TYPE_VARIABLE:
    return TP_VARIABLE;
  case F_TYPE_INTEGER_CONSTANT:
    return TP_INTEGER_CONSTANT;
  case F_TYPE_FLOATING_CONSTANT:
    return TP_FLOATING_CONSTANT;
  case F_TYPE_BOOLEAN_CONSTANT:
    return TP_BOOLEAN_CONSTANT;
  case F_TYPE_STRING_CONSTANT:
    return TP_STRING_CONSTANT;
  default:
    F_THROW(ScriptException, "type value not found:{}", value);
  }
}

const Type &Type::fromName(const icu::UnicodeString &name) {
  static const std::unordered_map<icu::UnicodeString, Type> typeMapping = {
      // token type
      {UNICODE_STRING_SIMPLE("EOF"), TP_EOF},
      {UNICODE_STRING_SIMPLE("INTEGER"), TP_INTEGER},
      {UNICODE_STRING_SIMPLE("FLOATING"), TP_FLOATING},
      {UNICODE_STRING_SIMPLE("OPERATOR"), TP_OPERATOR},
      {UNICODE_STRING_SIMPLE("COMPARATOR"), TP_COMPARATOR},
      {UNICODE_STRING_SIMPLE("ASSIGNMENT"), TP_ASSIGNMENT},
      {UNICODE_STRING_SIMPLE("BOOLEAN"), TP_BOOLEAN},
      {UNICODE_STRING_SIMPLE("IDENTIFIER"), TP_IDENTIFIER},
      {UNICODE_STRING_SIMPLE("PUNCTUATION"), TP_PUNCTUATION},
      {UNICODE_STRING_SIMPLE("KEYWORD"), TP_KEYWORD},
      {UNICODE_STRING_SIMPLE("STRING"), TP_STRING},
      // ast type
      {UNICODE_STRING_SIMPLE("PROGRAM"), TP_PROGRAM},
      {UNICODE_STRING_SIMPLE("STATEMENT_LIST"), TP_STATEMENT_LIST},
      {UNICODE_STRING_SIMPLE("DECLARATION"), TP_DECLARATION},
      {UNICODE_STRING_SIMPLE("STATEMENT"), TP_STATEMENT},
      {UNICODE_STRING_SIMPLE("VARIABLE_DECLARATION"), TP_VARIABLE_DECLARATION},
      {UNICODE_STRING_SIMPLE("FUNCTION_DECLARATION"), TP_FUNCTION_DECLARATION},
      {UNICODE_STRING_SIMPLE("CLASS_DECLARATION"), TP_CLASS_DECLARATION},
      {UNICODE_STRING_SIMPLE("COMPOUND_STATEMENT"), TP_COMPOUND_STATEMENT},
      {UNICODE_STRING_SIMPLE("ASSIGNMENT_STATEMENT"), TP_ASSIGNMENT_STATEMENT},
      {UNICODE_STRING_SIMPLE("EMPTY_STATEMENT"), TP_EMPTY_STATEMENT},
      {UNICODE_STRING_SIMPLE("RETURN_STATEMENT"), TP_RETURN_STATEMENT},
      {UNICODE_STRING_SIMPLE("BINARY_OP"), TP_BINARY_OP},
      {UNICODE_STRING_SIMPLE("UNARY_OP"), TP_UNARY_OP},
      {UNICODE_STRING_SIMPLE("VARIABLE"), TP_VARIABLE},
      {UNICODE_STRING_SIMPLE("INTEGER_CONSTANT"), TP_INTEGER_CONSTANT},
      {UNICODE_STRING_SIMPLE("FLOATING_CONSTANT"), TP_FLOATING_CONSTANT},
      {UNICODE_STRING_SIMPLE("BOOLEAN_CONSTANT"), TP_BOOLEAN_CONSTANT},
      {UNICODE_STRING_SIMPLE("STRING_CONSTANT"), TP_STRING_CONSTANT},
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
const Type Type::TP_EOF(UNICODE_STRING_SIMPLE("EOF"), F_TYPE_EOF);
const Type Type::TP_INTEGER(UNICODE_STRING_SIMPLE("INTEGER"), F_TYPE_INTEGER);
const Type Type::TP_FLOATING(UNICODE_STRING_SIMPLE("FLOATING"),
                             F_TYPE_FLOATING);
const Type Type::TP_OPERATOR(UNICODE_STRING_SIMPLE("OPERATOR"),
                             F_TYPE_OPERATOR);
const Type Type::TP_COMPARATOR(UNICODE_STRING_SIMPLE("COMPARATOR"),
                               F_TYPE_COMPARATOR);
const Type Type::TP_ASSIGNMENT(UNICODE_STRING_SIMPLE("ASSIGNMENT"),
                               F_TYPE_ASSIGNMENT);
const Type Type::TP_BOOLEAN(UNICODE_STRING_SIMPLE("BOOLEAN"), F_TYPE_BOOLEAN);
const Type Type::TP_IDENTIFIER(UNICODE_STRING_SIMPLE("IDENTIFIER"),
                               F_TYPE_IDENTIFIER);
const Type Type::TP_PUNCTUATION(UNICODE_STRING_SIMPLE("PUNCTUATION"),
                                F_TYPE_PUNCTUATION);
const Type Type::TP_KEYWORD(UNICODE_STRING_SIMPLE("KEYWORD"), F_TYPE_KEYWORD);
const Type Type::TP_STRING(UNICODE_STRING_SIMPLE("STRING"), F_TYPE_STRING);

// ast type
const Type Type::TP_PROGRAM(UNICODE_STRING_SIMPLE("PROGRAM"), F_TYPE_PROGRAM);
const Type Type::TP_STATEMENT_LIST(UNICODE_STRING_SIMPLE("STATEMENT_LIST"),
                                   F_TYPE_STATEMENT_LIST);
const Type Type::TP_DECLARATION(UNICODE_STRING_SIMPLE("DECLARATION"),
                                F_TYPE_DECLARATION);
const Type Type::TP_STATEMENT(UNICODE_STRING_SIMPLE("STATEMENT"),
                              F_TYPE_STATEMENT);
const Type
    Type::TP_VARIABLE_DECLARATION(UNICODE_STRING_SIMPLE("VARIABLE_DECLARATION"),
                                  F_TYPE_VARIABLE_DECLARATION);
const Type
    Type::TP_FUNCTION_DECLARATION(UNICODE_STRING_SIMPLE("FUNCTION_DECLARATION"),
                                  F_TYPE_FUNCTION_DECLARATION);
const Type
    Type::TP_CLASS_DECLARATION(UNICODE_STRING_SIMPLE("CLASS_DECLARATION"),
                               F_TYPE_CLASS_DECLARATION);
const Type
    Type::TP_COMPOUND_STATEMENT(UNICODE_STRING_SIMPLE("COMPOUND_STATEMENT"),
                                F_TYPE_COMPOUND_STATEMENT);
const Type
    Type::TP_ASSIGNMENT_STATEMENT(UNICODE_STRING_SIMPLE("ASSIGNMENT_STATEMENT"),
                                  F_TYPE_ASSIGNMENT_STATEMENT);
const Type Type::TP_EMPTY_STATEMENT(UNICODE_STRING_SIMPLE("EMPTY_STATEMENT"),
                                    F_TYPE_EMPTY_STATEMENT);
const Type Type::TP_RETURN_STATEMENT(UNICODE_STRING_SIMPLE("RETURN_STATEMENT"),
                                     F_TYPE_RETURN_STATEMENT);
const Type Type::TP_BINARY_OP(UNICODE_STRING_SIMPLE("BINARY_OP"),
                              F_TYPE_BINARY_OP);
const Type Type::TP_UNARY_OP(UNICODE_STRING_SIMPLE("UNARY_OP"),
                             F_TYPE_UNARY_OP);
const Type Type::TP_VARIABLE(UNICODE_STRING_SIMPLE("VARIABLE"),
                             F_TYPE_VARIABLE);
const Type Type::TP_INTEGER_CONSTANT(UNICODE_STRING_SIMPLE("INTEGER_CONSTANT"),
                                     F_TYPE_INTEGER_CONSTANT);
const Type
    Type::TP_FLOATING_CONSTANT(UNICODE_STRING_SIMPLE("FLOATING_CONSTANT"),
                               F_TYPE_FLOATING_CONSTANT);
const Type Type::TP_BOOLEAN_CONSTANT(UNICODE_STRING_SIMPLE("BOOLEAN_CONSTANT"),
                                     F_TYPE_BOOLEAN_CONSTANT);
const Type Type::TP_STRING_CONSTANT(UNICODE_STRING_SIMPLE("STRING_CONSTANT"),
                                    F_TYPE_STRING_CONSTANT);

} // namespace fastype

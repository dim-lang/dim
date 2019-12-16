// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Type.h"

namespace fastype {

Type::Type(const icu::UnicodeString &name, int typeId)
    : name_(name), value_(typeId) {}

const Type &Type::fromValue(int value) {}

const Type &Type::fromName(const icu::UnicodeString &name) {}

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

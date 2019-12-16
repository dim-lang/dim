// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Type.h"

namespace fastype {

Type::Type(const icu::UnicodeString &name) : name_(name), value_(TypeId++) {}

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

int Type::TypeId = 1;

const Type Type::T_EOF(UNICODE_STRING_SIMPLE("EOF"));
const Type Type::T_INTEGER(UNICODE_STRING_SIMPLE("INTEGER"));
const Type Type::T_FLOATING(UNICODE_STRING_SIMPLE("FLOATING"));
const Type Type::T_OPERATOR(UNICODE_STRING_SIMPLE("OPERATOR"));
const Type Type::T_COMPARATOR(UNICODE_STRING_SIMPLE("COMPARATOR"));
const Type Type::T_ASSIGNMENT(UNICODE_STRING_SIMPLE("ASSIGNMENT"));
const Type Type::T_BOOLEAN(UNICODE_STRING_SIMPLE("BOOLEAN"));
const Type Type::T_IDENTIFIER(UNICODE_STRING_SIMPLE("IDENTIFIER"));
const Type Type::T_PUNCTUATION(UNICODE_STRING_SIMPLE("PUNCTUATION"));
const Type Type::T_KEYWORD(UNICODE_STRING_SIMPLE("KEYWORD"));
const Type Type::T_STRING(UNICODE_STRING_SIMPLE("STRING"));

const Type Type::T_PROGRAM(UNICODE_STRING_SIMPLE("PROGRAM"));
const Type Type::T_STATEMENT_LIST(UNICODE_STRING_SIMPLE("STATEMENT_LIST"));
const Type Type::T_DECLARATION(UNICODE_STRING_SIMPLE("DECLARATION"));
const Type Type::T_STATEMENT(UNICODE_STRING_SIMPLE("STATEMENT"));
const Type
    Type::T_VARIABLE_DECLARATION(UNICODE_STRING_SIMPLE("VARIABLE_DECLARATION"));
const Type
    Type::T_FUNCTION_DECLARATION(UNICODE_STRING_SIMPLE("FUNCTION_DECLARATION"));
const Type
    Type::T_CLASS_DECLARATION(UNICODE_STRING_SIMPLE("CLASS_DECLARATION"));
const Type
    Type::T_COMPOUND_STATEMENT(UNICODE_STRING_SIMPLE("COMPOUND_STATEMENT"));
const Type
    Type::T_ASSIGNMENT_STATEMENT(UNICODE_STRING_SIMPLE("ASSIGNMENT_STATEMENT"));
const Type Type::T_EMPTY_STATEMENT(UNICODE_STRING_SIMPLE("EMPTY_STATEMENT"));
const Type Type::T_RETURN_STATEMENT(UNICODE_STRING_SIMPLE("RETURN_STATEMENT"));
const Type Type::T_BINARY_OP(UNICODE_STRING_SIMPLE("BINARY_OP"));
const Type Type::T_UNARY_OP(UNICODE_STRING_SIMPLE("UNARY_OP"));
const Type Type::T_VARIABLE(UNICODE_STRING_SIMPLE("VARIABLE"));
const Type Type::T_INTEGER_CONSTANT(UNICODE_STRING_SIMPLE("INTEGER_CONSTANT"));
const Type
    Type::T_FLOATING_CONSTANT(UNICODE_STRING_SIMPLE("FLOATING_CONSTANT"));
const Type Type::T_BOOLEAN_CONSTANT(UNICODE_STRING_SIMPLE("BOOLEAN_CONSTANT"));
const Type Type::T_STRING_CONSTANT(UNICODE_STRING_SIMPLE("STRING_CONSTANT"));

} // namespace fastype

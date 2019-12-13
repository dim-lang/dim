// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/MetaType.h"

namespace fastype {

MetaType::MetaType(const icu::UnicodeString &name)
    : name_(name), value_(TypeId++) {}

const MetaType &MetaType::fromValue(int value) {}

const MetaType &MetaType::fromName(const icu::UnicodeString &name) {}

int MetaType::value() const { return value_; }

const icu::UnicodeString &MetaType::name() const { return name_; }

std::string MetaType::nameUTF8() const {
  std::string _1;
  return name_.toUTF8String(_1);
}

std::string MetaType::toString() const {
  return fmt::format("[ @MetaType name_:{}, value_:{} ]", nameUTF8(), value_);
}

int MetaType::TypeId = 1;

const MetaType MetaType::MT_EOF(UNICODE_STRING_SIMPLE("EOF"));
const MetaType MetaType::MT_INTEGER(UNICODE_STRING_SIMPLE("INTEGER"));
const MetaType MetaType::MT_FLOATING;
const MetaType MetaType::MT_OPERATOR;
const MetaType MetaType::MT_COMPARATOR;
const MetaType MetaType::MT_ASSIGNMENT;
const MetaType MetaType::MT_BOOLEAN;
const MetaType MetaType::MT_IDENTIFIER;
const MetaType MetaType::MT_PUNCTUATION;
const MetaType MetaType::MT_KEYWORD;

const MetaType MetaType::MT_STRING;

const MetaType MetaType::MT_PROGRAM;
const MetaType MetaType::MT_STATEMENT_LIST;
const MetaType MetaType::MT_DECLARATION;
const MetaType MetaType::MT_STATEMENT;
const MetaType MetaType::MT_VARIABLE_DECLARATION;
const MetaType MetaType::MT_FUNCTION_DECLARATION;
const MetaType MetaType::MT_CLASS_DECLARATION;
const MetaType MetaType::MT_COMPOUND_STATEMENT;
const MetaType MetaType::MT_ASSIGNMENT_STATEMENT;
const MetaType MetaType::MT_EMPTY_STATEMENT;
const MetaType MetaType::MT_RETURN_STATEMENT;
const MetaType MetaType::MT_BINARY_OP;
const MetaType MetaType::MT_UNARY_OP;
const MetaType MetaType::MT_VARIABLE;
const MetaType MetaType::MT_INTEGER_CONSTANT;
const MetaType MetaType::MT_FLOATING_CONSTANT;
const MetaType MetaType::MT_BOOLEAN_CONSTANT;
const MetaType MetaType::MT_STRING_CONSTANT;

} // namespace fastype

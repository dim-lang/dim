// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include <string>

// token type
// end of file
#define F_TYPE_EOF 1
// integer number
#define F_TYPE_INTEGER 2
// floating number
#define F_TYPE_FLOATING 3
// operator: + - * / % ++ --
#define F_TYPE_OPERATOR 4
// comparator: == != < <= > >=
#define F_TYPE_COMPARATOR 5
// assignment: =
#define F_TYPE_ASSIGNMENT 6
// boolean: True False
#define F_TYPE_BOOLEAN 7
// identifier
#define F_TYPE_IDENTIFIER 8
// punctuation
#define F_TYPE_PUNCTUATION 9
// keyword: let null for if elseif else while break continue func class
#define F_TYPE_KEYWORD 10
// string
#define F_TYPE_STRING 11

// ast type
#define F_TYPE_PROGRAM 101
#define F_TYPE_STATEMENT_LIST 102
#define F_TYPE_DECLARATION 103
#define F_TYPE_STATEMENT 104
#define F_TYPE_VARIABLE_DECLARATION 105
#define F_TYPE_FUNCTION_DECLARATION 106
#define F_TYPE_CLASS_DECLARATION 107
#define F_TYPE_COMPOUND_STATEMENT 108
#define F_TYPE_ASSIGNMENT_STATEMENT 109
#define F_TYPE_EMPTY_STATEMENT 110
#define F_TYPE_RETURN_STATEMENT 111
#define F_TYPE_BINARY_OP 112
#define F_TYPE_UNARY_OP 113
#define F_TYPE_VARIABLE 114
#define F_TYPE_INTEGER_CONSTANT 115
#define F_TYPE_FLOATING_CONSTANT 116
#define F_TYPE_BOOLEAN_CONSTANT 117
#define F_TYPE_STRING_CONSTANT 118

// symbol type
#define F_TYPE_BUILTIN_SYMBOL 201
#define F_TYPE_VARIABLE_SYMBOL 202

namespace fastype {

class Type {
public:
  static icu::UnicodeString name(int value);
  static std::string nameUTF8(int value);
  static int value(const icu::UnicodeString &name);
};

} // namespace fastype

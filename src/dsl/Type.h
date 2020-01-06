// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include <string>

// token type
// end of file
#define FDSL_EOF 1
// integer number
#define FDSL_INTEGER 2
// floating number
#define FDSL_FLOATING 3
// operator: + - * / % ++ --
#define FDSL_OPERATOR 4
// comparator: == != < <= > >=
#define FDSL_COMPARATOR 5
// assignment: =
#define FDSL_ASSIGNMENT 6
// boolean: True False
#define FDSL_BOOLEAN 7
// identifier
#define FDSL_IDENTIFIER 8
// punctuation
#define FDSL_PUNCTUATION 9
// keyword: let null for if elseif else while break continue func class
#define FDSL_KEYWORD 10
// string
#define FDSL_STRING 11

// ast type
#define FDSL_PROGRAM 101
#define FDSL_STATEMENT_LIST 102
#define FDSL_DECLARATION 103
#define FDSL_STATEMENT 104
#define FDSL_VARIABLE_DECLARATION 105
#define FDSL_FUNCTION_DECLARATION 106
#define FDSL_CLASS_DECLARATION 107
#define FDSL_COMPOUND_STATEMENT 108
#define FDSL_ASSIGNMENT_STATEMENT 109
#define FDSL_EMPTY_STATEMENT 110
#define FDSL_RETURN_STATEMENT 111
#define FDSL_BINARY_OP 112
#define FDSL_UNARY_OP 113
#define FDSL_VARIABLE 114
#define FDSL_INTEGER_CONSTANT 115
#define FDSL_FLOATING_CONSTANT 116
#define FDSL_BOOLEAN_CONSTANT 117
#define FDSL_STRING_CONSTANT 118
#define FDSL_FUNCTION_DECLARATION_BODY 119
#define FDSL_SCOPE 120

// symbol type
#define FDSL_BUILTIN_TYPE_SYMBOL 201
#define FDSL_VARIABLE_SYMBOL 202
#define FDSL_FUNCTION_SYMBOL 203

namespace dsl {

class Type {
public:
  static std::string name(int value);
  static int value(const std::string &name);
};

} // namespace dsl

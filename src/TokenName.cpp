// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "TokenName.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.tab.hpp"
#include <string>
#include <unordered_map>

static std::unordered_map<int, std::string> TokenNameMap = {
    {T_EOF, "eof"},
    {T_TRUE, "true"},
    {T_FALSE, "false"},
    {T_LET, "let"},
    {T_VAR, "var"},
    {T_VAL, "val"},
    {T_NIL, "nil"},
    {T_NULL, "null"},
    {T_NEW, "new"},
    {T_DELETE, "delete"},
    {T_IF, "if"},
    {T_THEN, "then"},
    {T_ELSE, "else"},
    {T_FOR, "for"},
    {T_FOREACH, "foreach"},
    {T_IN, "in"},
    {T_WHILE, "while"},
    {T_BREAK, "break"},
    {T_CONTINUE, "continue"},
    {T_SWITCH, "switch"},
    {T_CASE, "case"},
    {T_MATCH, "match"},
    {T_DEFAULT, "default"},
    {T_FUNC, "func"},
    {T_CLASS, "class"},
    {T_TYPE, "type"},
    {T_IS, "is"},
    {T_ISA, "isa"},
    {T_ISINSTANCEOF, "isinstanceof"},
    {T_IMPORT, "import"},
    {T_RETURN, "return"},
    {T_VOID, "void"},
    {T_LOGIC_AND, "and"},
    {T_LOGIC_OR, "or"},
    {T_LOGIC_NOT, "not"},
    {T_ASYNC, "async"},
    {T_AWAIT, "await"},
    {T_STATIC, "static"},
    {T_PUBLIC, "public"},
    {T_PROTECT, "protect"},
    {T_PRIVATE, "private"},
    {T_ADD, "+"},
    {T_SUB, "-"},
    {T_MUL, "*"},
    {T_DIV, "/"},
    {T_MOD, "%"},
    {T_BIT_NOT, "~"},
    {T_BIT_AND, "&"},
    {T_BIT_OR, "|"},
    {T_BIT_XOR, "^"},
    {T_BIT_LSHIFT, "<<"},
    {T_BIT_RSHIFT, ">>"},
    {T_BIT_ARSHIFT, ">>>"},
    {T_ASSIGN, "="},
    {T_ADD_ASSIGN, "+="},
    {T_SUB_ASSIGN, "-="},
    {T_MUL_ASSIGN, "*="},
    {T_DIV_ASSIGN, "/="},
    {T_MOD_ASSIGN, "%="},
    {T_BIT_AND_ASSIGN, "&="},
    {T_BIT_OR_ASSIGN, "|="},
    {T_BIT_XOR_ASSIGN, "^="},
    {T_BIT_LSHIFT_ASSIGN, "<<="},
    {T_BIT_RSHIFT_ASSIGN, ">>="},
    {T_BIT_ARSHIFT_ASSIGN, ">>>="},
    {T_EQ, "=="},
    {T_NEQ, "!="},
    {T_LT, "<"},
    {T_LE, "<="},
    {T_GT, ">"},
    {T_GE, ">="},
    {T_LPAREN, "("},
    {T_RPAREN, ")"},
    {T_LBRACKET, "["},
    {T_RBRACKET, "]"},
    {T_LBRACE, "{"},
    {T_RBRACE, "}"},
    {T_COMMA, ","},
    {T_SEMI, ";"},
    {T_QUESTION, "?"},
    {T_COLON, ":"},
    {T_DOT, "."},
    {T_RIGHT_ARROW, "=>"},
};

std::string tokenName(int token) {
  EX_ASSERT(TokenNameMap.find(token) != TokenNameMap.end(),
            "token not exist: {}", token);
  return TokenNameMap[token];
}

bool tokenNameExist(int token) {
  return TokenNameMap.find(token) != TokenNameMap.end();
}

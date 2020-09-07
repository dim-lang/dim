// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Token.h"
#include "Log.h"
#include "parser.tab.hh"
#include <string>
#include <unordered_map>

Token::Token(int a_value, YYSTYPE a_yylval, YYLTYPE a_yylloc)
    : value(a_value), yylval(a_yylval), yylloc(a_yylloc) {}

#define NAME_VALUE(t, x)                                                       \
  { yytokentype::t, x }

const static std::unordered_map<int, std::string> TokenNameMap = {
    NAME_VALUE(T_TRUE, "true"),
    NAME_VALUE(T_FALSE, "false"),
    NAME_VALUE(T_TRY, "try"),
    NAME_VALUE(T_CATCH, "catch"),
    NAME_VALUE(T_FINALLY, "finally"),
    NAME_VALUE(T_THROW, "throw"),
    NAME_VALUE(T_YIELD, "yield"),
    NAME_VALUE(T_VAR, "var"),
    NAME_VALUE(T_VAL, "val"),
    NAME_VALUE(T_NIL, "nil"),
    NAME_VALUE(T_NEW, "new"),
    NAME_VALUE(T_DELETE, "delete"),
    NAME_VALUE(T_DEF, "def"),
    NAME_VALUE(T_IF, "if"),
    NAME_VALUE(T_THEN, "then"),
    NAME_VALUE(T_ELSE, "else"),
    NAME_VALUE(T_MATCH, "match"),
    NAME_VALUE(T_ENUM, "enum"),
    NAME_VALUE(T_SWITCH, "switch"),
    NAME_VALUE(T_CASE, "case"),
    NAME_VALUE(T_FOR, "for"),
    NAME_VALUE(T_FOREACH, "foreach"),
    NAME_VALUE(T_IN, "in"),
    NAME_VALUE(T_WHILE, "while"),
    NAME_VALUE(T_DO, "do"),
    NAME_VALUE(T_BREAK, "break"),
    NAME_VALUE(T_CONTINUE, "continue"),
    NAME_VALUE(T_CLASS, "class"),
    NAME_VALUE(T_TRAIT, "trait"),
    NAME_VALUE(T_TYPE, "type"),
    NAME_VALUE(T_THIS, "this"),
    NAME_VALUE(T_SUPER, "super"),
    NAME_VALUE(T_ISINSTANCEOF, "isinstanceof"),
    NAME_VALUE(T_ISA, "isa"),
    NAME_VALUE(T_IS, "is"),
    NAME_VALUE(T_IMPORT, "import"),
    NAME_VALUE(T_AS, "as"),
    NAME_VALUE(T_RETURN, "return"),
    NAME_VALUE(T_VOID, "void"),
    NAME_VALUE(T_ANY, "any"),
    NAME_VALUE(T_NAN, "nan"),
    NAME_VALUE(T_INF, "inf"),
    NAME_VALUE(T_ASYNC, "async"),
    NAME_VALUE(T_AWAIT, "await"),
    NAME_VALUE(T_STATIC, "static"),
    NAME_VALUE(T_PUBLIC, "public"),
    NAME_VALUE(T_PROTECT, "protect"),
    NAME_VALUE(T_PRIVATE, "private"),
    NAME_VALUE(T_PREFIX, "prefix"),
    NAME_VALUE(T_POSTFIX, "postfix"),
    NAME_VALUE(T_PACKAGE, "package"),
    NAME_VALUE(T_BYTE, "byte"),
    NAME_VALUE(T_UBYTE, "ubyte"),
    NAME_VALUE(T_SHORT, "short"),
    NAME_VALUE(T_USHORT, "ushort"),
    NAME_VALUE(T_INT, "int"),
    NAME_VALUE(T_UINT, "uint"),
    NAME_VALUE(T_LONG, "long"),
    NAME_VALUE(T_ULONG, "ulong"),
    NAME_VALUE(T_LLONG, "llong"),
    NAME_VALUE(T_ULLONG, "ullong"),
    NAME_VALUE(T_FLOAT, "float"),
    NAME_VALUE(T_DOUBLE, "double"),
    NAME_VALUE(T_BOOLEAN, "boolean"),
    NAME_VALUE(T_CHAR, "char"),
    NAME_VALUE(T_AND, "and"),
    NAME_VALUE(T_OR, "or"),
    NAME_VALUE(T_NOT, "not"),
    NAME_VALUE(T_PLUS, "+"),
    NAME_VALUE(T_PLUS2, "++"),
    NAME_VALUE(T_MINUS, "-"),
    NAME_VALUE(T_MINUS2, "--"),
    NAME_VALUE(T_ASTERISK, "*"),
    NAME_VALUE(T_ASTERISK2, "**"),
    NAME_VALUE(T_SLASH, "/"),
    NAME_VALUE(T_SLASH2, "//"),
    NAME_VALUE(T_PERCENT, "%"),
    NAME_VALUE(T_PERCENT2, "%%"),
    NAME_VALUE(T_AMPERSAND, "&"),
    NAME_VALUE(T_AMPERSAND2, "&&"),
    NAME_VALUE(T_BAR, "|"),
    NAME_VALUE(T_BAR2, "||"),
    NAME_VALUE(T_TILDE, "~"),
    NAME_VALUE(T_EXCLAM, "!"),
    NAME_VALUE(T_CARET, "^"),
    NAME_VALUE(T_CARET2, "^^"),
    NAME_VALUE(T_LSHIFT, "<<"),
    NAME_VALUE(T_RSHIFT, ">>"),
    NAME_VALUE(T_ARSHIFT, ">>>"),
    NAME_VALUE(T_EQUAL, "="),
    NAME_VALUE(T_PLUS_EQUAL, "+="),
    NAME_VALUE(T_MINUS_EQUAL, "-="),
    NAME_VALUE(T_ASTERISK_EQUAL, "*="),
    NAME_VALUE(T_SLASH_EQUAL, "/="),
    NAME_VALUE(T_PERCENT_EQUAL, "%="),
    NAME_VALUE(T_AMPERSAND_EQUAL, "&="),
    NAME_VALUE(T_BAR_EQUAL, "|="),
    NAME_VALUE(T_CARET_EQUAL, "^="),
    NAME_VALUE(T_LSHIFT_EQUAL, "<<="),
    NAME_VALUE(T_RSHIFT_EQUAL, ">>="),
    NAME_VALUE(T_ARSHIFT_EQUAL, ">>>="),
    NAME_VALUE(T_EQ, "=="),
    NAME_VALUE(T_NEQ, "!="),
    NAME_VALUE(T_LT, "<"),
    NAME_VALUE(T_LE, "<="),
    NAME_VALUE(T_GT, ">"),
    NAME_VALUE(T_GE, ">="),
    NAME_VALUE(T_LPAREN, "("),
    NAME_VALUE(T_RPAREN, ")"),
    NAME_VALUE(T_LBRACKET, "["),
    NAME_VALUE(T_RBRACKET, "]"),
    NAME_VALUE(T_LBRACE, "{"),
    NAME_VALUE(T_RBRACE, "}"),
    NAME_VALUE(T_UNDERSCORE, "_"),
    NAME_VALUE(T_COMMA, ","),
    NAME_VALUE(T_SEMI, ";"),
    NAME_VALUE(T_QUESTION, "?"),
    NAME_VALUE(T_COLON, ":"),
    NAME_VALUE(T_COLON2, "::"),
    NAME_VALUE(T_DOT, "."),
    NAME_VALUE(T_DOT2, ".."),
    NAME_VALUE(T_LARROW, "<-"),
    NAME_VALUE(T_RARROW, "->"),
    NAME_VALUE(T_DOUBLE_RARROW, "=>"),
    NAME_VALUE(T_COLON_LARROW, "<:"),
    NAME_VALUE(T_COLON_RARROW, ":>"),
    NAME_VALUE(T_NEWLINE, "\\n"),
    NAME_VALUE(T_INTEGER_LITERAL, "integer_literal"),
    NAME_VALUE(T_FLOAT_LITERAL, "float_literal"),
    NAME_VALUE(T_STRING_LITERAL, "string_literal"),
    NAME_VALUE(T_CHARACTER_LITERAL, "character_literal"),
    NAME_VALUE(T_VAR_ID, "varId"),
};

namespace detail {
struct TokenValueMapImpl {
  TokenValueMapImpl() {
    for (auto i = TokenNameMap.begin(); i != TokenNameMap.end(); i++) {
      tokenValueMap.insert(std::make_pair(i->second, i->first));
    }
  }
  std::unordered_map<std::string, int> tokenValueMap;
};
} // namespace detail

const static detail::TokenValueMapImpl tokenValueMapImpl;

std::string tokenName(int value) {
  LOG_ASSERT(tokenValid(value), "invalid token value: {}", value);
  return TokenNameMap.find(value)->second;
}

int tokenValue(const std::string &name) {
  LOG_ASSERT(tokenValid(name), "invalid token name: {}", name);
  return tokenValueMapImpl.tokenValueMap.find(name)->second;
}

bool tokenValid(int token) {
  return TokenNameMap.find(token) != TokenNameMap.end();
}

bool tokenValid(const std::string &name) {
  return tokenValueMapImpl.tokenValueMap.find(name) !=
         tokenValueMapImpl.tokenValueMap.end();
}

bool tokenIsLiteral(int value) {
  return value == T_INTEGER_LITERAL || value == T_FLOAT_LITERAL ||
         value == T_STRING_LITERAL || value == T_CHARACTER_LITERAL;
}

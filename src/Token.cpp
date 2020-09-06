// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Token.h"
#include "Log.h"
#include "boost/preprocessor/stringize.hpp"
#include "parser.tab.hh"
#include <string>
#include <unordered_map>

Token::Token(int a_value, YYSTYPE a_yylval, YYLTYPE a_yylloc)
    : value(a_value), yylval(a_yylval), yylloc(a_yylloc) {}

#define NAME_VALUE(t)                                                          \
  { yytokentype::t, BOOST_PP_STRINGIZE(t) }

const static std::unordered_map<int, std::string> TokenNameMap = {
    NAME_VALUE(T_TRUE),
    NAME_VALUE(T_FALSE),
    NAME_VALUE(T_TRY),
    NAME_VALUE(T_CATCH),
    NAME_VALUE(T_FINALLY),
    NAME_VALUE(T_THROW),
    NAME_VALUE(T_YIELD),
    NAME_VALUE(T_VAR),
    NAME_VALUE(T_VAL),
    NAME_VALUE(T_NIL),
    NAME_VALUE(T_NEW),
    NAME_VALUE(T_DELETE),
    NAME_VALUE(T_DEF),
    NAME_VALUE(T_IF),
    NAME_VALUE(T_THEN),
    NAME_VALUE(T_ELSE),
    NAME_VALUE(T_MATCH),
    NAME_VALUE(T_ENUM),
    NAME_VALUE(T_SWITCH),
    NAME_VALUE(T_CASE),
    NAME_VALUE(T_FOR),
    NAME_VALUE(T_FOREACH),
    NAME_VALUE(T_IN),
    NAME_VALUE(T_WHILE),
    NAME_VALUE(T_DO),
    NAME_VALUE(T_BREAK),
    NAME_VALUE(T_CONTINUE),
    NAME_VALUE(T_CLASS),
    NAME_VALUE(T_TRAIT),
    NAME_VALUE(T_TYPE),
    NAME_VALUE(T_THIS),
    NAME_VALUE(T_SUPER),
    NAME_VALUE(T_ISINSTANCEOF),
    NAME_VALUE(T_ISA),
    NAME_VALUE(T_IS),
    NAME_VALUE(T_IMPORT),
    NAME_VALUE(T_AS),
    NAME_VALUE(T_RETURN),
    NAME_VALUE(T_VOID),
    NAME_VALUE(T_ANY),
    NAME_VALUE(T_NAN),
    NAME_VALUE(T_INF),
    NAME_VALUE(T_ASYNC),
    NAME_VALUE(T_AWAIT),
    NAME_VALUE(T_STATIC),
    NAME_VALUE(T_PUBLIC),
    NAME_VALUE(T_PROTECT),
    NAME_VALUE(T_PRIVATE),
    NAME_VALUE(T_PREFIX),
    NAME_VALUE(T_POSTFIX),
    NAME_VALUE(T_PACKAGE),
    NAME_VALUE(T_BYTE),
    NAME_VALUE(T_UBYTE),
    NAME_VALUE(T_SHORT),
    NAME_VALUE(T_USHORT),
    NAME_VALUE(T_INT),
    NAME_VALUE(T_UINT),
    NAME_VALUE(T_LONG),
    NAME_VALUE(T_ULONG),
    NAME_VALUE(T_LLONG),
    NAME_VALUE(T_ULLONG),
    NAME_VALUE(T_FLOAT),
    NAME_VALUE(T_DOUBLE),
    NAME_VALUE(T_BOOLEAN),
    NAME_VALUE(T_CHAR),
    NAME_VALUE(T_AND),
    NAME_VALUE(T_OR),
    NAME_VALUE(T_NOT),
    NAME_VALUE(T_PLUS),
    NAME_VALUE(T_PLUS2),
    NAME_VALUE(T_MINUS),
    NAME_VALUE(T_MINUS2),
    NAME_VALUE(T_ASTERISK),
    NAME_VALUE(T_ASTERISK2),
    NAME_VALUE(T_SLASH),
    NAME_VALUE(T_SLASH2),
    NAME_VALUE(T_PERCENT),
    NAME_VALUE(T_PERCENT2),
    NAME_VALUE(T_AMPERSAND),
    NAME_VALUE(T_AMPERSAND2),
    NAME_VALUE(T_BAR),
    NAME_VALUE(T_BAR2),
    NAME_VALUE(T_TILDE),
    NAME_VALUE(T_EXCLAM),
    NAME_VALUE(T_CARET),
    NAME_VALUE(T_CARET2),
    NAME_VALUE(T_LSHIFT),
    NAME_VALUE(T_RSHIFT),
    NAME_VALUE(T_ARSHIFT),
    NAME_VALUE(T_EQUAL),
    NAME_VALUE(T_PLUS_EQUAL),
    NAME_VALUE(T_MINUS_EQUAL),
    NAME_VALUE(T_ASTERISK_EQUAL),
    NAME_VALUE(T_SLASH_EQUAL),
    NAME_VALUE(T_PERCENT_EQUAL),
    NAME_VALUE(T_AMPERSAND_EQUAL),
    NAME_VALUE(T_BAR_EQUAL),
    NAME_VALUE(T_CARET_EQUAL),
    NAME_VALUE(T_LSHIFT_EQUAL),
    NAME_VALUE(T_RSHIFT_EQUAL),
    NAME_VALUE(T_ARSHIFT_EQUAL),
    NAME_VALUE(T_EQ),
    NAME_VALUE(T_NEQ),
    NAME_VALUE(T_LT),
    NAME_VALUE(T_LE),
    NAME_VALUE(T_GT),
    NAME_VALUE(T_GE),
    NAME_VALUE(T_LPAREN),
    NAME_VALUE(T_RPAREN),
    NAME_VALUE(T_LBRACKET),
    NAME_VALUE(T_RBRACKET),
    NAME_VALUE(T_LBRACE),
    NAME_VALUE(T_RBRACE),
    NAME_VALUE(T_UNDERSCORE),
    NAME_VALUE(T_COMMA),
    NAME_VALUE(T_SEMI),
    NAME_VALUE(T_QUESTION),
    NAME_VALUE(T_COLON),
    NAME_VALUE(T_COLON2),
    NAME_VALUE(T_DOT),
    NAME_VALUE(T_DOT2),
    NAME_VALUE(T_LARROW),
    NAME_VALUE(T_RARROW),
    NAME_VALUE(T_DOUBLE_RARROW),
    NAME_VALUE(T_COLON_LARROW),
    NAME_VALUE(T_COLON_RARROW),
    NAME_VALUE(T_NEWLINE),
    NAME_VALUE(T_INTEGER_LITERAL),
    NAME_VALUE(T_FLOAT_LITERAL),
    NAME_VALUE(T_STRING_LITERAL),
    NAME_VALUE(T_CHARACTER_LITERAL),
    NAME_VALUE(T_VAR_ID),
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

bool isLiteralToken(int value) {
  return value == T_INTEGER_LITERAL || value == T_FLOAT_LITERAL ||
         value == T_STRING_LITERAL || value == T_CHARACTER_LITERAL;
}

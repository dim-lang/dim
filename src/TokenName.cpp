// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "TokenName.h"
#include "Ast.h"
#include "Log.h"
#include "parser.tab.hh"
#include <string>
#include <unordered_map>

const static std::unordered_map<std::string, int> Token_NameInt_Map = {
    {"empty", yy::parser::token::T_EMPTY},
    {"true", yy::parser::token::T_TRUE},
    {"false", yy::parser::token::T_FALSE},
    {"try", yy::parser::token::T_TRY},
    {"catch", yy::parser::token::T_CATCH},
    {"finally", yy::parser::token::T_FINALLY},
    {"throw", yy::parser::token::T_THROW},
    {"yield", yy::parser::token::T_YIELD},
    {"var", yy::parser::token::T_VAR},
    {"val", yy::parser::token::T_VAL},
    {"nil", yy::parser::token::T_NIL},
    {"new", yy::parser::token::T_NEW},
    {"delete", yy::parser::token::T_DELETE},
    {"def", yy::parser::token::T_DEF},
    {"if", yy::parser::token::T_IF},
    {"then", yy::parser::token::T_THEN},
    {"else", yy::parser::token::T_ELSE},
    {"semi_else", yy::parser::token::T_SEMI_ELSE},
    {"match", yy::parser::token::T_MATCH},
    {"enum", yy::parser::token::T_ENUM},
    {"switch", yy::parser::token::T_SWITCH},
    {"case", yy::parser::token::T_CASE},
    {"for", yy::parser::token::T_FOR},
    {"foreach", yy::parser::token::T_FOREACH},
    {"in", yy::parser::token::T_IN},
    {"while", yy::parser::token::T_WHILE},
    {"semi_while", yy::parser::token::T_SEMI_WHILE},
    {"do", yy::parser::token::T_DO},
    {"break", yy::parser::token::T_BREAK},
    {"continue", yy::parser::token::T_CONTINUE},
    {"class", yy::parser::token::T_CLASS},
    {"trait", yy::parser::token::T_TRAIT},
    {"type", yy::parser::token::T_TYPE},
    {"this", yy::parser::token::T_THIS},
    {"super", yy::parser::token::T_SUPER},
    {"isinstanceof", yy::parser::token::T_ISINSTANCEOF},
    {"isa", yy::parser::token::T_ISA},
    {"is", yy::parser::token::T_IS},
    {"import", yy::parser::token::T_IMPORT},
    {"as", yy::parser::token::T_AS},
    {"return", yy::parser::token::T_RETURN},
    {"void", yy::parser::token::T_VOID},
    {"nan", yy::parser::token::T_NAN},
    {"inf", yy::parser::token::T_INF},
    {"async", yy::parser::token::T_ASYNC},
    {"await", yy::parser::token::T_AWAIT},
    {"static", yy::parser::token::T_STATIC},
    {"public", yy::parser::token::T_PUBLIC},
    {"protect", yy::parser::token::T_PROTECT},
    {"private", yy::parser::token::T_PRIVATE},
    {"prefix", yy::parser::token::T_PREFIX},
    {"postfix", yy::parser::token::T_POSTFIX},
    {"package", yy::parser::token::T_PACKAGE},

    /* primitive type */
    {"byte", yy::parser::token::T_BYTE},
    {"ubyte", yy::parser::token::T_UBYTE},
    {"short", yy::parser::token::T_SHORT},
    {"ushort", yy::parser::token::T_USHORT},
    {"int", yy::parser::token::T_INT},
    {"uint", yy::parser::token::T_UINT},
    {"long", yy::parser::token::T_LONG},
    {"ulong", yy::parser::token::T_ULONG},
    {"llong", yy::parser::token::T_LLONG},
    {"ullong", yy::parser::token::T_ULLONG},
    {"float", yy::parser::token::T_FLOAT},
    {"double", yy::parser::token::T_DOUBLE},
    {"boolean", yy::parser::token::T_BOOLEAN},
    {"char", yy::parser::token::T_CHAR},

    /* and or not operator */
    {"and", yy::parser::token::T_AND},
    {"or", yy::parser::token::T_OR},
    {"not", yy::parser::token::T_NOT},

    /* operator */
    {"+", yy::parser::token::T_PLUS},
    {"++", yy::parser::token::T_PLUS2},
    {"-", yy::parser::token::T_MINUS},
    {"--", yy::parser::token::T_MINUS2},
    {"*", yy::parser::token::T_ASTERISK},
    {"**", yy::parser::token::T_ASTERISK2},
    {"/", yy::parser::token::T_SLASH},
    {"//", yy::parser::token::T_SLASH2},
    {"%", yy::parser::token::T_PERCENT},
    {"%%", yy::parser::token::T_PERCENT2},

    {"&", yy::parser::token::T_AMPERSAND},
    {"&&", yy::parser::token::T_AMPERSAND2},
    {"|", yy::parser::token::T_BAR},
    {"||", yy::parser::token::T_BAR2},
    {"~", yy::parser::token::T_TILDE},
    {"!", yy::parser::token::T_EXCLAM},
    {"^", yy::parser::token::T_CARET},
    {"^^", yy::parser::token::T_CARET2},

    {"<<", yy::parser::token::T_LSHIFT},
    {">>", yy::parser::token::T_RSHIFT},
    {">>>", yy::parser::token::T_ARSHIFT},

    /* equal operator */
    {"=", yy::parser::token::T_EQUAL},
    {"+=", yy::parser::token::T_PLUS_EQUAL},
    {"-=", yy::parser::token::T_MINUS_EQUAL},
    {"*=", yy::parser::token::T_ASTERISK_EQUAL},
    {"/=", yy::parser::token::T_SLASH_EQUAL},
    {"%=", yy::parser::token::T_PERCENT_EQUAL},
    {"&=", yy::parser::token::T_AMPERSAND_EQUAL},
    {"|=", yy::parser::token::T_BAR_EQUAL},
    {"^=", yy::parser::token::T_CARET_EQUAL},
    {"<<=", yy::parser::token::T_LSHIFT_EQUAL},
    {">>=", yy::parser::token::T_RSHIFT_EQUAL},
    {">>>=", yy::parser::token::T_ARSHIFT_EQUAL},

    /* compare operator */
    {"==", yy::parser::token::T_EQ},
    {"!=", yy::parser::token::T_NEQ},
    {"<", yy::parser::token::T_LT},
    {"<=", yy::parser::token::T_LE},
    {">", yy::parser::token::T_GT},
    {">=", yy::parser::token::T_GE},

    /* parentheses */
    {"(", yy::parser::token::T_LPAREN},
    {")", yy::parser::token::T_RPAREN},
    {"[", yy::parser::token::T_LBRACKET},
    {"]", yy::parser::token::T_RBRACKET},
    {"{", yy::parser::token::T_LBRACE},
    {"}", yy::parser::token::T_RBRACE},

    /* other punctuation */
    {"_", yy::parser::token::T_UNDERSCORE},
    {",", yy::parser::token::T_COMMA},
    {";", yy::parser::token::T_SEMI},
    {"?", yy::parser::token::T_QUESTION},
    {":", yy::parser::token::T_COLON},
    {"::", yy::parser::token::T_COLON2},
    {".", yy::parser::token::T_DOT},
    {"..", yy::parser::token::T_DOT2},
    {"<-", yy::parser::token::T_LARROW},
    {"->", yy::parser::token::T_RARROW},
    {"=>", yy::parser::token::T_DOUBLE_RARROW},
    {":>", yy::parser::token::T_COLON_LARROW},
    {"<:", yy::parser::token::T_COLON_RARROW},
};

namespace detail {

struct Token_IntName_Map_Impl {
  std::unordered_map<int, std::string> impl;

  Token_IntName_Map_Impl(const std::unordered_map<std::string, int> &nameint) {
    std::for_each(nameint.begin(), nameint.end(),
                  [&](std::unordered_map<std::string, int>::const_iterator i) {
                    impl.insert(std::make_pair(i->second, i->first));
                  });
    /* for (std::unordered_map<std::string, int>::const_iterator i = */
    /*          nameint.cbegin(); */
    /*      i != nameint.cend(); i++) { */
    /*   impl.insert(std::make_pair(i->second, i->first)); */
    /* } */
  }
};

} // namespace detail

const static detail::Token_IntName_Map_Impl
    Token_IntName_Map(Token_NameInt_Map);

const std::string &tokenName(int token) {
  EX_ASSERT(Token_IntName_Map.impl.find(token) != Token_IntName_Map.impl.end(),
            "token value {} not exist", token);
  return Token_IntName_Map.impl.find(token)->second;
}

const int &tokenValue(const std::string &token) {
  EX_ASSERT(Token_NameInt_Map.find(token) != Token_NameInt_Map.end(),
            "token name {} not exist", token);
  return Token_NameInt_Map.find(token)->second;
}

bool tokenExist(int token) {
  return Token_IntName_Map.impl.find(token) != Token_IntName_Map.impl.end();
}

bool tokenExist(const std::string &token) {
  return Token_NameInt_Map.find(token) != Token_NameInt_Map.end();
}

// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "TokenName.h"
#include "Ast.h"
#include "Log.h"
#include "Parser.tab.h"
#include <string>
#include <unordered_map>

const static std::unordered_map<std::string, int> Token_NameInt_Map = {
    {"true", T_TRUE},
    {"false", T_FALSE},
    {"try", T_TRY},
    {"catch", T_CATCH},
    {"var", T_VAR},
    {"val", T_VAL},
    {"nil", T_NIL},
    {"new", T_NEW},
    {"delete", T_DELETE},
    {"def", T_DEF},
    {"if", T_IF},
    {"then", T_THEN},
    {"else", T_ELSE},
    {"enum", T_ENUM},
    {"switch", T_SWITCH},
    {"case", T_CASE},
    {"match", T_MATCH},
    {"for", T_FOR},
    {"foreach", T_FOREACH},
    {"in", T_IN},
    {"while", T_WHILE},
    {"do", T_DO},
    {"break", T_BREAK},
    {"continue", T_CONTINUE},
    {"class", T_CLASS},
    {"type", T_TYPE},
    {"this", T_THIS},
    {"super", T_SUPER},
    {"is", T_IS},
    {"isa", T_ISA},
    {"isinstanceof", T_ISINSTANCEOF},
    {"import", T_IMPORT},
    {"return", T_RETURN},
    {"void", T_VOID},
    {"nan", T_NAN},
    {"inf", T_INF},
    {"async", T_ASYNC},
    {"await", T_AWAIT},
    {"static", T_STATIC},
    {"public", T_PUBLIC},
    {"protect", T_PROTECT},
    {"private", T_PRIVATE},

    /***
     * primitive integer type
     */
    {"byte", T_BYTE},
    {"ubyte", T_UBYTE},
    {"short", T_SHORT},
    {"ushort", T_USHORT},
    {"int", T_INT},
    {"uint", T_UINT},
    {"long", T_LONG},
    {"ulong", T_ULONG},
    {"llong", T_LLONG},
    {"ullong", T_ULLONG},

    /***
     * primitive integer type
     */
    {"float", T_FLOAT},
    {"double", T_DOUBLE},
    {"ldouble", T_LDOUBLE},

    /***
     * primitive boolean type
     */
    {"boolean", T_BOOLEAN},

    /* and or not operator */
    {"and", T_LOGIC_AND},
    {"&&", T_LOGIC_AND},
    {"or", T_LOGIC_OR},
    {"||", T_LOGIC_OR},
    {"not", T_LOGIC_NOT},
    {"!", T_LOGIC_NOT},

    /* operator */
    {"+", T_PLUS},
    {"++", T_PLUS2},
    {"-", T_MINUS},
    {"--", T_MINUS2},
    {"*", T_ASTERISK},
    {"**", T_ASTERISK2},
    {"/", T_SLASH},
    {"//", T_SLASH2},
    {"%", T_PERCENT},
    {"%%", T_PERCENT2},

    {"&", T_AMPERSAND},
    {"|", T_BAR},
    {"~", T_TILDE},
    {"^", T_CARET},

    {"<<", T_LSHIFT},
    {">>", T_RSHIFT},
    {">>>", T_ARSHIFT},

    /* equal operator */
    {"=", T_EQUAL},
    {"+=", T_PLUS_EQUAL},
    {"-=", T_MINUS_EQUAL},
    {"*=", T_ASTERISK_EQUAL},
    {"/=", T_SLASH_EQUAL},
    {"%=", T_PERCENT_EQUAL},
    {"&=", T_AMPERSAND_EQUAL},
    {"|=", T_BAR_EQUAL},
    {"^=", T_CARET_EQUAL},
    {"<<=", T_LSHIFT_EQUAL},
    {">>=", T_RSHIFT_EQUAL},
    {">>>=", T_ARSHIFT_EQUAL},

    /* compare operator */
    {"==", T_EQ},
    {"!=", T_NEQ},
    {"<", T_LT},
    {"<=", T_LE},
    {">", T_GT},
    {">=", T_GE},

    /* parentheses */
    {"(", T_LPAREN},
    {")", T_RPAREN},
    {"[", T_LBRACKET},
    {"]", T_RBRACKET},
    {"{", T_LBRACE},
    {"}", T_RBRACE},

    /* other punctuation */
    {"_", T_UNDERSCORE},
    {",", T_COMMA},
    {";", T_SEMI},
    {"?", T_QUESTION},
    {":", T_COLON},
    {".", T_DOT},
    {"<-", T_THIN_LARROW},
    {"->", T_THIN_RARROW},
    {"=>", T_FAT_RARROW},
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

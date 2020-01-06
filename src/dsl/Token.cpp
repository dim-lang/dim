// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "dsl/Token.h"
#include "Logging.h"
#include <fmt/format.h>
#include <string>

namespace dsl {

const std::shared_ptr<Token> Token::TEOF(new Token(FDSL_EOF));

// operator
const std::shared_ptr<Token> Token::TADD(new Token(FDSL_OPERATOR,
                                                   UNICODE_STRING_SIMPLE("+")));
const std::shared_ptr<Token> Token::TSUB(new Token(FDSL_OPERATOR,
                                                   UNICODE_STRING_SIMPLE("-")));
const std::shared_ptr<Token> Token::TMUL(new Token(FDSL_OPERATOR,
                                                   UNICODE_STRING_SIMPLE("*")));
const std::shared_ptr<Token> Token::TDIV(new Token(FDSL_OPERATOR,
                                                   UNICODE_STRING_SIMPLE("/")));
const std::shared_ptr<Token> Token::TMOD(new Token(FDSL_OPERATOR,
                                                   UNICODE_STRING_SIMPLE("%")));
const std::shared_ptr<Token> Token::TNOT(new Token(FDSL_OPERATOR,
                                                   UNICODE_STRING_SIMPLE("!")));
const std::shared_ptr<Token>
    Token::TINC(new Token(FDSL_OPERATOR, UNICODE_STRING_SIMPLE("++")));
const std::shared_ptr<Token>
    Token::TDEC(new Token(FDSL_OPERATOR, UNICODE_STRING_SIMPLE("--")));
const std::shared_ptr<Token>
    Token::TADDASSIGN(new Token(FDSL_OPERATOR, UNICODE_STRING_SIMPLE("+=")));
const std::shared_ptr<Token>
    Token::TSUBASSIGN(new Token(FDSL_OPERATOR, UNICODE_STRING_SIMPLE("-=")));
const std::shared_ptr<Token>
    Token::TMULASSIGN(new Token(FDSL_OPERATOR, UNICODE_STRING_SIMPLE("*=")));
const std::shared_ptr<Token>
    Token::TDIVASSIGN(new Token(FDSL_OPERATOR, UNICODE_STRING_SIMPLE("/=")));
const std::shared_ptr<Token>
    Token::TMODASSIGN(new Token(FDSL_OPERATOR, UNICODE_STRING_SIMPLE("%=")));

const std::shared_ptr<Token>
    Token::TASSIGN(new Token(FDSL_OPERATOR, UNICODE_STRING_SIMPLE("=")));

const std::shared_ptr<Token> Token::TEQ(new Token(FDSL_COMPARATOR,
                                                  UNICODE_STRING_SIMPLE("==")));
const std::shared_ptr<Token>
    Token::TNEQ(new Token(FDSL_COMPARATOR, UNICODE_STRING_SIMPLE("!=")));
const std::shared_ptr<Token> Token::TLT(new Token(FDSL_COMPARATOR,
                                                  UNICODE_STRING_SIMPLE("<")));
const std::shared_ptr<Token> Token::TLE(new Token(FDSL_COMPARATOR,
                                                  UNICODE_STRING_SIMPLE("<=")));
const std::shared_ptr<Token> Token::TGT(new Token(FDSL_COMPARATOR,
                                                  UNICODE_STRING_SIMPLE(">")));
const std::shared_ptr<Token> Token::TGE(new Token(FDSL_COMPARATOR,
                                                  UNICODE_STRING_SIMPLE(">=")));

const std::shared_ptr<Token>
    Token::TTRUE(new Token(FDSL_BOOLEAN, UNICODE_STRING_SIMPLE("True")));
const std::shared_ptr<Token>
    Token::TFALSE(new Token(FDSL_BOOLEAN, UNICODE_STRING_SIMPLE("False")));

const std::shared_ptr<Token>
    Token::TLPAREN(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE("(")));
const std::shared_ptr<Token>
    Token::TRPAREN(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE("))")));
const std::shared_ptr<Token>
    Token::TLBRACKET(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE("[")));
const std::shared_ptr<Token>
    Token::TRBRACKET(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE("]")));
const std::shared_ptr<Token>
    Token::TLBRACE(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE("{")));
const std::shared_ptr<Token>
    Token::TRBRACE(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE("}")));
const std::shared_ptr<Token>
    Token::TCOMMA(new Token(FDSL_PUNCTUATION,
                            UNICODE_STRING_SIMPLE(",UNICODE_STRING_SIMPLE(")));
const std::shared_ptr<Token>
    Token::TSEMI(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE(";")));
const std::shared_ptr<Token>
    Token::TQUESTION(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE("?")));
const std::shared_ptr<Token>
    Token::TCOLON(new Token(FDSL_PUNCTUATION, UNICODE_STRING_SIMPLE(":")));

const std::shared_ptr<Token>
    Token::TLET(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("let")));
const std::shared_ptr<Token>
    Token::TNULL(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("null")));
const std::shared_ptr<Token> Token::TIF(new Token(FDSL_KEYWORD,
                                                  UNICODE_STRING_SIMPLE("if")));
const std::shared_ptr<Token>
    Token::TELSEIF(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("elseif")));
const std::shared_ptr<Token>
    Token::TELSE(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("else")));
const std::shared_ptr<Token>
    Token::TFOR(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("for")));
const std::shared_ptr<Token>
    Token::TWHILE(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("while")));
const std::shared_ptr<Token>
    Token::TBREAK(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("break")));
const std::shared_ptr<Token> Token::TCONTINUE(
    new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("continue")));
const std::shared_ptr<Token>
    Token::TFUNC(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("func")));
const std::shared_ptr<Token>
    Token::TCLASS(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("class")));
const std::shared_ptr<Token>
    Token::TTYPE(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("type")));
const std::shared_ptr<Token> Token::TISINSTANCE(
    new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("isinstance")));
const std::shared_ptr<Token>
    Token::TIMPORT(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("import")));
const std::shared_ptr<Token>
    Token::TRETURN(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("return")));
const std::shared_ptr<Token>
    Token::TVOID(new Token(FDSL_KEYWORD, UNICODE_STRING_SIMPLE("void")));

Token::Token(int type) : type(type), literal() {}

Token::Token(int type, const icu::UnicodeString &literal)
    : type(type), literal(literal) {}

const icu::UnicodeString &Token::literalValue() const { return literal; }

int64_t Token::integerValue() const {
  std::string _1;
  FCHECK(u_isdigit(literal[0]), "invalid integer token value:{}",
         literal.toUTF8String(_1));
  return (int64_t)std::stoll(literal);
}

double Token::realValue() const {
  std::string _1;
  FCHECK(u_isdigit(literal[0]) || literal[0] == (UChar)'.',
         "invalid real token value:{}", literal.toUTF8String(_1));
  return std::stod(literal);
}

bool Token::booleanValue() const {
  std::string _1;
  FCHECK((int)literal.compare(UNICODE_STRING_SIMPLE("True")) == 0 ||
             (int)literal.compare(UNICODE_STRING_SIMPLE("False")) == 0,
         "invalid boolean token value:{}", literal.toUTF8String(_1));
  return (int)literal.compare(UNICODE_STRING_SIMPLE("True")) == 0;
}

bool Token::equal(std::shared_ptr<Token> t) const {
  return t && (t->type == type) && (t->literal == literal);
}

std::string Token::toString() const {
  std::string _1;
  return fmt::format("[ @Token type:{}, literal:{} ]", type,
                     literal.toUTF8String(_1));
}

} // namespace dsl

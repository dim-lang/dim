// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "dsl/Token.h"
#include "Logging.h"
#include <fmt/format.h>
#include <string>

namespace dsl {

const std::shared_ptr<Token> Token::TEOF(new Token(FDSL_EOF));

const std::shared_ptr<Token> Token::TADD(new Token(FDSL_OPERATOR, "+"));
const std::shared_ptr<Token> Token::TSUB(new Token(FDSL_OPERATOR, "-"));
const std::shared_ptr<Token> Token::TMUL(new Token(FDSL_OPERATOR, "*"));
const std::shared_ptr<Token>
    Token::TDIV(new OperatorToken(UNICODE_STRING_SIMPLE("/")));
const std::shared_ptr<Token>
    Token::TMOD(new OperatorToken(UNICODE_STRING_SIMPLE("%")));
const std::shared_ptr<Token>
    Token::TNOT(new OperatorToken(UNICODE_STRING_SIMPLE("!")));
const std::shared_ptr<Token>
    Token::TINC(new OperatorToken(UNICODE_STRING_SIMPLE("++")));
const std::shared_ptr<Token>
    Token::TDEC(new OperatorToken(UNICODE_STRING_SIMPLE("--")));
const std::shared_ptr<Token>
    Token::TADDASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("+=")));
const std::shared_ptr<Token>
    Token::TSUBASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("-=")));
const std::shared_ptr<Token>
    Token::TMULASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("*=")));
const std::shared_ptr<Token>
    Token::TDIVASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("/=")));
const std::shared_ptr<Token>
    Token::TMODASSIGN(new OperatorToken(UNICODE_STRING_SIMPLE("%=")));

const std::shared_ptr<Token>
    Token::TASSIGNMENT(new AssignmentToken(UNICODE_STRING_SIMPLE("=")));

const std::shared_ptr<Token>
    Token::TEQ(new ComparatorToken(UNICODE_STRING_SIMPLE("==")));
const std::shared_ptr<Token>
    Token::TNEQ(new ComparatorToken(UNICODE_STRING_SIMPLE("!=")));
const std::shared_ptr<Token>
    Token::TLT(new ComparatorToken(UNICODE_STRING_SIMPLE("<")));
const std::shared_ptr<Token>
    Token::TLE(new ComparatorToken(UNICODE_STRING_SIMPLE("<=")));
const std::shared_ptr<Token>
    Token::TGT(new ComparatorToken(UNICODE_STRING_SIMPLE(">")));
const std::shared_ptr<Token>
    Token::TGE(new ComparatorToken(UNICODE_STRING_SIMPLE(">=")));

const std::shared_ptr<Token> Token::TTRUE(new BooleanToken(true));
const std::shared_ptr<Token> Token::TFALSE(new BooleanToken(false));

const std::shared_ptr<Token>
    Token::TLP(new PunctuationToken(UNICODE_STRING_SIMPLE("(")));
const std::shared_ptr<Token>
    Token::TRP(new PunctuationToken(UNICODE_STRING_SIMPLE(")")));
const std::shared_ptr<Token>
    Token::TLBRACKET(new PunctuationToken(UNICODE_STRING_SIMPLE("[")));
const std::shared_ptr<Token>
    Token::TRBRACKET(new PunctuationToken(UNICODE_STRING_SIMPLE("]")));
const std::shared_ptr<Token>
    Token::TLBRACE(new PunctuationToken(UNICODE_STRING_SIMPLE("{")));
const std::shared_ptr<Token>
    Token::TRBRACE(new PunctuationToken(UNICODE_STRING_SIMPLE("}")));
const std::shared_ptr<Token>
    Token::TCOMMA(new PunctuationToken(UNICODE_STRING_SIMPLE(",")));
const std::shared_ptr<Token>
    Token::TSEMI(new PunctuationToken(UNICODE_STRING_SIMPLE(";")));
const std::shared_ptr<Token>
    Token::TQUESTION(new PunctuationToken(UNICODE_STRING_SIMPLE("?")));
const std::shared_ptr<Token>
    Token::TCOLON(new PunctuationToken(UNICODE_STRING_SIMPLE(":")));

const std::shared_ptr<Token>
    Token::TLET(new KeywordToken(UNICODE_STRING_SIMPLE("let")));
const std::shared_ptr<Token>
    Token::TNULL(new KeywordToken(UNICODE_STRING_SIMPLE("null")));
const std::shared_ptr<Token>
    Token::TIF(new KeywordToken(UNICODE_STRING_SIMPLE("if")));
const std::shared_ptr<Token>
    Token::TELSEIF(new KeywordToken(UNICODE_STRING_SIMPLE("elseif")));
const std::shared_ptr<Token>
    Token::TELSE(new KeywordToken(UNICODE_STRING_SIMPLE("else")));
const std::shared_ptr<Token>
    Token::TFOR(new KeywordToken(UNICODE_STRING_SIMPLE("for")));
const std::shared_ptr<Token>
    Token::TWHILE(new KeywordToken(UNICODE_STRING_SIMPLE("while")));
const std::shared_ptr<Token>
    Token::TBREAK(new KeywordToken(UNICODE_STRING_SIMPLE("break")));
const std::shared_ptr<Token>
    Token::TCONTINUE(new KeywordToken(UNICODE_STRING_SIMPLE("continue")));
const std::shared_ptr<Token>
    Token::TFUNC(new KeywordToken(UNICODE_STRING_SIMPLE("func")));
const std::shared_ptr<Token>
    Token::TCLASS(new KeywordToken(UNICODE_STRING_SIMPLE("class")));
const std::shared_ptr<Token>
    Token::TTYPE(new KeywordToken(UNICODE_STRING_SIMPLE("type")));
const std::shared_ptr<Token>
    Token::TISINSTANCE(new KeywordToken(UNICODE_STRING_SIMPLE("isinstance")));
const std::shared_ptr<Token>
    Token::TIMPORT(new KeywordToken(UNICODE_STRING_SIMPLE("import")));
const std::shared_ptr<Token>
    Token::TRETURN(new KeywordToken(UNICODE_STRING_SIMPLE("return")));
const std::shared_ptr<Token>
    Token::TVOID(new KeywordToken(UNICODE_STRING_SIMPLE("void")));

Token::Token(int type) : type(type), literal() {}

Token::Token(int type, const std::string &literal)
    : type(type), literal(literal) {}

int64_t Token::integerValue() const { return (int64_t)std::stoll(literal); }

double Token::realValue() const { return std::stod(literal); }

bool Token::booleanValue() const { return literal.compare("True") == 0; }

bool Token::equal(std::shared_ptr<Token> t) const {
  return t && (t->type == type) && (t->literal == literal);
}

std::string Token::toString() const {
  return fmt::format("[ @Token type:{}, literal:{} ]", type, literal);
}

} // namespace dsl

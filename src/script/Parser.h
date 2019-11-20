// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"
#include "script/Ast.h"
#include "script/Lexer.h"
#include "script/Token.h"
#include <deque>
#include <unicode/numfmt.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class Parser : public Stringify {
public:
  Parser(Sptr<Lexer> lexer);
  virtual ~Parser() = default;
  Ast *parse();
  virtual std::string toString() const;

private:
  void eat(Token::TokenType tokenType);
  void eat(Sptr<Token> token);
  Ast *expr();
  Ast *term();
  Ast *factor();

  Sptr<Token> token_;
  Sptr<Lexer> lexer_;
};

} // namespace fastype
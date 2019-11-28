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
#include <vector>

namespace fastype {

class Parser : public Stringify {
public:
  Parser(Sptr<Lexer> lexer);
  virtual ~Parser() = default;
  Sptr<Ast> parse();
  virtual std::string toString() const;

private:
  void eat(Token::TokenType tokenType);
  void eat(Sptr<Token> token);

  // block
  Sptr<Ast> parseProgram();
  Sptr<Ast> parseStatementList();

  // declaration
  Sptr<Ast> parseVariableDeclaration();
  Sptr<Ast> parseFunctionDeclaration();
  Sptr<Ast> parseClassDeclaration();

  // statement
  Sptr<Ast> parseCompoundStatement();
  Sptr<Ast> parseAssignmentStatement();
  Sptr<Ast> parseEmptyStatement();
  Sptr<Ast> parseReturnStatement();

  // expression
  Sptr<Ast> parseExpression();
  Sptr<Ast> parseTerm();
  Sptr<Ast> parseFactor();
  Sptr<Ast> parseVariable();

  Sptr<Token> token_;
  Sptr<Lexer> lexer_;
};

} // namespace fastype

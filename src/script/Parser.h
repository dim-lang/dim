// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include "script/Ast.h"
#include "script/Lexer.h"
#include "script/Token.h"
#include <deque>
#include <memory>
#include <vector>

namespace fastype {

class Parser : public Stringify {
public:
  Parser(std::shared_ptr<Lexer> lexer);
  virtual ~Parser() = default;
  std::shared_ptr<Ast> parse();
  virtual std::string toString() const;

private:
  void eat(int tokenType);
  void eat(std::shared_ptr<Token> token);

  // block
  std::shared_ptr<Ast> parseProgram();
  std::shared_ptr<Ast> parseStatementList();

  // declaration
  std::shared_ptr<Ast> parseVariableDeclaration();
  std::shared_ptr<Ast> parseFunctionDeclaration();
  std::shared_ptr<Ast> parseClassDeclaration();

  // statement
  std::shared_ptr<Ast> parseCompoundStatement();
  std::shared_ptr<Ast> parseAssignmentStatement();
  std::shared_ptr<Ast> parseEmptyStatement();
  std::shared_ptr<Ast> parseReturnStatement();

  // expression
  std::shared_ptr<Ast> parseExpression();
  std::shared_ptr<Ast> parseTerm();
  std::shared_ptr<Ast> parseFactor();
  std::shared_ptr<Ast> parseVariable();

  std::shared_ptr<Token> token_;
  std::shared_ptr<Lexer> lexer_;
};

} // namespace fastype

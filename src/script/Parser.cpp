// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Parser.h"
#include "Logging.h"
#include "exception/ScriptException.h"
#include "script/ast/AssignmentStatement.h"
#include "script/ast/BinaryOp.h"
#include "script/ast/CompoundStatement.h"
#include "script/ast/EmptyStatement.h"
#include "script/ast/IdentifierConstant.h"
#include "script/ast/IntegerConstant.h"
#include "script/ast/Program.h"
#include "script/ast/StatementList.h"
#include "script/ast/UnaryOp.h"
#include "script/ast/Variable.h"
#include "script/token/BooleanToken.h"
#include "script/token/EofToken.h"
#include "script/token/FloatingToken.h"
#include "script/token/IdentifierToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/OperatorToken.h"
#include "script/token/StringToken.h"
#include <cstdlib>
#include <cstring>
#include <fmt/format.h>
#include <regex>
#include <string>

namespace fastype {

Parser::Parser(Sptr<Lexer> lexer) : token_(nullptr), lexer_(lexer) {
  token_ = lexer_->read();
}

void Parser::eat(Token::TokenType tokenType) {
  F_CHECK(tokenType == token_->type(), "tokenType {} == token_#type {}",
          tokenType, token_->toString());
  if (tokenType != token_->type()) {
    F_THROW(ScriptException, "invlid token type: {}, token_->type: {}",
            tokenType, token_->type());
  }
  token_ = lexer_->read();
}

void Parser::eat(Sptr<Token> token) {
  F_CHECK(token.get(), "token {} is not null", (void *)token.get());
  if (!token.get()) {
    F_THROW(ScriptException, "token pointer must not null: {}",
            (void *)token.get());
  }
  token_ = lexer_->read();
}

Ast *Parser::parseTerm() {
  Ast *node = parseFactor();
  while (Token::T_MUL->equal(token_) || Token::T_DIV->equal(token_) ||
         token_ == Token::T_MOD) {
    Sptr<Token> t = token_;
    eat(token_);
    node = new BinaryOp(node, t, parseFactor());
  }
  return node;
}

Ast *Parser::parseFactor() {
  Sptr<Token> token = token_;
  if (token == Token::T_ADD) {
    eat(Token::T_ADD);
    Ast *node = new UnaryOp(token, parseFactor());
    return node;
  } else if (token == Token::T_SUB) {
    eat(Token::T_ADD);
    Ast *node = new UnaryOp(token, parseFactor());
    return node;
  } else if (token->type() == Token::TokenType::TT_INTEGER) {
    eat(Token::TokenType::TT_INTEGER);
    return new IntegerConstant(token);
  } else if (token == Token::T_LP) {
    eat(Token::T_LP);
    Ast *node = parseExpr();
    eat(Token::T_RP);
    return node;
  } else {
    return parseVariable();
  }
}

Ast *Parser::parseExpr() {
  Ast *node = parseTerm();
  while (token_ == Token::T_ADD || token_ == Token::T_SUB) {
    Sptr<Token> t = token_;
    eat(t);
    node = new BinaryOp(node, t, parseTerm());
  }
  return node;
}

Ast *Parser::parseProgram() {
  std::vector<Ast *> childrenList = parseStatementList();
  return new Program(childrenList);
}

Ast *Parser::parseCompoundStatement() {
  eat(Token::T_LBRACE);
  std::vector<Ast *> childrenList = parseStatementList();
  eat(Token::T_RBRACE);
  return new CompoundStatement(childrenList);
}

std::vector<Ast *> Parser::parseStatementList() {
  std::vector<Ast *> ret;
  Ast *node;
  while ((node = parseStatement()) != nullptr) {
    ret.push_back(node);
  }
  return ret;
}

Ast *Parser::parseStatement() {
  if (token_ == Token::T_LBRACE) {
    return parseCompoundStatement();
  } else if (token_ == Token::T_LET) {
    return parseAssignmentStatement();
  } else {
    return parseEmptyStatement();
  }
}

Ast *Parser::parseAssignmentStatement() {
  Sptr<Token> letToken = token_;
  eat(Token::T_LET);
  Ast *left = parseVariable();
  Sptr<Token> assignToken = token_;
  eat(Token::T_ASSIGNMENT);
  Ast *right = parseExpr();
  return new AssignmentStatement(letToken, left, assignToken, right);
}

Ast *Parser::parseVariable() {
  Ast *node = new Variable(token_);
  eat(Token::TokenType::TT_IDENTIFIER);
  return node;
}

Ast *Parser::parseEmptyStatement() {
  // eat all ';'
  while (token_ == Token::T_SEMI) {
    eat(Token::T_SEMI);
  }
  return new EmptyStatement();
}

Ast *Parser::parse() {
  Ast *node = parseProgram();
  F_CHECK(token_->isEof(), "token_ {} is eof", token_->toString());
  if (!token_->isEof()) {
    F_THROW(ScriptException, "token_ {} must be eof", token_->toString());
  }
  return node;
}

std::string Parser::toString() const {
  return fmt::format("[ @Parser token_:{}, lexer_:{} ]", token_->toString(),
                     lexer_->toString());
}

} // namespace fastype

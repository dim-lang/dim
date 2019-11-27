// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Parser.h"
#include "Logging.h"
#include "exception/ScriptException.h"
#include "script/ast/AssignmentStatement.h"
#include "script/ast/BinaryOp.h"
#include "script/ast/BooleanConstant.h"
#include "script/ast/CompoundStatement.h"
#include "script/ast/EmptyStatement.h"
#include "script/ast/FloatingConstant.h"
#include "script/ast/IdentifierConstant.h"
#include "script/ast/IntegerConstant.h"
#include "script/ast/Program.h"
#include "script/ast/StatementList.h"
#include "script/ast/StringConstant.h"
#include "script/ast/UnaryOp.h"
#include "script/ast/Variable.h"
#include "script/ast/VariableDeclaration.h"
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

Ast *Parser::parseProgram() { return new Program(parseStatementList()); }

Ast *Parser::parseStatementList() {
  std::vector<Ast *> nodes;
  Ast *e = nullptr;

  do {
    if (token_ == Token::T_LET) {
      // variable_declaration
      e = parseVariableDeclaration();
    } else if (token_ == Token::T_FUNC) {
      // function_declaration
      e = parseFunctionDeclaration();
    } else if (token_ == Token::T_CLASS) {
      // class_declaration
      e = parseClassDeclaration();
    } else if (token_ == Token::T_LBRACE) {
      // compound_statement
      e = parseCompoundStatement();
    } else if (token_ == Token::T_SEMI) {
      // empty_statement
      e = parseEmptyStatement();
    } else if (token_->isIdentifier()) {
      // assignment_statement
      e = parseAssignmentStatement();
    } else if (token_ == Token::T_RETURN) {
      // return_statement
      e = parseReturnStatement();
    } else {
      F_CHECK(false, "invalid statement_list at token_:{}", token_->toString());
      F_THROW(ScriptException, "invalid statement_list at token_:{}",
              token_->toString());
    }
    nodes.push_back(e);
  } while (e != nullptr);

  return new StatementList(nodes);
}

Ast *Parser::parseVariableDeclaration() {
  std::vector<Ast *> nodes;

  // first
  eat(Token::T_LET);
  Ast *var = parseVariable();
  eat(Token::T_ASSIGNMENT);
  Ast *expr = parseExpression();
  nodes.push_back(new AssignmentStatement(var, expr));

  // others
  while (token_ == Token::T_COMMA) {
    eat(Token::T_COMMA);
    Ast *var = parseVariable();
    eat(Token::T_ASSIGNMENT);
    Ast *expr = parseExpression();
    nodes.push_back(new AssignmentStatement(var, expr));
  }

  // finish
  eat(Token::T_SEMI);

  return new VariableDeclaration(nodes);
}

Ast *Parser::parseFunctionDeclaration() { return nullptr; }

Ast *Parser::parseClassDeclaration() { return nullptr; }

Ast *Parser::parseCompoundStatement() {
  eat(Token::T_LBRACE);
  Ast *node = parseStatementList();
  eat(Token::T_RBRACE);
  return new CompoundStatement(node);
}

Ast *Parser::parseAssignmentStatement() {
  Ast *var = parseVariable();
  eat(Token::T_ASSIGNMENT);
  Ast *expr = parseExpression();
  eat(Token::T_SEMI);
  return new AssignmentStatement(var, expr);
}

Ast *Parser::parseEmptyStatement() {
  eat(Token::T_SEMI);
  return new EmptyStatement();
}

Ast *Parser::parseReturnStatement() {
  eat(Token::T_RETURN);
  Ast *node = parseExpression();
  eat(Token::T_SEMI);
  return node;
}

Ast *Parser::parseExpression() {
  Ast *node = parseTerm();
  while (token_ == Token::T_ADD || token_ == Token::T_SUB) {
    Sptr<Token> t = token_;
    eat(t);
    node = new BinaryOp(node, t, parseTerm());
  }
  return node;
}

Ast *Parser::parseTerm() {
  Ast *node = parseFactor();
  while (token_ == Token::T_MUL || token_ == Token::T_DIV ||
         token_ == Token::T_MOD) {
    Sptr<Token> t = token_;
    eat(token_);
    node = new BinaryOp(node, t, parseFactor());
  }
  return node;
}

Ast *Parser::parseFactor() {
  Sptr<Token> t = token_;
  if (t == Token::T_ADD) {
    eat(Token::T_ADD);
    return new UnaryOp(t, parseFactor());
  } else if (t == Token::T_SUB) {
    eat(Token::T_ADD);
    return new UnaryOp(t, parseFactor());
  } else if (t == Token::T_INC) {
    eat(Token::T_INC);
    return new UnaryOp(t, parseFactor());
  } else if (t == Token::T_DEC) {
    eat(Token::T_DEC);
    return new UnaryOp(t, parseFactor());
  } else if (t->isInteger()) {
    eat(Token::TokenType::TT_INTEGER);
    return new IntegerConstant(t);
  } else if (t->isFloating()) {
    eat(Token::TokenType::TT_FLOATING);
    return new FloatingConstant(t);
  } else if (t->isBoolean()) {
    eat(Token::TokenType::TT_BOOLEAN);
    return new BooleanConstant(t);
  } else if (t->isString()) {
    eat(Token::TokenType::TT_STRING);
    return new StringConstant(t);
  } else if (t == Token::T_LP) {
    eat(Token::T_LP);
    Ast *node = parseExpression();
    eat(Token::T_RP);
    return node;
  } else {
    return parseVariable();
  }
}

Ast *Parser::parseVariable() {
  Ast *node = new Variable(token_);
  eat(Token::TokenType::TT_IDENTIFIER);
  return node;
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

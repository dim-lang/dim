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

Parser::Parser(std::shared_ptr<Lexer> lexer) : token_(nullptr), lexer_(lexer) {
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

void Parser::eat(std::shared_ptr<Token> token) {
  F_CHECK(token.get(), "token {} is not null", (void *)token.get());
  if (!token.get()) {
    F_THROW(ScriptException, "token pointer must not null: {}",
            (void *)token.get());
  }
  token_ = lexer_->read();
}

std::shared_ptr<Ast> Parser::parseProgram() {
  return std::shared_ptr<Ast>(new Program(parseStatementList()));
}

std::shared_ptr<Ast> Parser::parseStatementList() {
  std::vector<std::shared_ptr<Ast>> nodes;
  std::shared_ptr<Ast> e(nullptr);

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
  } while (e);

  return std::shared_ptr<Ast>(new StatementList(nodes));
}

std::shared_ptr<Ast> Parser::parseVariableDeclaration() {
  std::vector<std::shared_ptr<Ast>> nodes;

  // first
  eat(Token::T_LET);
  std::shared_ptr<Ast> var = parseVariable();
  eat(Token::T_ASSIGNMENT);
  std::shared_ptr<Ast> expr = parseExpression();
  nodes.push_back(std::shared_ptr<Ast>(new AssignmentStatement(var, expr)));

  // others
  while (token_ == Token::T_COMMA) {
    eat(Token::T_COMMA);
    std::shared_ptr<Ast> var = parseVariable();
    eat(Token::T_ASSIGNMENT);
    std::shared_ptr<Ast> expr = parseExpression();
    nodes.push_back(std::shared_ptr<Ast>(new AssignmentStatement(var, expr)));
  }

  // finish
  eat(Token::T_SEMI);

  return std::shared_ptr<Ast>(new VariableDeclaration(nodes));
}

std::shared_ptr<Ast> Parser::parseFunctionDeclaration() {
  return std::shared_ptr<Ast>(nullptr);
}

std::shared_ptr<Ast> Parser::parseClassDeclaration() {
  return std::shared_ptr<Ast>(nullptr);
}

std::shared_ptr<Ast> Parser::parseCompoundStatement() {
  eat(Token::T_LBRACE);
  std::shared_ptr<Ast> node = parseStatementList();
  eat(Token::T_RBRACE);
  return std::shared_ptr<Ast>(new CompoundStatement(node));
}

std::shared_ptr<Ast> Parser::parseAssignmentStatement() {
  std::shared_ptr<Ast> var = parseVariable();
  eat(Token::T_ASSIGNMENT);
  std::shared_ptr<Ast> expr = parseExpression();
  eat(Token::T_SEMI);
  return std::shared_ptr<Ast>(new AssignmentStatement(var, expr));
}

std::shared_ptr<Ast> Parser::parseEmptyStatement() {
  eat(Token::T_SEMI);
  return std::shared_ptr<Ast>(new EmptyStatement());
}

std::shared_ptr<Ast> Parser::parseReturnStatement() {
  eat(Token::T_RETURN);
  std::shared_ptr<Ast> node = parseExpression();
  eat(Token::T_SEMI);
  return node;
}

#define F_ISC(x)                                                               \
  ((x)->type() == Ast::AstType::INTEGER_CONSTANT ||                            \
   (x)->type() == Ast::AstType::FLOATING_CONSTANT)

#define F_ISIC(x) ((x)->type() == Ast::AstType::INTEGER_CONSTANT)

#define F_ISFC(x) ((x)->type() == Ast::AstType::FLOATING_CONSTANT)

std::shared_ptr<Ast> Parser::parseExpression() {
  std::shared_ptr<Ast> node = parseTerm();
  while (token_ == Token::T_ADD || token_ == Token::T_SUB) {
    std::shared_ptr<Token> t = token_;
    eat(t);
    std::shared_ptr<Ast> right = parseTerm();

    // calculate constants
    if (F_ISC(node) && F_ISC(right)) {
      if (F_ISIC(node) && F_ISIC(right)) {
        int64_t a = std::static_pointer_cast<IntegerConstant>(node)->value();
        int64_t b = std::static_pointer_cast<IntegerConstant>(right)->value();
        node = std::shared_ptr<Ast>(new IntegerConstant(
            std::shared_ptr<IntegerToken>(new IntegerToken(a + b))));
      } else if (F_ISIC(node) && !F_ISIC(right)) {
        int64_t a = std::static_pointer_cast<IntegerConstant>(node)->value();
        double b = std::static_pointer_cast<FloatingConstant>(right)->value();
        node = std::shared_ptr<Ast>(new FloatingConstant(
            std::shared_ptr<FloatingToken>(new FloatingToken((double)a + b))));
      } else if (!F_ISIC(node) && F_ISIC(right)) {
        double a = std::static_pointer_cast<FloatingConstant>(node)->value();
        int64_t b = std::static_pointer_cast<IntegerConstant>(right)->value();
        node = std::shared_ptr<Ast>(new FloatingConstant(
            std::shared_ptr<FloatingToken>(new FloatingToken(a + (double)b))));
      } else if (!F_ISIC(node) && !F_ISIC(right)) {
        double a = std::static_pointer_cast<FloatingConstant>(node)->value();
        double b = std::static_pointer_cast<FloatingConstant>(right)->value();
        node = std::shared_ptr<Ast>(new FloatingConstant(
            std::shared_ptr<FloatingToken>(new FloatingToken(a + b))));
      }
    } else {
      node = std::shared_ptr<Ast>(new BinaryOp(node, t, right));
    }
  }
  return node;
}

std::shared_ptr<Ast> Parser::parseTerm() {
  std::shared_ptr<Ast> node = parseFactor();
  while (token_ == Token::T_MUL || token_ == Token::T_DIV ||
         token_ == Token::T_MOD) {
    std::shared_ptr<Token> t = token_;
    eat(token_);
    node = std::shared_ptr<Ast>(new BinaryOp(node, t, parseFactor()));
  }
  return node;
}

std::shared_ptr<Ast> Parser::parseFactor() {
  std::shared_ptr<Token> t = token_;
  if (t == Token::T_ADD) {
    eat(Token::T_ADD);
    return std::shared_ptr<Ast>(new UnaryOp(t, parseFactor()));
  } else if (t == Token::T_SUB) {
    eat(Token::T_ADD);
    return std::shared_ptr<Ast>(new UnaryOp(t, parseFactor()));
  } else if (t == Token::T_INC) {
    eat(Token::T_INC);
    return std::shared_ptr<Ast>(new UnaryOp(t, parseFactor()));
  } else if (t == Token::T_DEC) {
    eat(Token::T_DEC);
    return std::shared_ptr<Ast>(new UnaryOp(t, parseFactor()));
  } else if (t->isInteger()) {
    eat(Token::TokenType::TT_INTEGER);
    return std::shared_ptr<Ast>(new IntegerConstant(t));
  } else if (t->isFloating()) {
    eat(Token::TokenType::TT_FLOATING);
    return std::shared_ptr<Ast>(new FloatingConstant(t));
  } else if (t->isBoolean()) {
    eat(Token::TokenType::TT_BOOLEAN);
    return std::shared_ptr<Ast>(new BooleanConstant(t));
  } else if (t->isString()) {
    eat(Token::TokenType::TT_STRING);
    return std::shared_ptr<Ast>(new StringConstant(t));
  } else if (t == Token::T_LP) {
    eat(Token::T_LP);
    std::shared_ptr<Ast> node = parseExpression();
    eat(Token::T_RP);
    return node;
  } else {
    return parseVariable();
  }
}

std::shared_ptr<Ast> Parser::parseVariable() {
  std::shared_ptr<Ast> node(new Variable(token_));
  eat(Token::TokenType::TT_IDENTIFIER);
  return node;
}

std::shared_ptr<Ast> Parser::parse() {
  std::shared_ptr<Ast> node = parseProgram();
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

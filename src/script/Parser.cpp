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
#include "script/ast/FunctionDeclaration.h"
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

void Parser::eat(int tokenType) {
  F_CHECK(tokenType == token_->type(), "tokenType {} == token_#type {}",
          Type::nameUTF8(tokenType), token_->toString());
  if (tokenType != token_->type()) {
    F_THROW(ScriptException, "invlid token type: {}, token_: {}",
            Type::nameUTF8(tokenType), token_->toString());
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
  std::shared_ptr<Ast> r =
      std::shared_ptr<Ast>(new Program(parseStatementList()));
  F_CHECK(r, "r {} not null", (void *)r.get());
  return r;
}

std::shared_ptr<Ast> Parser::parseStatementList() {
  std::vector<std::shared_ptr<Ast>> nodes;
  std::shared_ptr<Ast> e(nullptr);

  do {
    if (token_ == Token::T_LET) {
      // variable_declaration
      e = parseVariableDeclaration();
      F_CHECK(e, "e {} not null", (void *)e.get());
    } else if (token_ == Token::T_FUNC) {
      // function_declaration
      e = parseFunctionDeclaration();
      F_CHECK(e, "e {} not null", (void *)e.get());
    } else if (token_ == Token::T_CLASS) {
      // class_declaration
      e = parseClassDeclaration();
      F_CHECK(e, "e {} not null", (void *)e.get());
    } else if (token_ == Token::T_LBRACE) {
      // compound_statement
      e = parseCompoundStatement();
      F_CHECK(e, "e {} not null", (void *)e.get());
    } else if (token_ == Token::T_SEMI) {
      // empty_statement
      e = parseEmptyStatement();
      F_CHECK(e, "e {} not null", (void *)e.get());
    } else if (token_->isIdentifier()) {
      // assignment_statement
      e = parseAssignmentStatement();
      F_CHECK(e, "e {} not null", (void *)e.get());
    } else if (token_ == Token::T_RETURN) {
      // return_statement
      e = parseReturnStatement();
      F_CHECK(e, "e {} not null", (void *)e.get());
    } else {
      break;
    }
    nodes.push_back(e);
  } while (token_ != Token::T_EOF);

  std::shared_ptr<Ast> r = std::shared_ptr<Ast>(new StatementList(nodes));
  F_CHECK(r, "r {} not null", (void *)r.get());
  return r;
}

std::shared_ptr<Ast> Parser::parseVariableDeclaration() {
  eat(Token::T_LET);
  std::shared_ptr<Ast> var = parseVariable();
  eat(Token::T_ASSIGNMENT);
  std::shared_ptr<Ast> expr = parseExpression();
  eat(Token::T_SEMI);
  std::shared_ptr<Ast> r =
      std::shared_ptr<Ast>(new VariableDeclaration(var, expr));
  F_CHECK(r, "r {} not null", (void *)r.get());
  return r;
}

std::shared_ptr<Ast> Parser::parseFunctionDeclaration() {
  eat(Token::T_FUNC);
  std::shared_ptr<Ast> functionId = parseIdentifierConstant();
  eat(Token::T_LP);
  std::vector<std::shared_ptr<Ast>> parameterList;
  while (token_->isIdentifier()) {
    parameterList.push_back(parseVariable());
    if (token_->isPunctuation() && token_->equal(Token::T_COMMA)) {
      eat(Token::T_COMMA);
    }
  }
  eat(Token::T_RP);
  std::shared_ptr<Ast> compoundStatement = parseCompoundStatement();
  return std::shared_ptr<Ast>(
      new FunctionDeclaration(functionId, parameterList, compoundStatement));
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

#define F_IS_IF(x)                                                             \
  ((x)->type() == F_TYPE_INTEGER_CONSTANT ||                                   \
   (x)->type() == F_TYPE_FLOATING_CONSTANT)

#define F_IS_IC(x) ((x)->type() == F_TYPE_INTEGER_CONSTANT)

#define F_IS_FC(x) ((x)->type() == F_TYPE_FLOATING_CONSTANT)

#define F_OP_I_AND_F(node, r, op)                                              \
  if (F_IS_IC(node) && F_IS_IC(r)) {                                           \
    int64_t a1 = std::static_pointer_cast<IntegerConstant>(node)->value();     \
    int64_t b1 = std::static_pointer_cast<IntegerConstant>(r)->value();        \
    node = std::shared_ptr<Ast>(new IntegerConstant(                           \
        std::shared_ptr<IntegerToken>(new IntegerToken(a1 op b1))));           \
  } else if (F_IS_IC(node) && F_IS_FC(r)) {                                    \
    double a2 =                                                                \
        (double)std::static_pointer_cast<IntegerConstant>(node)->value();      \
    double b2 = std::static_pointer_cast<FloatingConstant>(r)->value();        \
    node = std::shared_ptr<Ast>(new FloatingConstant(                          \
        std::shared_ptr<FloatingToken>(new FloatingToken(a2 op b2))));         \
  } else if (F_IS_FC(node) && F_IS_IC(r)) {                                    \
    double a3 = std::static_pointer_cast<FloatingConstant>(node)->value();     \
    double b3 = (double)std::static_pointer_cast<IntegerConstant>(r)->value(); \
    node = std::shared_ptr<Ast>(new FloatingConstant(                          \
        std::shared_ptr<FloatingToken>(new FloatingToken(a3 op b3))));         \
  } else if (F_IS_FC(node) && F_IS_FC(r)) {                                    \
    double a4 = std::static_pointer_cast<FloatingConstant>(node)->value();     \
    double b4 = std::static_pointer_cast<FloatingConstant>(r)->value();        \
    node = std::shared_ptr<Ast>(new FloatingConstant(                          \
        std::shared_ptr<FloatingToken>(new FloatingToken(a4 op b4))));         \
  }

std::shared_ptr<Ast> Parser::parseExpression() {
  std::shared_ptr<Ast> node = parseTerm();
  while (token_ == Token::T_ADD || token_ == Token::T_SUB) {
    std::shared_ptr<Token> t = token_;
    eat(t);
    std::shared_ptr<Ast> right = parseTerm();

    // optimization: calculate constants
    if (F_IS_IF(node) && F_IS_IF(right)) {
      if (t == Token::T_ADD) {
        F_OP_I_AND_F(node, right, +)
      } else if (t == Token::T_SUB) {
        F_OP_I_AND_F(node, right, -)
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
    std::shared_ptr<Ast> right = parseFactor();

    // optimization: calculate constants
    if (F_IS_IF(node) && F_IS_IF(right)) {
      if (t == Token::T_MUL) {
        F_OP_I_AND_F(node, right, *)
      } else if (t == Token::T_DIV) {
        F_OP_I_AND_F(node, right, /)
      } else if (t == Token::T_MOD) {
        if (F_IS_IC(node) && F_IS_IC(right)) {
          int64_t a1 = std::static_pointer_cast<IntegerConstant>(node)->value();
          int64_t b1 =
              std::static_pointer_cast<IntegerConstant>(right)->value();
          node = std::shared_ptr<Ast>(new IntegerConstant(
              std::shared_ptr<IntegerToken>(new IntegerToken(a1 % b1))));
        }
      } else {
        F_CHECK(
            false,
            "floating number can't calculate MOD operation, node:{}, right:{}",
            node->toString(), right->toString());
        F_THROW(
            ScriptException,
            "floating number can't calculate MOD operation, node:{}, right:{}",
            node->toString(), right->toString());
      }
    } else {
      node = std::shared_ptr<Ast>(new BinaryOp(node, t, right));
    }
  }
  return node;
}

std::shared_ptr<Ast> Parser::parseFactor() {
  std::shared_ptr<Token> t = token_;
  if (t == Token::T_ADD) {
    eat(Token::T_ADD);
    // optimization: ignore unary + operator
    return parseFactor();
  } else if (t == Token::T_SUB) {
    eat(Token::T_SUB);
    // optimization: negative unary - operator
    std::shared_ptr<Ast> right = parseFactor();
    if (F_IS_IC(right)) {
      return std::shared_ptr<Ast>(
          new IntegerConstant(std::shared_ptr<IntegerToken>(new IntegerToken(
              -std::static_pointer_cast<IntegerConstant>(right)->value()))));
    } else if (F_IS_FC(right)) {
      return std::shared_ptr<Ast>(
          new FloatingConstant(std::shared_ptr<FloatingToken>(new FloatingToken(
              -std::static_pointer_cast<FloatingConstant>(right)->value()))));
    }
    return std::shared_ptr<Ast>(new UnaryOp(t, right));
  } else if (t == Token::T_INC) {
    eat(Token::T_INC);
    return std::shared_ptr<Ast>(new UnaryOp(t, parseFactor()));
  } else if (t == Token::T_DEC) {
    eat(Token::T_DEC);
    return std::shared_ptr<Ast>(new UnaryOp(t, parseFactor()));
  } else if (t->isInteger()) {
    eat(F_TYPE_INTEGER);
    return std::shared_ptr<Ast>(new IntegerConstant(t));
  } else if (t->isFloating()) {
    eat(F_TYPE_FLOATING);
    return std::shared_ptr<Ast>(new FloatingConstant(t));
  } else if (t->isBoolean()) {
    eat(F_TYPE_BOOLEAN);
    return std::shared_ptr<Ast>(new BooleanConstant(t));
  } else if (t->isString()) {
    eat(F_TYPE_STRING);
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

#undef F_IS_IF
#undef F_IS_IC
#undef F_IS_FC
#undef F_OP_I_AND_F

std::shared_ptr<Ast> Parser::parseVariable() {
  std::shared_ptr<Ast> node(new Variable(token_));
  eat(F_TYPE_IDENTIFIER);
  return node;
}

std::shared_ptr<Ast> Parser::parseIdentifierConstant() {
  std::shared_ptr<Ast> node(new IdentifierConstant(token_));
  eat(F_TYPE_IDENTIFIER);
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

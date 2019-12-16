// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Interpreter.h"
#include "Logging.h"
#include "exception/ScriptException.h"
#include "script/Token.h"
#include "script/Type.h"
#include "script/ast/AssignmentStatement.h"
#include "script/ast/BinaryOp.h"
#include "script/ast/BooleanConstant.h"
#include "script/ast/CompoundStatement.h"
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
#include "script/token/FloatingToken.h"
#include "script/token/IdentifierToken.h"
#include "script/token/IntegerToken.h"
#include "script/token/StringToken.h"
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <utility>

namespace fastype {

Interpreter::Interpreter(std::shared_ptr<Parser> parser)
    : tree_(nullptr), parser_(parser), globalScope_() {}

Interpreter::~Interpreter() { globalScope_.clear(); }

void Interpreter::visit(std::shared_ptr<Ast> node) {
  switch (node->type().value()) {
  case F_TYPE_PROGRAM:
    visitProgram(node);
    break;
  case F_TYPE_STATEMENT_LIST:
    visitStatementList(node);
    break;
  case F_TYPE_VARIABLE_DECLARATION:
    visitVariableDeclaration(node);
    break;
  case F_TYPE_FUNCTION_DECLARATION:
    visitFunctionDeclaration(node);
    break;
  case F_TYPE_CLASS_DECLARATION:
    visitClassDeclaration(node);
    break;
  case F_TYPE_COMPOUND_STATEMENT:
    visitCompoundStatement(node);
    break;
  case F_TYPE_ASSIGNMENT_STATEMENT:
    visitAssignmentStatement(node);
    break;
  case F_TYPE_EMPTY_STATEMENT:
    visitEmptyStatement(node);
    break;
  case F_TYPE_RETURN_STATEMENT:
    visitReturnStatement(node);
    break;
  default:
    F_CHECK(false, "must not reach here, node:{}", node->toString());
    F_THROW(ScriptException, "must not reach here, node: {}", node->toString());
  }
}

void Interpreter::visitProgram(std::shared_ptr<Ast> node) {
  std::shared_ptr<Program> e = std::static_pointer_cast<Program>(node);
  visit(e->statementList());
}

void Interpreter::visitStatementList(std::shared_ptr<Ast> node) {
  std::shared_ptr<StatementList> e =
      std::static_pointer_cast<StatementList>(node);
  for (int i = 0; i < e->size(); i++) {
    std::shared_ptr<Ast> child = e->get(i);
    visit(child);
  }
}

void Interpreter::visitVariableDeclaration(std::shared_ptr<Ast> node) {
  std::shared_ptr<VariableDeclaration> e =
      std::static_pointer_cast<VariableDeclaration>(node);
  std::shared_ptr<Variable> var = std::static_pointer_cast<Variable>(e->var());
  std::shared_ptr<Ast> expr = e->expr();
  globalScope_[var->value()] = visitExpression(expr);
}

void Interpreter::visitFunctionDeclaration(std::shared_ptr<Ast> node) {
  F_CHECK(false, "not implement! node:{}", node->toString());
  F_THROW(ScriptException, "not implement, node: {}", node->toString());
}

void Interpreter::visitClassDeclaration(std::shared_ptr<Ast> node) {
  F_CHECK(false, "not implement! node:{}", node->toString());
  F_THROW(ScriptException, "not implement, node: {}", node->toString());
}

void Interpreter::visitCompoundStatement(std::shared_ptr<Ast> node) {
  std::shared_ptr<CompoundStatement> e =
      std::static_pointer_cast<CompoundStatement>(node);
  std::shared_ptr<StatementList> l =
      std::static_pointer_cast<StatementList>(e->statementList());
  for (int i = 0; i < l->size(); i++) {
    std::shared_ptr<Ast> child = l->get(i);
    visit(child);
  }
}

void Interpreter::visitAssignmentStatement(std::shared_ptr<Ast> node) {
  std::shared_ptr<AssignmentStatement> e =
      std::static_pointer_cast<AssignmentStatement>(node);
  std::shared_ptr<Variable> var = std::static_pointer_cast<Variable>(e->var());
  std::shared_ptr<Ast> expr = e->expr();
  globalScope_[var->value()] = visitExpression(expr);
}

void Interpreter::visitEmptyStatement(std::shared_ptr<Ast> node) {}

void Interpreter::visitReturnStatement(std::shared_ptr<Ast> node) {
  F_CHECK(false, "not implement! node:{}", node->toString());
  F_THROW(ScriptException, "not implement, node:{}", node->toString());
}

std::shared_ptr<Ast> Interpreter::visitExpression(std::shared_ptr<Ast> node) {
  switch (node->type().value()) {
  case F_TYPE_BINARY_OP:
    return visitBinaryOp(node);
    break;
  case F_TYPE_UNARY_OP:
    return visitUnaryOp(node);
    break;
  case F_TYPE_VARIABLE:
    return visitVariable(node);
    break;
  case F_TYPE_INTEGER_CONSTANT:
    return node;
    break;
  case F_TYPE_FLOATING_CONSTANT:
    return node;
    break;
  case F_TYPE_BOOLEAN_CONSTANT:
    return node;
    break;
  case F_TYPE_STRING_CONSTANT:
    return node;
    break;
  default:
    F_CHECK(false, "must not reach here, node:{}", node->toString());
    F_THROW(ScriptException, "must not reach here, node: {}", node->toString());
  }
}

#define F_IS_IC(x) ((x)->type() == Type::T_INTEGER_CONSTANT)

#define F_IS_FC(x) ((x)->type() == Type::T_FLOATING_CONSTANT)

#define F_OP_I_AND_F(l, r, op)                                                 \
  if (F_IS_IC(l) && F_IS_IC(r)) {                                              \
    int64_t a1 = std::static_pointer_cast<IntegerConstant>(l)->value();        \
    int64_t b1 = std::static_pointer_cast<IntegerConstant>(r)->value();        \
    return std::shared_ptr<Ast>(new IntegerConstant(                           \
        std::shared_ptr<Token>(new IntegerToken(a1 op b1))));                  \
  } else if (F_IS_IC(l) && F_IS_FC(r)) {                                       \
    double a2 = (double)std::static_pointer_cast<IntegerConstant>(l)->value(); \
    double b2 = std::static_pointer_cast<FloatingConstant>(r)->value();        \
    return std::shared_ptr<Ast>(new FloatingConstant(                          \
        std::shared_ptr<Token>(new FloatingToken(a2 op b2))));                 \
  } else if (F_IS_FC(l) && F_IS_IC(r)) {                                       \
    double a3 = std::static_pointer_cast<FloatingConstant>(l)->value();        \
    double b3 = (double)std::static_pointer_cast<IntegerConstant>(r)->value(); \
    return std::shared_ptr<Ast>(new FloatingConstant(                          \
        std::shared_ptr<Token>(new FloatingToken(a3 op b3))));                 \
  } else if (F_IS_FC(l) && F_IS_FC(r)) {                                       \
    double a4 = std::static_pointer_cast<FloatingConstant>(l)->value();        \
    double b4 = std::static_pointer_cast<FloatingConstant>(r)->value();        \
    return std::shared_ptr<Ast>(new FloatingConstant(                          \
        std::shared_ptr<Token>(new FloatingToken(a4 op b4))));                 \
  }

std::shared_ptr<Ast> Interpreter::visitBinaryOp(std::shared_ptr<Ast> node) {
  std::shared_ptr<BinaryOp> e = std::static_pointer_cast<BinaryOp>(node);
  std::shared_ptr<Ast> l = visitExpression(e->left());
  std::shared_ptr<Ast> r = visitExpression(e->right());

  if (e->op() == Token::T_ADD) {
    F_OP_I_AND_F(l, r, +)
  } else if (e->op() == Token::T_SUB) {
    F_OP_I_AND_F(l, r, -)
  } else if (e->op() == Token::T_MUL) {
    F_OP_I_AND_F(l, r, *)
  } else if (e->op() == Token::T_DIV) {
    F_OP_I_AND_F(l, r, /)
  } else if (e->op() == Token::T_MOD) {
    if (F_IS_IC(l) && F_IS_IC(r)) {
      int64_t a1 = std::static_pointer_cast<IntegerConstant>(l)->value();
      int64_t b1 = std::static_pointer_cast<IntegerConstant>(r)->value();
      return std::shared_ptr<Ast>(new IntegerConstant(
          std::shared_ptr<Token>(new IntegerToken(a1 % b1))));
    }
  }
  F_CHECK(false, "must not reach here, node:{}, l:{}, r:{}", node->toString(),
          l->toString(), r->toString());
  F_THROW(ScriptException, "must not reach here, node:{}, l:{}, r:{}",
          node->toString(), l->toString(), r->toString());
  return std::shared_ptr<Ast>(nullptr);
}

std::shared_ptr<Ast> Interpreter::visitUnaryOp(std::shared_ptr<Ast> node) {
  std::shared_ptr<UnaryOp> e = std::static_pointer_cast<UnaryOp>(node);
  std::shared_ptr<Ast> expr = visitExpression(e->expr());
  if (e->op() == Token::T_ADD) {
    return expr;
  } else if (e->op() == Token::T_SUB) {
    if (F_IS_IC(expr)) {
      return std::shared_ptr<Ast>(
          new IntegerConstant(std::shared_ptr<Token>(new IntegerToken(
              -std::static_pointer_cast<IntegerConstant>(expr)->value()))));
    } else if (F_IS_FC(expr)) {
      return std::shared_ptr<Ast>(
          new FloatingConstant(std::shared_ptr<Token>(new FloatingToken(
              -std::static_pointer_cast<FloatingConstant>(expr)->value()))));
    }
  }
  F_CHECK(false, "must not reach here, node:{}, expr:{}", node->toString(),
          expr->toString());
  F_THROW(ScriptException, "must not reach here, node:{}, expr:{}",
          node->toString(), expr->toString());
  return std::shared_ptr<Ast>(nullptr);
}

#undef F_IS_IC
#undef F_IS_FC
#undef F_OP_I_AND_F

std::shared_ptr<Ast> Interpreter::visitVariable(std::shared_ptr<Ast> node) {
  std::shared_ptr<Variable> e = std::static_pointer_cast<Variable>(node);
  if (globalScope_.find(e->value()) == globalScope_.end()) {
    F_THROW(ScriptException, "variable {} not found", e->toString());
  }
  return globalScope_[e->value()];
}

void Interpreter::interpret() {
  tree_ = parser_->parse();
  visit(tree_);
}

std::string Interpreter::toString() const {
  std::stringstream ss;
  ss << "[ @Interpreter globalScope_#size:";
  ss << globalScope_.size() << " ";
  int n = 0;
  for (auto i = globalScope_.begin(); i != globalScope_.end(); i++) {
    std::string _1;
    ss << fmt::format("[{}:{} - {}], ", n++, i->first.toUTF8String(_1),
                      i->second->toString());
  }
  ss << "]";
  return ss.str();
}

} // namespace fastype

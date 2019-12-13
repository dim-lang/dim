// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "exception/ScriptException.h"
#include "script/NodeVisitor.h"
#include "script/SymbolTable.h"

namespace fastype {

SymbolTableBuilder::SymbolTableBuilder(), symbolTable_() {}

void SymbolTableBuilder::travel() {}

std::string SymbolTableBuilder::toString() const {
  return fmt::format("[ @SymbolTableBuilder symbolTable_:{} ]",
                     symbolTable_.toString());
}

void SymbolTableBuilder::visit(std::shared_ptr<Ast> node) {
  switch (node->type()) {
  case Ast::AstType::PROGRAM:
    visitProgram(node);
    break;
  case Ast::AstType::STATEMENT_LIST:
    visitStatementList(node);
    break;
  case Ast::AstType::VARIABLE_DECLARATION:
    visitVariableDeclaration(node);
    break;
  case Ast::AstType::FUNCTION_DECLARATION:
    visitFunctionDeclaration(node);
    break;
  case Ast::AstType::CLASS_DECLARATION:
    visitClassDeclaration(node);
    break;
  case Ast::AstType::COMPOUND_STATEMENT:
    visitCompoundStatement(node);
    break;
  case Ast::AstType::ASSIGNMENT_STATEMENT:
    visitAssignmentStatement(node);
    break;
  case Ast::AstType::EMPTY_STATEMENT:
    visitEmptyStatement(node);
    break;
  case Ast::AstType::RETURN_STATEMENT:
    visitReturnStatement(node);
    break;
  default:
    F_CHECK(false, "must not reach here, node:{}", node->toString());
    F_THROW(ScriptException, "must not reach here, node: {}", node->toString());
  }
}

void SymbolTableBuilder::visitProgram(std::shared_ptr<Ast> node) {
  std::shared_ptr<Program> e = std::static_pointer_cast<Program>(node);
  visit(e->statementList());
}

void SymbolTableBuilder::visitStatementList(std::shared_ptr<Ast> node) {
  std::shared_ptr<StatementList> e =
      std::static_pointer_cast<StatementList>(node);
  for (int i = 0; i < e->size(); i++) {
    std::shared_ptr<Ast> child = e->get(i);
    visit(child);
  }
}

void SymbolTableBuilder::visitVariableDeclaration(std::shared_ptr<Ast> node) {
  std::shared_ptr<VariableDeclaration> e =
      std::static_pointer_cast<VariableDeclaration>(node);
  for (int i = 0; i < e->size(); i++) {
    std::shared_ptr<Ast> child = e->get(i);
    visit(child);
  }
}

void SymbolTableBuilder::visitFunctionDeclaration(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitClassDeclaration(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitCompoundStatement(std::shared_ptr<Ast> node) {
  std::shared_ptr<CompoundStatement> e =
      std::static_pointer_cast<CompoundStatement>(node);
  std::shared_ptr<StatementList> l =
      std::static_pointer_cast<StatementList>(e->statementList());
  for (int i = 0; i < l->size(); i++) {
    std::shared_ptr<Ast> child = l->get(i);
    visit(child);
  }
}

void SymbolTableBuilder::visitAssignmentStatement(std::shared_ptr<Ast> node) {
  std::shared_ptr<AssignmentStatement> e =
      std::static_pointer_cast<AssignmentStatement>(node);
  std::shared_ptr<Variable> var = std::static_pointer_cast<Variable>(e->var());
  std::shared_ptr<Ast> expr = e->expr();
  globalScope_[var->value()] = visitExpression(expr);
}

void SymbolTableBuilder::visitEmptyStatement(std::shared_ptr<Ast> node) {}

void SymbolTableBuilder::visitReturnStatement(std::shared_ptr<Ast> node) {}

std::shared_ptr<Ast>
SymbolTableBuilder::visitExpression(std::shared_ptr<Ast> node) {
  switch (node->type()) {
  case Ast::AstType::BINARY_OP:
    return visitBinaryOp(node);
    break;
  case Ast::AstType::UNARY_OP:
    return visitUnaryOp(node);
    break;
  case Ast::AstType::VARIABLE:
    return visitVariable(node);
    break;
  case Ast::AstType::INTEGER_CONSTANT:
    return node;
    break;
  case Ast::AstType::FLOATING_CONSTANT:
    return node;
    break;
  case Ast::AstType::BOOLEAN_CONSTANT:
    return node;
    break;
  case Ast::AstType::STRING_CONSTANT:
    return node;
    break;
  default:
    F_CHECK(false, "must not reach here, node:{}", node->toString());
    F_THROW(ScriptException, "must not reach here, node: {}", node->toString());
  }
}

std::shared_ptr<Ast>
SymbolTableBuilder::visitBinaryOp(std::shared_ptr<Ast> node) {
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

std::shared_ptr<Ast> Interpreter::visitUnaryOp(std::shared_ptr<Ast> node) {}

std::shared_ptr<Ast> Interpreter::visitVariable(std::shared_ptr<Ast> node) {}

std::shared_ptr<Ast>
SymbolTableBuilder::visitUnaryOp(std::shared_ptr<Ast> node) {
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

std::shared_ptr<Ast>
SymbolTableBuilder::visitVariable(std::shared_ptr<Ast> node) {
  std::shared_ptr<Variable> e = std::static_pointer_cast<Variable>(node);
  if (globalScope_.find(e->value()) == globalScope_.end()) {
    F_THROW(ScriptException, "variable {} not found", e->toString());
  }
  return globalScope_[e->value()];
}

} // namespace fastype

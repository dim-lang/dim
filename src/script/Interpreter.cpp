// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Interpreter.h"
#include "Logging.h"
#include "Token.h"
#include "exception/ScriptException.h"
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
#include <utility>

namespace fastype {

Interpreter::Interpreter(std::shared_ptr<Parser> parser)
    : tree_(nullptr), parser_(parser), globalScope_() {}

Interpreter::~Interpreter() {
  globalScope_.clear();
  release(tree_);
}

void Interpreter::visit(std::shared_ptr<Ast> node) {
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

void Interpreter::visitProgram(std::shared_ptr<Ast> node) {
  std::shared_ptr<Program> e((Program *)node.get());
  visit(e->statementList());
}

void Interpreter::visitStatementList(std::shared_ptr<Ast> node) {
  std::shared_ptr<StatementList> e((StatementList *)node.get());
  for (int i = 0; i < e->size(); i++) {
    std::shared_ptr<Ast> child = e->get(i);
    visit(child);
  }
}

void Interpreter::visitVariableDeclaration(std::shared_ptr<Ast> node) {
  std::shared_ptr<VariableDeclaration> e((VariableDeclaration *)node.get());
  for (int i = 0; i < e->size(); i++) {
    std::shared_ptr<Ast> child = e->get(i);
    visit(child);
  }
}

void Interpreter::visitFunctionDeclaration(std::shared_ptr<Ast> node) {}

void Interpreter::visitClassDeclaration(std::shared_ptr<Ast> node) {}

void Interpreter::visitCompoundStatement(std::shared_ptr<Ast> node) {
  std::shared_ptr<CompoundStatement> e((CompoundStatement *)node.get());
  std::shared_ptr<StatementList> sl((StatementList *)e->statementList().get());
  for (int i = 0; i < sl->size(); i++) {
    std::shared_ptr<Ast> child = sl->get(i);
    visit(child);
  }
}

void Interpreter::visitAssignmentStatement(std::shared_ptr<Ast> node) {
  std::shared_ptr<AssignmentStatement> e((AssignmentStatement *)node.get());
  std::shared_ptr<Variable> var((Variable *)e->var().get());
  std::shared_ptr<Ast> expr = e->expr();
  globalScope_[var->value()] = visitBinaryOp(expr);
}

void Interpreter::visitEmptyStatement(std::shared_ptr<Ast> node) {}

void Interpreter::visitReturnStatement(std::shared_ptr<Ast> node) {}

std::shared_ptr<Ast> Interpreter::visitExpression(std::shared_ptr<Ast> node) {
  switch (node->type()) {
  case Ast::AstType::BINARY_OP:
    return visitBinaryOp(node);
    break;
  case Ast::AstType::UNARY_OP:
    return visitUnaryOp(node);
    break;
  default:
    return node;
  }
}

static inline bool isIC(std::shared_ptr<Ast> node) {
  return node->type() == Ast::AstType::INTEGER_CONSTANT;
}

static inline bool isFC(std::shared_ptr<Ast> node) {
  return node->type() == Ast::AstType::FLOATING_CONSTANT;
}

#define F_NEWIC(a, b, op)                                                      \
  (std::shared_ptr<Ast>(                                                       \
      new IntegerConstant(std::shared_ptr<Token>(new IntegerToken(a op b)))))

#define F_NEWFC(a, b, op)                                                      \
  (std::shared_ptr<Ast>(new FloatingConstant(                                  \
      std::shared_ptr<Token>(new FloatingToken(a op b)))))

std::shared_ptr<Ast> Interpreter::visitBinaryOp(std::shared_ptr<Ast> node) {
  std::shared_ptr<BinaryOp> e((BinaryOp *)node.get());
  std::shared_ptr<Ast> l = visitExpression(e->left());
  std::shared_ptr<Ast> r = visitExpression(e->right());

  if (e->op() == Token::T_ADD) {
    if (isIC(l) && isIC(r)) {
      return F_NEWIC(std::static_pointer_cast<IntegerConstant>(l)->value(),
                     std::static_pointer_cast<IntegerConstant>(r)->value(), +);
    } else if (isIC(l) && isFC(r)) {
      return F_NEWFC(
          (double)std::static_pointer_cast<IntegerConstant>(l)->value(),
          std::static_pointer_cast<FloatingConstant>(r)->value(), +);
    } else if (isFC(l) && isIC(r)) {
      return F_NEWFC(
          std::static_pointer_cast<FloatingConstant>(l)->value(),
          (double)std::static_pointer_cast<IntegerConstant>(r)->value(), +);
    } else if (isFC(l) && isFC(r)) {
      return F_NEWFC(std::static_pointer_cast<FloatingConstant>(l)->value(),
                     std::static_pointer_cast<FloatingConstant>(r)->value(), +);
    }
  } else if (e->op() == Token::T_SUB) {
    if (isIC(l) && isIC(r)) {
      return new IntegerConstant(((IntegerConstant *)l)->value() -
                                 ((IntegerConstant *)r)->value());
    } else if (isIC(l) && isFC(r)) {
      return new FloatingConstant((double)((IntegerConstant *)l)->value() -
                                  ((FloatingConstant *)r)->value());
    } else if (isFC(l) && isIC(r)) {
      return new FloatingConstant(((FloatingConstant *)l)->value() -
                                  (double)((IntegerConstant *)r)->value());
    } else if (isFC(l) && isFC(r)) {
      return new FloatingConstant(((FloatingConstant *)l)->value() -
                                  ((FloatingConstant *)r)->value());
    }
  } else if (e->op() == Token::T_MUL) {
    if (isIC(l) && isIC(r)) {
      return new IntegerConstant(((IntegerConstant *)l)->value() *
                                 ((IntegerConstant *)r)->value());
    } else if (isIC(l) && isFC(r)) {
      return new FloatingConstant((double)((IntegerConstant *)l)->value() *
                                  ((FloatingConstant *)r)->value());
    } else if (isFC(l) && isIC(r)) {
      return new FloatingConstant(((FloatingConstant *)l)->value() *
                                  (double)((IntegerConstant *)r)->value());
    } else if (isFC(l) && isFC(r)) {
      return new FloatingConstant(((FloatingConstant *)l)->value() *
                                  ((FloatingConstant *)r)->value());
    }
  } else if (e->op() == Token::T_DIV) {
    if (isIC(l) && isIC(r)) {
      return new IntegerConstant(((IntegerConstant *)l)->value() /
                                 ((IntegerConstant *)r)->value());
    } else if (isIC(l) && isFC(r)) {
      return new FloatingConstant((double)((IntegerConstant *)l)->value() /
                                  ((FloatingConstant *)r)->value());
    } else if (isFC(l) && isIC(r)) {
      return new FloatingConstant(((FloatingConstant *)l)->value() /
                                  (double)((IntegerConstant *)r)->value());
    } else if (isFC(l) && isFC(r)) {
      return new FloatingConstant(((FloatingConstant *)l)->value() /
                                  ((FloatingConstant *)r)->value());
    }
  } else if (e->op() == Token::T_MOD) {
    if (isIC(l) && isIC(r)) {
      return new IntegerConstant(((IntegerConstant *)l)->value() %
                                 ((IntegerConstant *)r)->value());
    } else if (isIC(l) && isFC(r)) {
      return new FloatingConstant((double)((IntegerConstant *)l)->value() %
                                  ((FloatingConstant *)r)->value());
    } else if (isFC(l) && isIC(r)) {
      return new FloatingConstant(((FloatingConstant *)l)->value() %
                                  (double)((IntegerConstant *)r)->value());
    } else if (isFC(l) && isFC(r)) {
      return new FloatingConstant(((FloatingConstant *)l)->value() %
                                  ((FloatingConstant *)r)->value());
    }
  }
  F_CHECK(false, "must not reach here, node:{}", node->toString());
  F_THROW(ScriptException, "must not reach here, node: {}", node->toString());
  return nullptr;
}

std::shared_ptr<Ast> Interpreter::visitUnaryOp(std::shared_ptr<Ast> node) {
  UnaryOp *e = (UnaryOp *)node;
  std::shared_ptr<Ast> expr = visitExpression(e->expr());
  if (e->op() == Token::T_ADD) {
    return expr;
  } else if (e->op() == Token::T_SUB) {
    std::shared_ptr<Ast> expr = e->expr();
    if (isIC(expr)) {
      return new IntegerConstant(-((IntegerConstant *)expr)->value());
    } else if (isFC(expr)) {
      return new FloatingConstant(-((FloatingConstant *)l)->value());
    }
  }
  F_CHECK(false, "must not reach here, node:{}", node->toString());
  F_THROW(ScriptException, "must not reach here, node: {}", node->toString());
  return nullptr;
}

void Interpreter::interpret() {
  release(tree_);
  tree_ = parser_->parse();
  visit(tree_);
}

void Interpreter::release(std::shared_ptr<Ast> node) {
  if (!node) {
    return;
  }
  switch (node->type()) {
  case Ast::AstType::BINARY_OP: {
    releaseImpl(((BinaryOp *)node)->left());
    releaseImpl(((BinaryOp *)node)->right());
  } break;
  case Ast::AstType::UNARY_OP: {
    releaseImpl(((UnaryOp *)node)->expr());
  } break;
  case Ast::AstType::INTEGER_CONSTANT:
  case Ast::AstType::FLOATING_CONSTANT:
    break;
  default:
    F_CHECK(false, "must not reach here, node:{}", node->toString());
    F_THROW(ScriptException, "must not reach here, node: {}", node->toString());
  }
  node.reset();
}

const std::unordered_map<icu::UnicodeString, std::shared_ptr<Ast>>
Interpreter::globalScope() const {
  return globalScope_;
}

std::string Interpreter::toString() const {
  std::stringstream ss;
  ss << "[ @Interpreter globalScope_#size:";
  ss << globalScope_.size() << " ";
  int n = 0;
  for (auto i = globalScope_.begin(); i != globalScope_.end(); i++) {
    std::string _1;
    ss << fmt::format("[{}-{}:{}], ", n++, i->first.toUTF8String(_1),
                      i->second->toString());
  }
  ss << "]";
  return ss.str();
}

} // namespace fastype

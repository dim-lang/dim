// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Interpreter.h"
#include "Logging.h"
#include "Token.h"
#include "exception/ParseException.h"
#include "script/ast/AssignmentStatement.h"
#include "script/ast/BinaryOp.h"
#include "script/ast/BooleanConstant.h"
#include "script/ast/CompoundStatement.h"
#include "script/ast/FloatingConstant.h"
#include "script/ast/IdentifierConstant.h"
#include "script/ast/IntegerConstant.h"
#include "script/ast/Program.h"
#include "script/ast/StringConstant.h"
#include "script/ast/UnaryOp.h"
#include "script/ast/Variable.h"
#include <sstream>

namespace fastype {

Interpreter::Interpreter(Sptr<Parser> parser)
    : tree_(nullptr), parser_(parser), globalScope_() {}

Interpreter::~Interpreter() {
  globalScope_.clear();
  globalScope_.shrink_to_fit();
  release(tree_);
  tree_ = nullptr;
}

long long Interpreter::visit(Ast *node) {
  switch (node->type()) {
  case Ast::AstType::BINARY_OP:
    return visitBinaryOp(node);
    break;
  case Ast::AstType::UNARY_OP:
    return visitUnaryOp(node);
    break;
  case Ast::AstType::INTEGER_CONSTANT:
    return visitIntergerConstant(node);
    break;
  case Ast::AstType::FLOATING_CONSTANT:
    return (long long)visitFloatingConstant(node);
    break;
  default:
    F_CHECK(false, "must not reach here, node:{}", node->toString());
    F_THROW(ParseException, "invalid node type: {}", node->toString());
  }
  return -1LL;
}

long long Interpreter::visitBinaryOp(Ast *node) {
  BinaryOp *e = (BinaryOp *)node;
  if (e->op() == Token::T_ADD) {
    return visit(e->left()) + visit(e->right());
  } else if (e->op() == Token::T_SUB) {
    return visit(e->left()) - visit(e->right());
  } else if (e->op() == Token::T_MUL) {
    return visit(e->left()) * visit(e->right());
  } else if (e->op() == Token::T_DIV) {
    return visit(e->left()) / visit(e->right());
  } else if (e->op() == Token::T_MOD) {
    return visit(e->left()) % visit(e->right());
  }
  F_CHECK(false, "must not reach here, node:{}", node->toString());
  F_THROW(ParseException, "invalid node type: {}", node->toString());
  return -1LL;
}

long long Interpreter::visitUnaryOp(Ast *node) {
  UnaryOp *e = (UnaryOp *)node;
  if (e->op() == Token::T_ADD) {
    return visit(e->expr());
  } else if (e->op() == Token::T_SUB) {
    return -visit(e->expr());
  }
  F_CHECK(false, "must not reach here, node:{}", node->toString());
  F_THROW(ParseException, "invalid node type: {}", node->toString());
  return -1LL;
}

long long Interpreter::visitIntergerConstant(Ast *node) {
  IntegerConstant *e = (IntegerConstant *)node;
  return e->value();
}

double Interpreter::visitFloatingConstant(Ast *node) {
  FloatingConstant *e = (FloatingConstant *)node;
  return e->value();
}

void Interpreter::visitCompoundStatement(Ast *node) {
  CompoundStatement *e = (CompoundStatement *)node;
  for (int i = 0; i < e->size(); i++) {
    Ast *stmt = e->get(i);
    visit(stmt);
  }
}

void Interpreter::visitAssignmentStatement(Ast *node) {
  AssignmentStatement *e = (AssignmentStatement *)node;
  Variable *left = (Variable *)e->left();
  Ast *right = e->right();
  globalScope_.insert(std::make_pair(left->value(), right));
}

void Interpreter::visitEmptyStatement(Ast *node) {}

Ast *Interpreter::visitVariable(Ast *node) {
  Variable *e = (Variable *)node;
  F_CHECK(globalScope_.find(e->value) == globalScope_.end(),
          "variable {} not found in globalScope_", e->toString());
  if (globalScope_.find(e->value) == globalScope_.end()) {
    F_THROW(ParseException, "variable {} not found in globalScope_",
            e->toString());
  }
  return globalScope_.at(e->value());
}

void Interpreter::interpret() {
  release(tree_);
  tree_ = parser_->parse();
  visit(tree_);
}

void Interpreter::release(Ast *node) {
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
    F_THROW(ParseException, "invalid node type: {}", node->toString());
  }
  delete node;
}

const std::unordered_map<icu::UnicodeString, Ast *>
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

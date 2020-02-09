// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Semant.h"
#include "Log.h"
#include "Symbol.h"
#include "Type.h"

Semant::Semant(Ast *program)
    : program_(program), gsym_(nullptr), csym_(nullptr), gty_(nullptr),
      cty_(nullptr) {
  CASSERT(program_, "program_ is null");
  CASSERT(program_->type() == A_DECLARATION_LIST, "invalid program_#type: {}",
          program_->toString());
}

Semant::~Semant() {
  program_ = nullptr;
  gsym_ = nullptr;
  csym_ = nullptr;
  gty_ = nullptr;
  cty_ = nullptr;
}

void Semant::buildImpl(Ast *node) {
  CASSERT(node, "node is null");
  switch (node->type()) {
  case A_VARIABLE_DECLARATION: {
    AstVariableDeclaration *e = dynamic_cast<AstVariableDeclaration *>(node);
    AstDeclarationList *declList = e->declarationList();
    CASSERT(declList, "declList is null");
    for (int i = 0; i < declList->size(); i++) {
      buildImpl(declList->get(i));
    }
  } break;
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    AstVariableAssignmentDeclaration *e =
        dynamic_cast<AstVariableAssignmentDeclaration *>(node);
    csym_->define(new VariableSymbol(e->identifier()));
  } break;
  case A_FUNCTION_DECLARATION: {
    AstFunctionDeclaration *e = dynamic_cast<AstFunctionDeclaration *>(node);
    FunctionSymbol *fs = new FunctionSymbol(e->identifier(), csym_);
    csym_->define(fs);
    Symbol::push(gsym_, csym_, fs);
    buildImpl(e->argumentList());
    buildImpl(e->statement());
    Symbol::pop();
  } break;
  case A_DECLARATION_LIST: {
    AstDeclarationList *declList = dynamic_cast<AstDeclarationList *>(node);
    for (int i = 0; i < declList->size(); i++) {
      buildImpl(declList->get(i));
    }
  } break;
  case A_FUNCTION_ARGUMENT_DECLARATION: {
    AstFunctionArgumentDeclaration *e =
        dynamic_cast<AstFunctionArgumentDeclaration *>(node);
    csym_->define(new VariableSymbol(e->value()));
  } break;
  case A_COMPOUND_STATEMENT: {
    AstCompoundStatement *e = dynamic_cast<AstCompoundStatement *>(node);
    LocalSymtab *ls = new LocalSymtab("AnonymousLocalSymtab", csym_);
    Symbol::push(gsym_, csym_, ls);
    AstStatementList *stmtList = e->statementList();
    if (stmtList) {
      for (int i = 0; i < stmtList->size(); i++) {
        buildImpl(stmtList->get(i));
      }
    }
    Symbol::pop();
  } break;
  case A_IF_STATEMENT: {
    AstIfStatement *e = dynamic_cast<AstIfStatement *>(node);
    buildImpl(e->left());
    buildImpl(e->right());
  } break;
  case A_WHILE_STATEMENT: {
    AstWhileStatement *e = dynamic_cast<AstWhileStatement *>(node);
    buildImpl(e->statement());
  } break;
  case A_FOR_STATEMENT: {
    AstForStatement *e = dynamic_cast<AstForStatement *>(node);
    LocalSymtab *ls = new LocalSymtab("ForLoopLocalSymtab", csym_);
    Symbol::push(gsym_, csym_, ls);
    buildImpl(e->initialize());
    buildImpl(e->statement());
    Symbol::pop();
  } break;
  default:
    // do nothing
  }
}

void Semant::build() {
  // handle program routine
  CASSERT(!gsym_, "gsym_ is not null: {}", gsym_->toString());
  CASSERT(!csym_, "csym_ is not null: {}", csym_->toString());
  AstDeclarationList *e = dynamic_cast<AstDeclarationList *>(program_);
  Symtab *scope = new GlobalSymtab();
  Symbol::push(gsym_, csym_, scope);
  for (int i = 0; i < e->size(); i++) {
    buildImpl(e->get(i));
  }
  Symbol::pop();
  CASSERT(!csym_, "csym_ is not null: {}", csym_->toString());
  CASSERT(gsym_, "gsym_ is null");
}

void Semant::checkImpl(Ast *node) {
  CASSERT(node, "node is null");
  switch (node->type()) {
  case A_DECLARATION_LIST:
  default:
    CASSERT(false, "invalid node: {}", node->toString());
  }
}

void Semant::check() {
  // handle program routine
  CASSERT(!gty_, "gty_ is not null: {}", gty_->toString());
  CASSERT(!cty_, "cty_ is not null: {}", cty_->toString());
  AstDeclarationList *e = dynamic_cast<AstDeclarationList *>(program_);
  Symtab *scope = new GlobalSymtab();
  Type::push(gty_, cty_, scope);
  for (int i = 0; i < e->size(); i++) {
    buildImpl(e->get(i));
  }
  Type::pop();
  CASSERT(!cty_, "cty_ is not null: {}", cty_->toString());
  CASSERT(gty_, "gty_ is null");
  checkImpl(program_);
}

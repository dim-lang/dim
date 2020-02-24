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
  CASSERT(program_->type() == A_PROGRAM, "program is program: {}",
          program_->toString());
}

Semant::~Semant() {
  program_ = nullptr;
  gsym_ = nullptr;
  csym_ = nullptr;
  gty_ = nullptr;
  cty_ = nullptr;
}

Symtab *Semant::globalSymbolTable() { return gsym_; }

Tytab *Semant::globalTypeTable() { return gty_; }

void Semant::buildImpl(Ast *node) {
  CASSERT(node, "node is null");
  switch (node->type()) {
  case A_VARIABLE_DECLARATION: {
    AstVariableDeclaration *e = dynamic_cast<AstVariableDeclaration *>(node);
    AstDeclarationList *declList = e->declarationList();
    CASSERT(declList, "declList is null");
    for (int i = 0; i < declList->size(); i++) {
      CASSERT(declList->get(i), "declList#get {} is null", i);
      buildImpl(declList->get(i));
    }
  } break;
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    AstVariableAssignmentDeclaration *e =
        dynamic_cast<AstVariableAssignmentDeclaration *>(node);
    VariableSymbol *vs = new VariableSymbol(e->identifier());
    csym_->define(vs);
    switch (e->expression()->type()) {
    case A_I64_CONSTANT: {
      cty_->define(vs, BuiltinType::ty_i64());
    } break;
    case A_F64_CONSTANT: {
      cty_->define(vs, BuiltinType::ty_f64());
    } break;
    case A_STRING_CONSTANT: {
      cty_->define(vs, BuiltinType::ty_string());
    } break;
    case A_BOOLEAN_CONSTANT: {
      cty_->define(vs, BuiltinType::ty_boolean());
    } break;
    default: {
      cty_->define(vs, BuiltinType::ty_void());
    } break;
    }
  } break;
  case A_FUNCTION_DECLARATION: {
    AstFunctionDeclaration *e = dynamic_cast<AstFunctionDeclaration *>(node);
    FunctionSymbol *fs = new FunctionSymbol(e->identifier(), csym_);
    std::vector<std::pair<Symbol *, Type *>> argumentTypeList;
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        AstFunctionArgumentDeclaration *fa =
            dynamic_cast<AstFunctionArgumentDeclaration *>(
                e->argumentList()->get(i));
        argumentTypeList.push_back(std::make_pair(
            new FunctionArgumentSymbol(fa->value()), BuiltinType::ty_void()));
      }
    }
    FunctionType *ft =
        new FunctionType(argumentTypeList, BuiltinType::ty_void(), cty_);
    csym_->define(fs);
    cty_->define(fs, ft);
    Symbol::push(gsym_, csym_, fs);
    Type::push(gty_, cty_, ft);
    if (e->argumentList()) {
      buildImpl(e->argumentList());
    }
    CASSERT(e->statement(), "e#statement is null");
    buildImpl(e->statement());
    Symbol::pop(gsym_, csym_);
    Type::pop(gty_, cty_);
  } break;
  case A_DECLARATION_LIST: {
    AstDeclarationList *declList = dynamic_cast<AstDeclarationList *>(node);
    for (int i = 0; i < declList->size(); i++) {
      CASSERT(declList->get(i), "declList#get {} is null", i);
      buildImpl(declList->get(i));
    }
  } break;
  case A_FUNCTION_ARGUMENT_DECLARATION: {
    AstFunctionArgumentDeclaration *e =
        dynamic_cast<AstFunctionArgumentDeclaration *>(node);
    FunctionArgumentSymbol *fa = new FunctionArgumentSymbol(e->value());
    csym_->define(fa);
    cty_->define(fa, BuiltinType::ty_string());
  } break;
  case A_COMPOUND_STATEMENT: {
    AstCompoundStatement *e = dynamic_cast<AstCompoundStatement *>(node);
    LocalSymtab *ls = new LocalSymtab(e->name(), csym_);
    LocalTytab *lt = new LocalTytab(e->name(), cty_);
    Symbol::push(gsym_, csym_, ls);
    Type::push(gty_, cty_, lt);
    AstStatementList *stmtList = e->statementList();
    CASSERT(stmtList, "stmtList is null");
    for (int i = 0; i < stmtList->size(); i++) {
      CASSERT(stmtList->get(i), "stmtList#get {} is null", i);
      buildImpl(stmtList->get(i));
    }
    Symbol::pop(gsym_, csym_);
    Type::pop(gty_, cty_);
  } break;
  case A_IF_STATEMENT: {
    AstIfStatement *e = dynamic_cast<AstIfStatement *>(node);
    CASSERT(e->hit(), "e#hit is null");
    CASSERT(e->miss(), "e#miss is null");
    buildImpl(e->hit());
    buildImpl(e->miss());
  } break;
  case A_WHILE_STATEMENT: {
    AstWhileStatement *e = dynamic_cast<AstWhileStatement *>(node);
    CASSERT(e->statement(), "e#statement is null");
    buildImpl(e->statement());
  } break;
  case A_FOR_STATEMENT: {
    AstForStatement *e = dynamic_cast<AstForStatement *>(node);
    LocalSymtab *ls = new LocalSymtab(e->name(), csym_);
    LocalTytab *lt = new LocalTytab(e->name(), cty_);
    Symbol::push(gsym_, csym_, ls);
    Type::push(gty_, cty_, lt);
    CASSERT(e->initial(), "e#initial is null");
    CASSERT(e->statement(), "e#statement is null");
    buildImpl(e->initial());
    buildImpl(e->statement());
    Symbol::pop(gsym_, csym_);
    Type::pop(gty_, cty_);
  } break;
  default: {
    CINFO("do nothing for node:{}", node->toString());
  } break;
  }
}

void Semant::build() {
  CASSERT(!gsym_, "gsym_ is not null: {}", gsym_->toString());
  CASSERT(!csym_, "csym_ is not null: {}", csym_->toString());
  CASSERT(!gty_, "gty_ is not null: {}", gty_->toString());
  CASSERT(!cty_, "cty_ is not null: {}", cty_->toString());
  CASSERT(program_->type() == A_PROGRAM, "program_ is program: {}",
          program_->toString());
  AstProgram *e = dynamic_cast<AstProgram *>(program_);
  CASSERT(e, "program is null: {}", e ? e->toString() : "null");
  Symbol::push(gsym_, csym_, GlobalSymtab::instance());
  Type::push(gty_, cty_, GlobalTytab::instance());
  for (int i = 0; i < e->size(); i++) {
    buildImpl(e->get(i));
  }
  Symbol::pop(gsym_, csym_);
  Type::pop(gty_, cty_);
  CASSERT(!csym_, "csym_ is not null: {}", csym_->toString());
  CASSERT(gsym_, "gsym_ is null");
  CASSERT(!cty_, "cty_ is not null: {}", cty_->toString());
  CASSERT(gty_, "gty_ is null");
}

void Semant::checkImpl(Ast *node) {
  CASSERT(node, "node is null");
  switch (node->type()) {
  case A_IDENTIFIER_CONSTANT: {
    AstIdentifierConstant *e = dynamic_cast<AstIdentifierConstant *>(node);
    Symbol *sym = csym_->resolve(e->value());
    Type *ty = cty_->resolve(sym);
    CASSERT(sym, "sym is null");
    CASSERT(ty, "ty is null");
  } break;
  case A_UNARY_EXPRESSION: {
    AstUnaryExpression *e = dynamic_cast<AstUnaryExpression *>(node);
    checkImpl(e->expression());
  } break;
  case A_BINARY_EXPRESSION: {
    AstBinaryExpression *e = dynamic_cast<AstBinaryExpression *>(node);
    checkImpl(e->left());
    checkImpl(e->right());
  } break;
  case A_CONDITIONAL_EXPRESSION: {
    AstBinaryExpression *e = dynamic_cast<AstBinaryExpression *>(node);
    checkImpl(e->left());
    checkImpl(e->right());
  } break;
  case A_ASSIGNMENT_EXPRESSION: {
    AstAssignmentExpression *e = dynamic_cast<AstAssignmentExpression *>(node);
    checkImpl(e->variable());
  } break;
  case A_EXPRESSION_STATEMENT: {
    AstExpressionStatement *e = dynamic_cast<AstExpressionStatement *>(node);
    checkImpl(e->expression());
  } break;
  case A_COMPOUND_STATEMENT: {
    AstCompoundStatement *e = dynamic_cast<AstCompoundStatement *>(node);
    Symbol *sym = csym_->resolve(e->name());
    Type *ty = cty_->resolve(sym);
    Symbol::push(gsym_, csym_, dynamic_cast<Symtab *>(sym));
    Type::push(gty_, cty_, dynamic_cast<Tytab *>(ty));
    for (int i = 0; i < e->statementList()->size(); i++) {
      checkImpl(e->statementList()->get(i));
    }
    Symbol::pop(gsym_, csym_);
    Type::pop(gty_, cty_);
  } break;
  case A_IF_STATEMENT: {
    AstIfStatement *e = dynamic_cast<AstIfStatement *>(node);
    checkImpl(e->hit());
    checkImpl(e->miss());
  } break;
  case A_WHILE_STATEMENT: {
    AstWhileStatement *e = dynamic_cast<AstWhileStatement *>(node);
    checkImpl(e->condition());
    checkImpl(e->statement());
  } break;
  case A_FOR_STATEMENT: {
    AstForStatement *e = dynamic_cast<AstForStatement *>(node);
    Symbol *sym = csym_->resolve(e->name());
    Type *ty = cty_->resolve(sym);
    Symbol::push(gsym_, csym_, dynamic_cast<Symtab *>(sym));
    Type::push(gty_, cty_, dynamic_cast<Tytab *>(ty));
    checkImpl(e->initial());
    checkImpl(e->condition());
    checkImpl(e->post());
    checkImpl(e->statement());
    Symbol::pop(gsym_, csym_);
    Type::pop(gty_, cty_);
  } break;
  default: {
    CINFO("do nothing for node:{}", node->toString());
  } break;
  }
}

void Semant::check() {
  CASSERT(gsym_, "gsym_ is null");
  CASSERT(gty_, "gty_ is null");
  AstProgram *e = dynamic_cast<AstProgram *>(program_);
  CASSERT(e, "program is null: {}", e ? e->toString() : "null");
  Symbol::push(gsym_, csym_, GlobalSymtab::instance());
  Type::push(gty_, cty_, GlobalTytab::instance());
  for (int i = 0; i < e->size(); i++) {
    checkImpl(e->get(i));
  }
  Symbol::pop(gsym_, csym_);
  Type::pop(gty_, cty_);
}

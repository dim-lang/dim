// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Exception.h"
#include "TokenName.h"
#include "container/LinkedHashMap.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <sstream>
#include <string>

#define DS std::string(depth, ' ')
#define DC(x, y) dynamic_cast<x *>(y)

static std::string dumpAstImpl(Ast *node, int depth) {
  if (!node)
    return "null";
  switch (node->type()) {
  case AstType::IdentifierConstant:
  case AstType::Int8Constant:
  case AstType::UInt8Constant:
  case AstType::Int16Constant:
  case AstType::UInt16Constant:
  case AstType::Int32Constant:
  case AstType::UInt32Constant:
  case AstType::Int64Constant:
  case AstType::UInt64Constant:
  case AstType::Float32Constant:
  case AstType::Float64Constant:
  case AstType::StringConstant:
  case AstType::BooleanConstant:
    return node->name();
  case AstType::CallExpression: {
    AstCallExpression *e = DC(AstCallExpression, node);
    std::stringstream ss;
    ss << e->name() << "(";
    if (e->argumentList()) {
      for (int i = 0; i < e->argumentList()->size(); i++) {
        ss << dumpAstImpl(e->argumentList()->get(i), depth);
        if (i < e->argumentList()->size() - 1) {
          ss << ", ";
        }
      }
    }
    ss << ")";
    return ss.str();
  }
  case AstType::UnaryExpression:
    return tokenName(DC(AstUnaryExpression, node)->token()) + " " +
           dumpAstImpl(DC(AstUnaryExpression, node)->expression(), depth);
  case AstType::BinaryExpression:
    return dumpAstImpl(DC(AstBinaryExpression, node)->left(), depth) + " " +
           tokenName(DC(AstBinaryExpression, node)->token()) + " " +
           dumpAstImpl(DC(AstBinaryExpression, node)->right(), depth);
  case AstType::ConditionalExpression:
    return dumpAstImpl(DC(AstConditionalExpression, node)->condition(), depth) +
           "?" +
           dumpAstImpl(DC(AstConditionalExpression, node)->thens(), depth) +
           ":" +
           dumpAstImpl(DC(AstConditionalExpression, node)->elses(), depth);
  case AstType::AssignmentExpression:
    return dumpAstImpl(DC(AstAssignmentExpression, node)->variable(), depth) +
           "=" + dumpAstImpl(DC(AstAssignmentExpression, node)->value(), depth);
  case AstType::SequelExpression: {
    AstSequelExpression *e = DC(AstSequelExpression, node);
    std::stringstream ss;
    for (int i = 0; i < e->expressionList()->size(); i++) {
      ss << dumpAstImpl(e->expressionList()->get(i), depth);
      if (i < (int)e->expressionList()->size() - 1) {
        ss << ",";
      }
    }
    return ss.str();
  }
  case AstType::ExpressionStatement:
    return DS +
           dumpAstImpl(DC(AstExpressionStatement, node)->expression(), depth) +
           ";\n";
  case AstType::CompoundStatement: {
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    std::stringstream ss;
    ss << DS << "{";
    if (e->statementList() && e->statementList()->size() > 0) {
      ss << "\n";
      for (int i = 0; i < e->statementList()->size(); i++) {
        ss << dumpAstImpl(e->statementList()->get(i), depth + 1);
      }
      ss << DS;
    }
    ss << "}\n\n";
    return ss.str();
  }
  case AstType::IfStatement: {
    AstIfStatement *is = DC(AstIfStatement, node);
    std::stringstream ss;
    ss << DS << "if (";
    X_ASSERT(is->thens(), "is#thens is null");
    ss << dumpAstImpl(is->condition(), depth) << ") \n"
       << dumpAstImpl(is->thens(), depth + 1);
    if (is->elses()) {
      ss << DS << "else\n" << dumpAstImpl(is->elses(), depth + 1);
    }
    return ss.str();
  }
  case AstType::WhileStatement: {
    AstWhileStatement *ws = DC(AstWhileStatement, node);
    std::stringstream ss;
    ss << DS << "while (";
    ss << dumpAstImpl(ws->condition(), depth) << ")\n"
       << dumpAstImpl(ws->statement(), depth + 1);
    return ss.str();
  }
  case AstType::ForStatement: {
    AstForStatement *fs = DC(AstForStatement, node);
    std::stringstream ss;
    ss << DS << "for (" << dumpAstImpl(fs->start(), depth)
       << dumpAstImpl(fs->step(), depth) << dumpAstImpl(fs->end(), depth) + ") "
       << dumpAstImpl(fs->statement(), depth) << "\n";
    return ss.str();
  }
  case AstType::ContinueStatement:
    return DS + DC(AstContinueStatement, node)->name() + ";\n";
  case AstType::BreakStatement:
    return DS + DC(AstBreakStatement, node)->name() + ";\n";
  case AstType::ReturnStatement:
    return DS + DC(AstReturnStatement, node)->name() + " " +
           dumpAstImpl(DC(AstReturnStatement, node)->expression(), depth) +
           ";\n";
  case AstType::EmptyStatement:
    return DS + DC(AstEmptyStatement, node)->name() + ";\n";
  case AstType::VariableDefinition: {
    AstVariableDefinition *vd = DC(AstVariableDefinition, node);
    std::stringstream ss;
    ss << DS << vd->name() << ": ";
    for (int i = 0; i < vd->definitionList()->size(); i++) {
      ss << dumpAstImpl(vd->definitionList()->get(i), depth);
      if (i < vd->definitionList()->size() - 1) {
        ss << ", ";
      }
    }
    ss << ";\n";
    return ss.str();
  }
  case AstType::VariableInitialDefinition: {
    AstVariableInitialDefinition *vad = DC(AstVariableInitialDefinition, node);
    return vad->identifier() + " = " + dumpAstImpl(vad->expression(), depth);
  }
  case AstType::FunctionDefinition: {
    AstFunctionDefinition *fd = DC(AstFunctionDefinition, node);
    AstFunctionSignatureDefinition *fsd = fd->signature();
    std::stringstream ss;
    ss << DS << fd->name() << "(";
    if (fsd->argumentList() && fsd->argumentList()->size() > 0) {
      for (int i = 0; i < fsd->argumentList()->size(); i++) {
        ss << fsd->argumentList()->get(i)->name();
        if (i < fsd->argumentList()->size() - 1) {
          ss << ",";
        }
      }
    }
    ss << ") =>\n" << dumpAstImpl(fd->statement(), depth);
    return ss.str();
  }
  case AstType::DefinitionList: {
    AstDefinitionList *dl = DC(AstDefinitionList, node);
    std::stringstream ss;
    ss << DS << dl->name() << "{\n";
    for (int i = 0; i < dl->size(); i++) {
      ss << dumpAstImpl(dl->get(i), depth);
    }
    ss << "}\n";
    return ss.str();
  }
  case AstType::TranslateUnit: {
    AstTranslateUnit *p = DC(AstTranslateUnit, node);
    std::stringstream ss;
    ss << DS << p->name() << " {\n";
    for (int i = 0; i < p->size(); i++) {
      ss << dumpAstImpl(p->get(i), depth + 1);
    }
    ss << "}\n";
    return ss.str();
  }
  // case AstType::ExpressionList: {
  // return DC(AstExpressionList, node)->name();
  //}
  default: {
    X_ASSERT(false, "invalid node:{}", node->toString());
  }
  }
}

std::string dumpAst(Ast *node) { return dumpAstImpl(node, 0); }

#define IS_SCOPE(x)                                                            \
  ((Scope::sym(x))->type() == (+SymType::Function) ||                          \
   (Scope::sym(x))->type() == (+SymType::Class) ||                             \
   (Scope::sym(x))->type() == (+SymType::Global) ||                            \
   (Scope::sym(x))->type() == (+SymType::Local))

static std::string dumpScopeImpl(const Scope::SNode &snode, int depth) {
  const Symbol *s = Scope::sym(snode);
  const Type *t = Scope::ty(snode);
  const Ast *a = Scope::ast(snode);
  if (!s)
    return "null";
  X_ASSERT(s && t && a, "snode is null: {} {} {}", (void *)s, (void *)t,
           (void *)a);
  switch (s->type()) {
  case SymType::Variable:
    return std::string("var ") + s->name() + ":" + t->name();
  case SymType::Function: {
    const FunctionSymbol *funcsym = DC(const FunctionSymbol, s);
    std::stringstream ss;
    ss << "func " << funcsym->name() << " {";
    if (!funcsym->empty()) {
      for (FunctionSymbol::CIterator i = funcsym->begin(); i != funcsym->end();
           i++) {
        ss << "\n" << DS << i->first;
        if (IS_SCOPE(i->second)) {
          ss << " => " << dumpScopeImpl(i->second, depth + 1);
        }
      }
      ss << "\n" << std::string(depth - 1, ' ');
    }
    ss << "}\n";
    return ss.str();
  }
  case SymType::FunctionArgument:
    return std::string("var ") + s->name() + ":" + t->name();
  case SymType::Class: {
    const ClassSymbol *clssym = DC(const ClassSymbol, s);
    std::stringstream ss;
    ss << DS << "class " << clssym->name() << " {";
    if (!clssym->empty()) {
      for (ClassSymbol::CIterator i = clssym->begin(); i != clssym->end();
           i++) {
        ss << "\n" << DS << i->first;
        if (IS_SCOPE(i->second)) {
          ss << " => " << dumpScopeImpl(i->second, depth + 1);
        }
      }
      ss << "\n" << std::string(depth - 1, ' ');
    }
    ss << "}\n";
    return ss.str();
  }
  case SymType::Local: {
    const LocalScope *locsym = DC(const LocalScope, s);
    std::stringstream ss;
    ss << "local " << locsym->name() << " {";
    if (!locsym->empty()) {
      for (LocalScope::CIterator i = locsym->begin(); i != locsym->end(); i++) {
        ss << "\n"
           << DS << i->first << " => " << dumpScopeImpl(i->second, depth + 1);
      }
      ss << "\n" << std::string(depth - 1, ' ');
    }
    ss << "}\n";
    return ss.str();
  }
  case SymType::Global: {
    const GlobalScope *glbsym = DC(const GlobalScope, s);
    std::stringstream ss;
    ss << DS << glbsym->name() << " {";
    if (!glbsym->empty()) {
      for (GlobalScope::CIterator i = glbsym->begin(); i != glbsym->end();
           i++) {
        ss << "\n" << DS << i->first;
        if (IS_SCOPE(i->second)) {
          ss << " => " << dumpScopeImpl(i->second, depth + 1);
        }
      }
      ss << "\n" << DS;
    }
    ss << "}\n";
    return ss.str();
  }
  default:
    X_ASSERT(false, "invalid symbol: {} {}", s->name(), s->type()._to_string());
  }
}

#undef IS_SCOPE

std::string dumpScope(const Scope::SNode &snode) {
  return dumpScopeImpl(snode, 0);
}

#undef DS
#undef DC

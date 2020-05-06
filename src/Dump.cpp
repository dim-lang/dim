// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Log.h"
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
  case AstType::I8Constant:
  case AstType::U8Constant:
  case AstType::I16Constant:
  case AstType::U16Constant:
  case AstType::I32Constant:
  case AstType::U32Constant:
  case AstType::I64Constant:
  case AstType::U64Constant:
  case AstType::F32Constant:
  case AstType::F64Constant:
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
           "?" + dumpAstImpl(DC(AstConditionalExpression, node)->hit(), depth) +
           ":" + dumpAstImpl(DC(AstConditionalExpression, node)->miss(), depth);
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
    LOG_ASSERT(is->hit(), "is#hit is null");
    ss << dumpAstImpl(is->condition(), depth) << ") \n"
       << dumpAstImpl(is->hit(), depth + 1);
    if (is->miss()) {
      ss << DS << "else\n" << dumpAstImpl(is->miss(), depth + 1);
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
    ss << DS << "for (" << dumpAstImpl(fs->initial(), depth)
       << dumpAstImpl(fs->condition(), depth)
       << dumpAstImpl(fs->post(), depth) + ") "
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
  case AstType::VariableDeclaration: {
    AstVariableDeclaration *vd = DC(AstVariableDeclaration, node);
    std::stringstream ss;
    ss << DS << vd->name() << ": ";
    for (int i = 0; i < vd->declarationList()->size(); i++) {
      ss << dumpAstImpl(vd->declarationList()->get(i), depth);
      if (i < vd->declarationList()->size() - 1) {
        ss << ", ";
      }
    }
    ss << ";\n";
    return ss.str();
  }
  case AstType::VariableInitialDeclaration: {
    AstVariableInitialDeclaration *vad =
        DC(AstVariableInitialDeclaration, node);
    return vad->identifier() + " = " + dumpAstImpl(vad->expression(), depth);
  }
  case AstType::FunctionDeclaration: {
    AstFunctionDeclaration *fd = DC(AstFunctionDeclaration, node);
    AstFunctionSignatureDeclaration *fsd = fd->signature();
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
  case AstType::DeclarationList: {
    AstDeclarationList *dl = DC(AstDeclarationList, node);
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
    LOG_ASSERT(false, "invalid node:{}", node->toString());
  }
  }
}

std::string dumpAst(Ast *node) { return dumpAstImpl(node, 0); }

#define IS_SYMTAB(x)                                                           \
  ((x)->type() == (+SymType::Function) || (x)->type() == (+SymType::Class) ||  \
   (x)->type() == (+SymType::Global) || (x)->type() == (+SymType::Local))

static std::string dumpSymbolImpl(Symbol *sym, int depth) {
  if (!sym)
    return "null";
  switch (sym->type()) {
  case SymType::Variable:
    return "var " + DC(VariableSymbol, sym)->name();
  case SymType::Function: {
    FunctionSymbol *fs = DC(FunctionSymbol, sym);
    std::stringstream ss;
    ss << "func " << fs->name() << " {";
    if (!fs->empty()) {
      for (auto i = fs->begin(); i != fs->end(); i++) {
        ss << "\n" << DS << i->first;
        if (IS_SYMTAB(i->second)) {
          ss << " => " << dumpSymbolImpl(i->second, depth + 1);
        }
      }
      ss << "\n" << std::string(depth - 1, ' ');
    }
    ss << "}\n";
    return ss.str();
  }
  case SymType::FunctionArgument:
    return DC(FunctionArgumentSymbol, sym)->name();
  case SymType::Class: {
    ClassSymbol *e = DC(ClassSymbol, sym);
    std::stringstream ss;
    ss << DS << "class " << e->name() << " {";
    if (!e->empty()) {
      for (auto i = e->begin(); i != e->end(); i++) {
        ss << "\n" << DS << i->first;
        if (IS_SYMTAB(i->second)) {
          ss << " => " << dumpSymbolImpl(i->second, depth + 1);
        }
      }
      ss << "\n" << std::string(depth - 1, ' ');
    }
    ss << "}\n";
    return ss.str();
  }
  case SymType::Local: {
    LocalSymtab *e = DC(LocalSymtab, sym);
    std::stringstream ss;
    ss << "local " << e->name() << " {";
    if (!e->empty()) {
      for (auto i = e->begin(); i != e->end(); i++) {
        ss << "\n"
           << DS << i->first << " => " << dumpSymbolImpl(i->second, depth + 1);
      }
      ss << "\n" << std::string(depth - 1, ' ');
    }
    ss << "}\n";
    return ss.str();
  }
  case SymType::Global: {
    GlobalSymtab *e = DC(GlobalSymtab, sym);
    std::stringstream ss;
    ss << DS << e->name() << " {";
    if (!e->empty()) {
      for (auto i = e->begin(); i != e->end(); i++) {
        ss << "\n" << DS << i->first;
        if (IS_SYMTAB(i->second)) {
          ss << " => " << dumpSymbolImpl(i->second, depth + 1);
        }
      }
      ss << "\n" << DS;
    }
    ss << "}\n";
    return ss.str();
  }
  default:
    LOG_ASSERT(false, "invalid symbol: {} {}", sym->name(), sym->type());
  }
}

#undef IS_SYMTAB

std::string dumpSymbol(Symbol *sym) { return dumpSymbolImpl(sym, 0); }

#define IS_TYTAB(x)                                                            \
  ((x)->type() == (+TyType::Function) || (x)->type() == (+TyType::Class) ||    \
   (x)->type() == (+TyType::Global) || (x)->type() == (+TyType::Local))

static std::string dumpTypeImpl(Type *ty, int depth) {
  if (!ty)
    return "null";
  switch (ty->type()) {
  case TyType::Builtin:
    return DC(BuiltinType, ty)->name();
  case TyType::Function: {
    FunctionType *ft = DC(FunctionType, ty);
    std::stringstream ss;
    ss << ft->name() << ": {";
    for (auto i = ft->begin(); i != ft->end(); i++) {
      ss << "\n"
         << DS << i->first->name() << " => "
         << dumpTypeImpl(i->second, depth + 1);
    }
    ss << "\n}\n";
    return ss.str();
  }
  case TyType::Class: {
    ClassType *ct = DC(ClassType, ty);
    std::stringstream ss;
    ss << ct->name() << ": {";
    for (auto i = ct->begin(); i != ct->end(); i++) {
      ss << "\n"
         << DS << i->first->name() << ", "
         << dumpTypeImpl(i->second, depth + 1);
    }
    ss << "\n}\n";
    return ss.str();
  }
  case TyType::Local: {
    LocalTytab *lt = DC(LocalTytab, ty);
    std::stringstream ss;
    ss << lt->name() << ": {";
    for (auto i = lt->begin(); i != lt->end(); i++) {
      ss << "\n"
         << DS << i->first->name() << " => "
         << dumpTypeImpl(i->second, depth + 1);
    }
    ss << "\n}\n";
    return ss.str();
  }
  case TyType::Global: {
    GlobalTytab *gt = DC(GlobalTytab, ty);
    std::stringstream ss;
    ss << gt->name() << ": {";
    for (auto i = gt->begin(); i != gt->end(); i++) {
      ss << "\n"
         << DS << i->first->name() << ", "
         << dumpTypeImpl(i->second, depth + 1);
    }
    ss << "\n}\n";
    return ss.str();
  }
  default:
    LOG_ASSERT(false, "invalid type: {} {}", ty->name(), ty->type());
  }
}

std::string dumpType(Type *ty) { return dumpTypeImpl(ty, 0); }

std::string dumpLLVMValue(llvm::Value *v) {
  std::stringstream ss;
  LOG_ASSERT(v, "v is null");
  std::string tmp;
  llvm::raw_string_ostream os(tmp);
  v->print(os);
  ss << os.str();
  ss << "\n";
  return ss.str();
}

std::string dumpLLVMFunction(llvm::Function *f) {
  std::stringstream ss;
  LOG_ASSERT(f, "f is null");
  std::string tmp;
  llvm::raw_string_ostream os(tmp);
  f->print(os);
  ss << os.str();
  ss << "\n";
  return ss.str();
}

#undef DS
#undef DC

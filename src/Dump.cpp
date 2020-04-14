// Copyright 2019- <ac-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Log.h"
#include "TokenName.h"
#include "container/LinkedHashMap.hpp"
#include <cstdio>
#include <sstream>
#include <string>

#define DS std::string(depth, ' ')
#define DC(x, y) dynamic_cast<x *>(y)

static std::string dumpAstImpl(Ast *node, int depth) {
  if (!node)
    return "null";
  switch (node->type()) {
  case AstType::ID_CONST:
  case AstType::I8_CONST:
  case AstType::U8_CONST:
  case AstType::I16_CONST:
  case AstType::U16_CONST:
  case AstType::I32_CONST:
  case AstType::U32_CONST:
  case AstType::I64_CONST:
  case AstType::U64_CONST:
  case AstType::F32_CONST:
  case AstType::F64_CONST:
  case AstType::STR_CONST:
  case AstType::BOOL_CONST:
    return node->name();
  case AstType::CALL_EXPR: {
    AstCallExpression *ce = DC(AstCallExpression, node);
    std::stringstream ss;
    ss << ce->name() << "(";
    if (ce->argumentList()) {
      for (int i = 0; i < ce->argumentList()->size(); i++) {
        ss << dumpAstImpl(ce->argumentList()->get(i), depth);
        if (i < ce->argumentList()->size() - 1) {
          ss << ", ";
        }
      }
    }
    ss << ")";
    return ss.str();
  }
  case AstType::UNA_EXPR:
    return tokenName(DC(AstUnaryExpression, node)->token()) + " " +
           dumpAstImpl(DC(AstUnaryExpression, node)->expression(), depth);
  case AstType::BIN_EXPR:
    return dumpAstImpl(DC(AstBinaryExpression, node)->left(), depth) + " " +
           tokenName(DC(AstBinaryExpression, node)->token()) + " " +
           dumpAstImpl(DC(AstBinaryExpression, node)->right(), depth);
  case AstType::COND_EXPR:
    return dumpAstImpl(DC(AstConditionalExpression, node)->condition(), depth) +
           "?" + dumpAstImpl(DC(AstConditionalExpression, node)->hit(), depth) +
           ":" + dumpAstImpl(DC(AstConditionalExpression, node)->miss(), depth);
  case AstType::ASSIGN_EXPR:
    return dumpAstImpl(DC(AstAssignmentExpression, node)->variable(), depth) +
           "=" + dumpAstImpl(DC(AstAssignmentExpression, node)->value(), depth);
  case AstType::SEQ_EXPR: {
    AstSequelExpression *se = DC(AstSequelExpression, node);
    std::stringstream ss;
    for (int i = 0; i < se->expressionList()->size(); i++) {
      ss << dumpAstImpl(se->expressionList()->get(i), depth);
      if (i < (int)se->expressionList()->size() - 1) {
        ss << ",";
      }
    }
    return ss.str();
  }
  case AstType::EXPR_STMT:
    return DS +
           dumpAstImpl(DC(AstExpressionStatement, node)->expression(), depth) +
           ";\n";
  case AstType::COMP_STMT: {
    AstCompoundStatement *cs = DC(AstCompoundStatement, node);
    std::stringstream ss;
    ss << DS << "{";
    if (cs->statementList() && cs->statementList()->size() > 0) {
      ss << "\n";
      for (int i = 0; i < cs->statementList()->size(); i++) {
        ss << dumpAstImpl(cs->statementList()->get(i), depth + 1);
      }
      ss << DS;
    }
    ss << "}\n\n";
    return ss.str();
  }
  case AstType::IF_STMT: {
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
  case AstType::WHILE_STMT: {
    AstWhileStatement *ws = DC(AstWhileStatement, node);
    std::stringstream ss;
    ss << DS << "while (";
    ss << dumpAstImpl(ws->condition(), depth) << ")\n"
       << dumpAstImpl(ws->statement(), depth + 1);
    return ss.str();
  }
  case AstType::FOR_STMT: {
    AstForStatement *fs = DC(AstForStatement, node);
    std::stringstream ss;
    ss << DS << "for (" << dumpAstImpl(fs->initial(), depth)
       << dumpAstImpl(fs->condition(), depth)
       << dumpAstImpl(fs->post(), depth) + ") "
       << dumpAstImpl(fs->statement(), depth) << "\n";
    return ss.str();
  }
  case AstType::CONT_STMT:
    return DS + DC(AstContinueStatement, node)->name() + ";\n";
  case AstType::BRK_STMT:
    return DS + DC(AstBreakStatement, node)->name() + ";\n";
  case AstType::RET_STMT:
    return DS + DC(AstReturnStatement, node)->name() + " " +
           dumpAstImpl(DC(AstReturnStatement, node)->expression(), depth) +
           ";\n";
  case AstType::EMP_STMT:
    return DS + DC(AstEmptyStatement, node)->name() + ";\n";
  case AstType::VAR_DECL: {
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
  case AstType::VAR_ASSIGN_DECL: {
    AstVariableAssignmentDeclaration *vad =
        DC(AstVariableAssignmentDeclaration, node);
    return vad->identifier() + " = " + dumpAstImpl(vad->expression(), depth);
  }
  case AstType::FUNC_DECL: {
    AstFunctionDeclaration *fd = DC(AstFunctionDeclaration, node);
    std::stringstream ss;
    ss << DS << fd->name() << "(";
    if (fd->argumentList() && fd->argumentList()->size() > 0) {
      for (int i = 0; i < fd->argumentList()->size(); i++) {
        ss << fd->argumentList()->get(i)->name();
        if (i < fd->argumentList()->size() - 1) {
          ss << ",";
        }
      }
    }
    ss << ") =>\n" << dumpAstImpl(fd->statement(), depth);
    return ss.str();
  }
  case AstType::DECL_LIST: {
    AstDeclarationList *dl = DC(AstDeclarationList, node);
    std::stringstream ss;
    ss << DS << dl->name() << "{\n";
    for (int i = 0; i < dl->size(); i++) {
      ss << dumpAstImpl(dl->get(i), depth);
    }
    ss << "}\n";
    return ss.str();
  }
  case AstType::TUNIT: {
    AstTranslateUnit *p = DC(AstTranslateUnit, node);
    std::stringstream ss;
    ss << DS << p->name() << " {\n";
    for (int i = 0; i < p->size(); i++) {
      ss << dumpAstImpl(p->get(i), depth + 1);
    }
    ss << "}\n";
    return ss.str();
  }
  // case AstType::EXPR_LIST: {
  // return DC(AstExpressionList, node)->name();
  //}
  default: {
    LOG_ASSERT(false, "invalid node:{}", node->toString());
  }
  }
}

std::string dumpAst(Ast *node) { return dumpAstImpl(node, 0); }

#define IS_SYMTAB(x)                                                           \
  ((x)->type() == SYM_FUNC || (x)->type() == SYM_CLASS ||                      \
   (x)->type() == SYM_GLOBAL || (x)->type() == SYM_LOCAL)

static std::string dumpSymbolImpl(Symbol *sym, int depth) {
  if (!sym)
    return "null";
  switch (sym->type()) {
  case SYM_VAR:
    return "var " + DC(VariableSymbol, sym)->name();
  case SYM_FUNC: {
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
  case SYM_FUNC_ARG:
    return DC(FunctionArgumentSymbol, sym)->name();
  case SYM_CLASS: {
    ClassSymbol *cs = DC(ClassSymbol, sym);
    std::stringstream ss;
    ss << DS << "class " << cs->name() << " {";
    if (!cs->empty()) {
      for (auto i = cs->begin(); i != cs->end(); i++) {
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
  case SYM_LOCAL: {
    LocalSymtab *ls = DC(LocalSymtab, sym);
    std::stringstream ss;
    ss << "local " << ls->name() << " {";
    if (!ls->empty()) {
      for (auto i = ls->begin(); i != ls->end(); i++) {
        ss << "\n"
           << DS << i->first << " => " << dumpSymbolImpl(i->second, depth + 1);
      }
      ss << "\n" << std::string(depth - 1, ' ');
    }
    ss << "}\n";
    return ss.str();
  }
  case SYM_GLOBAL: {
    GlobalSymtab *gs = DC(GlobalSymtab, sym);
    std::stringstream ss;
    ss << DS << gs->name() << " {";
    if (!gs->empty()) {
      for (auto i = gs->begin(); i != gs->end(); i++) {
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
  ((x)->type() == TY_FUNC || (x)->type() == TY_CLASS ||                        \
   (x)->type() == TY_LOCAL || (x)->type() == TY_GLOBAL)

static std::string dumpTypeImpl(Type *ty, int depth) {
  if (!ty)
    return "null";
  switch (ty->type()) {
  case TY_BUILTIN:
    return DC(BuiltinType, ty)->name();
  case TY_FUNC: {
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
  case TY_CLASS: {
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
  case TY_LOCAL: {
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
  case TY_GLOBAL: {
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

#undef DS
#undef DC

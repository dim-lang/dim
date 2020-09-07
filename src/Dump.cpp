// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "File.h"
#include "Log.h"
#include "Token.h"
#include "container/LinkedHashMap.hpp"
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <string>

#define INDENT std::string(indent, ' ')
#define SP_CAST(x, y) std::static_pointer_cast<x>(y)
#define SP_CASTC(x, y) dynamic_cast<const x *>(y)

static std::string dumpAstImpl(std::shared_ptr<Ast> node, int indent) {
  if (!node)
    return "nil";
  if (Ast::isLiteral(node)) {
    return node->name().raw();
  }
  if (Ast::isId(node)) {
    return node->name().raw();
  }
  switch (node->category()) {
  case AstCategory::Throw: {
    std::shared_ptr<A_Throw> e = SP_CAST(A_Throw, node);
    std::stringstream ss;
    ss << "throw " << e->expr->name().raw();
    return ss.str();
  }
  case AstCategory::Return: {
    std::shared_ptr<A_Return> e = SP_CAST(A_Return, node);
    std::stringstream ss;
    ss << "return";
    if (e->expr) {
      ss << " " << e->expr->name().raw();
    }
    return ss.str();
  }
  case AstCategory::Break:
    return "break";
  case AstCategory::Continue:
    return "continue";
  case AstCategory::Assign: {
    std::shared_ptr<A_Assign> e = SP_CAST(A_Assign, node);
    return dumpAstImpl(e->assignee, indent) + "=" +
           dumpAstImpl(e->assignor, indent);
  }
  case AstCategory::PostfixExpr: {
    std::shared_ptr<A_PostfixExpr> e = SP_CAST(A_PostfixExpr, node);
    return dumpAstImpl(e->expr, indent) + tokenName(e->postfixOp);
  }
  case AstCategory::PrefixExpr: {
    std::shared_ptr<A_PrefixExpr> e = SP_CAST(A_PrefixExpr, node);
    return tokenName(e->prefixOp) + dumpAstImpl(e->expr, indent);
  }
  case AstCategory::InfixExpr: {
    std::shared_ptr<A_InfixExpr> e = SP_CAST(A_InfixExpr, node);
    return dumpAstImpl(e->left, indent) + tokenName(e->infixOp) +
           dumpAstImpl(e->right, indent);
  }
  case AstCategory::Call: {
    std::shared_ptr<A_Call> e = SP_CAST(A_Call, node);
    std::shared_ptr<A_VarId> id = SP_CAST(A_VarId, e->id);
    std::stringstream ss;
    ss << id->name().raw() << "(";
    if (e->args) {
      ss << dumpAstImpl(e->args, indent);
    }
    ss << ")";
    return ss.str();
  }
  case AstCategory::Exprs: {
    std::shared_ptr<A_Exprs> e = SP_CAST(A_Exprs, node);
    std::stringstream ss;
    ss << dumpAstImpl(e->expr, indent) << "," << dumpAstImpl(e->next, indent);
    return ss.str();
  }
  case AstCategory::If: {
    std::shared_ptr<A_If> e = SP_CAST(A_If, node);
    LOG_ASSERT(e->condition, "e->condition must not null");
    LOG_ASSERT(e->thenp, "e->thenp must not null");
    std::stringstream ss;
    ss << "if(" << dumpAstImpl(e->condition, indent) << ") "
       << dumpAstImpl(e->thenp, indent);
    if (e->elsep) {
      ss << " else " << dumpAstImpl(e->elsep, indent);
    }
    return ss.str();
  }
  case AstCategory::Loop: {
    std::shared_ptr<A_Loop> e = SP_CAST(A_Loop, node);
    std::stringstream ss;
    ss << "loop(" << dumpAstImpl(e->condition, indent) << ")"
       << dumpAstImpl(e->body, indent);
    return ss.str();
  }
  case AstCategory::LoopCondition: {
    std::shared_ptr<A_LoopCondition> e = SP_CAST(A_LoopCondition, node);
    std::stringstream ss;
    ss << dumpAstImpl(e->init, indent) << ";"
       << dumpAstImpl(e->condition, indent) << ";"
       << dumpAstImpl(e->update, indent);
    return ss.str();
  }
  case AstCategory::LoopEnumerator: {
    std::shared_ptr<A_LoopEnumerator> e = SP_CAST(A_LoopEnumerator, node);
    std::stringstream ss;
    ss << e->id->name().raw() << " <- " << dumpAstImpl(e->expr, indent);
    return ss.str();
  }
  case AstCategory::Try: {
    std::shared_ptr<A_Try> e = SP_CAST(A_Try, node);
    LOG_ASSERT(e->tryp, "e->tryp must not null");
    LOG_ASSERT(e->catchp, "e->catchp must not null");
    std::stringstream ss;
    ss << "try " << dumpAstImpl(e->tryp, indent) << " catch "
       << dumpAstImpl(e->catchp, indent);
    if (e->finallyp) {
      ss << " finally " << dumpAstImpl(e->finallyp, indent);
    }
    return ss.str();
  }
  case AstCategory::Block: {
    std::shared_ptr<A_Block> e = SP_CAST(A_Block, node);
    std::stringstream ss;
    ss << "{";
    if (e->blockStats) {
      ss << "\n";
      dumpAstImpl(e->blockStats, indent + 1);
    }
    ss << "}\n";
    return ss.str();
  }
  case AstCategory::BlockStats: {
    std::shared_ptr<A_BlockStats> e = SP_CAST(A_BlockStats, node);
    std::stringstream ss;
    ss << INDENT;
    if (e->blockStat) {
      ss << dumpAstImpl(e->blockStat, indent);
    } else {
      ss << "nil";
    }
    ss << "\n";
    ss << dumpAstImpl(e->next, indent);
    return ss.str();
  }
  case AstCategory::PlainType: {
    std::shared_ptr<A_PlainType> e = SP_CAST(A_PlainType, node);
    return tokenName(e->token);
  }
  case AstCategory::FuncDef: {
    std::shared_ptr<A_FuncDef> e = SP_CAST(A_FuncDef, node);
    std::stringstream ss;
    ss << "def " << dumpAstImpl(e->funcSign, indent) << ":"
       << dumpAstImpl(e->resultType, indent) << dumpAstImpl(e->body, indent);
    return ss.str();
  }
  case AstCategory::FuncSign: {
    std::shared_ptr<A_FuncSign> e = SP_CAST(A_FuncSign, node);
    std::stringstream ss;
    ss << dumpAstImpl(e->id, indent) << "(";
    if (e->params) {
      ss << dumpAstImpl(e->params, indent);
    }
    ss << ")";
    return ss.str();
  }
  case AstCategory::Params: {
    std::shared_ptr<A_Params> e = SP_CAST(A_Params, node);
    std::stringstream ss;
    ss << dumpAstImpl(e->param, indent);
    if (e->next) {
      ss << "," << dumpAstImpl(e->next, indent);
    }
    return ss.str();
  }
  case AstCategory::Param: {
    std::shared_ptr<A_Param> e = SP_CAST(A_Param, node);
    std::stringstream ss;
    ss << dumpAstImpl(e->id, indent) << ":" << dumpAstImpl(e->type, indent);
    return ss.str();
  }
  case AstCategory::VarDef: {
    std::shared_ptr<A_VarDef> e = SP_CAST(A_VarDef, node);
    std::stringstream ss;
    ss << "def " << dumpAstImpl(e->id, indent) << ":"
       << dumpAstImpl(e->type, indent) << "=" << dumpAstImpl(e->expr, indent);
    return ss.str();
  }
  case AstCategory::TopStats: {
    std::shared_ptr<A_TopStats> e = SP_CAST(A_TopStats, node);
    std::stringstream ss;
    ss << INDENT << dumpAstImpl(e->topStat, indent) << "\n";
    ss << dumpAstImpl(e->next, indent);
    return ss.str();
  }
  case AstCategory::CompileUnit: {
    std::shared_ptr<A_CompileUnit> e = SP_CAST(A_CompileUnit, node);
    return dumpAstImpl(e->topStats, indent);
  }
  default: {
    LOG_ASSERT(false, "invalid node:{}", node->toString());
  }
  }
}

std::string dumpAst(std::shared_ptr<Ast> node) { return dumpAstImpl(node, 0); }

/* #define IS_SCOPE(x) \ */
/*   ((x)->symbol->type() == (+S_ty::Function) || \ */
/*    (x)->symbol->type() == (+S_ty::Class) || \ */
/*    (x)->symbol->type() == (+S_ty::Global) || \ */
/*    (x)->symbol->type() == (+S_ty::Local)) */

/* static std::string dumpScopeImpl(const ScopeNode *snode, int indent) { */
/*   if (!snode->symbol) */
/*     return "null"; */
/*   LOG_ASSERT(snode->symbol, "snode.symbol must not be null"); */
/*   switch (snode->symbol->type()) { */
/*   case S_ty::Variable: */
/*     return std::string("var ") + snode->symbol->name() + ":" + */
/*            snode->type->name(); */
/*   case S_ty::Function: { */
/*     const FunctionSymbol *funcsym = SP_CASTC(FunctionSymbol, snode->symbol);
 */
/*     std::stringstream ss; */
/*     ss << "func " << funcsym->name() << " {"; */
/*     if (!funcsym->empty()) { */
/*       for (FunctionSymbol::CIterator i = funcsym->begin(); i !=
 * funcsym->end(); */
/*            i++) { */
/*         ss << "\n" << INDENT << i->first; */
/*         if (IS_SCOPE(i->second)) { */
/*           ss << " => " << dumpScopeImpl(i->second, indent + 1); */
/*         } */
/*       } */
/*       ss << "\n" << std::string(indent - 1, ' '); */
/*     } */
/*     ss << "}\n"; */
/*     return ss.str(); */
/*   } */
/*   case S_ty::FunctionArgument: */
/*     return std::string("var ") + snode->symbol->name() + ":" + */
/*            snode->type->name(); */
/*   case S_ty::Class: { */
/*     const ClassSymbol *clssym = SP_CASTC(ClassSymbol, snode->symbol); */
/*     std::stringstream ss; */
/*     ss << INDENT << "class " << clssym->name() << " {"; */
/*     if (!clssym->empty()) { */
/*       for (ClassSymbol::CIterator i = clssym->begin(); i != clssym->end(); */
/*            i++) { */
/*         ss << "\n" << INDENT << i->first; */
/*         if (IS_SCOPE(i->second)) { */
/*           ss << " => " << dumpScopeImpl(i->second, indent + 1); */
/*         } */
/*       } */
/*       ss << "\n" << std::string(indent - 1, ' '); */
/*     } */
/*     ss << "}\n"; */
/*     return ss.str(); */
/*   } */
/*   case S_ty::Local: { */
/*     const LocalScope *locsym = SP_CASTC(LocalScope, snode->symbol); */
/*     std::stringstream ss; */
/*     ss << "local " << locsym->name() << " {"; */
/*     if (!locsym->empty()) { */
/*       for (LocalScope::CIterator i = locsym->begin(); i != locsym->end();
 * i++) { */
/*         ss << "\n" */
/*            << INDENT << i->first << " => " << dumpScopeImpl(i->second, indent
 * + 1); */
/*       } */
/*       ss << "\n" << std::string(indent - 1, ' '); */
/*     } */
/*     ss << "}\n"; */
/*     return ss.str(); */
/*   } */
/*   case S_ty::Global: { */
/*     const GlobalScope *glbsym = SP_CASTC(GlobalScope, snode->symbol); */
/*     std::stringstream ss; */
/*     ss << INDENT << glbsym->name() << " {"; */
/*     if (!glbsym->empty()) { */
/*       for (GlobalScope::CIterator i = glbsym->begin(); i != glbsym->end(); */
/*            i++) { */
/*         ss << "\n" << INDENT << i->first; */
/*         if (IS_SCOPE(i->second)) { */
/*           ss << " => " << dumpScopeImpl(i->second, indent + 1); */
/*         } */
/*       } */
/*       ss << "\n" << INDENT; */
/*     } */
/*     ss << "}\n"; */
/*     return ss.str(); */
/*   } */
/*   default: */
/*     LOG_ASSERT(false, "invalid symbol: {} {}", snode->symbol->name(), */
/*                snode->symbol->type()._to_string()); */
/*   } */
/* } */

/* std::string dumpScope(ScopeNode *snode) { return dumpScopeImpl(snode, 0); }
 */

std::string dumpSource(const std::string &fileName, const Location &location) {
  std::stringstream ss;
  ss << fmt::format("{}[{}]\n", fileName, location.toString());
  FileReader fileReader(fileName);
  FileReader::LineIterator lineIterator = fileReader.lineIterator();
  for (int i = 1; lineIterator.hasNext() && i <= location.end.line + 2; i++) {
    std::string oneline = lineIterator.next();
    if (i < location.begin.line - 1) {
      continue;
    }
    if (i > location.end.line + 1) {
      break;
    }
    ss << oneline;
    if (i >= location.begin.line && i <= location.end.line) {
      int hc = [&oneline]() {
        for (int i = 0; i < (int)oneline.length(); i++) {
          if (!std::isspace(oneline[i])) {
            return i;
          }
        }
        return (int)oneline.length();
      }();
      int tc = [&oneline]() {
        for (int i = (int)oneline.length() - 1; i >= 0; i--) {
          if (!std::isspace(oneline[i])) {
            return i;
          }
        }
        return -1;
      }();
      if (hc < (int)oneline.length() && tc >= 0 && hc <= tc) {
        ss << std::string(hc, ' ');
        ss << std::string(tc - hc + 1, '^');
      }
      ss << "\n";
    }
  }
  return ss.str();
}

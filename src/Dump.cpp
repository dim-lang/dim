// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "File.h"
#include "Log.h"
#include "TokenName.h"
#include "container/LinkedHashMap.hpp"
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <string>

#define INDENT std::string(indent, ' ')
#define DC(x, y) std::static_pointer_cast<x>(y)
#define DCC(x, y) dynamic_cast<const x *>(y)

static std::string dumpAstImpl(std::shared_ptr<Ast> node, int indent) {
  if (!node)
    return "nil";
  if (Ast::isLiteral(node)) {
    return node->name().raw();
  }
  if (Ast::isId(node)) {
    return node->name().raw();
  }
  switch (node->type()) {
  case AstCategory::Throw: {
    std::shared_ptr<A_Throw> e = DC(A_Throw, node);
    return (std::stringstream() << "throw " << e->expr->name().raw()).str();
  }
  case AstCategory::Return: {
    std::shared_ptr<A_Return> e = DC(A_Return, node);
    return e->expr ? (std::stringstream() << "return " << e->expr->name().raw())
                         .str()
                   : "return";
  }
  case AstCategory::Break:
    return "break";
  case AstCategory::Continue:
    return "continue";
  case AstCategory::Assign: {
    std::shared_ptr<A_Assign> e = DC(A_Assign, node);
    return dumpAstImpl(e->assignee, indent) + "=" +
           dumpAstImpl(e->assignor, indent);
  }
  case AstCategory::PostfixExpr: {
    std::shared_ptr<A_PostfixExpr> e = DC(A_PostfixExpr, node);
    return dumpAstImpl(e->expr, indent) + tokenName(e->postfixOp);
  }
  case AstCategory::PrefixExpr: {
    std::shared_ptr<A_PrefixExpr> e = DC(A_PrefixExpr, node);
    return tokenName(e->prefixOp) + dumpAstImpl(e->expr, indent);
  }
  case AstCategory::InfixExpr: {
    std::shared_ptr<A_InfixExpr> e = DC(A_InfixExpr, node);
    return dumpAstImpl(e->left, indent) + tokenName(e->infixOp) +
           dumpAstImpl(e->right, indent);
  }
  case AstCategory::Call: {
    std::shared_ptr<A_Call> e = DC(A_Call, node);
    std::shared_ptr<A_VarId> id = DC(A_VarId, e->id);
    return e->args
               ? (std::stringstream() << id->name().raw() << "("
                                      << dumpAstImpl(e->args, indent) << ")")
                     .str()
               : (std::stringstream() << id->name().raw() << "()").str();
  }
  case AstCategory::Exprs: {
    std::shared_ptr<A_Exprs> e = DC(A_Exprs, node);
    return (std::stringstream() << dumpAstImpl(e->expr, indent) << ","
                                << dumpAstImpl(e->next, indent))
        .str();
  }
  case AstCategory::If: {
    std::shared_ptr<A_If> e = DC(A_If, node);
    LOG_ASSERT(e->condition, "e->condition must not null");
    LOG_ASSERT(e->thenp, "e->thenp must not null");
    std::stringstream ss;
    ss << "if(" << dumpAstImpl(e->condition) << ") " << dumpAstImpl(e->thenp);
    if (e->elsep) {
      ss << " else " << dumpAstImpl(e->elsep);
    }
    return ss.str();
  }
  case AstCategory::Loop: {
    std::shared_ptr<A_Loop> e = DC(A_Loop, node);
    return (std::stringstream() << "loop(" << dumpAstImpl(e->condition, indent)
                                << ")" << dumpAstImpl(e->body, indent))
        .str();
  }
  case AstCategory::LoopCondition: {
    std::shared_ptr<A_LoopCondition> e = DC(A_LoopCondition, node);
    return (std::stringstream()
            << dumpAstImpl(e->expr) << ", doOnceAtFirst:" << e->doOnceAtFirst)
        .str();
  }
  case AstCategory::LoopEnumerator: {
    std::shared_ptr<A_LoopEnumerator> e = DC(A_LoopEnumerator, node);
    return (std::stringstream()
            << e->id->name().raw() << " <- " << dumpAstImpl(e->expr))
        .str();
  }
  case AstCategory::Try: {
    std::shared_ptr<A_Try> e = DC(A_Try, node);
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
    AstCompoundStatement *e = DC(AstCompoundStatement, node);
    std::stringstream ss;
    ss << INDENT << "{";
    if (e->statementList() && e->statementList()->size() > 0) {
      ss << "\n";
      for (int i = 0; i < e->statementList()->size(); i++) {
        ss << dumpAstImpl(e->statementList()->get(i), indent + 1);
      }
      ss << INDENT;
    }
    ss << "}\n\n";
    return ss.str();
  }
  case AstCategory::ContinueStatement:
    return INDENT + DC(AstContinueStatement, node)->name() + ";\n";
  case AstCategory::BreakStatement:
    return INDENT + DC(AstBreakStatement, node)->name() + ";\n";
  case AstCategory::ReturnStatement:
    return INDENT + DC(AstReturnStatement, node)->name() + " " +
           dumpAstImpl(DC(AstReturnStatement, node)->expression(), indent) +
           ";\n";
  case AstCategory::EmptyStatement:
    return INDENT + DC(AstEmptyStatement, node)->name() + ";\n";
  case AstCategory::VariableDefinition: {
    AstVariableDefinition *e = DC(AstVariableDefinition, node);
    std::stringstream ss;
    ss << INDENT << e->name() << ": ";
    for (int i = 0; i < e->definitionList()->size(); i++) {
      ss << dumpAstImpl(e->definitionList()->get(i), indent);
      if (i < e->definitionList()->size() - 1) {
        ss << ", ";
      }
    }
    ss << ";\n";
    return ss.str();
  }
  case AstCategory::VariableInitialDefinition: {
    AstVariableInitialDefinition *e = DC(AstVariableInitialDefinition, node);
    return e->identifier() + " = " + dumpAstImpl(e->expression(), indent);
  }
  case AstCategory::FunctionDefinition: {
    AstFunctionDefinition *e = DC(AstFunctionDefinition, node);
    AstFunctionSignatureDefinition *fsd = e->signature();
    std::stringstream ss;
    ss << INDENT << e->name() << "(";
    if (fsd->argumentList() && fsd->argumentList()->size() > 0) {
      for (int i = 0; i < fsd->argumentList()->size(); i++) {
        ss << fsd->argumentList()->get(i)->name();
        if (i < fsd->argumentList()->size() - 1) {
          ss << ",";
        }
      }
    }
    ss << ") =>\n" << dumpAstImpl(e->statement(), indent);
    return ss.str();
  }
  case AstCategory::DefinitionList: {
    AstDefinitionList *e = DC(AstDefinitionList, node);
    std::stringstream ss;
    ss << INDENT << e->name() << "{\n";
    for (int i = 0; i < e->size(); i++) {
      ss << dumpAstImpl(e->get(i), indent);
    }
    ss << "}\n";
    return ss.str();
  }
  case AstCategory::TranslateUnit: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    std::stringstream ss;
    ss << INDENT << e->name() << " {\n";
    for (int i = 0; i < e->size(); i++) {
      ss << dumpAstImpl(e->get(i), indent + 1);
    }
    ss << "}\n";
    return ss.str();
  }
    // case AstCategory::ExpressionList: {
    // return DC(AstExpressionList, node)->name();
    //}
  case AstCategory::VoidExpression: {
    AstVoidExpression *e = DC(AstVoidExpression, node);
    std::stringstream ss;
    ss << INDENT << e->name();
    return ss.str();
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
/*     const FunctionSymbol *funcsym = DCC(FunctionSymbol, snode->symbol); */
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
/*     const ClassSymbol *clssym = DCC(ClassSymbol, snode->symbol); */
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
/*     const LocalScope *locsym = DCC(LocalScope, snode->symbol); */
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
/*     const GlobalScope *glbsym = DCC(GlobalScope, snode->symbol); */
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

/* static int whitespaceHeadCount(const std::string &line) { */
/*   for (int i = 0; i < (int)line.length(); i++) { */
/*     if (!std::isspace(line[i])) { */
/*       return i; */
/*     } */
/*   } */
/*   return line.length(); */
/* } */

/* static int whitespaceTailCount(const std::string &line) { */
/*   for (int i = (int)line.length() - 1; i >= 0; i--) { */
/*     if (!std::isspace(line[i])) { */
/*       return i; */
/*     } */
/*   } */
/*   return -1; */
/* } */

std::string dumpSource(const std::string &fileName,
                       const yy::location &location) {
  std::stringstream ss;
  ss << fmt::format("{}[{}]\n", fileName,
                    (std::stringstream() << location).str());
  FileReader fileReader(fileName);
  FileReader::LineIterator lineIterator = fileReader.lineIterator();
  for (int i = 1; lineIterator.hasNext() && i <= location.end.line + 2; i++) {
    std::string line = lineIterator.next();
    if (i < location.begin.line - 1) {
      continue;
    }
    if (i > location.end.line + 1) {
      break;
    }
    ss << line;
    if (i >= location.begin.line && i <= location.end.line) {
      int hc = (std::find_if(line.begin(), line.end(),
                             [](const char &c) { return !std::isspace(c); })) -
               line.begin();
      int tc = (std::find_if(line.rbegin(), line.rend(),
                             [](const char &c) { return !std::isspace(c); })) -
               line.begin();
      if (hc < line.length() && tc >= 0 && hc <= tc) {
        ss << std::string(hc, ' ');
        ss << std::string(tc - hc + 1, '^');
      }
      ss << "\n";
    }
  }
  return ss.str();
}

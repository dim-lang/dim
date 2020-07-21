// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Exception.h"
#include "File.h"
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
#define DCC(x, y) dynamic_cast<const x *>(y)

static std::string dumpAstImpl(Ast *node, int depth) {
  if (!node)
    return "null";
  switch (node->type()) {
  case A_ty::Identifier:
  case A_ty::Int8Literal:
  case A_ty::UInt8Literal:
  case A_ty::Int16Literal:
  case A_ty::UInt16Literal:
  case A_ty::Int32Literal:
  case A_ty::UInt32Literal:
  case A_ty::Int64Literal:
  case A_ty::UInt64Literal:
  case A_ty::Float32Literal:
  case A_ty::Float64Literal:
  case A_ty::StringLiteral:
  case A_ty::BooleanLiteral:
    return node->name();
  case A_ty::CallExpression: {
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
  case A_ty::UnaryExpression:
    return tokenName(DC(AstUnaryExpression, node)->token()) + " " +
           dumpAstImpl(DC(AstUnaryExpression, node)->expression(), depth);
  case A_ty::BinaryExpression:
    return dumpAstImpl(DC(AstBinaryExpression, node)->left(), depth) + " " +
           tokenName(DC(AstBinaryExpression, node)->token()) + " " +
           dumpAstImpl(DC(AstBinaryExpression, node)->right(), depth);
  case A_ty::ConditionalExpression:
    return dumpAstImpl(DC(AstConditionalExpression, node)->condition(), depth) +
           "?" +
           dumpAstImpl(DC(AstConditionalExpression, node)->thens(), depth) +
           ":" +
           dumpAstImpl(DC(AstConditionalExpression, node)->elses(), depth);
  case A_ty::AssignmentExpression:
    return dumpAstImpl(DC(AstAssignmentExpression, node)->variable(), depth) +
           "=" + dumpAstImpl(DC(AstAssignmentExpression, node)->value(), depth);
  case A_ty::SequelExpression: {
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
  case A_ty::ExpressionStatement:
    return DS +
           dumpAstImpl(DC(AstExpressionStatement, node)->expression(), depth) +
           ";\n";
  case A_ty::CompoundStatement: {
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
  case A_ty::IfStatement: {
    AstIfStatement *e = DC(AstIfStatement, node);
    std::stringstream ss;
    ss << DS << "if (";
    EX_ASSERT(e->thens(), "e->thens() must not null");
    ss << dumpAstImpl(e->condition(), depth) << ") \n"
       << dumpAstImpl(e->thens(), depth + 1);
    if (e->elses()) {
      ss << DS << "else\n" << dumpAstImpl(e->elses(), depth + 1);
    }
    return ss.str();
  }
  case A_ty::WhileStatement: {
    AstWhileStatement *e = DC(AstWhileStatement, node);
    std::stringstream ss;
    ss << DS << "while (";
    ss << dumpAstImpl(e->condition(), depth) << ")\n"
       << dumpAstImpl(e->statement(), depth + 1);
    return ss.str();
  }
  case A_ty::ForStatement: {
    AstForStatement *e = DC(AstForStatement, node);
    std::stringstream ss;
    ss << DS << "for (" << dumpAstImpl(e->start(), depth)
       << dumpAstImpl(e->step(), depth) << dumpAstImpl(e->end(), depth) + ") "
       << dumpAstImpl(e->statement(), depth) << "\n";
    return ss.str();
  }
  case A_ty::ContinueStatement:
    return DS + DC(AstContinueStatement, node)->name() + ";\n";
  case A_ty::BreakStatement:
    return DS + DC(AstBreakStatement, node)->name() + ";\n";
  case A_ty::ReturnStatement:
    return DS + DC(AstReturnStatement, node)->name() + " " +
           dumpAstImpl(DC(AstReturnStatement, node)->expression(), depth) +
           ";\n";
  case A_ty::EmptyStatement:
    return DS + DC(AstEmptyStatement, node)->name() + ";\n";
  case A_ty::VariableDefinition: {
    AstVariableDefinition *e = DC(AstVariableDefinition, node);
    std::stringstream ss;
    ss << DS << e->name() << ": ";
    for (int i = 0; i < e->definitionList()->size(); i++) {
      ss << dumpAstImpl(e->definitionList()->get(i), depth);
      if (i < e->definitionList()->size() - 1) {
        ss << ", ";
      }
    }
    ss << ";\n";
    return ss.str();
  }
  case A_ty::VariableInitialDefinition: {
    AstVariableInitialDefinition *e = DC(AstVariableInitialDefinition, node);
    return e->identifier() + " = " + dumpAstImpl(e->expression(), depth);
  }
  case A_ty::FunctionDefinition: {
    AstFunctionDefinition *e = DC(AstFunctionDefinition, node);
    AstFunctionSignatureDefinition *fsd = e->signature();
    std::stringstream ss;
    ss << DS << e->name() << "(";
    if (fsd->argumentList() && fsd->argumentList()->size() > 0) {
      for (int i = 0; i < fsd->argumentList()->size(); i++) {
        ss << fsd->argumentList()->get(i)->name();
        if (i < fsd->argumentList()->size() - 1) {
          ss << ",";
        }
      }
    }
    ss << ") =>\n" << dumpAstImpl(e->statement(), depth);
    return ss.str();
  }
  case A_ty::DefinitionList: {
    AstDefinitionList *e = DC(AstDefinitionList, node);
    std::stringstream ss;
    ss << DS << e->name() << "{\n";
    for (int i = 0; i < e->size(); i++) {
      ss << dumpAstImpl(e->get(i), depth);
    }
    ss << "}\n";
    return ss.str();
  }
  case A_ty::TranslateUnit: {
    AstTranslateUnit *e = DC(AstTranslateUnit, node);
    std::stringstream ss;
    ss << DS << e->name() << " {\n";
    for (int i = 0; i < e->size(); i++) {
      ss << dumpAstImpl(e->get(i), depth + 1);
    }
    ss << "}\n";
    return ss.str();
  }
    // case A_ty::ExpressionList: {
    // return DC(AstExpressionList, node)->name();
    //}
  case A_ty::VoidExpression: {
    AstVoidExpression *e = DC(AstVoidExpression, node);
    std::stringstream ss;
    ss << DS << e->name();
    return ss.str();
  }
  default: {
    EX_ASSERT(false, "invalid node:{}", node->toString());
  }
  }
}

std::string dumpAst(Ast *node) { return dumpAstImpl(node, 0); }

#define IS_SCOPE(x)                                                            \
  ((x)->symbol->type() == (+S_ty::Function) ||                                 \
   (x)->symbol->type() == (+S_ty::Class) ||                                    \
   (x)->symbol->type() == (+S_ty::Global) ||                                   \
   (x)->symbol->type() == (+S_ty::Local))

static std::string dumpScopeImpl(const ScopeNode *snode, int depth) {
  if (!snode->symbol)
    return "null";
  EX_ASSERT(snode->symbol, "snode.symbol must not be null");
  switch (snode->symbol->type()) {
  case S_ty::Variable:
    return std::string("var ") + snode->symbol->name() + ":" +
           snode->type->name();
  case S_ty::Function: {
    const FunctionSymbol *funcsym = DCC(FunctionSymbol, snode->symbol);
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
  case S_ty::FunctionArgument:
    return std::string("var ") + snode->symbol->name() + ":" +
           snode->type->name();
  case S_ty::Class: {
    const ClassSymbol *clssym = DCC(ClassSymbol, snode->symbol);
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
  case S_ty::Local: {
    const LocalScope *locsym = DCC(LocalScope, snode->symbol);
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
  case S_ty::Global: {
    const GlobalScope *glbsym = DCC(GlobalScope, snode->symbol);
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
    EX_ASSERT(false, "invalid symbol: {} {}", snode->symbol->name(),
              snode->symbol->type()._to_string());
  }
}

std::string dumpScope(ScopeNode *snode) { return dumpScopeImpl(snode, 0); }

static int whitespaceHeadCount(const std::string &line) {
  for (int i = 0; i < (int)line.length(); i++) {
    if (!std::isspace(line[i])) {
      return i;
    }
  }
  return line.length();
}

static int whitespaceTailCount(const std::string &line) {
  for (int i = (int)line.length() - 1; i >= 0; i--) {
    if (!std::isspace(line[i])) {
      return i;
    }
  }
  return -1;
}

std::string dumpSource(const std::string &fileName, const Position &position) {
  std::stringstream ss;
  ss << fmt::format("{}[{}]\n", fileName, position.toString());
  FileReader fileReader(fileName);
  FileReader::LineIterator lineIterator = fileReader.lineIterator();
  for (int i = 1; lineIterator.hasNext() && i <= position.lastLine + 2; i++) {
    std::string line = lineIterator.next();
    if (i < position.firstLine - 1) {
      continue;
    }
    if (i > position.lastLine + 1) {
      break;
    }
    ss << line;
    if (i >= position.firstLine && i <= position.lastLine) {
      int hc = whitespaceHeadCount(line);
      int tc = whitespaceTailCount(line);
      if (hc < line.length() && tc >= 0 && hc <= tc) {
        ss << std::string(hc, ' ');
        ss << std::string(tc - hc + 1, '^');
      }
      ss << "\n";
    }
  }
  return ss.str();
}

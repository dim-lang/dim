// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Log.h"
#include "TokenName.h"
#include <cstdio>
#include <string>

static std::string dumpForAst(Ast *node) {
  if (!node)
    return "null";
  switch (node->type()) {
  case A_IDENTIFIER_CONSTANT:
    return node->name();
  case A_I8_CONSTANT:
  case A_U8_CONSTANT:
  case A_I16_CONSTANT:
  case A_U16_CONSTANT:
  case A_I32_CONSTANT:
  case A_U32_CONSTANT:
  case A_I64_CONSTANT:
  case A_U64_CONSTANT:
  case A_F32_CONSTANT:
  case A_F64_CONSTANT:
  case A_STRING_CONSTANT:
  case A_BOOLEAN_CONSTANT:
    return node->name();
  case A_CALL_EXPRESSION:
    return node->name() + "(" +
           dumpForAst(dynamic_cast<AstCallExpression *>(node)->argumentList()) +
           ")";
  case A_UNARY_EXPRESSION:
    return tokenName(dynamic_cast<AstUnaryExpression *>(node)->token()) + " " +
           dumpForAst(dynamic_cast<AstUnaryExpression *>(node)->expression());
  case A_BINARY_EXPRESSION:
    return dumpForAst(dynamic_cast<AstBinaryExpression *>(node)->left()) + " " +
           tokenName(dynamic_cast<AstBinaryExpression *>(node)->token()) + " " +
           dumpForAst(dynamic_cast<AstBinaryExpression *>(node)->right());
  case A_CONDITIONAL_EXPRESSION:
    return dumpForAst(
               dynamic_cast<AstConditionalExpression *>(node)->condition()) +
           "?" +
           dumpForAst(dynamic_cast<AstConditionalExpression *>(node)->hit()) +
           ":" +
           dumpForAst(dynamic_cast<AstConditionalExpression *>(node)->miss());
  case A_ASSIGNMENT_EXPRESSION:
    return dumpForAst(
               dynamic_cast<AstAssignmentExpression *>(node)->variable()) +
           "=" +
           dumpForAst(dynamic_cast<AstAssignmentExpression *>(node)->value());
  case A_EMPTY_EXPRESSION:
    return dynamic_cast<AstEmptyExpression *>(node)->name();
  case A_EXPRESSION_STATEMENT:
    return dumpForAst(
               dynamic_cast<AstExpressionStatement *>(node)->expression()) +
           ";";
  case A_COMPOUND_STATEMENT: {
    std::string r = "{";
    AstCompoundStatement *cs = dynamic_cast<AstCompoundStatement *>(node);
    if (cs->statementList() && cs->statementList()->size() > 0) {
      for (int i = 0; i < cs->statementList()->size(); i++) {
        r += ("\n\t" + dumpForAst(cs->statementList()->get(i)));
      }
    }
    r += "\n}\n";
    return r;
  }
  case A_IF_STATEMENT: {
    std::string r = "if (";
    AstIfStatement *is = dynamic_cast<AstIfStatement *>(node);
    CASSERT(is->hit(), "is#hit is null");
    r += dumpForAst(is->condition()) + ") \n";
    r += std::string("\t") + dumpForAst(is->hit());
    if (is->miss()) {
      r += " \nelse ";
      r += std::string("\t") + dumpForAst(is->miss());
    }
    return r;
  }
  case A_WHILE_STATEMENT: {
    std::string r = "while (";
    AstWhileStatement *ws = dynamic_cast<AstWhileStatement *>(node);
    r += dumpForAst(ws->condition()) + ")\n";
    r += dumpForAst(ws->statement());
    return r;
  }
  case A_FOR_STATEMENT: {
    AstForStatement *fs = dynamic_cast<AstForStatement *>(node);
    std::string r = "for (";
    r += dumpForAst(fs->initial());
    r += dumpForAst(fs->condition());
    r += dumpForAst(fs->post()) + ")\n";
    r += dumpForAst(fs->statement());
    return r;
  }
  case A_CONTINUE_STATEMENT:
    return dynamic_cast<AstContinueStatement *>(node)->name() + ";";
  case A_BREAK_STATEMENT:
    return dynamic_cast<AstBreakStatement *>(node)->name() + ";";
  case A_RETURN_STATEMENT:
    return dynamic_cast<AstReturnStatement *>(node)->name() + " " +
           dumpForAst(dynamic_cast<AstReturnStatement *>(node)->expression()) +
           ";";
  case A_VARIABLE_DECLARATION: {
    AstVariableDeclaration *vd = dynamic_cast<AstVariableDeclaration *>(node);
    std::string r = vd->name();
    for (int i = 0; i < vd->declarationList()->size(); i++) {
      r += dumpForAst(vd->declarationList()->get(i));
      if (i < vd->declarationList()->size() - 1) {
        r += ", ";
      }
    }
    return r;
  }
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    AstVariableAssignmentDeclaration *vad =
        dynamic_cast<AstVariableAssignmentDeclaration *>(node);
    return vad->identifier() + " = " + dumpForAst(vad->expression());
  }
  case A_FUNCTION_DECLARATION: {
    AstFunctionDeclaration *fd = dynamic_cast<AstFunctionDeclaration *>(node);
    std::string r = "\n" + fd->name() + "(";
    if (fd->argumentList() && fd->argumentList()->size() > 0) {
      for (int i = 0; i < fd->argumentList()->size(); i++) {
        r += fd->argumentList()->get(i)->name();
        if (i < fd->argumentList()->size() - 1) {
          r += ",";
        }
      }
    }
    r += ") => " + dumpForAst(fd->statement());
    return r;
  }
  case A_DECLARATION_LIST: {
    AstDeclarationList *dl = dynamic_cast<AstDeclarationList *>(node);
    std::string r = dl->name() + " {\n";
    for (int i = 0; i < dl->size(); i++) {
      r += dumpForAst(dl->get(i)) + "\n";
    }
    r += "}\n";
    return r;
  }
  default:
    return fmt::format("unknown node:{}", node->toString());
  }
}

void dump(const std::string &fileName, Ast *node) {
  FILE *fp = std::fopen(fileName.c_str(), "w");
  std::fprintf(fp, "%s", dumpForAst(node).c_str());
  std::fclose(fp);
}

void dump(const std::string &fileName, Symtab *sym) {}

void dump(const std::string &fileName, Tytab *ty) {}

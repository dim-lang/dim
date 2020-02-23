// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#include "Dump.h"
#include "Log.h"
#include "TokenName.h"
#include <cstdio>
#include <string>

static std::string dumpImpl(Ast *node) {
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
  case A_CALL_EXPRESSION: {
    AstCallExpression *ce = dynamic_cast<AstCallExpression *>(node);
    std::string r = ce->name() + "(";
    if (ce->argumentList()) {
      for (int i = 0; i < ce->argumentList()->size(); i++) {
        r += dumpImpl(ce->argumentList()->get(i));
        if (i < ce->argumentList()->size() - 1) {
          r += ", ";
        }
      }
    }
    r += ")";
    return r;
  }
  case A_UNARY_EXPRESSION:
    return tokenName(dynamic_cast<AstUnaryExpression *>(node)->token()) + " " +
           dumpImpl(dynamic_cast<AstUnaryExpression *>(node)->expression());
  case A_BINARY_EXPRESSION:
    return dumpImpl(dynamic_cast<AstBinaryExpression *>(node)->left()) + " " +
           tokenName(dynamic_cast<AstBinaryExpression *>(node)->token()) + " " +
           dumpImpl(dynamic_cast<AstBinaryExpression *>(node)->right());
  case A_CONDITIONAL_EXPRESSION:
    return dumpImpl(
               dynamic_cast<AstConditionalExpression *>(node)->condition()) +
           "?" +
           dumpImpl(dynamic_cast<AstConditionalExpression *>(node)->hit()) +
           ":" +
           dumpImpl(dynamic_cast<AstConditionalExpression *>(node)->miss());
  case A_ASSIGNMENT_EXPRESSION:
    return dumpImpl(dynamic_cast<AstAssignmentExpression *>(node)->variable()) +
           "=" +
           dumpImpl(dynamic_cast<AstAssignmentExpression *>(node)->value());
  case A_EMPTY_EXPRESSION:
    return dynamic_cast<AstEmptyExpression *>(node)->name();
  case A_EXPRESSION_STATEMENT:
    return dumpImpl(
               dynamic_cast<AstExpressionStatement *>(node)->expression()) +
           ";";
  case A_COMPOUND_STATEMENT: {
    std::string r = "{";
    AstCompoundStatement *cs = dynamic_cast<AstCompoundStatement *>(node);
    if (cs->statementList() && cs->statementList()->size() > 0) {
      for (int i = 0; i < cs->statementList()->size(); i++) {
        r += ("\n\t" + dumpImpl(cs->statementList()->get(i)));
      }
    }
    r += "\n}\n";
    return r;
  }
  case A_IF_STATEMENT: {
    std::string r = "if (";
    AstIfStatement *is = dynamic_cast<AstIfStatement *>(node);
    CASSERT(is->hit(), "is#hit is null");
    r += dumpImpl(is->condition()) + ") \n";
    r += std::string("\t") + dumpImpl(is->hit());
    if (is->miss()) {
      r += " \nelse ";
      r += std::string("\t") + dumpImpl(is->miss());
    }
    return r;
  }
  case A_WHILE_STATEMENT: {
    std::string r = "while (";
    AstWhileStatement *ws = dynamic_cast<AstWhileStatement *>(node);
    r += dumpImpl(ws->condition()) + ")\n";
    r += dumpImpl(ws->statement());
    return r;
  }
  case A_FOR_STATEMENT: {
    AstForStatement *fs = dynamic_cast<AstForStatement *>(node);
    std::string r = "for (";
    r += dumpImpl(fs->initial());
    r += dumpImpl(fs->condition());
    r += dumpImpl(fs->post()) + ")\n";
    r += dumpImpl(fs->statement());
    return r;
  }
  case A_CONTINUE_STATEMENT:
    return dynamic_cast<AstContinueStatement *>(node)->name() + ";";
  case A_BREAK_STATEMENT:
    return dynamic_cast<AstBreakStatement *>(node)->name() + ";";
  case A_RETURN_STATEMENT:
    return dynamic_cast<AstReturnStatement *>(node)->name() + " " +
           dumpImpl(dynamic_cast<AstReturnStatement *>(node)->expression()) +
           ";";
  case A_VARIABLE_DECLARATION: {
    AstVariableDeclaration *vd = dynamic_cast<AstVariableDeclaration *>(node);
    std::string r = vd->name() + ": ";
    for (int i = 0; i < vd->declarationList()->size(); i++) {
      r += dumpImpl(vd->declarationList()->get(i));
      if (i < vd->declarationList()->size() - 1) {
        r += ", ";
      }
    }
    r += ";";
    return r;
  }
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    AstVariableAssignmentDeclaration *vad =
        dynamic_cast<AstVariableAssignmentDeclaration *>(node);
    return vad->identifier() + " = " + dumpImpl(vad->expression());
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
    r += ") => " + dumpImpl(fd->statement());
    return r;
  }
  case A_DECLARATION_LIST: {
    AstDeclarationList *dl = dynamic_cast<AstDeclarationList *>(node);
    std::string r = dl->name() + " {\n\n";
    for (int i = 0; i < dl->size(); i++) {
      r += dumpImpl(dl->get(i)) + "\n";
    }
    r += "}\n";
    return r;
  }
  case A_EXPRESSION_LIST: {
  }
  default:
    return fmt::format("unknown_node:{}", node->toString());
  }
}

std::string dump(Ast *node) { return dumpImpl(node); }

static std::string dumpImpl(Symtab *sym) {
  if (!sym)
    return "null";
  switch (sym->type()) {
  case A_IDENTIFIER_CONSTANT:
    return sym->name();
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
    return sym->name();
  case A_CALL_EXPRESSION: {
    AstCallExpression *ce = dynamic_cast<AstCallExpression *>(sym);
    std::string r = ce->name() + "(";
    if (ce->argumentList()) {
      for (int i = 0; i < ce->argumentList()->size(); i++) {
        r += dumpImpl(ce->argumentList()->get(i));
        if (i < ce->argumentList()->size() - 1) {
          r += ", ";
        }
      }
    }
    r += ")";
    return r;
  }
  case A_UNARY_EXPRESSION:
    return tokenName(dynamic_cast<AstUnaryExpression *>(sym)->token()) + " " +
           dumpImpl(dynamic_cast<AstUnaryExpression *>(sym)->expression());
  case A_BINARY_EXPRESSION:
    return dumpImpl(dynamic_cast<AstBinaryExpression *>(sym)->left()) + " " +
           tokenName(dynamic_cast<AstBinaryExpression *>(sym)->token()) + " " +
           dumpImpl(dynamic_cast<AstBinaryExpression *>(sym)->right());
  case A_CONDITIONAL_EXPRESSION:
    return dumpImpl(
               dynamic_cast<AstConditionalExpression *>(sym)->condition()) +
           "?" +
           dumpImpl(dynamic_cast<AstConditionalExpression *>(sym)->hit()) +
           ":" +
           dumpImpl(dynamic_cast<AstConditionalExpression *>(sym)->miss());
  case A_ASSIGNMENT_EXPRESSION:
    return dumpImpl(dynamic_cast<AstAssignmentExpression *>(sym)->variable()) +
           "=" +
           dumpImpl(dynamic_cast<AstAssignmentExpression *>(sym)->value());
  case A_EMPTY_EXPRESSION:
    return dynamic_cast<AstEmptyExpression *>(sym)->name();
  case A_EXPRESSION_STATEMENT:
    return dumpImpl(dynamic_cast<AstExpressionStatement *>(sym)->expression()) +
           ";";
  case A_COMPOUND_STATEMENT: {
    std::string r = "{";
    AstCompoundStatement *cs = dynamic_cast<AstCompoundStatement *>(sym);
    if (cs->statementList() && cs->statementList()->size() > 0) {
      for (int i = 0; i < cs->statementList()->size(); i++) {
        r += ("\n\t" + dumpImpl(cs->statementList()->get(i)));
      }
    }
    r += "\n}\n";
    return r;
  }
  case A_IF_STATEMENT: {
    std::string r = "if (";
    AstIfStatement *is = dynamic_cast<AstIfStatement *>(sym);
    CASSERT(is->hit(), "is#hit is null");
    r += dumpImpl(is->condition()) + ") \n";
    r += std::string("\t") + dumpImpl(is->hit());
    if (is->miss()) {
      r += " \nelse ";
      r += std::string("\t") + dumpImpl(is->miss());
    }
    return r;
  }
  case A_WHILE_STATEMENT: {
    std::string r = "while (";
    AstWhileStatement *ws = dynamic_cast<AstWhileStatement *>(sym);
    r += dumpImpl(ws->condition()) + ")\n";
    r += dumpImpl(ws->statement());
    return r;
  }
  case A_FOR_STATEMENT: {
    AstForStatement *fs = dynamic_cast<AstForStatement *>(sym);
    std::string r = "for (";
    r += dumpImpl(fs->initial());
    r += dumpImpl(fs->condition());
    r += dumpImpl(fs->post()) + ")\n";
    r += dumpImpl(fs->statement());
    return r;
  }
  case A_CONTINUE_STATEMENT:
    return dynamic_cast<AstContinueStatement *>(sym)->name() + ";";
  case A_BREAK_STATEMENT:
    return dynamic_cast<AstBreakStatement *>(sym)->name() + ";";
  case A_RETURN_STATEMENT:
    return dynamic_cast<AstReturnStatement *>(sym)->name() + " " +
           dumpImpl(dynamic_cast<AstReturnStatement *>(sym)->expression()) +
           ";";
  case A_VARIABLE_DECLARATION: {
    AstVariableDeclaration *vd = dynamic_cast<AstVariableDeclaration *>(sym);
    std::string r = vd->name() + ": ";
    for (int i = 0; i < vd->declarationList()->size(); i++) {
      r += dumpImpl(vd->declarationList()->get(i));
      if (i < vd->declarationList()->size() - 1) {
        r += ", ";
      }
    }
    r += ";";
    return r;
  }
  case A_VARIABLE_ASSIGNMENT_DECLARATION: {
    AstVariableAssignmentDeclaration *vad =
        dynamic_cast<AstVariableAssignmentDeclaration *>(sym);
    return vad->identifier() + " = " + dumpImpl(vad->expression());
  }
  case A_FUNCTION_DECLARATION: {
    AstFunctionDeclaration *fd = dynamic_cast<AstFunctionDeclaration *>(sym);
    std::string r = "\n" + fd->name() + "(";
    if (fd->argumentList() && fd->argumentList()->size() > 0) {
      for (int i = 0; i < fd->argumentList()->size(); i++) {
        r += fd->argumentList()->get(i)->name();
        if (i < fd->argumentList()->size() - 1) {
          r += ",";
        }
      }
    }
    r += ") => " + dumpImpl(fd->statement());
    return r;
  }
  case A_DECLARATION_LIST: {
    AstDeclarationList *dl = dynamic_cast<AstDeclarationList *>(sym);
    std::string r = dl->name() + " {\n\n";
    for (int i = 0; i < dl->size(); i++) {
      r += dumpImpl(dl->get(i)) + "\n";
    }
    r += "}\n";
    return r;
  }
  case A_EXPRESSION_LIST: {
  }
  default:
    return fmt::format("unknown_sym:{}", sym->toString());
  }
}

std::string dump(Symtab *sym) { return dumpImpl(sym); }

static std::string dumpImpl(Tytab *ty) {}

std::string dump(Tytab *ty) { return dumpImpl(ty); }

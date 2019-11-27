// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "Stringify.h"
#include "script/Parser.h"
#include <unicode/numfmt.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace std {
template <> class hash<icu::UnicodeString> {
public:
  size_t operator()(const icu::UnicodeString &s) const {
    return (size_t)s.hashCode();
  }
};
}; // namespace std

namespace fastype {

class Interpreter : public Stringify {
public:
  Interpreter(Sptr<Parser> parser);
  virtual ~Interpreter();

  virtual void interpret();
  const std::unordered_map<icu::UnicodeString, Ast *> globalScope() const;
  virtual std::string toString() const;

private:
  // general
  virtual void visit(Ast *node);

  // program
  virtual void visitProgram(Ast *node);
  virtual void visitStatementList(Ast *node);

  // declaration
  virtual void visitVariableDeclaration(Ast *node);
  virtual void visitFunctionDeclaration(Ast *node);
  virtual void visitClassDeclaration(Ast *node);

  // statement
  virtual void visitCompoundStatement(Ast *node);
  virtual void visitAssignmentStatement(Ast *node);
  virtual void visitEmptyStatement(Ast *node);
  virtual void visitReturnStatement(Ast *node);

  // expression
  virtual Ast *visitBinaryOp(Ast *node);
  virtual Ast *visitUnaryOp(Ast *node);

  static void release(Ast *node);

  Ast *tree_;
  Sptr<Parser> parser_;
  std::unordered_map<icu::UnicodeString, Ast *> globalScope_;
};

} // namespace fastype

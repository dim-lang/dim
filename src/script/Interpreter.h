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

template <> struct hash<icu::UnicodeString> {
public:
  size_t operator()(const icu::UnicodeString &s) const {
    return (size_t)s.hashCode();
  }
};

template <> class hash<fastype::Sptr<fastype::Ast>> {
public:
  size_t operator()(const fastype::Sptr<fastype::Ast> &s) const {
    return (size_t)s.get();
  }
};
}; // namespace std

namespace fastype {

class Interpreter : public Stringify {
public:
  Interpreter(Sptr<Parser> parser);
  virtual ~Interpreter();

  virtual void interpret();
  const std::unordered_map<icu::UnicodeString, Sptr<Ast>> globalScope() const;
  virtual std::string toString() const;

private:
  // general
  virtual void visit(Sptr<Ast> node);

  // program
  virtual void visitProgram(Sptr<Ast> node);
  virtual void visitStatementList(Sptr<Ast> node);

  // declaration
  virtual void visitVariableDeclaration(Sptr<Ast> node);
  virtual void visitFunctionDeclaration(Sptr<Ast> node);
  virtual void visitClassDeclaration(Sptr<Ast> node);

  // statement
  virtual void visitCompoundStatement(Sptr<Ast> node);
  virtual void visitAssignmentStatement(Sptr<Ast> node);
  virtual void visitEmptyStatement(Sptr<Ast> node);
  virtual void visitReturnStatement(Sptr<Ast> node);

  // expression
  virtual Sptr<Ast> visitExpression(Sptr<Ast> node);
  virtual Sptr<Ast> visitBinaryOp(Sptr<Ast> node);
  virtual Sptr<Ast> visitUnaryOp(Sptr<Ast> node);

  static void release(Sptr<Ast> node);

  Sptr<Ast> tree_;
  Sptr<Parser> parser_;
  std::unordered_map<icu::UnicodeString, Sptr<Ast>> globalScope_;
};

} // namespace fastype

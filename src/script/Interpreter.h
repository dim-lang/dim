// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
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

template <> class hash<fastype::std::shared_ptr<fastype::Ast>> {
public:
  size_t operator()(const fastype::std::shared_ptr<fastype::Ast> &s) const {
    return (size_t)s.get();
  }
};
}; // namespace std

namespace fastype {

class Interpreter : public Stringify {
public:
  Interpreter(std::shared_ptr<Parser> parser);
  virtual ~Interpreter();

  virtual void interpret();
  const std::unordered_map<icu::UnicodeString, std::shared_ptr<Ast>>
  globalScope() const;
  virtual std::string toString() const;

private:
  // general
  virtual void visit(std::shared_ptr<Ast> node);

  // program
  virtual void visitProgram(std::shared_ptr<Ast> node);
  virtual void visitStatementList(std::shared_ptr<Ast> node);

  // declaration
  virtual void visitVariableDeclaration(std::shared_ptr<Ast> node);
  virtual void visitFunctionDeclaration(std::shared_ptr<Ast> node);
  virtual void visitClassDeclaration(std::shared_ptr<Ast> node);

  // statement
  virtual void visitCompoundStatement(std::shared_ptr<Ast> node);
  virtual void visitAssignmentStatement(std::shared_ptr<Ast> node);
  virtual void visitEmptyStatement(std::shared_ptr<Ast> node);
  virtual void visitReturnStatement(std::shared_ptr<Ast> node);

  // expression
  virtual std::shared_ptr<Ast> visitExpression(std::shared_ptr<Ast> node);
  virtual std::shared_ptr<Ast> visitBinaryOp(std::shared_ptr<Ast> node);
  virtual std::shared_ptr<Ast> visitUnaryOp(std::shared_ptr<Ast> node);

  static void release(std::shared_ptr<Ast> node);

  std::shared_ptr<Ast> tree_;
  std::shared_ptr<Parser> parser_;
  std::unordered_map<icu::UnicodeString, std::shared_ptr<Ast>> globalScope_;
};

} // namespace fastype

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

  // general visit
  virtual void visit(Ast *node);
  virtual void visitStatement(Ast *node);
  virtual void visitOp(Ast *node);

  // operations
  virtual long long visitBinaryOp(Ast *node);
  virtual long long visitUnaryOp(Ast *node);

  // constants
  virtual long long visitIntergerConstant(Ast *node);
  virtual double visitFloatingConstant(Ast *node);
  virtual icu::UnicodeString visitStringConstant(Ast *node);
  virtual bool visitBooleanConstant(Ast *node);
  virtual Ast *visitIdentifierConstant(Ast *node);

  // statements
  virtual void visitCompoundStatement(Ast *node);
  virtual void visitAssignmentStatement(Ast *node);
  virtual void visitEmptyStatement(Ast *node);
  virtual void visitStatement(Ast *node);

  // variable
  virtual Ast *visitVariable(Ast *node);

  // program
  virtual void visitProgram(Ast *node);

  virtual void interpret();

  const std::unordered_map<icu::UnicodeString, Ast *> globalScope() const;
  virtual std::string toString() const;
  static void release(Ast *node);

private:
  Ast *tree_;
  Sptr<Parser> parser_;
  std::unordered_map<icu::UnicodeString, Ast *> globalScope_;
};

} // namespace fastype

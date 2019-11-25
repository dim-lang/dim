// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/NodeVisitor.h"
#include "script/Parser.h"

namespace fastype {

class Interpreter : public NodeVisitor {
public:
  Interpreter(Sptr<Parser> parser);
  virtual ~Interpreter();

  virtual long long visit(Ast *node);
  virtual long long visitBinaryOp(Ast *node);
  virtual long long visitUnaryOp(Ast *node);
  virtual long long visitIntergerConstant(Ast *node);
  virtual double visitFloatingConstant(Ast *node);

  long long interpret();
  static void release(Ast *node);

private:
  Ast *tree_;
  Sptr<Parser> parser_;
};

} // namespace fastype

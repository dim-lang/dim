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
  virtual ~Interpreter() = default;

  virtual long long visit(Ast *node);
  virtual long long visitBinaryOp(Ast *node);
  virtual long long visitIntergerConstant(Ast *node);
  virtual long long visitUnaryOp(Ast *node);

  long long interpret();

private:
  Sptr<Parser> parser_;
};

} // namespace fastype

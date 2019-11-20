// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

namespace fastype {

class Ast;

class NodeVisitor {
public:
  virtual ~NodeVisitor() = default;
  virtual long long visit(Ast *node) = 0;
  virtual long long visitBinaryOp(Ast *node) = 0;
  virtual long long visitIntergerConstant(Ast *node) = 0;
  virtual long long visitUnaryOp(Ast *node) = 0;
};

} // namespace fastype

// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once

namespace fastype {

class AstTree;

class NodeVisitor {
public:
  virtual ~NodeVisitor() = default;
  virtual long long visit(AstTree *node) = 0;
  virtual long long visitBinaryOp(AstTree *node) = 0;
  virtual long long visitIntergerConstant(AstTree *node) = 0;
  virtual long long visitUnaryOp(AstTree *node) = 0;
};

} // namespace fastype

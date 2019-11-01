// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/AstTree.h"
#include "script/Token.h"

namespace fastype {

class AstLeaf : public AstTree {
public:
  AstLeaf(Sptr<Token> token);
  virtual ~AstLeaf() = default;

  virtual Sptr<AstTree> child(int i) const;
  virtual int childrenNumber() const;
  virtual std::string toString() const;
  virtual Sptr<Token> token() const;

protected:
  Sptr<Token> token_;
};

} // namespace fastype

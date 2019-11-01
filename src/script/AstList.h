// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/AstTree.h"
#include "script/Token.h"
#include <vector>

namespace fastype {

class AstList : public AstTree {
public:
  AstList(const std::vector<Sptr<AstTree>> &children);
  virtual ~AstList() = default;

  virtual Sptr<AstTree> child(int i) const;
  virtual int childrenNumber() const;
  virtual std::string toString() const;

protected:
  std::vector<Sptr<AstTree>> children_;
};

} // namespace fastype

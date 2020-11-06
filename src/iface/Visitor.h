// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "AstClasses.h"

class Visitor {
public:
  virtual ~Visitor() = default;
  virtual void visitInteger(A_Integer *ast) = 0;
  virtual void visitFloat(A_Float *ast) = 0;
  virtual void visitBoolean(A_Boolean *ast) = 0;
  virtual void visitCharacter(A_Character *ast) = 0;
  // ...
};

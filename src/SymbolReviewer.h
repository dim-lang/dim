// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Phase.h"
#include "Visitor.h"
#include <vector>

class SymbolReviewer : public Phase {
public:
  SymbolReviewer();
  virtual ~SymbolReviewer();
  virtual void run(Ast *ast);

private:
  std::vector<Visitor *> visitors_;
  VisitorContext *context_;
  VisitorBinder binder_;
};

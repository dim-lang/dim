// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "Phase.h"
#include "Visitor.h"
#include "llvm/IR/LLVMContext.h"
#include <vector>

class IrBuilder : public Phase {
public:
  IrBuilder(const Cowstr &fileName = "");
  virtual ~IrBuilder();
  virtual void run(Ast *ast);
  virtual Cowstr &fileName();
  virtual const Cowstr &fileName() const;

private:
  Cowstr fileName_;
  std::vector<Visitor *> visitors_;
  VisitorContext *context_;
  VisitorBinder binder_;
};

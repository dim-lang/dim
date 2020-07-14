// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Ir.h"
#include "Position.h"
#include "Symbol.h"
#include <string>

std::string dumpAst(Ast *node);

std::string dumpScope(ScopeNode *snode);

std::string dumpSource(const std::string &fileName, const Position &position);

std::string dumpLLVMValue(const llvm::Value *value);

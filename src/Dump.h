// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Ir.h"
#include "Symbol.h"
#include "Type.h"
#include <string>

std::string dumpAst(Ast *node);

std::string dumpSymbol(Symbol *sym);

std::string dumpType(Type *ty);

std::string dumpLLVMValue(llvm::Value *v);

std::string dumpLLVMFunction(llvm::Function *f);

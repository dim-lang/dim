// Copyright 2019- <collie.org>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"
#include "Type.h"
#include <string>

std::string dumpAst(Ast *node);

std::string dumpSymbol(Symbol *sym);

std::string dumpType(Type *ty);

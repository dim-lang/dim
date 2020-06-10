// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Ir.h"
#include "Symbol.h"
#include <string>

std::string dumpAst(Ast *node);

std::string dumpScope(const Scope::SNode &snode);

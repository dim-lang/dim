// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"
#include "Type.h"
#include <string>

std::string dump(Ast *node);

std::string dump(Symtab *sym);

std::string dump(Tytab *ty);

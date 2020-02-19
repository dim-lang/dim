// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Symbol.h"
#include "Type.h"
#include <string>

void dump(const std::string &fileName, Ast *node);

void dump(const std::string &fileName, Symtab *sym);

void dump(const std::string &fileName, Tytab *ty);

// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Location.h"
#include <string>

std::string dumpAst(std::shared_ptr<Ast> node);

std::string dumpSource(const std::string &fileName, const Location &location);

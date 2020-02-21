// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <cstdio>
#include <stack>
#include <string>

class AstDeclarationList;

extern FILE *yyin;
extern int yylineno;
extern int yylex(void);
extern int yyparse(AstDeclarationList **program);

extern int tokenPushImport(const std::string &module);
extern int tokenPopImport();

extern std::string tokenModuleToFile(const std::string &name);
extern std::string tokenFileToModule(const std::string &name);

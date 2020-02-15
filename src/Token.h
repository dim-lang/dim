// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <string>

class AstDeclarationList;
class TokenBuffer;

extern FILE *yyin;
extern int yylineno;

extern int yylex(void);
extern int yyparse(AstDeclarationList **program, char **errorMsg);

extern std::string CurrentTokenFile;
extern std::stack<TokenBuffer *> TokenBufferStack;
extern int tokenPushImport(const char *module);
extern int tokenPushImport(const std::string &module);
extern int tokenPopImport();
extern std::string tokenModuleToFile(const std::string &name);
extern std::string tokenFileToModule(const std::string &name);

std::string TokenName(int token);

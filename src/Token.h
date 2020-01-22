// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>

class TokenBuffer;

extern FILE *yyin;
extern int yylineno;

extern int yylex(void);
extern int yyparse(void);

extern std::string TokenCurrentFile;
extern std::stack<TokenBuffer *> TokenBufferStack;
extern int tokenImportFile(const char *module);
extern int tokenImportFile(const std::string &module);
extern int tokenPopFile();

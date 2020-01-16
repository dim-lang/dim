// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>

class TokenBuffer;

extern int yylex(void);
extern FILE *yyin;

extern char *TokenCurrentFile;
extern std::stack<TokenBuffer *> TokenBufferStack;
extern int tokenImportFile(const char *module);
extern int tokenImportFile(const std::string &module);
extern int tokenPopFile();

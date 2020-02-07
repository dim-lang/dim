// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <string>

class TkFileBuffer;

extern FILE *yyin;
extern int yylineno;

extern int yylex(void);
extern int yyparse(void);

extern std::string TkCurrentFile;
extern std::stack<TkFileBuffer *> TkFileBufferStack;
extern int tkPushImport(const char *module);
extern int tkPushImport(const std::string &module);
extern int tkPopImport();
extern std::string tkModuleToFile(const std::string &name);
extern std::string tkFileToModule(const std::string &name);

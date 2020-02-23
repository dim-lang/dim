// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include <cstdio>

class AstProgram;

extern FILE *yyin;
extern int yylineno;
extern int yylex(void);

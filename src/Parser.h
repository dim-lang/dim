// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

class AstProgram;
struct YYLTYPE;

extern int yyparse(AstProgram *&program);

void yyerror(YYLTYPE *yyllocp, AstProgram *&program, const char *fmt, ...);

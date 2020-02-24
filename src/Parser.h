// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

class AstProgram;

extern int yyparse(AstProgram **program);

void yyerror(AstProgram **program, const char *fmt, ...);

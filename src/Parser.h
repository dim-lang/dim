// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

class Scanner;
struct YYLTYPE;

extern int yyparse(Scanner *scanner);

void yyerror(YYLTYPE *yyllocp, Scanner *scanner, const char *fmt, ...);

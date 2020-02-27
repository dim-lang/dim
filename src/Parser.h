// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

class Scanner;
struct YYLTYPE;
using yyscan_t = void *;

extern int yyparse(Scanner *scanner);

void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, Scanner *scanner,
             const char *fmt, ...);

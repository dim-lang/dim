// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once

struct YYLTYPE;
using yyscan_t = void *;

extern int yyparse(yyscan_t yyscanner);

void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, const char *fmt, ...);

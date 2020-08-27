// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#define YY_DECL yy::parser::symbol_type yylex(yyscan_t yyscanner)
#define YY_EXTRA_TYPE void *

class Scanner;
struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
typedef void *yyscan_t;
YY_EXTRA_TYPE yyget_extra(yyscan_t yyscanner);

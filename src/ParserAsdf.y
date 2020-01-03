%{
#include "Node.h"
NBlock *programBlock;
extern int yylex();
void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

%union {
Node *node;
NBlock *block;
NExpression *expr;
NStatement *stmt;
NIdentifier *ident;
NVariableDeclaration *var_decl;
std::vector<NVariableDeclaration*> varvec;
std::vector<NExpression*> *exprvec;
std::string *string;
int token;
}

%token <string> FIDENTIFIER FINTEGER FDOUBLE FSTRING_LITERAL
%token <token> FTRUE FFALSE FNIL FLET FIF FELSE FFOR FWHILE FBREAK FCONTINUE FVOID FRETURN FFUNC FSTRUCT FCLASS FIMPORT FENUM FLOGICAND FLOGICOR
%token <token> FEQ FNEQ FLT FLE FGT FGE FLPAREN FRPAREN FLBRACKET FRBRACKET FLBRACE FRBRACE FDOT FCOMMA FQUESTION FCOLON FSEMI FNOT FCOMPLEMENT FAND FOR FXOR
%token <token> FADD FSUB FMUL FDIV FMOD FASSIGN FADDASSIGN FSUBASSIGN FMULASSIGN FDIVASSIGN FMODASSIGN

%type <ident> ident
%type <expr> constant expr
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> binary_op

%left FADD FSUB
%left FMUL FDIV FMOD

%start program

%%

program : stmt { programBlock = $1; }
        ;

stmts : stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); }
      ;

stmt : var_decl
     | func_decl
     | expr { $$ = new NExpressionStatement(*$1); }
     ;

block : FLBRACE stmts FRBRACE { $$ = $2; }
      | FLBRACE FRBRACE { $$ = new NBlock(); }
      ;

var_decl : FLET ident FASSIGN expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
         ;

func_decl : FFUNC ident FLPAREN func_decl_args FRPAREN block { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
          ;

func_decl_args : /*blank*/  { $$ = new VariableList(); }
          | var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
          | func_decl_args FCOMMA var_decl { $1->push_back($<var_decl>3); }
          ;

ident : FIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
      ;

constant : FINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
         | FDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; } 
         | FSTRING_LITERAL { $$ = new NStringLiteral($1); delete $1; }
         | FTRUE { $$ = new NBoolean($1); delete $1; }
         | FFALSE { $$ = new NBoolean($1); delete $1; }
         ;

expr : ident FASSIGN expr { $$ = new NAssignment(*$<ident>1, *$3); }
     | ident FLPAREN call_args FRPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }
     | constant { $<constant>$ = $1; }
     | expr binary_op expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | FLPAREN expr FRPAREN { $$ = $2; }
     ;

call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($1); }
          | call_args FCOMMA expr  { $1->push_back($3); }
          ;

binary_op : FEQ
          | FNEQ
          | FLT
          | FLE
          | FGT
          | FGE
          | FADD
          | FSUB
          | FMUL
          | FDIV
          | FMOD
          ;

%%

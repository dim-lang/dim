%{
#include "Ast.h"
AstBlock *programBlock;
extern int yylex();
void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

%union {
std::shared_ptr<Ast> node;
std::shared_ptr<AstBlock> block;
std::shared_ptr<AstExpression> expression;
std::shared_ptr<AstStatement> statement;
std::shared_ptr<AstIdentifier> identifier;
/*AstVariableDeclaration *var_decl;*/
std::shared_ptr<std::vector<std::shared_ptr<AstVariableDeclaration>>> variableList;
std::shared_ptr<std::vector<std::shared_ptr<AstExpression>>> expressionList;
std::shared_ptr<std::string> string;
int token;
}

%token <string> FIDENTIFIER FINTEGER FDOUBLE FSTRING_LITERAL
%token <token> FTRUE FFALSE FNIL FLET FIF FELSE FFOR FWHILE FBREAK FCONTINUE FVOID FRETURN FFUNC FSTRUCT FCLASS FIMPORT FENUM FLOGICAND FLOGICOR
%token <token> FEQ FNEQ FLT FLE FGT FGE FLPAREN FRPAREN FLBRACKET FRBRACKET FLBRACE FRBRACE FDOT FCOMMA FQUESTION FCOLON FSEMI FNOT FCOMPLEMENT FAND FOR FXOR
%token <token> FADD FSUB FMUL FDIV FMOD FASSIGN FADDASSIGN FSUBASSIGN FMULASSIGN FDIVASSIGN FMODASSIGN

%type <identifier> ident
%type <expression> primary_expr expr
%type <variableList> func_decl_args
%type <expressionList> call_args
%type <block> program stmts block
%type <statement> stmt func_decl
%type <token> binary_op

%left FADD FSUB
%left FMUL FDIV FMOD

%start program

%%

program : stmt { programBlock = $1; }
        ;

stmts : stmt { $$ = std::shared_ptr<AstBlock>(new AstBlock()); $$->statementList->push_back(std::shared_ptr<AstStatement>($<stmt>1)); }
      | stmts stmt { $1->statementList->push_back(std::shared_ptr<AstStatement>($<stmt>2)); }
      ;

stmt : func_decl
     | expr FSEMI { $$ = std::shared_ptr<AstExpressionStatement>(new AstExpressionStatement($1)); }
     ;

block : FLBRACE stmts FRBRACE { $$ = $2; }
      | FLBRACE FRBRACE { $$ = std::shared_ptr<AstBlock>(new AstBlock()); }
      ;

func_decl : FFUNC ident FLPAREN func_decl_args FRPAREN block { $$ = std::shared_ptr<AstStatement>(new AstFunctionDeclaration($2, $4, $6)); }
          ;

func_decl_args : /*blank*/  { $$ = std::shared_ptr<std::vector<std::shared_ptr<AstVariableDeclaration>>>(std::vector<std::shared_ptr<AstVariableDeclaration>>()); }
               | func_decl_args FCOMMA ident { $1->push_back($<ident>3); }
               ;

ident : FIDENTIFIER { $$ = std::shared_ptr<AstIdentifier>(new AstIdentifier($1)); }
      ;

primary_expr : FINTEGER { $$ = new AstInteger(atol($1->c_str())); delete $1; }
             | FDOUBLE { $$ = new AstDouble(atof($1->c_str())); delete $1; } 
             | FSTRING_LITERAL { $$ = new AstStringLiteral($1); delete $1; }
             | FTRUE { $$ = new AstBoolean($1); delete $1; }
             | FFALSE { $$ = new AstBoolean($1); delete $1; }
             ;

expr : ident FASSIGN expr { $$ = new AstAssignment(*$<ident>1, *$3); }
     | ident FLPAREN call_args FRPAREN { $$ = new AstMethodCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }
     | primary_expr { $<primary_expr>$ = $1; }
     | expr binary_op expr { $$ = new AstBinaryOperator(*$1, $2, *$3); }
     | FLPAREN expr FRPAREN { $$ = $2; }
     ;

call_args : /*blank*/  { $$ = new AstExpressionList(); }
          | expr { $$ = new AstExpressionList(); $$->push_back($1); }
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

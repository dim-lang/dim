%{
#include "Ast.h"
fastype::AstBlock *programBlock;
extern int yylex();
void yyerror(const char *s) { printf("ERROR: %sn", s); }
#define YYSTYPE yylval
%}

%union {
std::shared_ptr<fastype::Ast> node;
std::shared_ptr<fastype::AstBlock> block;
std::shared_ptr<fastype::AstExpression> expression;
std::shared_ptr<fastype::AstStatement> statement;
std::shared_ptr<fastype::AstIdentifier> identifier;
/*AstVariableDeclaration *var_decl;*/
std::shared_ptr<fastype::AstVariableDeclarationList> variableList;
std::shared_ptr<fastype::AstExpressionList> expressionList;
std::shared_ptr<std::string> string;
int token;
}

%token <string> FIDENTIFIER FINTEGER FDOUBLE FSTRING_LITERAL
%token <token> FTRUE FFALSE FNIL FLET FIF FELSE FFOR FWHILE FBREAK FCONTINUE FVOID FRETURN FFUNC FSTRUCT FCLASS FIMPORT FENUM FLOGICAND FLOGICOR
%token <token> FEQ FNEQ FLT FLE FGT FGE FLPAREN FRPAREN FLBRACKET FRBRACKET FLBRACE FRBRACE FDOT FCOMMA FQUESTION FCOLON FSEMI FNOT FCOMPLEMENT FAND FOR FXOR
%token <token> FADD FSUB FMUL FDIV FMOD FASSIGN FADDASSIGN FSUBASSIGN FMULASSIGN FDIVASSIGN FMODASSIGN

%type <identifier> ident
%type <expression> expr
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

stmts : stmt { $$ = std::shared_ptr<fastype::AstBlock>(new fastype::AstBlock()); $$->statementList->push_back(std::shared_ptr<fastype::AstStatement>($<stmt>1)); }
      | stmts stmt { $1->statementList->push_back(std::shared_ptr<fastype::AstStatement>($<stmt>2)); }
      ;

stmt : func_decl
     | expr FSEMI { $$ = std::shared_ptr<fastype::AstExpressionStatement>(new fastype::AstExpressionStatement($1)); }
     ;

block : FLBRACE stmts FRBRACE { $$ = $2; }
      | FLBRACE FRBRACE { $$ = std::shared_ptr<fastype::AstBlock>(new fastype::AstBlock()); }
      ;

func_decl : FFUNC ident FLPAREN func_decl_args FRPAREN block { $$ = std::shared_ptr<fastype::AstStatement>(new fastype::AstFunctionDeclaration($2, $4, $6)); }
          ;

func_decl_args : /*blank*/  { $$ = std::shared_ptr<fastype::AstVariableDeclarationList>(new fastype::AstVariableDeclarationList()); }
               | func_decl_args FCOMMA ident { $1->push_back($<ident>3); }
               ;

ident : FIDENTIFIER { $$ = std::shared_ptr<fastype::AstIdentifier>(new fastype::AstIdentifier($1)); }
      ;


expr : ident FASSIGN expr { $$ = std::shared_ptr<fastype::AstExpression>(new fastype::AstAssignment($<ident>1, $3)); }
     | ident FLPAREN call_args FRPAREN { $$ = std::shared_ptr<fastype::AstExpression>(new fastype::AstMethodCall($1, $3)); }
     | ident { $<ident>$ = std::static_pointer_cast<fastype::AstExpression>($1); }
     | expr binary_op expr { $$ = std::shared_ptr<fastype::AstExpression>(new fastype::AstBinaryOperator($1, $2, $3)); }
     | FLPAREN expr FRPAREN { $$ = $2; }
     | FINTEGER { $$ = std::shared_ptr<fastype::AstExpression>(new fastype::AstInteger(atol($1->c_str()))); delete $1; }
     | FDOUBLE { $$ = std::shared_ptr<fastype::AstExpression>(new AstDouble(atof($1->c_str()))); delete $1; }
     | FSTRING_LITERAL { $$ = std::shared_ptr<fastype::AstExpression>(new fastype::AstStringLiteral(*$1)); delete $1; }
     | FTRUE { $$ = std::shared_ptr<fastype::AstExpression>(new fastype::AstBoolean(true)); delete $1; }
     | FFALSE { $$ = std::shared_ptr<fastype::AstExpression>(new fastype::AstBoolean(false)); delete $1; }
     ;

call_args : /*blank*/  { $$ = std::shared_ptr<fastype::AstExpressionList>(new fastype::AstExpressionList()); }
          | expr { $$ = std::shared_ptr<fastype::AstExpressionList>(new fastype::AstExpressionList()); $$->push_back($1); }
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

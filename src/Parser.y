%{
#include "Node.h"
NBlock *programBlock;
extern int yylex();
void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

%union {
std::shared_ptr<Node> nodeValue;
std::shared_ptr<NBlock> blockValue;
std::shared_ptr<NExpression> expressionValue;
std::shared_ptr<NStatement> statementValue;
std::shared_ptr<NIdentifier> identifierValue;
/*NVariableDeclaration *var_decl;*/
std::shared_ptr<std::vector<std::shared_ptr<NVariableDeclaration>>> variableListValue;
std::shared_ptr<std::vector<std::shared_ptr<NExpression>>> expressionListValue;
std::shared_ptr<std::string> stringValue;
int tokenValue;
}

%token <stringValue> FIDENTIFIER FINTEGER FDOUBLE FSTRING_LITERAL
%token <tokenValue> FTRUE FFALSE FNIL FLET FIF FELSE FFOR FWHILE FBREAK FCONTINUE FVOID FRETURN FFUNC FSTRUCT FCLASS FIMPORT FENUM FLOGICAND FLOGICOR
%token <tokenValue> FEQ FNEQ FLT FLE FGT FGE FLPAREN FRPAREN FLBRACKET FRBRACKET FLBRACE FRBRACE FDOT FCOMMA FQUESTION FCOLON FSEMI FNOT FCOMPLEMENT FAND FOR FXOR
%token <tokenValue> FADD FSUB FMUL FDIV FMOD FASSIGN FADDASSIGN FSUBASSIGN FMULASSIGN FDIVASSIGN FMODASSIGN

%type <identifierValue> ident
%type <expressionValue> primary_expr expr
%type <variableListValue> func_decl_args
%type <expressionListValue> call_args
%type <blockValue> program stmts block
%type <statementValue> stmt func_decl
%type <tokenValue> binary_op

%left FADD FSUB
%left FMUL FDIV FMOD

%start program

%%

program : stmt { programBlock = $1; }
        ;

stmts : stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); }
      ;

stmt : func_decl
     | expr FSEMI { $$ = new NExpressionStatement(*$1); }
     ;

block : FLBRACE stmts FRBRACE { $$ = $2; }
      | FLBRACE FRBRACE { $$ = new NBlock(); }
      ;

func_decl : FFUNC ident FLPAREN func_decl_args FRPAREN block { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
          ;

func_decl_args : /*blank*/  { $$ = new VariableList(); }
               | func_decl_args FCOMMA ident { $1->push_back($<ident>3); }
               ;

ident : FIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
      ;

primary_expr : FINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
             | FDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; } 
             | FSTRING_LITERAL { $$ = new NStringLiteral($1); delete $1; }
             | FTRUE { $$ = new NBoolean($1); delete $1; }
             | FFALSE { $$ = new NBoolean($1); delete $1; }
             ;

expr : ident FASSIGN expr { $$ = new NAssignment(*$<ident>1, *$3); }
     | ident FLPAREN call_args FRPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }
     | primary_expr { $<primary_expr>$ = $1; }
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

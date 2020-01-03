%{
#include "node.h"
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

%token <string> FIDENTIFIER FINTEGER FDOUBLE
%token <token> FEQ FNEQ FLT FLE FGT FGE FASSIGN FADDASSIGN FSUBASSIGN FMULASSIGN FDIVASSIGN FMODASSIGN
%token <token> FLPAREN FRPAREN FLBRACKET FRBRACKET FLBRACE FRBRACE FCOMMA FDOT FQUESTION FSEMI FCOLON
%token <token> FADD FSUB FMUL FDIV FMOD

%type <ident> ident

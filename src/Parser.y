%define api.pure
%locations
%parse-param { AstProgram *&program }

%code top {
#include <cstdarg>
#include <cstdio>
#include "Log.h"
#include "Ast.h"
#include "Token.h"
#include "Parser.h"
}

%code requires {
class AstProgram;
}

 /* different ways to access data */
%union {
    AstExpressionList *exprList;
    AstStatementList *stmtList;
    AstDeclarationList *declList;
    AstExpression *expr;
    AstStatement *stmt;
    AstDeclaration *decl;
    AstStringConstant *str;
    char *literal;
    int token;
}

 /* union */
%token <token> T_EOF
%token <token> T_TRUE T_FALSE T_LET T_VAR T_VAL T_NIL T_IF T_ELSEIF T_ELSE T_FOR T_FOREACH T_IN T_WHILE T_BREAK T_CONTINUE T_SWITCH T_CASE T_MATCH T_DEFAULT
%token <token> T_FUNC T_CLASS T_TYPE T_IS T_ISINSTANCE T_IMPORT T_RETURN T_VOID T_LOGIC_AND T_LOGIC_OR T_LOGIC_NOT
%token <token> T_I8 T_U8 T_I16 T_U16 T_I32 T_U32 T_I64 T_U64 T_F32 T_F64 T_STRING T_BOOLEAN T_ASYNC T_AWAIT
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD T_UNDERLINE T_BIT_NOT T_BIT_AND T_BIT_OR T_BIT_XOR T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ARSHIFT
%token <token> T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%token <token> T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN T_BIT_LSHIFT_ASSIGN T_BIT_RSHIFT_ASSIGN T_BIT_ARSHIFT_ASSIGN
%token <token> T_EQ T_NEQ T_LT T_LE T_GT T_GE
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE T_COMMA T_SEMI T_QUESTION T_COLON T_DOT T_BIG_ARROW

%token <literal> T_IDENTIFIER T_I8_CONSTANT T_U8_CONSTANT T_I16_CONSTANT T_U16_CONSTANT T_I32_CONSTANT T_U32_CONSTANT
%token <literal> T_I64_CONSTANT T_U64_CONSTANT T_F32_CONSTANT T_F64_CONSTANT T_STRING_CONSTANT T_CHAR_CONSTANT

%type <expr> postfix_expression primary_expression unary_expression binary_expression
%type <expr> conditional_expression assignment_expression constant_expression  sequel_expression
%type <expr> expression
%type <exprList> argument_expression_list sequel_expression_list

%type <stmt> compound_statement expression_statement iteration_statement jump_statement empty_statement if_statement
%type <stmt> statement
%type <stmtList> statement_list

%type <decl> variable_declaration function_declaration variable_assignment_declaration function_argument_declaration
%type <decl> declaration
%type <declList> variable_assignment_declaration_list translation_unit function_argument_declaration_list

%type <str> join_string_helper

 /* operator precedence */
 /* comma */
%left T_COMMA
 /* assignment */
%right T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN
%right T_BIT_LSHIFT_ASSIGN T_BIT_RSHIFT_ASSIGN T_BIT_ARSHIFT_ASSIGN
%right T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%right T_ADD_ASSIGN T_SUB_ASSIGN
%right T_ASSIGN
 /* conditional */
%left T_QUESTION T_COLON
 /* binary operator */
%left T_LOGIC_OR
%left T_LOGIC_AND
%left T_BIT_OR
%left T_BIT_XOR
%left T_BIT_AND
%left T_EQ T_NEQ
%left T_LT T_LE T_GT T_GE
%left T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ARSHIFT
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
 /* other */
%nonassoc T_LOGIC_NOT T_BIT_NOT
%left T_DOT
 /* fix if statement shift/reduce */
%nonassoc "lower_than_else"
%nonassoc T_ELSE

%start translation_unit

%%

 /* part-1 expression */

join_string_helper : T_STRING_CONSTANT { $$ = new AstStringConstant($1); std::free($1); CINFO("{}: first_line: {}, first_column: {} last_line: {}, last_column: {}", "join_string_helper", @$.first_line, @$.first_column, @$.last_line, @$.last_column); }
                   | T_STRING_CONSTANT join_string_helper { $2->add($1); $$ = $2; std::free($1); }
                   ;

primary_expression : T_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); }
                   /*| T_I8_CONSTANT { $$ = new AstI8Constant((int8_t)std::stoi($1)); std::free($1); }*/
                   /*| T_U8_CONSTANT { $$ = new AstU8Constant((uint8_t)std::stoul($1)); std::free($1); }*/
                   /*| T_I16_CONSTANT { $$ = new AstI16Constant((int16_t)std::stoi($1)); std::free($1); }*/
                   /*| T_U16_CONSTANT { $$ = new AstU16Constant((uint16_t)std::stoul($1)); std::free($1); }*/
                   /*| T_I32_CONSTANT { $$ = new AstI32Constant((int32_t)std::stol($1)); std::free($1); }*/
                   /*| T_U32_CONSTANT { $$ = new AstU32Constant((uint32_t)std::stoul($1)); std::free($1); }*/
                   | T_I64_CONSTANT { $$ = new AstI64Constant((int64_t)std::stoll($1)); std::free($1); }
                   /*| T_U64_CONSTANT { $$ = new AstU64Constant((uint64_t)std::stoull($1)); std::free($1); }*/
                   /*| T_F32_CONSTANT { $$ = new AstF32Constant((float)std::stof($1)); std::free($1); }*/
                   | T_F64_CONSTANT { $$ = new AstF64Constant((double)std::stod($1)); std::free($1); }
                   | join_string_helper { $$ = $1; }
                   | T_TRUE { $$ = new AstBooleanConstant(true); }
                   | T_FALSE { $$ = new AstBooleanConstant(false); }
                   | T_LPAREN expression T_RPAREN { $$ = $2; }
                   ;

postfix_expression : primary_expression { $$ = $1; }
                   /*| postfix_expression '[' expression ']'*/
                   | T_IDENTIFIER T_LPAREN T_RPAREN { $$ = new AstCallExpression($1, nullptr); std::free($1); }
                   | T_IDENTIFIER T_LPAREN argument_expression_list T_RPAREN { $$ = new AstCallExpression($1, $3); std::free($1); }
                   /*| postfix_expression '.' T_IDENTIFIER */
                   ;

argument_expression_list : assignment_expression { $$ = new AstExpressionList(); $$->add($1); }
                         | assignment_expression T_COMMA argument_expression_list { $3->add($1); $$ = $3; }
                         ;

unary_expression : postfix_expression { $$ = $1; }
                 | T_ADD unary_expression { $$ = new AstUnaryExpression($1, $2); }
                 | T_SUB unary_expression { $$ = new AstUnaryExpression($1, $2); }
                 | T_BIT_NOT unary_expression { $$ = new AstUnaryExpression($1, $2); }
                 | T_LOGIC_NOT unary_expression { $$ = new AstUnaryExpression($1, $2); }
                 ;

 /*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
 */

binary_expression : unary_expression { $$ = $1; }
                  | binary_expression T_MUL unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_DIV unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_MOD unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_ADD unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_SUB unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_BIT_LSHIFT unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_BIT_RSHIFT unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_BIT_ARSHIFT unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_LT unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_LE unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_GT unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_GE unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_EQ unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_NEQ unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_BIT_AND unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_BIT_XOR unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_BIT_OR unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_LOGIC_AND unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  | binary_expression T_LOGIC_OR unary_expression { $$ = new AstBinaryExpression($1, $2, $3); }
                  ;

conditional_expression : binary_expression { $$ = $1; }
                       | binary_expression T_QUESTION expression T_COLON conditional_expression { $$ = new AstConditionalExpression($1, $3, $5); }
                       ;

assignment_expression : conditional_expression { $$ = $1; }
                      | unary_expression T_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_MUL_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_DIV_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_MOD_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_ADD_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_SUB_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_BIT_AND_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_BIT_OR_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_BIT_XOR_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_BIT_LSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_BIT_RSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      | unary_expression T_BIT_ARSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); }
                      ;

sequel_expression_list : assignment_expression { $$ = new AstExpressionList(); $$->add($1); }
                       | assignment_expression T_COMMA sequel_expression_list { $3->add($1); $$ = $3; }
                       ;

sequel_expression : sequel_expression_list { $$ = new AstSequelExpression($1); }
                  ;

expression : sequel_expression { $$ = $1; }
           ;

constant_expression : conditional_expression { $$ = $1; }
                    ;

 /* part-2 declaration */

declaration : function_declaration { $$ = $1; }
            | variable_declaration { $$ = $1; }
            /*| import_module { $$ = $1; }*/
            ;

 /* 
import_module : T_IMPORT import_module_list T_SEMI
              ;

import_module_list : T_IDENTIFIER
                   | T_IDENTIFIER T_DOT import_module_list 
                   ;
 */


 /**
  * var x = 1;
  * var x:i64 = 100;
  * var x:i64 = 100, y = 1, z:string = "hello";
  */
variable_declaration : T_VAR variable_assignment_declaration_list T_SEMI { $$ = new AstVariableDeclaration($2); }
                     ;

variable_assignment_declaration_list : variable_assignment_declaration { $$ = new AstDeclarationList(); $$->add($1); }
                                     | variable_assignment_declaration T_COMMA variable_assignment_declaration_list { $3->add($1); $$ = $3; }
                                     ;

variable_assignment_declaration : T_IDENTIFIER T_ASSIGN constant_expression { $$ = new AstVariableAssignmentDeclaration($1, $3); }
                                ;

 /**
  * func hello() { print("world"); }
  * func hello2() => { print("world"); }
  * func hello3() => print("world");
  * func min(a: i64, b: i64): i64 { return a < b ? a : b; }
  * func min2(a: i64, b: i64): i64 => { return a < b ? a : b; }
  * func max(a: i64, b: i64): i64 => a > b ? a : b;
  * func abs(x: i64): i64 => if (x > 0) return x; else return -x;
  */
function_declaration : T_FUNC T_IDENTIFIER T_LPAREN function_argument_declaration_list T_RPAREN statement {
                            $$ = new AstFunctionDeclaration($2, $4, nullptr, $6);
                            std::free($2);
                        }
                     | T_FUNC T_IDENTIFIER T_LPAREN T_RPAREN statement {
                            $$ = new AstFunctionDeclaration($2, nullptr, nullptr, $5);
                            std::free($2);
                        }
                     | T_FUNC T_IDENTIFIER T_LPAREN function_argument_declaration_list T_RPAREN T_BIG_ARROW statement {
                            $$ = new AstFunctionDeclaration($2, $4, nullptr, $7);
                            std::free($2);
                        }
                     | T_FUNC T_IDENTIFIER T_LPAREN T_RPAREN T_BIG_ARROW statement {
                            $$ = new AstFunctionDeclaration($2, nullptr, nullptr, $6);
                            std::free($2);
                        }
                     ;

function_argument_declaration_list : function_argument_declaration { $$ = new AstDeclarationList(); $$->add($1); }
                                   | function_argument_declaration T_COMMA function_argument_declaration_list { $3->add($1); $$ = $3; }
                                   ;

function_argument_declaration : T_IDENTIFIER { $$ = new AstFunctionArgumentDeclaration($1); std::free($1); }
                              ;

 /* part-3 statement */
compound_statement : T_LBRACE T_RBRACE { $$ = new AstCompoundStatement(nullptr); }
                   | T_LBRACE statement_list T_RBRACE { $$ = new AstCompoundStatement($2); }
                   ;

statement_list : statement { $$ = new AstStatementList(); $$->add($1); }
               | statement statement_list { $2->add($1); $$ = $2; }
               ;

statement : if_statement { $$ = $1; }
          | compound_statement { $$ = $1; }
          | expression_statement { $$ = $1; }
          | iteration_statement { $$ = $1; }
          | jump_statement { $$ = $1; }
          | empty_statement { $$ = $1; }
          /*| switch_statement { $$ = $1; }*/
          /*| switch_body_statement { $$ = $1; }*/
          /*| match_statement { $$ = $1; }*/
          /*| match_body_statement { $$ = $1; }*/
          | declaration { $$ = $1; }
          ;

if_statement : T_IF T_LPAREN expression T_RPAREN statement                  %prec "lower_than_else" { $$ = new AstIfStatement($3, $5, nullptr); }
             | T_IF T_LPAREN expression T_RPAREN statement T_ELSE statement { $$ = new AstIfStatement($3, $5, $7); }
             ;

 /*

switch_statement : T_SWITCH T_LPAREN assignment_expression T_RPAREN statement { $$ = new AstSwitchStatement($3, $5); }
                 ;

switch_body_statement : T_BIT_OR constant_expression T_COLON statement_list { $$ = new AstSwitchBodyStatement($2, $4); }
                      | T_BIT_OR T_UNDERLINE T_COLON statement_list {}
                      ;

match_statement : T_MATCH T_LPAREN argument_expression_list T_RPAREN statement { $$ = new AstMatchStatement($3, $5); }
                ;

match_body_statement : T_BIT_OR assignment_expression T_COLON statement { $$ = new AstMatchBodyStatement($1, $3); }
                     | T_BIT_OR T_UNDERLINE T_COLON statement {}
                     ;
 */

expression_statement : expression T_SEMI { $$ = new AstExpressionStatement($1); }
                     ;

iteration_statement : T_WHILE T_LPAREN expression T_RPAREN statement { $$ = new AstWhileStatement($3, $5); }
                    | T_FOR T_LPAREN expression_statement expression_statement T_RPAREN statement { $$ = new AstForStatement($3, $4, nullptr, $6); }
                    | T_FOR T_LPAREN expression_statement expression_statement expression T_RPAREN statement { $$ = new AstForStatement($3, $4, $5, $7); }
                    ;

jump_statement : T_CONTINUE T_SEMI { $$ = new AstContinueStatement(); }
               | T_BREAK T_SEMI { $$ = new AstBreakStatement(); }
               | T_RETURN T_SEMI { $$ = new AstReturnStatement(nullptr); }
               | T_RETURN expression T_SEMI { $$ = new AstReturnStatement($2); }
               ;

empty_statement : /* */ T_SEMI { $$ = new AstEmptyStatement(); }
                ;

translation_unit : declaration {
                        if (!program) {
                            program = new AstProgram();
                        }
                        program->add($1); 
                    }
                 | declaration translation_unit {
                        if (program) {
                            program->add($1);
                        }
                    }
                 ;

%%

void yyerror(YYLTYPE *yyllocp, AstProgram *&program, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  if (yyllocp && yyllocp->first_line) {
    fprintf(stderr, "%d.%d-%d.%d: error: ", yyllocp->first_line,
            yyllocp->first_column, yyllocp->last_line, yyllocp->last_column);
  }
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

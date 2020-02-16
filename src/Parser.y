%{
#include "Log.h"
#include "Ast.h"
#include "Token.h"
#include "Counter.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <cctype>
#include <unordered_set>
void yyerror(AstDeclarationList **program, char **errorMsg, const char *s) { printf("yyerror: %s:%s\n", s, *errorMsg); }
static Counter counter;
%}

%parse-param { AstDeclarationList **program }
%parse-param { char **errorMsg }

 /* Represents the many different ways we can access our data */
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

 /**
  * union
  */
%token <token> T_EOF
%token <token> T_TRUE T_FALSE T_LET T_VAR T_VAL T_NIL T_IF T_ELSEIF T_ELSE T_FOR T_FOREACH T_IN T_WHILE T_BREAK T_CONTINUE T_SWITCH T_CASE T_DEFAULT
%token <token> T_FUNC T_CLASS T_TYPE T_IS T_ISINSTANCE T_IMPORT T_RETURN T_VOID T_LOGIC_AND T_LOGIC_OR T_LOGIC_NOT
%token <token> T_I8 T_U8 T_I16 T_U16 T_I32 T_U32 T_I64 T_U64 T_F32 T_F64 T_STRING T_BOOLEAN T_ASYNC T_AWAIT
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD T_BIT_NOT T_BIT_AND T_BIT_OR T_BIT_XOR T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ARSHIFT
%token <token> T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%token <token> T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN T_BIT_LSHIFT_ASSIGN T_BIT_RSHIFT_ASSIGN T_BIT_ARSHIFT_ASSIGN
%token <token> T_EQ T_NEQ T_LT T_LE T_GT T_GE
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE T_COMMA T_SEMI T_QUESTION T_COLON T_DOT T_BIG_ARROW

%token <literal> T_IDENTIFIER T_I8_CONSTANT T_U8_CONSTANT T_I16_CONSTANT T_U16_CONSTANT T_I32_CONSTANT T_U32_CONSTANT
%token <literal> T_I64_CONSTANT T_U64_CONSTANT T_F32_CONSTANT T_F64_CONSTANT T_STRING_CONSTANT

%type <expr> postfix_expression primary_expression unary_expression logical_or_expression logical_and_expression
%type <expr> conditional_expression assignment_expression constant_expression bit_and_expression bit_or_expression bit_xor_expression
%type <expr> equality_expression relational_expression shift_expression additive_expression multiplicative_expression
%type <expr> expression
%type <exprList> argument_expression_list 

%type <stmt> compound_statement expression_statement selection_statement iteration_statement jump_statement
%type <stmt> statement
%type <stmtList> statement_list

%type <decl> variable_declaration function_declaration variable_assignment_declaration function_argument_declaration
%type <decl> declaration
%type <declList> variable_assignment_declaration_list translation_unit function_argument_declaration_list

%type <str> join_string_helper

 /**
  * operator precedence
  */

 /* assignment */
%left T_COMMA
%right T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN
%right T_BIT_LSHIFT_ASSIGN T_BIT_RSHIFT_ASSIGN T_BIT_ARSHIFT_ASSIGN
%right T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%right T_ADD_ASSIGN T_SUB_ASSIGN
%right T_ASSIGN
 /* conditional */
%right T_QUESTION T_COLON
 /* comparison */
%left T_LOGIC_OR
%left T_LOGIC_AND
%left T_BIT_OR
%left T_BIT_XOR
%left T_BIT_AND
%left T_EQ T_NEQ
%left T_LT T_LE T_GT T_GE
 /* calculator */
%left T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ARSHIFT
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
 /* other */
%left T_LOGIC_NOT T_BIT_NOT
%left T_DOT
%left T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET

%start translation_unit

%%

 /* part-1 expression */

join_string_helper : T_STRING_CONSTANT { $$ = new AstStringConstant($1); std::free($1); CINFO("join_string_helper: {}", $$->toString()); }
                   | join_string_helper T_STRING_CONSTANT { $$->append($2); std::free($2); CINFO("join_string_helper: {}", $$->toString()); }
                   ;

primary_expression : T_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); CINFO("primary_expression: {}", $$->toString()); }
                   /*| T_I8_CONSTANT { $$ = new AstI8Constant((int8_t)std::stoi($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_U8_CONSTANT { $$ = new AstU8Constant((uint8_t)std::stoul($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_I16_CONSTANT { $$ = new AstI16Constant((int16_t)std::stoi($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_U16_CONSTANT { $$ = new AstU16Constant((uint16_t)std::stoul($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_I32_CONSTANT { $$ = new AstI32Constant((int32_t)std::stol($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_U32_CONSTANT { $$ = new AstU32Constant((uint32_t)std::stoul($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }*/
                   | T_I64_CONSTANT { $$ = new AstI64Constant((int64_t)std::stoll($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }
                   /*| T_U64_CONSTANT { $$ = new AstU64Constant((uint64_t)std::stoull($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_F32_CONSTANT { $$ = new AstF32Constant((float)std::stof($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }*/
                   | T_F64_CONSTANT { $$ = new AstF64Constant((double)std::stod($1)); std::free($1); CINFO("primary_expression: {}", $$->toString()); }
                   | join_string_helper { $$ = $1; CINFO("primary_expression: {}", $$->toString()); }
                   | T_TRUE { $$ = new AstBooleanConstant(true); CINFO("primary_expression: {}", $$->toString()); }
                   | T_FALSE { $$ = new AstBooleanConstant(false); CINFO("primary_expression: {}", $$->toString()); }
                   | T_LPAREN expression T_RPAREN { $$ = $2; CINFO("primary_expression: {}", $$ ? $$->toString() : "null"); }
                   ;

postfix_expression : primary_expression { $$ = $1; CINFO("postfix_expression: {}", $$ ? $$->toString() : "null"); }
                   /*| postfix_expression '[' expression ']'*/
                   | T_IDENTIFIER T_LPAREN T_RPAREN { $$ = new AstCallExpression($1, nullptr); std::free($1); CINFO("postfix_expression: {}", $$->toString()); }
                   | T_IDENTIFIER T_LPAREN argument_expression_list T_RPAREN { $$ = new AstCallExpression($1, $3); std::free($1); CINFO("postfix_expression: {}", $$->toString()); }
                   /*| postfix_expression '.' T_IDENTIFIER */
                   ;

argument_expression_list : assignment_expression { $$ = new AstExpressionList(); $$->add($1); CINFO("argument_expression_list: {}", $$->toString()); }
                         | argument_expression_list T_COMMA assignment_expression { $$->add($3); CINFO("argument_expression_list: {}", $$->toString()); }
                         ;

unary_expression : postfix_expression { $$ = $1; CINFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_ADD unary_expression { $$ = new AstUnaryExpression($1, $2); CINFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_SUB unary_expression { $$ = new AstUnaryExpression($1, $2); CINFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_BIT_NOT unary_expression { $$ = new AstUnaryExpression($1, $2); CINFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_LOGIC_NOT unary_expression { $$ = new AstUnaryExpression($1, $2); CINFO("unary_expression: {}", $$->toString()); }
                 ;

 /*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
 */

multiplicative_expression : unary_expression { $$ = $1; CINFO("multiplicative_expression : {}", $$ ? $$->toString() : "null"); }
                          | multiplicative_expression T_MUL unary_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("multiplicative_expression : {}", $$->toString()); }
                          | multiplicative_expression T_DIV unary_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("multiplicative_expression : {}", $$->toString()); }
                          | multiplicative_expression T_MOD unary_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("multiplicative_expression : {}", $$->toString()); }
                          ;

additive_expression : multiplicative_expression { $$ = $1; CINFO("additive_expression: {}", $$ ? $$->toString() : "null"); }
                    | additive_expression T_ADD multiplicative_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("additive_expression: {}", $$->toString()); }
                    | additive_expression T_SUB multiplicative_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("additive_expression: {}", $$->toString()); }
                    ;

shift_expression : additive_expression { $$ = $1; CINFO("shift_expression: {}", $$ ? $$->toString() : "null"); }
                 | shift_expression T_BIT_LSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("shift_expression: {}", $$->toString()); }
                 | shift_expression T_BIT_RSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("shift_expression: {}", $$->toString()); }
                 | shift_expression T_BIT_ARSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("shift_expression: {}", $$->toString()); }
                 ;

relational_expression : shift_expression { $$ = $1; CINFO("relational_expression: {}", $$ ? $$->toString() : "null"); }
                      | relational_expression T_LT shift_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_LE shift_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_GT shift_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_GE shift_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("relational_expression: {}", $$->toString()); }
                      ;

equality_expression : relational_expression { $$ = $1; CINFO("equality_expression: {}", $$ ? $$->toString() : "null"); }
                    | equality_expression T_EQ relational_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("equality_expression: {}", $$->toString()); }
                    | equality_expression T_NEQ relational_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("equality_expression: {}", $$->toString()); }
                    ;

bit_and_expression : equality_expression { $$ = $1; CINFO("bit_and_expression: {}", $$ ? $$->toString() : "null"); }
                   | bit_and_expression T_BIT_AND equality_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("bit_and_expression: {}", $$->toString()); }
                   ;

bit_xor_expression : bit_and_expression { $$ = $1; CINFO("bit_xor_expression: {}", $$ ? $$->toString() : "null"); }
                   | bit_xor_expression T_BIT_XOR bit_and_expression { $$ = new AstBinaryExpression($1, $2, $3); break; CINFO("bit_xor_expression: {}", $$->toString()); }
                   ;

bit_or_expression : bit_xor_expression { $$ = $1; CINFO("bit_or_expression: {}", $$ ? $$->toString() : "null"); }
                  | bit_or_expression T_BIT_OR bit_xor_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("bit_or_expression: {}", $$->toString()); }
                  ;

logical_and_expression : bit_or_expression { $$ = $1; CINFO("logical_and_expression: {}", $$ ? $$->toString() : "null"); }
                       | logical_and_expression T_LOGIC_AND bit_or_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("logical_and_expression: {}", $$->toString()); }
                       ;

logical_or_expression : logical_and_expression { $$ = $1; CINFO("logical_or_expression: {}", $$ ? $$->toString() : "null"); }
                      | logical_or_expression T_LOGIC_OR logical_and_expression { $$ = new AstBinaryExpression($1, $2, $3); CINFO("logical_or_expression: {}", $$->toString()); }
                      ;

conditional_expression : logical_or_expression { $$ = $1; CINFO("conditional_expression: {}", $$ ? $$->toString() : "null"); }
                       | logical_or_expression T_QUESTION expression T_COLON conditional_expression {
                                $$ = new AstConditionalExpression($1, $3, $5); CINFO("conditional_expression: {}", $$->toString());
                            }
                       ;

 /**
  * 1. allow multiple assignment in an expression: x, y, z = 1, "hello", 2.3
  * 2. disallow multiple assignment as a chain: x = y = z = 1
  */
assignment_expression : conditional_expression { $$ = $1; CINFO("assignment_expression: {}", $$ ? $$->toString() : "null"); }
                      | unary_expression T_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_MUL_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_DIV_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_MOD_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_ADD_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_SUB_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_BIT_AND_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_BIT_OR_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_BIT_XOR_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_BIT_LSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_BIT_RSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression T_BIT_ARSHIFT_ASSIGN assignment_expression { $$ = new AstAssignmentExpression($1, $2, $3); CINFO("assignment_expression: {}", $$->toString()); }
                      ;

expression : /* nothing */ { $$ = new AstEmptyExpression(); CINFO("expression: null"); }
           | assignment_expression { $$ = $1; CINFO("expression: {}", $$ ? $$->toString() : "null"); }
           | expression T_COMMA assignment_expression { $$ = $3; CINFO("expression: {}", $$ ? $$->toString() : "null"); }
           ;

constant_expression : conditional_expression { $$ = $1; CINFO("constant_expression: {}", $$ ? $$->toString() : "null"); }
                    ;

 /* part-2 declaration */

declaration : function_declaration { $$ = $1; CINFO("declaration: {}", $$ ? $$->toString() : "null"); }
            | variable_declaration { $$ = $1; CINFO("declaration: {}", $$ ? $$->toString() : "null"); }
            ;

 /**
  * var x = 1;
  * var x:i64 = 100;
  * var x:i64 = 100, y = 1, z:string = "hello";
  */
variable_declaration : T_VAR variable_assignment_declaration_list T_SEMI { $$ = new AstVariableDeclaration($2); CINFO("variable_declaration: {}", $$->toString()); }
                     ;

variable_assignment_declaration_list : variable_assignment_declaration { $$ = new AstDeclarationList(); $$->add($1); CINFO("variable_assignment_declaration_list: {}", $$->toString()); }
                                     | variable_assignment_declaration_list T_COMMA variable_assignment_declaration { $$->add($3); CINFO("variable_assignment_declaration_list: {}", $$->toString()); }
                                     ;

variable_assignment_declaration : T_IDENTIFIER T_ASSIGN constant_expression { $$ = new AstVariableAssignmentDeclaration($1, $3); CINFO("variable_assignment_declaration: {}", $$->toString()); }
                                ;

 /**
  * func hello() { print("world"); }
  * func hello2() => { print("world"); }
  * func hello3() => print("world");
  * func min(a: i64, b: i64): i64 { return a < b ? a : b; }
  * func max(a: i64, b: i64): i64 => a > b ? a : b;
  * func abs(x: i64): i64 => { if (x > 0) { return x; } else { return -x; } }
  * val x = func() => { print("hello world"); };
  * val y = func(x:i64, y:i64):i64 => { return x + y; };
  */
function_declaration : T_FUNC T_IDENTIFIER T_LPAREN function_argument_declaration_list T_RPAREN function_declaration_arrow statement {
                            $$ = new AstFunctionDeclaration($2, $4, nullptr, $7);
                            std::free($2);
                            CINFO("function_declaration: {}", $$->toString());
                        }
                     | T_FUNC T_IDENTIFIER T_LPAREN T_RPAREN function_declaration_arrow statement {
                            $$ = new AstFunctionDeclaration($2, nullptr, nullptr, $6);
                            std::free($2);
                            CINFO("function_declaration: {}", $$->toString());
                        }
                     /*| T_FUNC T_IDENTIFIER T_LPAREN function_argument_declaration_list T_RPAREN function_declaration_arrow statement {*/
                            /*AstStatementList *statementList = new AstStatementList();*/
                            /*statementList->add($7);*/
                            /*AstCompoundStatement *compoundStatement = new AstCompoundStatement(statementList);*/
                            /*$$ = new AstFunctionDeclaration($2, $4, compoundStatement);*/
                            /*std::free($2);*/
                            /*CINFO("function_declaration: {}", $$->toString());*/
                        /*}*/
                     /*| T_FUNC T_IDENTIFIER T_LPAREN T_RPAREN function_declaration_arrow statement {*/
                            /*AstStatementList *statementList = new AstStatementList();*/
                            /*statementList->add($6);*/
                            /*AstCompoundStatement *compoundStatement = new AstCompoundStatement(statementList);*/
                            /*$$ = new AstFunctionDeclaration($2, nullptr, compoundStatement);*/
                            /*std::free($2);*/
                            /*CINFO("function_declaration: {}", $$->toString());*/
                        /*}*/
                     ;

function_argument_declaration_list : function_argument_declaration {
                                        $$ = new AstDeclarationList(); 
                                        $$->add($1); 
                                        CINFO("function_argument_declaration_list: {}", $$->toString());
                                    }
                                   | function_argument_declaration_list T_COMMA function_argument_declaration { 
                                        $$->add($3); 
                                        CINFO("function_argument_declaration_list: {}", $$->toString()); 
                                    }
                                   ;

function_argument_declaration : T_IDENTIFIER { $$ = new AstFunctionArgumentDeclaration($1); std::free($1); CINFO("function_argument_declaration: {}", $$->toString()); }
                              ;

function_declaration_arrow : /* nothing */
                           | T_BIG_ARROW
                           ;

 /* part-3 statement */
compound_statement : T_LBRACE T_RBRACE { $$ = new AstCompoundStatement(nullptr); CINFO("compound_statement: {}", $$->toString()); }
                   | T_LBRACE statement_list T_RBRACE { $$ = new AstCompoundStatement($2); CINFO("compound_statement: {}", $$->toString()); }
                   ;

statement_list : statement { $$ = new AstStatementList(); $$->add($1); CINFO("statement_list: {}", $$->toString()); }
               | statement_list statement { $$->add($2); CINFO("statement_list: {}", $$->toString()); }
               ;

statement : compound_statement { $$ = $1; CINFO("statement: {}", $$ ? $$->toString() : "null"); }
          | expression_statement { $$ = $1; CINFO("statement: {}", $$ ? $$->toString() : "null"); }
          | selection_statement { $$ = $1; CINFO("statement: {}", $$ ? $$->toString() : "null"); }
          | iteration_statement { $$ = $1; CINFO("statement: {}", $$ ? $$->toString() : "null"); }
          | jump_statement { $$ = $1; CINFO("statement: {}", $$ ? $$->toString() : "null"); }
          | declaration { $$ = $1; CINFO("statement: {}", $$ ? $$->toString() : "null"); }
          ;

expression_statement : expression T_SEMI { $$ = new AstExpressionStatement($1); CINFO("expression_statement: {}", $$ ? $$->toString() : "null"); }
                     ;

selection_statement : T_IF T_LPAREN expression T_RPAREN statement { $$ = new AstIfStatement($3, $5, nullptr); CINFO("selection_statement: {}", $$->toString()); }
                    | T_IF T_LPAREN expression T_RPAREN statement T_ELSE statement { $$ = new AstIfStatement($3, $5, $7); CINFO("selection_statement: {}", $$->toString()); }
                    /*| T_SWITCH T_LPAREN expression T_RPAREN statement*/
                    ;

iteration_statement : T_WHILE T_LPAREN expression T_RPAREN statement { $$ = new AstWhileStatement($3, $5); CINFO("iteration_statement: {}", $$->toString()); }
                    | T_FOR T_LPAREN expression_statement expression_statement T_RPAREN statement { $$ = new AstForStatement($3, $4, nullptr, $6); CINFO("iteration_statement: {}", $$->toString()); }
                    | T_FOR T_LPAREN expression_statement expression_statement expression T_RPAREN statement { $$ = new AstForStatement($3, $4, $5, $7); CINFO("iteration_statement: {}", $$->toString()); }
                    ;

jump_statement : T_CONTINUE T_SEMI { $$ = new AstContinueStatement(); CINFO("jump_statement: {}", $$->toString()); }
               | T_BREAK T_SEMI { $$ = new AstBreakStatement(); CINFO("jump_statement: {}", $$->toString()); }
               | T_RETURN T_SEMI { $$ = new AstReturnStatement(nullptr); CINFO("jump_statement: {}", $$->toString()); }
               | T_RETURN expression T_SEMI { $$ = new AstReturnStatement($2); CINFO("jump_statement: {}", $$->toString()); }
               ;

translation_unit : declaration { $$ = new AstDeclarationList(); $$->add($1); CINFO("translation_unit: {}", $$->toString()); }
                 | translation_unit declaration { $$->add($2); CINFO("translation_unit: {}", $$->toString()); }
                 ;

%%

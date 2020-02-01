%{
#include "config/Header.h"
#include "config/Platform.h"
#include "Log.h"
#include "Ast.h"
#include "Token.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <cctype>
#include <unordered_set>
void yyerror(const char *s) { printf("yyerror: %s\n", s); }

static std::unordered_set<int> IntConstants = {
    A_I8_CONSTANT,  A_UI8_CONSTANT,
    A_I16_CONSTANT, A_UI16_CONSTANT,
    A_I32_CONSTANT, A_UI32_CONSTANT,
    A_I64_CONSTANT, A_UI64_CONSTANT,
};
static std::unordered_set<int> FloatConstants = {
    A_F32_CONSTANT,  A_F64_CONSTANT,
};
static std::unordered_set<int> NumberConstants = {
    A_I8_CONSTANT,  A_UI8_CONSTANT,
    A_I16_CONSTANT, A_UI16_CONSTANT,
    A_I32_CONSTANT, A_UI32_CONSTANT,
    A_I64_CONSTANT, A_UI64_CONSTANT,
    A_F32_CONSTANT, A_F64_CONSTANT,
};

%}

 /* Represents the many different ways we can access our data */
%union {
    AstExpression *expression;
    AstStatement *statement;
    AstDeclaration *declaration;
    AstExpressionList *expressionList;
    AstStatementList *statementList;
    AstProgram *program;
    char *literal;
    int token;
}

 /**
  * union
  */
%token <token> T_EOF
%token <token> T_TRUE T_FALSE T_LET T_NIL T_IF T_ELSEIF T_ELSE T_FOR T_WHILE T_BREAK T_CONTINUE T_SWITCH T_CASE T_DEFAULT T_BIT
%token <token> T_FUNC T_CLASS T_TYPE T_ISINSTANCE T_IMPORT T_RETURN T_VOID T_LOGIC_AND T_LOGIC_OR T_LOGIC_NOT
%token <token> T_I8 T_UI8 T_I16 T_UI16 T_I32 T_UI32 T_I64 T_UI64 T_F32 T_F64 T_STRING T_BOOLEAN
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD T_BIT_NOT T_BIT_AND T_BIT_OR T_BIT_XOR T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ZERORSHIFT
%token <token> T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MUL_ASSIGN T_DIV_ASSIGN T_MOD_ASSIGN
%token <token> T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN T_BIT_LSHIT_ASSIGN T_BIT_RSHIT_ASSIGN T_BIT_ZERORSHIT_ASSIGN
%token <token> T_EQ T_NEQ T_LT T_LE T_GT T_GE
%token <token> T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET T_LBRACE T_RBRACE T_COMMA T_SEMI T_QUESTION T_COLON T_DOT

%token <literal> T_IDENTIFIER T_I8_CONSTANT T_UI8_CONSTANT T_I16_CONSTANT T_UI16_CONSTANT T_I32_CONSTANT T_UI32_CONSTANT
%token <literal> T_I64_CONSTANT T_UI64_CONSTANT T_F32_CONSTANT T_F64_CONSTANT T_STRING_CONSTANT

%type <expression> postfix_expression primary_expression unary_expression logical_or_expression logical_and_expression
%type <expression> conditional_expression assignment_expression constant_expression bit_and_expression bit_or_expression bit_xor_expression
%type <expression> equality_expression relational_expression shift_expression additive_expression multiplicative_expression
%type <expression> expression

%type <statement> compound_statement expression_statement selection_statement iteration_statement jump_statement
%type <statement> statement statement_or_declaration 

%type <declaration> declaration variable_declaration function_declaration

%type <expressionList> argument_expression_list conditional_expression_list unary_expression_list
%type <expressionList> function_argument_list variable_declaration_name_list variable_declaration_expression_list

%type <statementList> statement_or_declaration_list

%type <program> translation_unit

 /**
  * operator precedence
  */

 /* assignment */
%left T_COMMA
%right T_BIT_AND_ASSIGN T_BIT_OR_ASSIGN T_BIT_XOR_ASSIGN
%right T_BIT_LSHIT_ASSIGN T_BIT_RSHIT_ASSIGN T_BIT_ZERORSHIT_ASSIGN
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
%left T_BIT_LSHIFT T_BIT_RSHIFT T_BIT_ZERORSHIFT
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD
 /* other */
%left T_LOGIC_NOT T_BIT_NOT
%left T_DOT
%left T_LPAREN T_RPAREN T_LBRACKET T_RBRACKET

%start translation_unit

%%

 /* part-1 expression */

primary_expression : T_IDENTIFIER { $$ = new AstIdentifierConstant($1); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }
                   /*| T_I8_CONSTANT { $$ = new AstI8Constant((int8_t)std::stoi($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_UI8_CONSTANT { $$ = new AstUI8Constant((uint8_t)std::stoul($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_I16_CONSTANT { $$ = new AstI16Constant((int16_t)std::stoi($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_UI16_CONSTANT { $$ = new AstUI16Constant((uint16_t)std::stoul($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_I32_CONSTANT { $$ = new AstI32Constant((int32_t)std::stol($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_UI32_CONSTANT { $$ = new AstUI32Constant((uint32_t)std::stoul($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }*/
                   | T_I64_CONSTANT { $$ = new AstI64Constant((int64_t)std::stoll($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }
                   /*| T_UI64_CONSTANT { $$ = new AstUI64Constant((uint64_t)std::stoull($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }*/
                   /*| T_F32_CONSTANT { $$ = new AstF32Constant((float)std::stof($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }*/
                   | T_F64_CONSTANT { $$ = new AstF64Constant((double)std::stod($1)); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }
                   | T_STRING_CONSTANT { $$ = new AstStringConstant($1); std::free($1); LOG_INFO("primary_expression: {}", $$->toString()); }
                   | T_TRUE { $$ = new AstBooleanConstant(true); LOG_INFO("primary_expression: {}", $$->toString()); }
                   | T_FALSE { $$ = new AstBooleanConstant(false); LOG_INFO("primary_expression: {}", $$->toString()); }
                   | T_LPAREN expression T_RPAREN { $$ = $2; LOG_INFO("primary_expression: {}", $$ ? $$->toString() : "null"); }
                   ;

postfix_expression : primary_expression { $$ = $1; LOG_INFO("postfix_expression: {}", $$ ? $$->toString() : "null"); }
                   /*| postfix_expression '[' expression ']'*/
                   | T_IDENTIFIER T_LPAREN T_RPAREN { $$ = new AstFunctionCallExpression($1, nullptr); std::free($1); LOG_INFO("postfix_expression: {}", $$->toString()); }
                   | T_IDENTIFIER T_LPAREN argument_expression_list T_RPAREN { $$ = new AstFunctionCallExpression($1, $3); std::free($1); LOG_INFO("postfix_expression: {}", $$->toString()); }
                   /*| postfix_expression '.' T_IDENTIFIER */
                   ;

argument_expression_list : conditional_expression { $$ = new AstExpressionList(); $$->add($1); LOG_INFO("argument_expression_list: {}", $$->toString()); }
                         | argument_expression_list T_COMMA conditional_expression { $$->add($3); LOG_INFO("argument_expression_list: {}", $$->toString()); }
                         ;

unary_expression : postfix_expression { $$ = $1; LOG_INFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_ADD unary_expression { $$ = new AstUnaryExpression($1, $2); LOG_INFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_SUB unary_expression { $$ = new AstUnaryExpression($1, $2); LOG_INFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_BIT_NOT unary_expression { $$ = new AstUnaryExpression($1, $2); LOG_INFO("unary_expression: {}", $$ ? $$->toString() : "null"); }
                 | T_LOGIC_NOT unary_expression { $$ = new AstUnaryExpression($1, $2); LOG_INFO("unary_expression: {}", $$->toString()); }
                 ;

 /*
cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;
 */

multiplicative_expression : unary_expression { $$ = $1; LOG_INFO("multiplicative_expression : {}", $$ ? $$->toString() : "null"); }
                          | multiplicative_expression T_MUL unary_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("multiplicative_expression : {}", $$->toString()); }
                          | multiplicative_expression T_DIV unary_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("multiplicative_expression : {}", $$->toString()); }
                          | multiplicative_expression T_MOD unary_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("multiplicative_expression : {}", $$->toString()); }
                          ;

additive_expression : multiplicative_expression { $$ = $1; LOG_INFO("additive_expression: {}", $$ ? $$->toString() : "null"); }
                    | additive_expression T_ADD multiplicative_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("additive_expression: {}", $$->toString()); }
                    | additive_expression T_SUB multiplicative_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("additive_expression: {}", $$->toString()); }
                    ;

shift_expression : additive_expression { $$ = $1; LOG_INFO("shift_expression: {}", $$ ? $$->toString() : "null"); }
                 | shift_expression T_BIT_LSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("shift_expression: {}", $$->toString()); }
                 | shift_expression T_BIT_RSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("shift_expression: {}", $$->toString()); }
                 | shift_expression T_BIT_ZERORSHIFT additive_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("shift_expression: {}", $$->toString()); }
                 ;

relational_expression : shift_expression { $$ = $1; LOG_INFO("relational_expression: {}", $$ ? $$->toString() : "null"); }
                      | relational_expression T_LT shift_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_LE shift_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_GT shift_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("relational_expression: {}", $$->toString()); }
                      | relational_expression T_GE shift_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("relational_expression: {}", $$->toString()); }
                      ;

equality_expression : relational_expression { $$ = $1; LOG_INFO("equality_expression: {}", $$ ? $$->toString() : "null"); }
                    | equality_expression T_EQ relational_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("equality_expression: {}", $$->toString()); }
                    | equality_expression T_NEQ relational_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("equality_expression: {}", $$->toString()); }
                    ;

bit_and_expression : equality_expression { $$ = $1; LOG_INFO("bit_and_expression: {}", $$ ? $$->toString() : "null"); }
                   | bit_and_expression T_BIT_AND equality_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("bit_and_expression: {}", $$->toString()); }
                   ;

bit_xor_expression : bit_and_expression { $$ = $1; LOG_INFO("bit_xor_expression: {}", $$ ? $$->toString() : "null"); }
                   | bit_xor_expression T_BIT_XOR bit_and_expression { $$ = new AstBinaryExpression($1, $2, $3); break; LOG_INFO("bit_xor_expression: {}", $$->toString()); }
                   ;

bit_or_expression : bit_xor_expression { $$ = $1; LOG_INFO("bit_or_expression: {}", $$ ? $$->toString() : "null"); }
                  | bit_or_expression T_BIT_OR bit_xor_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("bit_or_expression: {}", $$->toString()); }
                  ;

logical_and_expression : bit_or_expression { $$ = $1; LOG_INFO("logical_and_expression: {}", $$ ? $$->toString() : "null"); }
                       | logical_and_expression T_LOGIC_AND bit_or_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("logical_and_expression: {}", $$->toString()); }
                       ;

logical_or_expression : logical_and_expression { $$ = $1; LOG_INFO("logical_or_expression: {}", $$ ? $$->toString() : "null"); }
                      | logical_or_expression T_LOGIC_OR logical_and_expression { $$ = new AstBinaryExpression($1, $2, $3); LOG_INFO("logical_or_expression: {}", $$->toString()); }
                      ;

conditional_expression : logical_or_expression { $$ = $1; LOG_INFO("conditional_expression: {}", $$ ? $$->toString() : "null"); }
                       | logical_or_expression T_QUESTION expression T_COLON conditional_expression {
                                $$ = new AstConditionalExpression($1, $3, $5); LOG_INFO("conditional_expression: {}", $$->toString());
                            }
                       ;

 /**
  * 1. allow multiple assignment in an expression: x, y, z = 1, "hello", 2.3
  * 2. disallow multiple assignment as a chain: x = y = z = 1
  */
assignment_expression : conditional_expression { $$ = $1; LOG_INFO("assignment_expression: {}", $$ ? $$->toString() : "null"); }
                      | unary_expression_list T_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_MUL_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_DIV_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_MOD_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_ADD_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_SUB_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_AND_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_OR_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_XOR_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_LSHIT_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_RSHIT_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      | unary_expression_list T_BIT_ZERORSHIT_ASSIGN conditional_expression_list { $$ = new AstAssignmentExpression($1, $2, $3); LOG_INFO("assignment_expression: {}", $$->toString()); }
                      ;

unary_expression_list : unary_expression { 
                            $$ = new AstExpressionList(); 
                            $$->add($1); 
                            LOG_INFO("unary_expression_list: {}", $$->toString()); 
                        }
                      | unary_expression_list T_COMMA unary_expression { 
                            $$->add($3); 
                            LOG_INFO("unary_expression_list: {}", $$->toString()); 
                        }
                      ;

conditional_expression_list : conditional_expression { 
                                    $$ = new AstExpressionList(); 
                                    $$->add($1); 
                                    LOG_INFO("conditional_expression_list: {}", $$->toString()); 
                                }
                            | conditional_expression_list T_COMMA conditional_expression { 
                                    $$->add($3); 
                                    LOG_INFO("conditional_expression_list: {}", $$->toString()); 
                                }
                            ;

expression : /* nothing */ { $$ = nullptr; LOG_INFO_MSG("expression: null"); }
           | assignment_expression { $$ = $1; LOG_INFO("expression: {}", $$ ? $$->toString() : "null"); }
           ;

constant_expression : conditional_expression { $$ = $1; LOG_INFO("constant_expression: {}", $$ ? $$->toString() : "null"); }
                    ;

 /* part-2 declaration */

declaration : function_declaration { $$ = $1; LOG_INFO("declaration: {}", $$ ? $$->toString() : "null"); }
            | variable_declaration { $$ = $1; LOG_INFO("declaration: {}", $$ ? $$->toString() : "null"); }
            ;

 /**
  * let x:i64, y:string, z:f64 = 1, "hello world", 2.3;
  */
variable_declaration : T_LET variable_declaration_name_list T_ASSIGN variable_declaration_expression_list T_SEMI { $$ = new AstVariableDeclaration($2, $4); LOG_INFO("variable_declaration: {}", $$->toString()); }
                     ;

variable_declaration_name_list : T_IDENTIFIER {
                                        $$ = new AstExpressionList(); 
                                        $$->add(new AstIdentifierConstant($1));
                                        std::free($1);
                                        LOG_INFO("variable_declaration_name_list: {}", $$->toString()); 
                                    }
                               | variable_declaration_name_list T_COMMA T_IDENTIFIER { 
                                        $$->add(new AstIdentifierConstant($3)); 
                                        std::free($3); 
                                        LOG_INFO("variable_declaration_name_list: {}", $$->toString()); 
                                    }
                               ;

variable_declaration_expression_list : constant_expression { 
                                            $$ = new AstExpressionList();
                                            $$->add($1);
                                            LOG_INFO("variable_declaration_expression_list: {}", $$->toString());
                                        }
                                     | variable_declaration_expression_list T_COMMA constant_expression {
                                            $$->add($3);
                                            LOG_INFO("variable_declaration_expression_list: {}", $$->toString());
                                        }
                                     ;

 /**
  * func hello() { print("world"); }
  * func min(a: i64, b: i64): i64 { return a < b ? a : b; }
  * func max(a: i64, b: i64): i64 { return a > b ? a : b; }
  * func abs(x: i64): i64 { if (x > 0) { return x; } else { return -x; } }
  * let x = func() { print("hello world"); };
  * let y = func(x: i64, y: i64): i64 { return x + y; };
  */
function_declaration : T_FUNC T_IDENTIFIER T_LPAREN function_argument_list T_RPAREN compound_statement {
                            $$ = new AstFunctionDeclaration($2, $4, dynamic_cast<AstCompoundStatement*>($6));
                            std::free($2);
                            LOG_INFO("function_declaration: {}", $$->toString());
                        }
                     | T_FUNC T_IDENTIFIER T_LPAREN T_RPAREN compound_statement {
                            $$ = new AstFunctionDeclaration($2, nullptr, dynamic_cast<AstCompoundStatement*>($5));
                            std::free($2);
                            LOG_INFO("function_declaration: {}", $$->toString());
                        }
                     /*| T_FUNC T_IDENTIFIER T_LPAREN function_argument_list T_RPAREN T_ASSIGN statement {*/
                            /*AstStatementList *statementList = new AstStatementList();*/
                            /*statementList->add($7);*/
                            /*AstCompoundStatement *compoundStatement = new AstCompoundStatement(statementList);*/
                            /*$$ = new AstFunctionDeclaration($2, $4, compoundStatement);*/
                            /*std::free($2);*/
                            /*LOG_INFO("function_declaration: {}", $$->toString());*/
                        /*}*/
                     /*| T_FUNC T_IDENTIFIER T_LPAREN T_RPAREN T_ASSIGN statement {*/
                            /*AstStatementList *statementList = new AstStatementList();*/
                            /*statementList->add($6);*/
                            /*AstCompoundStatement *compoundStatement = new AstCompoundStatement(statementList);*/
                            /*$$ = new AstFunctionDeclaration($2, nullptr, compoundStatement);*/
                            /*std::free($2);*/
                            /*LOG_INFO("function_declaration: {}", $$->toString());*/
                        /*}*/
                     ;

function_argument_list : T_IDENTIFIER { $$ = new AstExpressionList(); $$->add(new AstIdentifierConstant($1)); std::free($1); LOG_INFO("function_argument_list: {}", $$->toString()); }
                       | function_argument_list T_COMMA T_IDENTIFIER { $$->add(new AstIdentifierConstant($3)); std::free($3); LOG_INFO("function_argument_list: {}", $$->toString()); }
                       ;

 /* part-3 statement */
compound_statement : T_LBRACE T_RBRACE { $$ = new AstCompoundStatement(nullptr); LOG_INFO("compound_statement: {}", $$->toString()); }
                   | T_LBRACE statement_or_declaration_list T_RBRACE { $$ = new AstCompoundStatement($2); LOG_INFO("compound_statement: {}", $$->toString()); }
                   ;

statement_or_declaration_list : statement_or_declaration { $$ = new AstStatementList(); $$->add($1); LOG_INFO("statement_or_declaration_list: {}", $$->toString()); }
                              | statement_or_declaration_list statement_or_declaration { $$->add($2); LOG_INFO("statement_or_declaration_list: {}", $$->toString()); }
                              ;

statement_or_declaration : statement { $$ = $1; LOG_INFO("statement_or_declaration: {}", $$ ? $$->toString() : "null"); }
                         | declaration { $$ = $1; LOG_INFO("statement_or_declaration: {}", $$ ? $$->toString() : "null"); }
                         ;

statement : compound_statement { $$ = $1; LOG_INFO("statement: {}", $$ ? $$->toString() : "null"); }
          | expression_statement { $$ = $1; LOG_INFO("statement: {}", $$ ? $$->toString() : "null"); }
          | selection_statement { $$ = $1; LOG_INFO("statement: {}", $$ ? $$->toString() : "null"); }
          | iteration_statement { $$ = $1; LOG_INFO("statement: {}", $$ ? $$->toString() : "null"); }
          | jump_statement { $$ = $1; LOG_INFO("statement: {}", $$ ? $$->toString() : "null"); }
          ;

expression_statement : expression T_SEMI { $$ = new AstExpressionStatement($1); LOG_INFO("expression_statement: {}", $$ ? $$->toString() : "null"); }
                     ;

selection_statement : T_IF T_LPAREN expression T_RPAREN statement { $$ = new AstIfStatement($3, $5, nullptr); LOG_INFO("selection_statement: {}", $$->toString()); }
                    | T_IF T_LPAREN expression T_RPAREN statement T_ELSE statement { $$ = new AstIfStatement($3, $5, $7); LOG_INFO("selection_statement: {}", $$->toString()); }
                    /*| T_SWITCH T_LPAREN expression T_RPAREN statement*/
                    ;

iteration_statement : T_WHILE T_LPAREN expression T_RPAREN statement { $$ = new AstWhileStatement($3, $5); LOG_INFO("iteration_statement: {}", $$->toString()); }
                    | T_FOR T_LPAREN expression_statement expression_statement T_RPAREN statement { $$ = new AstForStatement($3, $4, nullptr, $6); LOG_INFO("iteration_statement: {}", $$->toString()); }
                    | T_FOR T_LPAREN expression_statement expression_statement expression T_RPAREN statement { $$ = new AstForStatement($3, $4, $5, $7); LOG_INFO("iteration_statement: {}", $$->toString()); }
                    ;

jump_statement : T_CONTINUE T_SEMI { $$ = new AstContinueStatement(); LOG_INFO("jump_statement: {}", $$->toString()); }
               | T_BREAK T_SEMI { $$ = new AstBreakStatement(); LOG_INFO("jump_statement: {}", $$->toString()); }
               | T_RETURN T_SEMI { $$ = new AstReturnStatement(nullptr); LOG_INFO("jump_statement: {}", $$->toString()); }
               | T_RETURN expression T_SEMI { $$ = new AstReturnStatement($2); LOG_INFO("jump_statement: {}", $$->toString()); }
               ;

translation_unit : declaration {
                        AstProgram *save = AstProgram::resetInstance(new AstProgram());
                        AstProgram::instance()->add($1);
                        LOG_INFO("translation_unit: {}, save: {}", AstProgram::instance()->toString(), save ? save->toString() : "null");
                        if (save) delete save;
                    }
                 | translation_unit declaration { AstProgram::instance()->add($2); LOG_INFO("translation_unit: {}", AstProgram::instance()->toString()); }
                 ;

%%

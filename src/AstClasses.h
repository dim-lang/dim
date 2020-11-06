// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once

/* ast */
class Ast;

/* literal */
class A_Integer;
class A_Float;
class A_Boolean;
class A_Character;
class A_String;
class A_Nil;
class A_Void;

/* id */
// class AstId;
class A_VarId;

/* expression without block */
class A_Throw;
class A_Return;
class A_Break;
class A_Continue;
class A_Assign;
class A_Postfix;
class A_Infix;
class A_Prefix;

/* expression with block */
class A_Call;
class A_Exprs;
class A_If;
class A_Loop;
class A_Yield;
class A_LoopCondition;
class A_LoopEnumerator;
class A_DoWhile;
class A_Try;
class A_Block;
class A_BlockStats;

/* type */
// class AstType;
class A_PlainType;

/* declaration and definition */
class A_FuncDef;
class A_FuncSign;
class A_Params;
class A_Param;
class A_VarDef;

/* compile unit */
class A_TopStats;
class A_CompileUnit;

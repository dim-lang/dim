// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "enum.h"

BETTER_ENUM(IrKind, int,
            // module
            Mod = 5000,
            // global variable
            GlobalVar,
            // function
            FuncDecl, FuncDef, Param, Block,
            // expression
            Var, ConstantExpr, Expr)

/*================ class ================*/

class Ir;
class I_Mod;
class I_GlobalVar;
class I_FuncDecl;
class I_FuncDef;
class I_Param;
class I_Block;
class I_Var;
class I_ConstantExpr;
class I_Expr;

class Ir {};

class I_Mod : public Ir {};

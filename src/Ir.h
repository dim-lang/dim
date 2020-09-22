// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "enum.h"

/*================ kind start from 5000 ================*/
BETTER_ENUM(IrKind, int,
            // literal
            Module = 4000,
            // function
            FuncDef, FuncDecl,
            // call
            Call, Constant, BinaryOp, UnaryOp, Loop, Condition, Return)

/*================ class ================*/

/* class list */
class Ir;
class I_Module;
class I_Function;
class I_Call;
class I_Constant;
class I_BinaryOp;
class I_UnaryOp;
class I_Condition;
class I_Loop;
class I_Return;

// Ir {

class Ir {
public:
  virtual ~Ir() = default;
};

// Ir }

class I_Module : public Ir {};

class I_Function : public Ir {};

class I_Condition : public Ir {};

class I_Call : public Ir {};

class I_BinaryOp : public Ir {};

class I_UnaryOp : public Ir {};

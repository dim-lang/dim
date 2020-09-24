// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Cowstr.h"
#include "Name.h"
#include "enum.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <memory>

class Ast;
class Symbol;
class TypeSymbol;
class Scope;

/*================ kind start from 5000 ================*/
BETTER_ENUM(IrKind, int,
            // literal
            Module = 4000,
            // function
            FuncDef, FuncDecl,
            // variable
            GVarDef, VarDef,
            // call
            Call, Constant, BinaryOp, UnaryOp, Loop, Condition, Return)

/*================ class ================*/

/* class list */
class Ir;
class I_Module;
class I_FuncDef;
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
  virtual Cowstr str() const = 0;
};

// Ir }

class I_Module : public Ir, public Nameable {
public:
  I_Module(Ast *a_ast, std::shared_ptr<Scope> a_scope, const Cowstr &a_name);
  virtual ~I_Module() = default;
  virtual Cowstr str() const;

  Ast *ast;
  std::shared_ptr<Scope> scope;

  llvm::LLVMContext llvmContext;
  llvm::IRBuilder<> llvmIrBuilder;
  llvm::Module *llvmModule;

  std::vector<std::shared_ptr<Ir>> functions;
  std::vector<std::shared_ptr<Ir>> globalVariables;
};

// class I_FuncDecl : public Ir, public Nameable {};

class I_FuncDef : public Ir, public Nameable {
public:
  I_FuncDef(I_Module *a_iModule, Ast *a_ast, std::shared_ptr<Scope> a_scope,
            const Cowstr &a_name);
  virtual ~I_FuncDef() = default;
  virtual Cowstr str() const;

  I_Module *iModule;

  Ast *ast;
  std::shared_ptr<Scope> scope;

  std::vector<std::shared_ptr<Ir>> parameters;
  std::vector<std::shared_ptr<Ir>> basicBlocks;
};

class IrGVarDef : public Ir {
public:
  virtual ~IrGVarDef() = default;
  virtual Cowstr str() const = 0;
};

class IrVarDef : public Ir {
public:
  virtual ~IrVarDef() = default;
  virtual Cowstr str() const = 0;
};

// boolean
// byte, ubyte
// short, ushort
// int, uint
// long, ulong
class I_IGVarDef : public IrGVarDef, public Nameable {
public:
  I_IGVarDef(I_Module *a_iModule, const Cowstr &a_name,
             llvm::ConstantInt *a_initializer = nullptr);
  virtual ~I_IGVarDef() = default;
  virtual Cowstr str() const;

  I_Module *iModule;
  llvm::ConstantInt *initializer;
};

// float, double
class I_FGVarDef : public IrGVarDef, public Nameable {
public:
  I_FGVarDef(I_Module *a_iModule, const Cowstr &a_name,
             llvm::ConstantFP *a_initializer = nullptr);
  virtual ~I_FGVarDef() = default;
  virtual Cowstr str() const;

  I_Module *iModule;
  llvm::ConstantFP *initializer;
};

class I_IVarDef : public IrVarDef, public Nameable {
public:
  I_IVarDef(I_FuncDef *a_iFunc, const Cowstr &a_name,
            llvm::ConstantInt *a_initializer = nullptr);
  virtual ~I_IVarDef() = default;
  virtual Cowstr str() const;

  I_FuncDef *iFunc;
  llvm::ConstantInt *initializer;
};

class I_FVarDef : public IrVarDef, public Nameable {
public:
  I_FVarDef(I_FuncDef *a_iFunc, const Cowstr &a_name,
            llvm::ConstantFP *a_initializer = nullptr);
  virtual ~I_FVarDef() = default;
  virtual Cowstr str() const;

  I_FuncDef *iFunc;
  llvm::ConstantFP *initializer;
};

class I_Condition : public Ir {
public:
  virtual ~I_Condition() = default;
  virtual Cowstr str() const;
};

class I_Call : public Ir {
public:
  virtual ~I_Call() = default;
  virtual Cowstr str() const;
};

class I_BinaryOp : public Ir {
public:
  virtual ~I_BinaryOp() = default;
  virtual Cowstr str() const;
};

class I_UnaryOp : public Ir {
public:
  virtual ~I_UnaryOp() = default;
  virtual Cowstr str() const;
};

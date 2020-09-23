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
struct Ir;
struct I_Module;
struct I_FuncDef;
struct I_Call;
struct I_Constant;
struct I_BinaryOp;
struct I_UnaryOp;
struct I_Condition;
struct I_Loop;
struct I_Return;

// Ir {

struct Ir {
  virtual ~Ir() = default;
  virtual Cowstr str() const = 0;
};

// Ir }

struct I_Module : public Ir, public Nameable {
  I_Module(Ast *a_ast, std::shared_ptr<Scope> a_scope);
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

struct I_FuncDef : public Ir, public Nameable {
  I_FuncDef(I_Module *a_iModule, Ast *a_ast, std::shared_ptr<Scope> a_scope,
            const Cowstr &a_name);
  virtual ~I_FuncDef() = default;
  virtual Cowstr str() const;

  I_Module *iModule;
  Ast *ast;
  std::shared_ptr<Scope> scope;

  Cowstr name;
  std::vector<std::shared_ptr<Ir>> parameters;
  std::vector<std::shared_ptr<Ir>> basicBlocks;
};

struct IrGVarDef : public Ir {
  virtual ~IrGVarDef() = default;
  virtual Cowstr str() const = 0;
};

struct IrVarDef : public Ir {
  virtual ~IrVarDef() = default;
  virtual Cowstr str() const = 0;
};

// boolean
// byte, ubyte
// short, ushort
// int, uint
// long, ulong
struct I_IGVarDef : public IrGVarDef, public Nameable {
  I_IGVarDef(I_Module *a_iModule, const Cowstr &a_name,
             llvm::ConstantInt *a_initializer = nullptr);
  virtual ~I_IGVarDef() = default;
  virtual Cowstr str() const;

  I_Module *iModule;
  Cowstr &name;
  llvm::ConstantInt *initializer;
};

// float, double
struct I_FGVarDef : public IrGVarDef, public Nameable {
  I_FGVarDef(I_Module *a_iModule, const Cowstr &a_name,
             llvm::ConstantFP *a_initializer = nullptr);
  virtual ~I_FGVarDef() = default;
  virtual Cowstr str() const;

  I_Module *iModule;
  Cowstr name;
  llvm::ConstantFP *initializer;
};

struct I_IVarDef : public IrVarDef, public Nameable {
  I_IVarDef(I_Module *a_iModule, Ast *a_ast);
};

struct I_Condition : public Ir {
  virtual ~I_Condition() = default;
  virtual Cowstr str() const;
};

struct I_Call : public Ir {
  virtual ~I_Call() = default;
  virtual Cowstr str() const;
};

struct I_BinaryOp : public Ir {
  virtual ~I_BinaryOp() = default;
  virtual Cowstr str() const;
};

struct I_UnaryOp : public Ir {
  virtual ~I_UnaryOp() = default;
  virtual Cowstr str() const;
};

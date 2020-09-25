// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Cowstr.h"
#include "Name.h"
#include "Symbol.h"
#include "enum.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <memory>

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
class I_GVarDef;
class I_VarDef;
class I_ConstantExpr;

// Ir {

class Ir {
public:
  virtual ~Ir() = default;
  virtual llvm::Value *value() const = 0;
  virtual Cowstr str() const = 0;

  static std::shared_ptr<Ir> expr(I_Module *a_iModule, Ast *a_ast,
                                  std::shared_ptr<Scope> a_scope);
  static std::shared_ptr<Ir> expr(I_FuncDef *a_iFuncDef, Ast *a_ast,
                                  std::shared_ptr<Scope> a_scope);
};

// Ir }

class I_Module : public Ir, public Nameable {
public:
  I_Module(Ast *a_ast, std::shared_ptr<Scope> a_scope);
  virtual ~I_Module();
  virtual llvm::Value *value() const;
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
  I_FuncDef(I_Module *a_iModule, Ast *a_ast, std::shared_ptr<Scope> a_scope);
  virtual ~I_FuncDef() = default;
  virtual llvm::Value *value() const;
  virtual Cowstr str() const;

  I_Module *iModule;

  Ast *ast;
  std::shared_ptr<Scope> scope;

  std::vector<std::shared_ptr<Ir>> parameters;
  std::vector<std::shared_ptr<Ir>> basicBlocks;
};

// boolean
// byte, ubyte
// short, ushort
// int, uint
// long, ulong
// float, double
class I_GVarDef : public Ir, public Nameable {
public:
  I_IGVarDef(I_Module *a_iModule, Ast *a_ast, std::shared_ptr<Scope> a_scope);
  virtual ~I_IGVarDef() = default;
  virtual llvm::Value *value() const;
  virtual Cowstr str() const;

  I_Module *iModule;
  Ast *ast;
  std::shared_ptr<Scope> scope;

  llvm::GlobalVariable *globalVariable;

  std::shared_ptr<Ir> expr;

private:
  llvm::GlobalVariable *getInt(const Cowstr &name,
                               llvm::ConstantInt *initializer);
  llvm::GlobalVariable *getFP(const Cowstr &name,
                              llvm::ConstantFP *initializer);
};

class I_VarDef : public Ir, public Nameable {
public:
  I_VarDef(I_FuncDef *a_iFunc, const Cowstr &a_name,
           llvm::ConstantInt *a_initializer = nullptr);
  virtual ~I_VarDef() = default;
  virtual llvm::Value *value();

  I_FuncDef *iFunc;
  Ast *ast;
  std::shared_ptr<Scope> scope;

  llvm::AllocaInst *allocaVariable;

  std::shared_ptr<Ir> expr;
};

class I_ConstantExpr : public Ir, public Nameable {
public:
  I_ConstantExpr();
};

class I_ConstantInt : public Ir, public Nameable {};

class I_ConstantFP : public Ir, public Nameable {};

class I_ConstantVoid : public Ir, public Nameable {};

class I_Condition : public Ir {
public:
  virtual ~I_Condition() = default;
  virtual llvm::Value *value();
};

class I_Call : public Ir {
public:
  virtual ~I_Call() = default;
  virtual llvm::Value *value();
};

class I_BinaryOp : public Ir {
public:
  virtual ~I_BinaryOp() = default;
  virtual llvm::Value *value();
};

class I_UnaryOp : public Ir {
public:
  virtual ~I_UnaryOp() = default;
  virtual llvm::Value *value();
};

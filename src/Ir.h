// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#pragma once
#include "Symbol.h"
#include "Visitor.h"
#include "enum.h"
#include "iface/LLVMTypable.h"
#include "iface/LLVMValuable.h"
#include "iface/Nameable.h"
#include "iface/Phase.h"
#include "iface/Scoped.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <vector>

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
class I_Stat;

namespace detail {

class Modular {
public:
  Modular(I_Mod *mod = nullptr);
  virtual ~Modular() = default;
  virtual I_Mod *&iMod();
  virtual I_Mod *iMod() const;
};

} // namespace detail

class Ir : public Nameable, public LLVMValuable, public LLVMTypable {
public:
  virtual ~Ir() = default;
  virtual IrKind kind() const = 0;
};

class I_Mod : public Ir, public Phase {
public:
  I_Mod();
  virtual ~I_Mod();
  virtual IrKind kind() const;
  virtual void run(Ast *ast);

  struct Context : public VisitorContext,
                   public Scoped,
                   public detail::Modular {
    Context(I_Mod *mod);
  };
  struct VISITOR(GlobalVar) : public Visitor {
    VISITOR(GlobalVar)();
    virtual void visit(Ast *ast);
  };
  struct VISITOR(FuncDecl) : public Visitor {
    VISITOR(FuncDecl)();
    virtual void visit(Ast *ast);
  };
  struct VISITOR(FuncDef) : public Visitor {
    VISITOR(FuncDef)();
    virtual void visit(Ast *ast);
  };

  VisitorContext *context;
  VisitorBinder binder;
  std::vector<Visitor *> visitors;

  I_GlobalVar *iGlobalVar;
  I_FuncDecl *iFuncDecl;
  I_FuncDef *iFuncDef;

  llvm::LLVMContext llvmContext;
  llvm::IRBuilder<> llvmIRBuilder;
  llvm::Module *llvmModule;
};

class I_GlobalVar : public Ir, public Phase {
public:
  I_GlobalVar();
  virtual ~I_GlobalVar();
  virtual IrKind kind() const;
  virtual void run(Ast *ast);

  struct Context : public VisitorContext {};

  S_Var *variableSymbol; // S_Var
  I_ConstantExpr *constantExpr;
};

class I_ConstantExpr : public Ir, public Phase {
public:
  I_ConstantExpr();
  virtual ~I_ConstantExpr();
  virtual IrKind kind() const;
};

class I_FuncDecl : public Ir, public Phase {
public:
  I_FuncDecl();
  virtual ~I_FuncDecl();
  virtual IrKind kind() const;

  S_Func *functionSymbol; // S_Func
  std::vector<I_Param> params;
};

class I_Param : public Ir, public Phase {
public:
  I_Param();
  virtual ~I_Param();
  virtual IrKind kind() const;

  S_Param *parameterSymbol; // S_Param
};

class I_FuncDef : public Ir, public Phase {
public:
  I_FuncDef();
  virtual ~I_FuncDef();
  virtual IrKind kind() const;

  I_FuncDecl *funcDecl;
  I_Block *funcBody;
  TypeSymbol *functionResultTypeSymbol;
};

class I_Block : public Ir {
public:
  I_Block();
  virtual ~I_Block();
  virtual IrKind kind() const;

  std::vector<I_Stat> stats;
};

class I_Stat : public Ir {
public:
  I_Stat();
  virtual ~I_Stat();
  virtual IrKind kind() const;
};

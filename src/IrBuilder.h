// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
// #include "Ir.h"
#include "Symbol.h"
#include "Visitor.h"
#include "iface/Phase.h"
#include "iface/Scoped.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <vector>

#define DECL(x)                                                                \
  struct VISITOR(x) : public Visitor {                                         \
    Ir *ir;                                                                    \
    VISITOR(x)();                                                              \
    virtual void visit(Ast *ast);                                              \
    virtual void finishVisit(Ast *ast);                                        \
  }

class IrBuilder;

namespace detail {

struct I_Mod;
struct I_GlobalVar;
struct I_FuncDef;
struct I_FuncDecl;
struct I_Param;
struct I_Block;
struct I_Var;
struct I_ConstantExpr;
struct I_Expr;

struct I_Mod : public Phase {
  I_Mod(IrBuilder *irBuilder);
  virtual ~I_Mod();
  virtual void run(Ast *ast);

  struct Context : public VisitorContext, public Scoped {
    Context(IrBuilder *a_irBuilder, I_Mod *mod)
        : irBuilder(a_irBuilder), iMod(mod) {}
    IrBuilder *irBuilder;
    I_Mod *iMod;
  };
  struct VISITOR(GlobalVar) : public Visitor {
    VISITOR(GlobalVar)();
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
  I_FuncDef *iFuncDef;
};

struct I_GlobalVar : public Phase {
  I_GlobalVar(IrBuilder *irBuilder);
  virtual ~I_GlobalVar();
  virtual void run(Ast *ast);

  struct Context : public VisitorContext, public Scoped {
    Context(IrBuilder *a_irBuilder, I_GlobalVar *globalVar)
        : irBuilder(a_irBuilder), iGlobalVar(globalVar) {}
    IrBuilder *irBuilder;
    I_GlobalVar *iGlobalVar;
  };
  struct VISITOR(VarId) : public Visitor {
    VISITOR(VarId)();
    virtual void visit(Ast *ast);
  };
  struct VISITOR(PlainType) : public Visitor {
    VISITOR(PlainType)();
    virtual void visit(Ast *ast);
  };
  struct VISITOR(ConstantExpr) : public Visitor {
    VISITOR(ConstantExpr)();
    virtual void visit(Ast *ast);
  };

  VisitorContext *context;
  VisitorBinder binder;
  std::vector<Visitor *> visitors;

  I_ConstantExpr *iConstantExpr;
};

struct I_FuncDef : public Phase {
  I_FuncDef(IrBuilder *irBuilder);
  virtual ~I_FuncDef();
  virtual void run(Ast *ast);

  struct Context : public VisitorContext, public Scoped {
    Context(IrBuilder *a_irBuilder, I_FuncDef *funcDef)
        : irBuilder(a_irBuilder), iFuncDef(funcDef) {}
    IrBuilder *irBuilder;
    I_FuncDef *iFuncDef;
  };
  struct VISITOR(FuncDecl) : public Visitor {
    VISITOR(FuncDecl)();
    virtual void visit(Ast *ast);
  };
  struct VISITOR(PlainType) : public Visitor {
    VISITOR(PlainType)();
    virtual void visit(Ast *ast);
  };
  struct VISITOR(ConstantExpr) : public Visitor {
    VISITOR(ConstantExpr)();
    virtual void visit(Ast *ast);
  };

  VisitorContext *context;
  VisitorBinder binder;
  std::vector<Visitor *> visitors;

  I_FuncDecl *iFuncDecl;
  I_Block *iBlock;
};

struct I_FuncDecl : public Phase {};
struct I_Param : public Phase {};
struct I_Block : public Phase {};
struct I_Var : public Phase {};
struct I_ConstantExpr : public Phase {};
struct I_Expr : public Phase {};

} // namespace detail

class IrBuilder : public Phase {
public:
  IrBuilder();
  virtual ~IrBuilder();
  virtual void run(Ast *ast);

  struct Context : public VisitorContext, public Scoped {
    Context(IrBuilder *a_irBuilder);
    IrBuilder *irBuilder;
  };

  struct VISITOR(CompileUnit) : public Visitor {
    VISITOR(CompileUnit)();
    virtual void visit(Ast *ast);
    virtual void finishVisit(Ast *ast);
  };
  struct VISITOR(VarId) : public Visitor {
    Symbol *symbol;
    VISITOR(VarId)();
    virtual void visit(Ast *ast);
  };
  struct VISITOR(PlainType) : public Visitor {
    TypeSymbol *typeSymbol;
    VISITOR(PlainType)();
    virtual void visit(Ast *ast);
  };
  struct VISITOR(VarDef) : public Visitor {
    Ir *ir;
    VISITOR(VarDef)();
    virtual void visit(Ast *ast);
    virtual void visitAfter(Ast *ast, Ast *child);
  };

private:
  llvm::LLVMContext llvmContext;
  llvm::IRBuilder<> llvmIRBuilder;
  llvm::Module *llvmModule;

  detail::I_Mod *iMod;
};

#undef DECL

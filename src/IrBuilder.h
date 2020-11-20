// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "AstClasses.h"
#include "SymbolClasses.h"
#include "enum.h"
#include "iface/Phase.h"
#include "iface/Visitor.h"
#include "infra/Cowstr.h"
#include "infra/LinkedHashMap.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include <unordered_set>
#include <vector>

// #define DECL(x)                                                                \
//   struct VISITOR(x) : public Visitor {                                         \
//     Ir *ir;                                                                    \
//     VISITOR(x)();                                                              \
//     virtual void visit(Ast *ast);                                              \
//     virtual void finishVisit(Ast *ast);                                        \
//   }
//
// class IrBuilder;
//
// namespace detail {
//
// struct I_Mod;
// struct I_GlobalVar;
// struct I_FuncDef;
// struct I_FuncDecl;
// struct I_Param;
// struct I_Block;
// struct I_Var;
// struct I_ConstantExpr;
// struct I_Expr;
//
// struct I_Mod : public Phase {
//   I_Mod(IrBuilder *irBuilder);
//   virtual ~I_Mod();
//   virtual void run(Ast *ast);
//
//   struct Context : public VisitorContext, public Scoped {
//     Context(IrBuilder *a_irBuilder, I_Mod *mod)
//         : irBuilder(a_irBuilder), iMod(mod) {}
//     IrBuilder *irBuilder;
//     I_Mod *iMod;
//   };
//   struct VISITOR(GlobalVar) : public Visitor {
//     VISITOR(GlobalVar)();
//     virtual void visit(Ast *ast);
//   };
//   struct VISITOR(FuncDef) : public Visitor {
//     VISITOR(FuncDef)();
//     virtual void visit(Ast *ast);
//   };
//
//   VisitorContext *context;
//   VisitorBinder binder;
//   std::vector<Visitor *> visitors;
//
//   I_GlobalVar *iGlobalVar;
//   I_FuncDef *iFuncDef;
// };
//
// struct I_GlobalVar : public Phase {
//   I_GlobalVar(IrBuilder *irBuilder);
//   virtual ~I_GlobalVar();
//   virtual void run(Ast *ast);
//
//   struct Context : public VisitorContext, public Scoped {
//     Context(IrBuilder *a_irBuilder, I_GlobalVar *globalVar)
//         : irBuilder(a_irBuilder), iGlobalVar(globalVar) {}
//     IrBuilder *irBuilder;
//     I_GlobalVar *iGlobalVar;
//   };
//   struct VISITOR(VarId) : public Visitor {
//     VISITOR(VarId)();
//     virtual void visit(Ast *ast);
//   };
//   struct VISITOR(PlainType) : public Visitor {
//     VISITOR(PlainType)();
//     virtual void visit(Ast *ast);
//   };
//   struct VISITOR(ConstantExpr) : public Visitor {
//     VISITOR(ConstantExpr)();
//     virtual void visit(Ast *ast);
//   };
//
//   VisitorContext *context;
//   VisitorBinder binder;
//   std::vector<Visitor *> visitors;
//
//   I_ConstantExpr *iConstantExpr;
// };
//
// struct I_FuncDef : public Phase {
//   I_FuncDef(IrBuilder *irBuilder);
//   virtual ~I_FuncDef();
//   virtual void run(Ast *ast);
//
//   struct Context : public VisitorContext, public Scoped {
//     Context(IrBuilder *a_irBuilder, I_FuncDef *funcDef)
//         : irBuilder(a_irBuilder), iFuncDef(funcDef) {}
//     IrBuilder *irBuilder;
//     I_FuncDef *iFuncDef;
//   };
//   struct VISITOR(FuncDecl) : public Visitor {
//     VISITOR(FuncDecl)();
//     virtual void visit(Ast *ast);
//   };
//   struct VISITOR(PlainType) : public Visitor {
//     VISITOR(PlainType)();
//     virtual void visit(Ast *ast);
//   };
//   struct VISITOR(ConstantExpr) : public Visitor {
//     VISITOR(ConstantExpr)();
//     virtual void visit(Ast *ast);
//   };
//
//   VisitorContext *context;
//   VisitorBinder binder;
//   std::vector<Visitor *> visitors;
//
//   I_FuncDecl *iFuncDecl;
//   I_Block *iBlock;
// };
//
// struct I_FuncDecl : public Phase {};
// struct I_Param : public Phase {};
// struct I_Block : public Phase {};
// struct I_Var : public Phase {};
// struct I_ConstantExpr : public Phase {};
// struct I_Expr : public Phase {};
//
// } // namespace detail

// class IrBuilder : public Phase {
// public:
//   IrBuilder();
//   virtual ~IrBuilder();
//   virtual void run(Ast *ast);
//
//   struct Context : public VisitorContext, public Scoped {
//     Context(IrBuilder *a_irBuilder);
//     IrBuilder *irBuilder;
//   };
//
//   struct VISITOR(CompileUnit) : public Visitor {
//     VISITOR(CompileUnit)();
//     virtual void visit(Ast *ast);
//     virtual void finishVisit(Ast *ast);
//   };
//   struct VISITOR(VarId) : public Visitor {
//     Symbol *symbol;
//     VISITOR(VarId)();
//     virtual void visit(Ast *ast);
//   };
//   struct VISITOR(PlainType) : public Visitor {
//     TypeSymbol *typeSymbol;
//     VISITOR(PlainType)();
//     virtual void visit(Ast *ast);
//   };
//   struct VISITOR(VarDef) : public Visitor {
//     Ir *ir;
//     VISITOR(VarDef)();
//     virtual void visit(Ast *ast);
//     virtual void visitAfter(Ast *ast, Ast *child);
//   };
//
// private:
//   llvm::LLVMContext llvmContext;
//   llvm::IRBuilder<> llvmIRBuilder;
//   llvm::Module *llvmModule;
//
//   detail::I_Mod *iMod;
// };
//
// #undef DECL

// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

class IrSpace {
public:
  IrSpace(IrSpace *owner = nullptr);
  virtual ~IrSpace();

  virtual IrSpace *owner() const;
  virtual void defineValue(const Cowstr &name, llvm::Value *value);
  virtual void defineType(const Cowstr &name, llvm::Type *type);
  virtual void defineConstant(const Cowstr &name, llvm::Constant *constant);
  virtual void defineFunction(const Cowstr &name, llvm::Function *function);
  virtual llvm::Value *resolveValue(const Cowstr &name) const;
  virtual llvm::Type *resolveType(const Cowstr &name) const;
  virtual llvm::Constant *resolveConstant(const Cowstr &name) const;
  virtual llvm::Function *resolveFunction(const Cowstr &name) const;

  struct Data {
  public:
    enum DataKind { UNKNOWN = 0, VALUE, TYPE, CONSTANT, FUNCTION };

    int kind;
    union {
      llvm::Value *value;
      llvm::Type *type;
      llvm::Constant *constant;
      llvm::Function *function;
    } data;

    virtual ~Data() = default;

    static Data fromUnknown();
    static Data fromValue(llvm::Value *a_value);
    static Data fromType(llvm::Type *a_type);
    static Data fromConstant(llvm::Constant *a_constant);
    static Data fromFunction(llvm::Function *a_function);

    virtual llvm::Value *asValue() const;
    virtual llvm::Type *asType() const;
    virtual llvm::Constant *asConstant() const;
    virtual llvm::Function *asFunction() const;
  };

private:
  IrSpace *owner_;
  LinkedHashMap<Cowstr, Data> dataMap_;
  std::vector<IrSpace *> children_;
};

class IrBuilder : public Phase, public Visitor {
public:
  IrBuilder();
  virtual ~IrBuilder();
  virtual void run(Ast *ast);
  virtual llvm::Module *llvmModule() const;

  virtual void visitInteger(A_Integer *ast);
  virtual void visitFloat(A_Float *ast);
  virtual void visitBoolean(A_Boolean *ast);
  virtual void visitCharacter(A_Character *ast);
  virtual void visitString(A_String *ast);
  virtual void visitNil(A_Nil *ast);
  virtual void visitVoid(A_Void *ast);
  virtual void visitVarId(A_VarId *ast);
  // virtual void visitBreak(A_Break *ast);
  // virtual void visitContinue(A_Continue *ast);

  // virtual void visitThrow(A_Throw *ast);
  virtual void visitReturn(A_Return *ast);
  virtual void visitAssign(A_Assign *ast);
  virtual void visitPostfix(A_Postfix *ast);
  virtual void visitInfix(A_Infix *ast);
  virtual void visitPrefix(A_Prefix *ast);
  virtual void visitCall(A_Call *ast);
  // virtual void visitExprs(A_Exprs *ast);
  // virtual void visitIf(A_If *ast);
  // virtual void visitLoop(A_Loop *ast);
  // virtual void visitYield(A_Yield *ast);
  // virtual void visitLoopCondition(A_LoopCondition *ast);
  // virtual void visitLoopEnumerator(A_LoopEnumerator *ast);
  // virtual void visitDoWhile(A_DoWhile *ast);
  // virtual void visitTry(A_Try *ast);
  virtual void visitBlock(A_Block *ast);
  // virtual void visitBlockStats(A_BlockStats *ast);
  virtual void visitPlainType(A_PlainType *ast);
  virtual void visitFuncDef(A_FuncDef *ast);
  // virtual void visitFuncSign(A_FuncSign *ast);
  // virtual void visitParams(A_Params *ast);
  // virtual void visitParam(A_Param *ast);
  virtual void visitVarDef(A_VarDef *ast);
  // virtual void visitTopStats(A_TopStats *ast);
  virtual void visitCompileUnit(A_CompileUnit *ast);

  struct ConstantBuilder : public Visitor {
    IrBuilder *irBuilder;

    ConstantBuilder(IrBuilder *a_irBuilder);
    virtual ~ConstantBuilder() = default;

    virtual void visitInteger(A_Integer *ast);
    virtual void visitFloat(A_Float *ast);
    virtual void visitBoolean(A_Boolean *ast);
    virtual void visitCharacter(A_Character *ast);
    virtual void visitString(A_String *ast);
    virtual void visitNil(A_Nil *ast);
    virtual void visitVoid(A_Void *ast);
    virtual void visitVarId(A_VarId *ast);
    virtual void visitAssign(A_Assign *ast);
    virtual void visitPostfix(A_Postfix *ast);
    virtual void visitInfix(A_Infix *ast);
    virtual void visitPrefix(A_Prefix *ast);
    // virtual void visitExprs(A_Exprs *ast);
  };

private:
  llvm::LLVMContext llvmContext_;
  llvm::IRBuilder<> llvmIRBuilder_;
  llvm::Module *llvmModule_;

  Scope *currentScope_;

  IrSpace *globalSpace_;
  IrSpace *currentSpace_;
};

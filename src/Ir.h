// Copyright 2019- <coli-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "boost/core/noncopyable.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"

class IrContext : private boost::noncopyable {
public:
  IrContext();
  virtual ~IrContext();
  llvm::LLVMContext &context();
  const llvm::LLVMContext &context() const;
  llvm::IRBuilder<> &builder();
  const llvm::IRBuilder<> &builder() const;
  llvm::Module *&module();
  const llvm::Module *module() const;
  std::map<std::string, llvm::Value *> &symtable();
  const td::map<std::string, llvm::Value *> &symtable() const;

private:
  llvm::LLVMContext context_;
  llvm::IRBuilder<> builder_;
  llvm::Module *module_;
  std::map<std::string, llvm::Value *> symtable_;
};

class Ir : public Namely,
           public Typely,
           public Stringify,
           private boost::noncopyable {
public:
  virtual ~Ir() = default;
  virtual int type() const = 0;
  virtual std::string toString() const = 0;
  virtual std::string name() const = 0;
  virtual llvm::Value *codegen() = 0;
};

/* translate unit */
class IrTranslateUnit : public Ir {
public:
  IrTranslateUnit(AstTranslateUnit *node);
  virtual ~IrTranslateUnit();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstTranslateUnit *node_;
};

/* identifier constant */
class IrIdentifierConstant : public Ir {
public:
  IrIdentifierConstant(AstIdentifierConstant *node);
  virtual ~IrIdentifierConstant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstIdentifierConstant *node_;
};

/* i8 constant */
class IrI8Constant : public Ir {
public:
  IrI8Constant(AstI8Constant *node);
  virtual ~IrI8Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstI8Constant *node_;
};

/* u8 constant */
class IrU8Constant : public Ir {
public:
  IrU8Constant(AstU8Constant *node);
  virtual ~IrU8Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstU8Constant *node_;
};

/* i16 constant */
class IrI16Constant : public Ir {
public:
  IrI16Constant(AstI16Constant *node);
  virtual ~IrI16Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstI16Constant *node_;
};

/* u16 constant */
class IrU16Constant : public Ir {
public:
  IrU16Constant(AstU16Constant *node);
  virtual ~IrU16Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstU16Constant *node_;
};

/* i32 constant */
class IrI32Constant : public Ir {
public:
  IrI32Constant(AstI32Constant *node);
  virtual ~IrI32Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstI32Constant *node_;
};

/* u32 constant */
class IrU32Constant : public Ir {
public:
  IrU32Constant(AstU32Constant *node);
  virtual ~IrU32Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstU32Constant *node_;
};

/* i64 constant */
class IrI64Constant : public Ir {
public:
  IrI64Constant(AstI64Constant *node);
  virtual ~IrI64Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstI64Constant *node_;
};

/* u64 constant */
class IrU64Constant : public Ir {
public:
  IrU64Constant(AstU64Constant *node);
  virtual ~IrU64Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstU64Constant *node_;
};

/* f32 constant */
class IrF32Constant : public Ir {
public:
  IrF32Constant(AstF32Constant *node);
  virtual ~IrF32Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstF32Constant *node_;
};

/* f64 constant */
class IrF64Constant : public Ir {
public:
  IrF64Constant(AstF64Constant *node);
  virtual ~IrF64Constant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstF64Constant *node_;
};

/* string constant */
class IrStringConstant : public Ir {
public:
  IrStringConstant(AstStringConstant *node);
  virtual ~IrStringConstant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstStringConstant *node_;
};

/* boolean constant */
class IrBooleanConstant : public Ir {
public:
  IrBooleanConstant(AstBooleanConstant *node);
  virtual ~IrBooleanConstant();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstBooleanConstant *node_;
};

/* call expression */
class IrCallExpression : public Ir {
public:
  IrCallExpression(AstCallExpression *node);
  virtual ~IrCallExpression();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstCallExpression *node_;
};

/* unary operation expression */
class IrUnaryExpression : public Ir {
public:
  IrUnaryExpression(AstUnaryExpression *node);
  virtual ~IrUnaryExpression();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstUnaryExpression *node_;
};

/* binary operation expression */
class IrBinaryExpression : public Ir {
public:
  IrBinaryExpression(AstBinaryExpression *node);
  virtual ~IrBinaryExpression();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstBinaryExpression *node_;
};

/* conditional expression */
class IrConditionalExpression : public Ir {
public:
  IrConditionalExpression(AstConditionalExpression *node);
  virtual ~IrConditionalExpression();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstConditionalExpression *node_;
};

/* assignment expression */
class IrAssignmentExpression : public Ir {
public:
  IrAssignmentExpression(AstAssignmentExpression *node);
  virtual ~IrAssignmentExpression();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstAssignmentExpression *node_;
};

/* sequel expression */
class IrSequelExpression : public Ir {
public:
  IrSequelExpression(AstSequelExpression *node);
  virtual ~IrSequelExpression();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstSequelExpression *node_;
};

/* expression statement */
class IrExpressionStatement : public Ir {
public:
  IrExpressionStatement(AstExpressionStatement *node);
  virtual ~IrExpressionStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstExpressionStatement *node_;
};

/* compound statement */
class IrCompoundStatement : public Ir {
public:
  IrCompoundStatement(AstCompoundStatement *node);
  virtual ~IrCompoundStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstCompoundStatement *node_;
};

/* if statement */
class IrIfStatement : public Ir {
public:
  IrIfStatement(AstIfStatement *node);
  virtual ~IrIfStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstIfStatement *node_;
};

/* while statement */
class IrWhileStatement : public Ir {
public:
  IrWhileStatement(AstWhileStatement *node);
  virtual ~IrWhileStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstWhileStatement *node_;
};

/* for statement */
class IrForStatement : public Ir {
public:
  IrForStatement(AstForStatement *node);
  virtual ~IrForStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstForStatement *node_;
};

/* continue statement */
class IrContinueStatement : public Ir {
public:
  IrContinueStatement(AstContinueStatement *node);
  virtual ~IrContinueStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstContinueStatement *node_;
};

/* break statement */
class IrBreakStatement : public Ir {
public:
  IrBreakStatement(AstBreakStatement *node);
  virtual ~IrBreakStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstBreakStatement *node_;
};

/* return statement */
class IrReturnStatement : public Ir {
public:
  IrReturnStatement(AstReturnStatement *node);
  virtual ~IrReturnStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstReturnStatement *node_;
};

/* empty statement */
class IrEmptyStatement : public Ir {
public:
  IrEmptyStatement(AstEmptyStatement *node);
  virtual ~IrEmptyStatement();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstEmptyStatement *node_;
};

/* variable declaration */
class IrVariableDeclaration : public Ir {
public:
  IrVariableDeclaration(AstVariableDeclaration *node);
  virtual ~IrVariableDeclaration();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstVariableDeclaration *node_;
};

/* variable assignment declaration */
class IrVariableAssignmentDeclaration : public Ir {
public:
  IrVariableAssignmentDeclaration(AstVariableAssignmentDeclaration *node);
  virtual ~IrVariableAssignmentDeclaration();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstVariableAssignmentDeclaration *node_;
};

/* function declaration */
class IrFunctionDeclaration : public Ir {
public:
  IrFunctionDeclaration(AstFunctionDeclaration *node);
  virtual ~IrFunctionDeclaration();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstFunctionDeclaration *node_;
};

/* function argument declaration */
class IrFunctionArgumentDeclaration : public Ir {
public:
  IrFunctionArgumentDeclaration(AstFunctionArgumentDeclaration *node);
  virtual ~IrFunctionArgumentDeclaration();
  virtual int type() const;
  virtual std::string toString() const;
  virtual std::string name() const;
  virtual llvm::Value *codegen(IrContext *context);

private:
  AstFunctionArgumentDeclaration *node_;
};

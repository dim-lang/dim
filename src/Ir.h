// Copyright 2019- <collie.org>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Log.h"
#include "boost/core/noncopyable.hpp"
//#include "llvm/ADT/APFloat.h"
//#include "llvm/ADT/STLExtras.h"
//#include "llvm/IR/BasicBlock.h"
//#include "llvm/IR/Constants.h"
//#include "llvm/IR/DerivedTypes.h"
//#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
//#include "llvm/IR/Type.h"
//#include "llvm/IR/Verifier.h"
//#include "llvm/Support/TargetSelect.h"
//#include "llvm/Target/TargetMachine.h"

class IrContext : protected boost::noncopyable {
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
  const std::map<std::string, llvm::Value *> &symtable() const;

private:
  llvm::LLVMContext context_;
  llvm::IRBuilder<> builder_;
  llvm::Module *module_;
  std::map<std::string, llvm::Value *> symtable_;
};

template <typename T>
class Ir : public Namely,
           public Typely,
           public Stringify,
           private boost::noncopyable {
public:
  Ir(T node, const std::string &name) : node_(node), name_(name) {}
  virtual ~Ir() = default;
  virtual std::string toString() const {
    return fmt::format("[ {} node_:{} ]", stringify(), node_->name());
  }
  virtual std::string name() const { return name_; }
  virtual int type() const = 0;
  virtual llvm::Value *codegen(IrContext *context) = 0;

protected:
  virtual std::string stringify() const = 0;
  T *node_;
  std::string name_;
};

/* translate unit */
class IrTranslateUnit : public Ir<AstTranslateUnit> {
public:
  IrTranslateUnit(AstTranslateUnit *node);
  virtual ~IrTranslateUnit() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* identifier constant */
class IrIdentifierConstant : public Ir<AstIdentifierConstant> {
public:
  IrIdentifierConstant(AstIdentifierConstant *node);
  virtual ~IrIdentifierConstant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* i8 constant */
class IrI8Constant : public Ir<AstI8Constant> {
public:
  IrI8Constant(AstI8Constant *node);
  virtual ~IrI8Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* u8 constant */
class IrU8Constant : public Ir<AstU8Constant> {
public:
  IrU8Constant(AstU8Constant *node);
  virtual ~IrU8Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* i16 constant */
class IrI16Constant : public Ir<AstI16Constant> {
public:
  IrI16Constant(AstI16Constant *node);
  virtual ~IrI16Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* u16 constant */
class IrU16Constant : public Ir<AstU16Constant> {
public:
  IrU16Constant(AstU16Constant *node);
  virtual ~IrU16Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* i32 constant */
class IrI32Constant : public Ir<AstI32Constant> {
public:
  IrI32Constant(AstI32Constant *node);
  virtual ~IrI32Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* u32 constant */
class IrU32Constant : public Ir<AstU32Constant> {
public:
  IrU32Constant(AstU32Constant *node);
  virtual ~IrU32Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* i64 constant */
class IrI64Constant : public Ir<AstI64Constant> {
public:
  IrI64Constant(AstI64Constant *node);
  virtual ~IrI64Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* u64 constant */
class IrU64Constant : public Ir<AstU64Constant> {
public:
  IrU64Constant(AstU64Constant *node);
  virtual ~IrU64Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* f32 constant */
class IrF32Constant : public Ir<AstF32Constant> {
public:
  IrF32Constant(AstF32Constant *node);
  virtual ~IrF32Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* f64 constant */
class IrF64Constant : public Ir<AstF64Constant> {
public:
  IrF64Constant(AstF64Constant *node);
  virtual ~IrF64Constant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* string constant */
class IrStringConstant : public Ir<AstStringConstant> {
public:
  IrStringConstant(AstStringConstant *node);
  virtual ~IrStringConstant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* boolean constant */
class IrBooleanConstant : public Ir<AstBooleanConstant> {
public:
  IrBooleanConstant(AstBooleanConstant *node);
  virtual ~IrBooleanConstant() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* call expression */
class IrCallExpression : public Ir<AstCallExpression> {
public:
  IrCallExpression(AstCallExpression *node);
  virtual ~IrCallExpression() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* unary expression */
class IrUnaryExpression : public Ir<AstUnaryExpression> {
public:
  IrUnaryExpression(AstUnaryExpression *node);
  virtual ~IrUnaryExpression() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* binary expression */
class IrBinaryExpression : public Ir<AstBinaryExpression> {
public:
  IrBinaryExpression(AstBinaryExpression *node);
  virtual ~IrBinaryExpression() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* conditional expression */
class IrConditionalExpression : public Ir<AstConditionalExpression> {
public:
  IrConditionalExpression(AstConditionalExpression *node);
  virtual ~IrConditionalExpression() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* assignment expression */
class IrAssignmentExpression : public Ir<AstAssignmentExpression> {
public:
  IrAssignmentExpression(AstAssignmentExpression *node);
  virtual ~IrAssignmentExpression() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* sequel expression */
class IrSequelExpression : public Ir<AstSequelExpression> {
public:
  IrSequelExpression(AstSequelExpression *node);
  virtual ~IrSequelExpression() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* expression statement */
class IrExpressionStatement : public Ir<AstExpressionStatement> {
public:
  IrExpressionStatement(AstExpressionStatement *node);
  virtual ~IrExpressionStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* compound statement */
class IrCompoundStatement : public Ir<AstCompoundStatement> {
public:
  IrCompoundStatement(AstCompoundStatement *node);
  virtual ~IrCompoundStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* if statement */
class IrIfStatement : public Ir<AstIfStatement> {
public:
  IrIfStatement(AstIfStatement *node);
  virtual ~IrIfStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* while statement */
class IrWhileStatement : public Ir<AstWhileStatement> {
public:
  IrWhileStatement(AstWhileStatement *node);
  virtual ~IrWhileStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* for statement */
class IrForStatement : public Ir<AstForStatement> {
public:
  IrForStatement(AstForStatement *node);
  virtual ~IrForStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* continue statement */
class IrContinueStatement : public Ir<AstContinueStatement> {
public:
  IrContinueStatement(AstContinueStatement *node);
  virtual ~IrContinueStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* break statement */
class IrBreakStatement : public Ir<AstBreakStatement> {
public:
  IrBreakStatement(AstBreakStatement *node);
  virtual ~IrBreakStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* return statement */
class IrReturnStatement : public Ir<AstReturnStatement> {
public:
  IrReturnStatement(AstReturnStatement *node);
  virtual ~IrReturnStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* empty statement */
class IrEmptyStatement : public Ir<AstEmptyStatement> {
public:
  IrEmptyStatement(AstEmptyStatement *node);
  virtual ~IrEmptyStatement() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* variable declaration */
class IrVariableDeclaration : public Ir<AstVariableDeclaration> {
public:
  IrVariableDeclaration(AstVariableDeclaration *node);
  virtual ~IrVariableDeclaration() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* variable assignment declaration */
class IrVariableAssignmentDeclaration
    : public Ir<AstVariableAssignmentDeclaration> {
public:
  IrVariableAssignmentDeclaration(AstVariableAssignmentDeclaration *node);
  virtual ~IrVariableAssignmentDeclaration() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* function declaration */
class IrFunctionDeclaration : public Ir<AstFunctionDeclaration> {
public:
  IrFunctionDeclaration(AstFunctionDeclaration *node);
  virtual ~IrFunctionDeclaration() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

/* function argument declaration */
class IrFunctionArgumentDeclaration
    : public Ir<AstFunctionArgumentDeclaration> {
public:
  IrFunctionArgumentDeclaration(AstFunctionArgumentDeclaration *node);
  virtual ~IrFunctionArgumentDeclaration() = default;
  virtual int type() const;
  virtual llvm::Value *codegen(IrContext *context);

protected:
  virtual std::string stringify() const;
};

// Copyright 2019- <shepherd-lang>
// Apache License Version 2.0

#pragma once
#include "Ast.h"
#include "Log.h"
#include "boost/core/noncopyable.hpp"
#include "enum.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <unordered_map>

/*================ type from start 4000 ================*/
BETTER_ENUM(IrType, int,
            // constant
            IdentifierConstant = 4000, F32Constant, F64Constant, I8Constant,
            U8Constant, I16Constant, U16Constant, I32Constant, U32Constant,
            I64Constant, U64Constant, StringConstant, BooleanConstant,
            // expression
            CallExpression, UnaryExpression, BinaryExpression,
            ConditionalExpression, AssignmentExpression, SequelExpression,
            // statement
            ExpressionStatement, CompoundStatement, IfStatement, WhileStatement,
            ForStatement, ContinueStatement, BreakStatement, ReturnStatement,
            EmptyStatement,
            // declaration
            VariableDeclaration, VariableInitialDeclaration,
            FunctionDeclaration, FunctionSignatureDeclaration,
            FunctionArgumentDeclaration,
            // list
            ExpressionList, StatementList, DeclarationList,
            // common
            TranslateUnit, Expression, Statement, Declaration)

/*================ class ================*/
// basic interface
class Ir;

// translate unit
class IrTranslateUnit;

// list
class IrExpressionList;
class IrStatementList;
class IrDeclarationList;

// constant
class IrIdentifierConstant;
class IrI8Constant;
class IrU8Constant;
class IrI16Constant;
class IrU16Constant;
class IrI32Constant;
class IrU32Constant;
class IrI64Constant;
class IrU64Constant;
class IrF32Constant;
class IrF64Constant;
class IrStringConstant;
class IrBooleanConstant;

// expression
class IrExpression;
class IrCallExpression;
class IrUnaryExpression;
class IrBinaryExpression;
class IrConditionalExpression;
class IrAssignmentExpression;
class IrSequelExpression;

// statement
class IrStatement;
class IrExpressionStatement;
class IrCompoundStatement;
class IrIfStatement;
class IrWhileStatement;
class IrForStatement;
class IrContinueStatement;
class IrBreakStatement;
class IrReturnStatement;
class IrEmptyStatement;

// declaration
class IrDeclaration;
class IrVariableDeclaration;
class IrVariableInitialDeclaration;
class IrFunctionDeclaration;
class IrFunctionArgumentDeclaration;

/*================ declaration ================*/

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
  std::unordered_map<std::string, llvm::Value *> &symtable();
  const std::unordered_map<std::string, llvm::Value *> &symtable() const;

private:
  llvm::LLVMContext context_;
  llvm::IRBuilder<> builder_;
  llvm::Module *module_;
  std::unordered_map<std::string, llvm::Value *> symtable_;
};

class Ir : public Namely, public Stringify, private boost::noncopyable {
public:
  Ir(const std::string &name);
  virtual ~Ir() = default;
  virtual std::string name() const;
  virtual std::string toString() const = 0;
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen(IrContext *context) = 0;

protected:
  std::string name_;
};

namespace detail {

template <class T> class IrList : public Ir {
public:
  IrList(const std::string &name) : Ir(name) {}
  virtual ~IrList() {
    for (int i = 0; i < (int)items_.size(); i++) {
      delete items_[i];
      items_[i] = nullptr;
    }
    items_.clear();
  }
  virtual IrType type() const = 0;
  virtual llvm::Value *codeGen(IrContext *context) = 0;
  virtual std::string toString() const {
    std::stringstream ss;
    ss << fmt::format("[ @{} size:{}", stringify(), items_.size());
    if (items_.empty()) {
      ss << " ]";
      return ss.str();
    }
    ss << ", ";
    for (int i = 0; i < (int)items_.size(); i++) {
      Ir *item = dynamic_cast<Ir *>(items_[i]);
      ss << fmt::format("{}:{}", i, item ? item->toString() : "null");
      if (i < (int)items_.size() - 1) {
        ss << ",";
      }
    }
    ss << " ]";
    return ss.str();
  }
  virtual bool empty() const { return items_.empty(); }
  virtual int size() const { return items_.size(); }
  virtual T *get(int pos) const { return items_[pos]; }
  virtual void add(T *item) { items_.push_back(item); }

protected:
  virtual std::string stringify() const = 0;
  std::deque<T *> items_;
};

} // namespace detail

/* translate unit */
class IrTranslateUnit : public detail::IrList<Ir> {
public:
  IrTranslateUnit(AstTranslateUnit *node);
  virtual ~IrTranslateUnit() = default;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstTranslateUnit *node_;
};

/* expression */
class IrExpression : public Ir {
public:
  IrExpression(AstExpression *node);
  virtual ~IrExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstExpression *node_;
};

/* identifier constant */
class IrIdentifierConstant : public Ir {
public:
  IrIdentifierConstant(AstIdentifierConstant *node);
  virtual ~IrIdentifierConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstIdentifierConstant *node_;
};

/* i8 constant */
class IrI8Constant : public Ir {
public:
  IrI8Constant(AstI8Constant *node);
  virtual ~IrI8Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstI8Constant *node_;
};

/* u8 constant */
class IrU8Constant : public Ir {
public:
  IrU8Constant(AstU8Constant *node);
  virtual ~IrU8Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstU8Constant *node_;
};

/* i16 constant */
class IrI16Constant : public Ir {
public:
  IrI16Constant(AstI16Constant *node);
  virtual ~IrI16Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstI16Constant *node_;
};

/* u16 constant */
class IrU16Constant : public Ir {
public:
  IrU16Constant(AstU16Constant *node);
  virtual ~IrU16Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstU16Constant *node_;
};

/* i32 constant */
class IrI32Constant : public Ir {
public:
  IrI32Constant(AstI32Constant *node);
  virtual ~IrI32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstI32Constant *node_;
};

/* u32 constant */
class IrU32Constant : public Ir {
public:
  IrU32Constant(AstU32Constant *node);
  virtual ~IrU32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstU32Constant *node_;
};

/* i64 constant */
class IrI64Constant : public Ir {
public:
  IrI64Constant(AstI64Constant *node);
  virtual ~IrI64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstI64Constant *node_;
};

/* u64 constant */
class IrU64Constant : public Ir {
public:
  IrU64Constant(AstU64Constant *node);
  virtual ~IrU64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstU64Constant *node_;
};

/* f32 constant */
class IrF32Constant : public Ir {
public:
  IrF32Constant(AstF32Constant *node);
  virtual ~IrF32Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstF32Constant *node_;
};

/* f64 constant */
class IrF64Constant : public Ir {
public:
  IrF64Constant(AstF64Constant *node);
  virtual ~IrF64Constant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstF64Constant *node_;
};

/* string constant */
class IrStringConstant : public Ir {
public:
  IrStringConstant(AstStringConstant *node);
  virtual ~IrStringConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstStringConstant *node_;
};

/* boolean constant */
class IrBooleanConstant : public Ir {
public:
  IrBooleanConstant(AstBooleanConstant *node);
  virtual ~IrBooleanConstant() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstBooleanConstant *node_;
};

/* call expression */
class IrCallExpression : public Ir {
public:
  IrCallExpression(AstCallExpression *node);
  virtual ~IrCallExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstCallExpression *node_;
};

/* unary expression */
class IrUnaryExpression : public Ir {
public:
  IrUnaryExpression(AstUnaryExpression *node);
  virtual ~IrUnaryExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstUnaryExpression *node_;
};

/* binary expression */
class IrBinaryExpression : public Ir {
public:
  IrBinaryExpression(AstBinaryExpression *node, IrExpression *left,
                     IrExpression *right);
  virtual ~IrBinaryExpression();
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstBinaryExpression *node_;
  IrExpression *left_;
  IrExpression *right_;
};

/* conditional expression */
class IrConditionalExpression : public Ir {
public:
  IrConditionalExpression(AstConditionalExpression *node);
  virtual ~IrConditionalExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstConditionalExpression *node_;
};

/* assignment expression */
class IrAssignmentExpression : public Ir {
public:
  IrAssignmentExpression(AstAssignmentExpression *node);
  virtual ~IrAssignmentExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstAssignmentExpression *node_;
};

/* sequel expression */
class IrSequelExpression : public Ir {
public:
  IrSequelExpression(AstSequelExpression *node);
  virtual ~IrSequelExpression() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstSequelExpression *node_;
};

/* expression statement */
class IrExpressionStatement : public Ir {
public:
  IrExpressionStatement(AstExpressionStatement *node);
  virtual ~IrExpressionStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstExpressionStatement *node_;
};

/* compound statement */
class IrCompoundStatement : public Ir {
public:
  IrCompoundStatement(AstCompoundStatement *node);
  virtual ~IrCompoundStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstCompoundStatement *node_;
};

/* if statement */
class IrIfStatement : public Ir {
public:
  IrIfStatement(AstIfStatement *node);
  virtual ~IrIfStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstIfStatement *node_;
};

/* while statement */
class IrWhileStatement : public Ir {
public:
  IrWhileStatement(AstWhileStatement *node);
  virtual ~IrWhileStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstWhileStatement *node_;
};

/* for statement */
class IrForStatement : public Ir {
public:
  IrForStatement(AstForStatement *node);
  virtual ~IrForStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstForStatement *node_;
};

/* continue statement */
class IrContinueStatement : public Ir {
public:
  IrContinueStatement(AstContinueStatement *node);
  virtual ~IrContinueStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstContinueStatement *node_;
};

/* break statement */
class IrBreakStatement : public Ir {
public:
  IrBreakStatement(AstBreakStatement *node);
  virtual ~IrBreakStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstBreakStatement *node_;
};

/* return statement */
class IrReturnStatement : public Ir {
public:
  IrReturnStatement(AstReturnStatement *node);
  virtual ~IrReturnStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstReturnStatement *node_;
};

/* empty statement */
class IrEmptyStatement : public Ir {
public:
  IrEmptyStatement(AstEmptyStatement *node);
  virtual ~IrEmptyStatement() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstEmptyStatement *node_;
};

/* variable declaration */
class IrVariableDeclaration : public Ir {
public:
  IrVariableDeclaration(AstVariableDeclaration *node);
  virtual ~IrVariableDeclaration() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstVariableDeclaration *node_;
};

/* variable assignment declaration */
class IrVariableInitialDeclaration : public Ir {
public:
  IrVariableInitialDeclaration(AstVariableInitialDeclaration *node);
  virtual ~IrVariableInitialDeclaration() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstVariableInitialDeclaration *node_;
};

/* function declaration */
class IrFunctionDeclaration : public Ir {
public:
  IrFunctionDeclaration(AstFunctionDeclaration *node);
  virtual ~IrFunctionDeclaration() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstFunctionDeclaration *node_;
};

/* function signature declaration */
class IrFunctionSignatureDeclaration : public Ir {
public:
  IrFunctionSignatureDeclaration(AstFunctionSignatureDeclaration *node);
  virtual ~IrFunctionSignatureDeclaration() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstFunctionSignatureDeclaration *node_;
};

/* function argument declaration */
class IrFunctionArgumentDeclaration : public Ir {
public:
  IrFunctionArgumentDeclaration(AstFunctionArgumentDeclaration *node);
  virtual ~IrFunctionArgumentDeclaration() = default;
  virtual std::string toString() const;
  virtual IrType type() const;
  virtual llvm::Value *codeGen(IrContext *context);

private:
  AstFunctionArgumentDeclaration *node_;
};

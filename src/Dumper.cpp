// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Dumper.h"
#include "Ast.h"
#include "boost/preprocessor/stringize.hpp"
#include <vector>

#define VINIT                                                                  \
  Context *ctx = static_cast<Context *>(context());                            \
  (void)ctx;

namespace detail {
namespace dumper {

struct Context : public VisitorContext {
  Context() : dumper(nullptr), indent(0) {}
  Dumper *dumper;
  int indent;
};

#define DECL0(x)                                                               \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Dumper::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      VINIT                                                                    \
      ctx->dumper->dumps().push_back(Cowstr(" ").repeat(ctx->indent) +         \
                                     ast->str());                              \
    }                                                                          \
  }

#define DECL1(x)                                                               \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Dumper::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      VINIT                                                                    \
      ctx->dumper->dumps().push_back(Cowstr(" ").repeat(ctx->indent) +         \
                                     ast->str());                              \
      ctx->indent += 1;                                                        \
    }                                                                          \
    virtual void finishVisit(Ast *ast) {                                       \
      VINIT                                                                    \
      ctx->indent -= 1;                                                        \
    }                                                                          \
  }

DECL0(Integer);
DECL0(Float);
DECL0(Boolean);
DECL0(Character);
DECL0(String);
DECL0(Nil);
DECL0(Void);
DECL0(VarId);
DECL0(Break);
DECL0(Continue);
DECL0(Exprs);
DECL0(BlockStats);
DECL0(PlainType);
DECL0(Params);
DECL0(TopStats);

DECL1(Throw);
DECL1(Return);
DECL1(Assign);
DECL1(Postfix);
DECL1(Prefix);
DECL1(Infix);
DECL1(Call);
DECL1(Group);
DECL1(If);
DECL1(Loop);
DECL1(Yield);
DECL1(LoopCondition);
DECL1(LoopEnumerator);
DECL1(DoWhile);
DECL1(Try);
DECL1(Block);
DECL1(FuncDef);
DECL1(FuncSign);
DECL1(Param);
DECL1(VarDef);
DECL1(CompileUnit);

} // namespace dumper
} // namespace detail

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::dumper::VISITOR(x)();                             \
    binder_.bind((+AstKind::x), v);                                            \
    visitors_.push_back(v);                                                    \
  } while (0)

Dumper::Dumper()
    : Phase("Dumper"), context_(new detail::dumper::Context()),
      binder_(context_) {
  BIND(Integer);
  BIND(Float);
  BIND(Boolean);
  BIND(Character);
  BIND(String);
  BIND(Nil);
  BIND(Void);
  BIND(VarId);
  BIND(Break);
  BIND(Continue);
  BIND(Exprs);
  BIND(BlockStats);
  BIND(PlainType);
  BIND(Params);
  BIND(TopStats);

  BIND(Throw);
  BIND(Return);
  BIND(Assign);
  BIND(Postfix);
  BIND(Prefix);
  BIND(Infix);
  BIND(Call);
  BIND(Group);
  BIND(If);
  BIND(Loop);
  BIND(Yield);
  BIND(LoopCondition);
  BIND(LoopEnumerator);
  BIND(DoWhile);
  BIND(Try);
  BIND(Block);
  BIND(FuncDef);
  BIND(FuncSign);
  BIND(Param);
  BIND(VarDef);
  BIND(CompileUnit);
}

Dumper::~Dumper() {
  delete context_;
  context_ = nullptr;
  for (int i = 0; i < (int)visitors_.size(); i++) {
    delete visitors_[i];
    visitors_[i] = nullptr;
  }
}

void Dumper::run(Ast *ast) { Visitor::traverse(&binder_, ast); }

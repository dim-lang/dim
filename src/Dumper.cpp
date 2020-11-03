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
      ctx->dumper->dumps().push_back(Cowstr::from(" ").repeat(ctx->indent) +   \
                                     ast->str());                              \
    }                                                                          \
  }

#define DECL1(x)                                                               \
  struct VISITOR(x) : public Visitor {                                         \
    VISITOR(x)() : Visitor("Dumper::" BOOST_PP_STRINGIZE(VISITOR(x))) {}       \
    virtual void visit(Ast *ast) {                                             \
      VINIT                                                                    \
      ctx->dumper->dumps().push_back(Cowstr::from(" ").repeat(ctx->indent) +   \
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
DECL0(Param);
DECL0(VarDef);
DECL0(CompileUnit);

} // namespace dumper
} // namespace detail

#define BIND(x)                                                                \
  do {                                                                         \
    Visitor *v = new detail::dumper::VISITOR(x)();                             \
  } while (0)

Dumper::Dumper()
    : Phase("Dumper"), context_(new detail::dumper::Context()),
      binder_(context_) {}

Dumper::~Dumper() {
  delete context_;
  context_ = nullptr;
}

void Dumper::run(Ast *ast) { Visitor::traverse(&binder_, ast); }

// Copyright 2019- <dim-lang>
// Apache License Version 2.0

#include "Dumper.h"
#include "Ast.h"
#include "Symbol.h"
#include "boost/preprocessor/stringize.hpp"
#include "fmt/format.h"

Dumper::Dumper() : Phase("Dumper"), indent_(0) {}

void Dumper::run(Ast *ast) { ast->accept(this); }

std::vector<Cowstr> &Dumper::dump() { return dump_; }

const std::vector<Cowstr> &Dumper::dump() const { return dump_; }

static Cowstr stringize(Ast *ast, int indent) {
  return Cowstr("|  ").repeat(indent) + Cowstr("`-") +
         fmt::format("{}@{} \'{}\' <{}>", ast->kind()._to_string(),
                     ast->identifier(), ast->name(), ast->location());
}

static Cowstr stringize(const Cowstr &hint, Ast *value) {
  return fmt::format("{}:{}@{}", hint, value->kind()._to_string(),
                     value->identifier());
}

static Cowstr stringize(const Cowstr &hint, Symbol *sym) {
  return fmt::format("{}:{}@{}", hint, sym->name(), sym->identifier());
}

static Cowstr stringize(const Cowstr &hint, TypeSymbol *tsym) {
  return fmt::format("{}:{}@{}", hint, tsym->name(), tsym->identifier());
}

#define STRINGIZE0(a) dump_.push_back(stringize(a, indent_))

#define STRINGIZE0_SYMBOL(a)                                                   \
  do {                                                                         \
    std::vector<Cowstr> joiner;                                                \
    joiner.push_back(stringize(a, indent_));                                   \
    if (a->symbol()) {                                                         \
      joiner.push_back(stringize("symbol", a->symbol()));                      \
    } else {                                                                   \
      joiner.push_back("symbol:null");                                         \
    }                                                                          \
    if (a->typeSymbol()) {                                                     \
      joiner.push_back(stringize("typeSymbol", a->typeSymbol()));              \
    } else {                                                                   \
      joiner.push_back("typeSymbol:null");                                     \
    }                                                                          \
    dump_.push_back(Cowstr::join(joiner.begin(), joiner.end(), " "));          \
  } while (0)

#define STRINGIZE1(a, b)                                                       \
  do {                                                                         \
    std::vector<Cowstr> joiner;                                                \
    joiner.push_back(stringize(a, indent_));                                   \
    if (a->b) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(b), a->b));                \
    }                                                                          \
    dump_.push_back(Cowstr::join(joiner.begin(), joiner.end(), " "));          \
    indent_ += 1;                                                              \
    if (a->b) {                                                                \
      a->b->accept(this);                                                      \
    }                                                                          \
    indent_ -= 1;                                                              \
  } while (0)

#define STRINGIZE1_NO_INDENT(a, b)                                             \
  do {                                                                         \
    std::vector<Cowstr> joiner;                                                \
    joiner.push_back(stringize(a, indent_));                                   \
    if (a->b) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(b), a->b));                \
    }                                                                          \
    dump_.push_back(Cowstr::join(joiner.begin(), joiner.end(), " "));          \
    if (a->b) {                                                                \
      a->b->accept(this);                                                      \
    }                                                                          \
  } while (0)

#define STRINGIZE2(a, b, c)                                                    \
  do {                                                                         \
    std::vector<Cowstr> joiner;                                                \
    joiner.push_back(stringize(a, indent_));                                   \
    if (a->b) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(b), a->b));                \
    }                                                                          \
    if (a->c) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(c), a->c));                \
    }                                                                          \
    dump_.push_back(Cowstr::join(joiner.begin(), joiner.end(), " "));          \
    indent_ += 1;                                                              \
    if (a->b) {                                                                \
      a->b->accept(this);                                                      \
    }                                                                          \
    if (a->c) {                                                                \
      a->c->accept(this);                                                      \
    }                                                                          \
    indent_ -= 1;                                                              \
  } while (0)

#define STRINGIZE2_NO_INDENT(a, b, c)                                          \
  do {                                                                         \
    std::vector<Cowstr> joiner;                                                \
    joiner.push_back(stringize(a, indent_));                                   \
    if (a->b) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(b), a->b));                \
    }                                                                          \
    if (a->c) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(c), a->c));                \
    }                                                                          \
    dump_.push_back(Cowstr::join(joiner.begin(), joiner.end(), " "));          \
    if (a->b) {                                                                \
      a->b->accept(this);                                                      \
    }                                                                          \
    if (a->c) {                                                                \
      a->c->accept(this);                                                      \
    }                                                                          \
  } while (0)

#define STRINGIZE3(a, b, c, d)                                                 \
  do {                                                                         \
    std::vector<Cowstr> joiner;                                                \
    joiner.push_back(stringize(a, indent_));                                   \
    if (a->b) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(b), a->b));                \
    }                                                                          \
    if (a->c) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(c), a->c));                \
    }                                                                          \
    if (a->d) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(d), a->d));                \
    }                                                                          \
    dump_.push_back(Cowstr::join(joiner.begin(), joiner.end(), " "));          \
    indent_ += 1;                                                              \
    if (a->b) {                                                                \
      a->b->accept(this);                                                      \
    }                                                                          \
    if (a->c) {                                                                \
      a->c->accept(this);                                                      \
    }                                                                          \
    if (a->d) {                                                                \
      a->d->accept(this);                                                      \
    }                                                                          \
    indent_ -= 1;                                                              \
  } while (0)

#define STRINGIZE3_NO_INDENT(a, b, c, d)                                       \
  do {                                                                         \
    std::vector<Cowstr> joiner;                                                \
    joiner.push_back(stringize(a, indent_));                                   \
    if (a->b) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(b), a->b));                \
    }                                                                          \
    if (a->c) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(c), a->c));                \
    }                                                                          \
    if (a->d) {                                                                \
      joiner.push_back(stringize(BOOST_PP_STRINGIZE(d), a->d));                \
    }                                                                          \
    dump_.push_back(Cowstr::join(joiner.begin(), joiner.end(), " "));          \
    if (a->b) {                                                                \
      a->b->accept(this);                                                      \
    }                                                                          \
    if (a->c) {                                                                \
      a->c->accept(this);                                                      \
    }                                                                          \
    if (a->d) {                                                                \
      a->d->accept(this);                                                      \
    }                                                                          \
  } while (0)

void Dumper::visitInteger(A_Integer *ast) { STRINGIZE0(ast); }
void Dumper::visitFloat(A_Float *ast) { STRINGIZE0(ast); }
void Dumper::visitBoolean(A_Boolean *ast) { STRINGIZE0(ast); }
void Dumper::visitCharacter(A_Character *ast) { STRINGIZE0(ast); }
void Dumper::visitString(A_String *ast) { STRINGIZE0(ast); }
void Dumper::visitNil(A_Nil *ast) { STRINGIZE0(ast); }
void Dumper::visitVoid(A_Void *ast) { STRINGIZE0(ast); }
void Dumper::visitVarId(A_VarId *ast) { STRINGIZE0_SYMBOL(ast); }
void Dumper::visitBreak(A_Break *ast) { STRINGIZE0(ast); }
void Dumper::visitContinue(A_Continue *ast) { STRINGIZE0(ast); }

void Dumper::visitThrow(A_Throw *ast) { STRINGIZE1(ast, expr); }
void Dumper::visitReturn(A_Return *ast) { STRINGIZE1(ast, expr); }
void Dumper::visitAssign(A_Assign *ast) { STRINGIZE2(ast, assignee, assignor); }
void Dumper::visitPostfix(A_Postfix *ast) { STRINGIZE1(ast, expr); }
void Dumper::visitInfix(A_Infix *ast) { STRINGIZE2(ast, left, right); }
void Dumper::visitPrefix(A_Prefix *ast) { STRINGIZE1(ast, expr); }
void Dumper::visitCall(A_Call *ast) { STRINGIZE2(ast, id, args); }
void Dumper::visitExprs(A_Exprs *ast) { STRINGIZE2_NO_INDENT(ast, expr, next); }
void Dumper::visitIf(A_If *ast) { STRINGIZE3(ast, condition, thenp, elsep); }
void Dumper::visitLoop(A_Loop *ast) { STRINGIZE2(ast, condition, body); }
void Dumper::visitYield(A_Yield *ast) { STRINGIZE1(ast, expr); }
void Dumper::visitLoopCondition(A_LoopCondition *ast) {
  STRINGIZE3(ast, init, condition, update);
}
void Dumper::visitLoopEnumerator(A_LoopEnumerator *ast) {
  STRINGIZE3(ast, id, type, expr);
}
void Dumper::visitDoWhile(A_DoWhile *ast) { STRINGIZE2(ast, body, condition); }
void Dumper::visitTry(A_Try *ast) { STRINGIZE3(ast, tryp, catchp, finallyp); }
void Dumper::visitBlock(A_Block *ast) { STRINGIZE1(ast, blockStats); }
void Dumper::visitBlockStats(A_BlockStats *ast) {
  STRINGIZE2_NO_INDENT(ast, blockStat, next);
}
void Dumper::visitPlainType(A_PlainType *ast) { STRINGIZE0(ast); }
void Dumper::visitFuncDef(A_FuncDef *ast) {
  STRINGIZE3(ast, funcSign, resultType, body);
}
void Dumper::visitFuncSign(A_FuncSign *ast) { STRINGIZE2(ast, id, params); }
void Dumper::visitParams(A_Params *ast) {
  STRINGIZE2_NO_INDENT(ast, param, next);
}
void Dumper::visitParam(A_Param *ast) { STRINGIZE2(ast, id, type); }
void Dumper::visitVarDef(A_VarDef *ast) { STRINGIZE3(ast, id, type, expr); }
void Dumper::visitTopStats(A_TopStats *ast) {
  STRINGIZE2_NO_INDENT(ast, topStat, next);
}
void Dumper::visitCompileUnit(A_CompileUnit *ast) { STRINGIZE1(ast, topStats); }

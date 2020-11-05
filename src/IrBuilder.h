// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Ir.h"
#include "Phase.h"
#include "Symbol.h"
#include "Visitor.h"
#include "iface/Scoped.h"
#include <vector>

#define DECL(x)                                                                \
  struct VISITOR(x) : public Visitor {                                         \
    Ir *ir;                                                                    \
    VISITOR(x)();                                                              \
    virtual void visit(Ast *ast);                                              \
    virtual void finishVisit(Ast *ast);                                        \
  }

namespace detail {

namespace ir_mod {}
namespace ir_global_var {}
namespace ir_func_decl {}
namespace ir_func_def {}

} // namespace detail

class IrBuilder : public Phase {
public:
  IrBuilder();
  virtual ~IrBuilder();
  virtual void run(Ast *ast);
  virtual Ir *&ir();
  virtual Ir *ir() const;

  struct Context : public VisitorContext, public Scoped {
    Context(IrBuilder *a_irBuilder);
    IrBuilder *irBuilder;
  };

  struct VISITOR(CompileUnit) : public Visitor {
    Ir *ir;
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
  Ir *ir_;
  VisitorContext *context_;
  VisitorBinder binder_;
  std::vector<Visitor *> visitors_;
};

#undef DECL

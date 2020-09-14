// Copyright 2019- <nerd-lang>
// Apache License Version 2.0

#pragma once
#include "Name.h"
#include "boost/core/noncopyable.hpp"
#include "container/LinkedHashMap.h"
#include "enum.h"

/*================ symbol kind enum start from 2000 ================*/
BETTER_ENUM(SymbolKind, int,
            // variable
            Variable = 2000,
            // function
            Function,
            // class
            Class, Field, Method,
            // scope
            Local, Global)

BETTER_ENUM(TypeSymbolKind, int,
            // builtin
            Builtin = 3000,
            // function
            Function,
            // class
            Class)

class Symbol : public Nameable, private boost::noncopyable {
public:
  Symbol(const Cowstr &name, const Symbol *owner);
  virtual ~Symbol() = default;
  virtual Symbol *owner() const;
  virtual SymbolKind kind() const = 0;

  // API from Nameable
  // virtual Cowstr &name();
  // virtual const Cowstr &name() const;

private:
  Symbol *owner_;
};

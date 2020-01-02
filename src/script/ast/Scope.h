// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "script/Ast.h"
#include "script/Token.h"
#include <memory>
#include <unordered_map>

namespace fastype {

class Scope : public Ast {
public:
  Scope(const icu::UnicodeString &name = UNICODE_STRING_SIMPLE("default_scope"),
        std::shared_ptr<Ast> father = std::shared_ptr<Ast>(nullptr));
  virtual ~Scope();
  virtual const icu::UnicodeString &name() const;
  virtual std::shared_ptr<Ast> father() const;
  virtual int size() const;
  virtual void insert(const icu::UnicodeString &name,
                      std::shared_ptr<Ast> node);
  virtual bool exist(const icu::UnicodeString &name) const;
  virtual std::shared_ptr<Ast> lookup(const icu::UnicodeString &name) const;
  virtual int remove(const icu::UnicodeString &name);
  virtual std::string toString() const;
  virtual int type() const;

private:
  icu::UnicodeString name_;
  std::shared_ptr<Ast> father_;
  std::unordered_map<icu::UnicodeString, std::shared_ptr<Ast>> table_;
};

} // namespace fastype

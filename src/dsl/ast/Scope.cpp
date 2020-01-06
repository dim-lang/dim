// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/ast/Scope.h"
#include "Logging.h"
#include "exception/ScriptException.h"

namespace fastype {

Scope::Scope(const icu::UnicodeString &name, std::shared_ptr<Ast> father)
    : name_(name), father_(father) {}

Scope::~Scope() {}

const icu::UnicodeString &Scope::name() const { return name_; }

std::shared_ptr<Ast> Scope::father() const { return father_; }

int Scope::size() const { return (int)table_.size(); }

void Scope::insert(const icu::UnicodeString &name, std::shared_ptr<Ast> node) {
  std::string _1, _2;
  F_CHECK(table_.find(name) != table_.end(),
          "name:{} already defined in scope:{}", name.toUTF8String(_1),
          name_.toUTF8String(_2));
  if (table_.find(name) == table_.end()) {
    F_THROW(ScriptException, "name:{} already defined in scope:{}",
            name.toUTF8String(_1), name_.toUTF8String(_2));
  }
  table_[name] = node;
}

bool Scope::exist(const icu::UnicodeString &name) const {
  return table_.find(name) != table_.end();
}

std::shared_ptr<Ast> Scope::lookup(const icu::UnicodeString &name) const {
  std::string _1;
  F_CHECK(table_.find(name) != table_.end(), "name:{} not found in scope:{}",
          name.toUTF8String(_1), name_.toUTF8String(_2));
  if (table_.find(name) == table_.end()) {
    F_THROW(ScriptException, "name:{} not found in scope:{}",
            name.toUTF8String(_1), name_.toUTF8String(_2));
  }
  return table_[name];
}

int Scope::remove(const icu::UnicodeString &name) {
  auto it = table_.find(name);
  if (it == table_.end()) {
    return -1;
  }
  table_.erase(it);
  return 0;
}

std::string Scope::toString() const {
  std::string _1;
  return fmt::format("[ @Scope name_:{}, father_:{}, table_#size:{} ]",
                     name_.toUTF8String(_1), father_->toString(),
                     table_.size());
}

int Scope::type() const { return F_TYPE_SCOPE; }

} // namespace fastype

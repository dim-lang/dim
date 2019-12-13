// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include "script/Symbol.h"
#include "script/symbol/BuiltinTypeSymbol.h"

namespace fastype {

const static icu::UnicodeString Symbol::T_NONE = UNICODE_STRING_SIMPLE("NONE");
const static icu::UnicodeString Symbol::T_BUILTIN =
    UNICODE_STRING_SIMPLE("BUILTIN");
const static icu::UnicodeString Symbol::T_VARIABLE =
    UNICODE_STRING_SIMPLE("VARIABLE");

const static std::shared_ptr<Symbol> Symbol::S_NIL =
    std::shared_ptr<Symbol>(nullptr);
const static std::shared_ptr<Symbol> Symbol::S_INTEGER =
    std::shared_ptr<Symbol>(
        new BuiltinTypeSymbol(UNICODE_STRING_SIMPLE("INTEGER")));
const static std::shared_ptr<Symbol> Symbol::S_FLOATING =
    std::shared_ptr<Symbol>(
        new BuiltinTypeSymbol(UNICODE_STRING_SIMPLE("FLOATING")));

} // namespace fastype

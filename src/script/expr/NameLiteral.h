// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "SmartPointer.h"
#include "script/AstLeaf.h"
#include "script/Token.h"
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

namespace fastype {

class NameLiteral : public AstLeaf {
public:
  NameLiteral(Sptr<Token> token);
  virtual ~NameLiteral() = default;
  virtual icu::UnicodeString value() const;
};

} // namespace fastype

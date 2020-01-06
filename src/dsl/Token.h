// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#pragma once
#include "Stringify.h"
#include "config/Header.h"
#include "dsl/Type.h"
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>

namespace dsl {

class Token : public Stringify {
public:
  // token constants

  // eofs
  static const std::shared_ptr<Token> TEOF;

  // operators
  static const std::shared_ptr<Token> TADD;       // +
  static const std::shared_ptr<Token> TSUB;       // -
  static const std::shared_ptr<Token> TMUL;       // *
  static const std::shared_ptr<Token> TDIV;       // /
  static const std::shared_ptr<Token> TMOD;       // %
  static const std::shared_ptr<Token> TNOT;       // !
  static const std::shared_ptr<Token> TINC;       // ++
  static const std::shared_ptr<Token> TDEC;       // --
  static const std::shared_ptr<Token> TADDASSIGN; // +=
  static const std::shared_ptr<Token> TSUBASSIGN; // -=
  static const std::shared_ptr<Token> TMULASSIGN; // *=
  static const std::shared_ptr<Token> TDIVASSIGN; // /=
  static const std::shared_ptr<Token> TMODASSIGN; // %=

  // assignment
  static const std::shared_ptr<Token> TASSIGNMENT; // =

  // comparator
  static const std::shared_ptr<Token> TEQ;  // ==
  static const std::shared_ptr<Token> TNEQ; // !=
  static const std::shared_ptr<Token> TLT;  // <
  static const std::shared_ptr<Token> TLE;  // <=
  static const std::shared_ptr<Token> TGT;  // >
  static const std::shared_ptr<Token> TGE;  // >=

  // booleans
  static const std::shared_ptr<Token> TTRUE;  // True
  static const std::shared_ptr<Token> TFALSE; // False

  // punctuations
  static const std::shared_ptr<Token> TLP;       // (
  static const std::shared_ptr<Token> TRP;       // )
  static const std::shared_ptr<Token> TLBRACKET; // [
  static const std::shared_ptr<Token> TRBRACKET; // ]
  static const std::shared_ptr<Token> TLBRACE;   // {
  static const std::shared_ptr<Token> TRBRACE;   // }
  static const std::shared_ptr<Token> TCOMMA;    // ,
  static const std::shared_ptr<Token> TSEMI;     // ;
  static const std::shared_ptr<Token> TQUESTION; // ?
  static const std::shared_ptr<Token> TCOLON;    // :

  // keywords
  static const std::shared_ptr<Token> TLET;        // let
  static const std::shared_ptr<Token> TNULL;       // null
  static const std::shared_ptr<Token> TIF;         // if
  static const std::shared_ptr<Token> TELSEIF;     // elseif
  static const std::shared_ptr<Token> TELSE;       // else
  static const std::shared_ptr<Token> TFOR;        // for
  static const std::shared_ptr<Token> TWHILE;      // while
  static const std::shared_ptr<Token> TBREAK;      // break
  static const std::shared_ptr<Token> TCONTINUE;   // continue
  static const std::shared_ptr<Token> TFUNC;       // func
  static const std::shared_ptr<Token> TCLASS;      // class
  static const std::shared_ptr<Token> TTYPE;       // type
  static const std::shared_ptr<Token> TISINSTANCE; // isinstance
  static const std::shared_ptr<Token> TIMPORT;     // import
  static const std::shared_ptr<Token> TRETURN;     // return
  static const std::shared_ptr<Token> TVOID;       // void

  Token(int type);
  Token(int type, const std::string &literal);
  virtual ~Token() = default;

  virtual std::string stringValue() const;
  virtual int64_t integerValue() const;
  virtual double realValue() const;
  virtual bool booleanValue() const;

  virtual bool equal(std::shared_ptr<Token> t) const;
  virtual std::string toString() const;

  int type;
  std::string literal;
};

} // namespace dsl
